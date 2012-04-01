#include "StdAfx.h"
#include "XmlFileSettings.h"
#include "rapidxml\rapidxml.hpp"
#include "Logger.h"
#include <fstream>
#include <vector>

using namespace rapidxml;
using namespace std;


const Logger LOG(L"FileServiceSettings");


XmlFileSettings::XmlFileSettings(const CString& configFilePath)
  : myConfigFile(configFilePath)
{
}


XmlFileSettings::~XmlFileSettings()
{
}

int XmlFileSettings::executeCommand() {
  LOG.LogDebugFormat(L"Loading service settings from %s", myConfigFile);

   /* "Read file into vector<char>"  See linked thread above*/
  vector<TCHAR> buffer;
  { 
    FILE* file;
    if (0 != _wfopen_s(&file, myConfigFile, L"r")) {
      LOG.LogWarnFormat(L"Failed to open settings file %s", myConfigFile);
      return 1;
    }

    wifstream myfile(file);
    for (istreambuf_iterator<TCHAR> it = istreambuf_iterator<TCHAR>(myfile); it != istreambuf_iterator<TCHAR>(); it++) {
      buffer.push_back(*it);
    }
    myfile.close();
    fclose(file);
  }

  buffer.push_back(L'\0');
  buffer.push_back(L'\0');
  
  TCHAR* buff = &buffer[0];

  LOG.LogDebugFormat(L"Loaded settings:\n%s", CString(buff));
  
  xml_document<TCHAR> doc;
  try {
    doc.parse<0>(buff); 
  } catch(parse_error e) {
    CString what(e.what());
    LOG.LogDebugFormat(L"Failed to parse document. %s", what);
    return 1;
  } catch(...) {
    LOG.LogDebugFormat(L"Failed to parse document");
    return 1;
  }
  
  //FILE handle should be closed at that point.
  return executeCommand(&doc);
}


CString XmlFileSettings::nodeText(xml_node<TCHAR>* node) {
  CString buff;
  for(xml_node<TCHAR>* ch = node->first_node(); ch != NULL; ch = ch->next_sibling()) {
    //TODO: recursive
    buff.Append(ch->value());
  }  
  buff = buff.Trim();
  return buff;
}


