#include "StdAfx.h"
#include "FileServiceSettings.h"
#include "rapidxml\rapidxml.hpp"
#include "Logger.h"
#include <fstream>
#include <vector>

using namespace rapidxml;
using namespace std;

const Logger LOG(L"FileServiceSettings");

FileServiceSettings::FileServiceSettings(const CString& configFile)
  : myConfigFile(configFile)
{
}


FileServiceSettings::~FileServiceSettings(void)
{
}

CString nodeText(xml_node<TCHAR>* node) {
  CString buff;
  for(xml_node<TCHAR>* ch = node->first_node(); ch != NULL; ch = ch->next_sibling()) {
    //TODO: recursive
    buff.Append(ch->value());
  }  
  return buff;
}


int FileServiceSettings::executeCommand() {
  LOG.LogDebugFormat(L"Loading service settings from %s", myConfigFile);

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

  LOG.LogDebugFormat(L"Loaded settings:\r\n%s", CString(buff));


  xml_document<TCHAR> doc;
  doc.parse<0>(buff); 

  xml_node<TCHAR>* root = doc.first_node(L"jetservice");
  if (root == NULL) {
    LOG.LogWarn(L"Failed to find <jetservice> node in document");
    return 1;
  }

  CString name;
  {
    xml_node<TCHAR>* nameNode = root->first_node(L"name");
    if (nameNode == NULL) {
      LOG.LogWarn(L"Failed to find <jetservice>/<name> element in document");
      return 1;
    }
    name = nodeText(nameNode);
  }


  CString descr;
  {
    xml_node<TCHAR>* descrNode = root->first_node(L"description");
    if (descrNode == NULL) {
      LOG.LogWarn(L"Failed to find <jetservice>/<description> element in document");
      return 1;
    }
    descr = nodeText(descrNode);
  }

  LOG.LogErrorFormat(L"Parsed name:%s", name);
  LOG.LogErrorFormat(L"Parsed descr:%s", descr);

  return 0;
}


int FileServiceSettings::executeCommnad(const CreateServiceSettings* settings) {return 0;}
int FileServiceSettings::executeCommnad(const ServiceSettings* settings) {return 0;}
