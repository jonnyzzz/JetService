#include "StdAfx.h"
#include "FileServiceSettings.h"
#include "rapidxml\rapidxml.hpp"
#include "rapidxml\rapidxml_iterators.hpp"
#include "rapidxml\rapidxml_utils.hpp"
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

CString parseAttribute(xml_node<TCHAR> node, const CString& name) {
  xml_node<TCHAR>* s = node.first_node(name);
  if (s == NULL) return L"";
  s.o
}

int FileServiceSettings::executeCommand() {
  LOG.LogDebugFormat(L"Loading service settings from %s", myConfigFile);

  FILE* file;
  if (0 != _wfopen_s(&file, L"sampleconfig.xml", L"r")) {
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
    LOG.LogWarnFormat(L"Failed to find <jetservice> node in document");
    return 1;
  }
  
  CString serviceName;
  CString serviceDescription;

  for(node_iterator<TCHAR> nodes(root);nodes != NULL; nodes++) {
    if (CString((*nodes).name()) == L"name") {
      serviceName = (*nodes).a
  }

  

  return 0;
}




int FileServiceSettings::executeCommnad(const CreateServiceSettings* settings) {return 0;}
int FileServiceSettings::executeCommnad(const ServiceSettings* settings) {return 0;}
