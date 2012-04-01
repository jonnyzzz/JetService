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

int XmlFileSettings::parseDocument(rapidxml::xml_document<TCHAR>* doc) {
  FILE* file;
  if (0 != _wfopen_s(&file, myConfigFile, L"r")) {
    LOG.LogWarnFormat(L"Failed to open settings file %s", myConfigFile);
    return 1;
  }

  wifstream myfile(file);
  /* "Read file into vector<char>"  See linked thread above*/
  vector<TCHAR> buffer((istreambuf_iterator<TCHAR>(myfile)), istreambuf_iterator<TCHAR>( ));
  buffer.push_back(L'\0');
  buffer.push_back(L'\0');

  TCHAR* buff = &buffer[0];

  LOG.LogDebugFormat(L"Loaded settings:\n%s", CString(buff));
  
  try {
    doc->parse<0>(buff); 
  } catch(parse_error e) {
    CString what(e.what());
    LOG.LogDebugFormat(L"Failed to parse document. %s", what);
    return 1;
  } catch(...) {
    LOG.LogDebugFormat(L"Failed to parse document");
    return 1;
  }

  return 0;
}

int XmlFileSettings::executeCommand() {
  LOG.LogDebugFormat(L"Loading service settings from %s", myConfigFile);
 
  xml_document<TCHAR> doc;
  parseDocument(&doc);  
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


