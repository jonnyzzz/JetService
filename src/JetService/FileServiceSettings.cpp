#include "StdAfx.h"
#include "FileServiceSettings.h"
#include "SimpleServiceSettings.h"
#include "rapidxml\rapidxml.hpp"
#include "Logger.h"

using namespace rapidxml;


const Logger LOG(L"FileServiceSettings");

FileServiceSettings::FileServiceSettings(const CString& configFile)
  : XmlFileSettings(configFile)
{
}


FileServiceSettings::~FileServiceSettings()
{
}

int FileServiceSettings::executeCommand(xml_document<TCHAR>* doc) {
  xml_node<TCHAR>* root = doc->first_node(L"jetservice");
  if (root == NULL) {
    LOG.LogWarn(L"Failed to find <jetservice> node in document");
    return 1;
  }

  CString name;
  {
    xml_node<TCHAR>* nameNode = root->first_node(L"name");
    if (nameNode == NULL) {
      LOG.LogWarn(L"Failed to find <jetservice>/<name> element in settings");
      return 1;
    }
    name = nodeText(nameNode);
  }


  CString descr;
  {
    xml_node<TCHAR>* descrNode = root->first_node(L"description");
    if (descrNode == NULL) {
      LOG.LogWarn(L"Failed to find <jetservice>/<description> element in settings");
      return 1;
    }
    descr = nodeText(descrNode);
  }

  LOG.LogDebugFormat(L"Parsed service name: %s", name);
  LOG.LogDebugFormat(L"Parsed service description: %s", descr);

  SimpleServiceSettings settings(name, descr);
  return executeCommand(&settings);  
}


