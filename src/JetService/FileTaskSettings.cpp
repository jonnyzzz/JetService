#include "StdAfx.h"
#include "FileTaskSettings.h"
#include "SimpleServiceSettings.h"
#include "rapidxml\rapidxml.hpp"
#include "Logger.h"

using namespace rapidxml;
const Logger LOG(L"FileTaskSettings");


FileTaskSettings::FileTaskSettings(const RunServiceSettings* runSettings)
  : XmlFileSettings(runSettings->getServiceTaskSettingsPath())
  , myRunSettings(runSettings)
{
}


FileTaskSettings::~FileTaskSettings()
{
}


int FileTaskSettings::executeCommand(xml_document<TCHAR>* doc) {
  LOG.LogInfo(L"Parsing service settings");
  xml_node<TCHAR>* jetnode = doc->first_node(L"jetservice");
  if (jetnode == NULL) {
    LOG.LogError(L"Failed to find <jetservice> root element in settings");
    return 1;
  }

  xml_node<TCHAR>* execnode = jetnode->first_node(L"execution");
  if (execnode == NULL) {
    LOG.LogError(L"Failed to find <jetservice>/<execution> in settings");
    return 1;
  }

  CString workDir = L"";
  CString program = L"";
  CString argz = L"";

  {
    xml_node<TCHAR>* nameNode = execnode->first_node(L"workdir");
    if (nameNode == NULL) {
      LOG.LogDebug(L"Failed to find <jetservice>/<execution>/<workdir> element in settings");      
    } else {
      workDir = nodeText(nameNode);
    }
  }
  {
    xml_node<TCHAR>* nameNode = execnode->first_node(L"program");
    if (nameNode == NULL) {
      LOG.LogError(L"Failed to find <jetservice>/<execution>/<program> element in settings");      
      return 1;
    } else {
      program = nodeText(nameNode);
    }
  }
  {
    xml_node<TCHAR>* nameNode = execnode->first_node(L"arguments");
    if (nameNode == NULL) {
      LOG.LogDebug(L"Failed to find <jetservice>/<execution>/<arguments> element in settings");            
    } else {
      argz = nodeText(nameNode);
    }
  }

  if (program.GetLength() == 0) {
    LOG.LogError(L"Program to execute was not specified");
    return 1;
  }

  LOG.LogDebugFormat(L"Program to execute: %s", program);
  LOG.LogDebugFormat(L"Program arguments:  %s", argz);
  LOG.LogDebugFormat(L"Program work dir:   %s", workDir);

  SimpleServiceTaskSettings settings(myRunSettings, workDir, program, argz);
  const ServiceTaskSettings* pSettings = &settings;
  return executeCommand(pSettings);
}
