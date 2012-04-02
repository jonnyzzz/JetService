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
  
  CString baseDir;
  int ret = GetBaseDirectory(baseDir);
  if (ret != 0) return ret;

  //resolve workdir
  {
    if (workDir.Find(L"\\\\") == 0) {
      LOG.LogErrorFormat(L"Program path may not refer to a network share");
      return 1;
    }
    //remove slashes to make Combine work right.
    workDir = workDir.TrimLeft(L'\\').TrimLeft(L'/');

    const int sz = max(MAX_PATH + 1, 65535);
    TCHAR buff[sz+1];
    if(NULL == PathCombine(buff, baseDir, workDir)) {
      LOG.LogErrorFormat(L"Failed to resolve workdir path: %s", workDir);
      return 1;
    }
    workDir = buff;
    *buff = L'\0';
    int n = GetFullPathName(workDir, sz, buff, NULL);
    if (n <= 0 || n >= sz) {
      LOG.LogErrorFormat(L"Failed to resolve workdir full path: %s", workDir);
      return 1;
    }
    workDir = buff;
    LOG.LogDebugFormat(L"Resolved workdir path: %s", workDir);

    DWORD attrs = GetFileAttributes(workDir);
    if (attrs == INVALID_FILE_ATTRIBUTES || (attrs & FILE_ATTRIBUTE_DIRECTORY) == 0) {
      LOG.LogErrorFormat(L"Workdir must exist and be directory: %s", workDir);
      return 1;
    }
  }

  //resolve program path
  {
    if (program.Find(L"\\\\") == 0) {
      LOG.LogErrorFormat(L"Program path may not refer to a network share");
      return 1;
    }

    const int sz = max(MAX_PATH+1, 65535);
    TCHAR buff[sz+1];

    if (program.GetLength() >= 3 && iswalpha(program.GetAt(0)) && program.GetAt(1) == L':') {
      DWORD attrs = GetFileAttributes(program);
      if (attrs == INVALID_FILE_ATTRIBUTES) {
        LOG.LogErrorFormat(L"File does not exists: %s", program);
        return 1;
      } 

      if ((attrs & FILE_ATTRIBUTE_DIRECTORY) != 0) {
        LOG.LogErrorFormat(L"File is a directory: %s", program);
        return 1;
      }      
    } else {
      LPCTSTR extraPaths[] = { baseDir, NULL};
      if (!CopyStringChars(program, buff, sz)) {
        LOG.LogErrorFormat(L"Program path is too long: %s", program);
        return 1;
      }
  
      if (FALSE == PathFindOnPath(buff, extraPaths)) {
        LOG.LogErrorFormat(L"Failed to find executable for: %s", program);
        return 1;
      }
      program = buff;
    }
    
    int n = GetFullPathName(program, sz, buff, NULL);
    if (n <= 0 || n >= sz) {
      LOG.LogErrorFormat(L"Failed to resolve program full path: %s", program);
      return 1;
    }
    program = buff;
    LOG.LogDebugFormat(L"Resolved path to program: %s", program);
  }

  SimpleServiceTaskSettings settings(myRunSettings, workDir, program, argz);
  const ServiceTaskSettings* pSettings = &settings;

  LOG.LogInfoFormat(L"Program to execute: %s", pSettings->getProgramPath());
  LOG.LogInfoFormat(L"Program arguments:  %s", pSettings->GetProgramArguments());
  LOG.LogInfoFormat(L"Program work dir:   %s", pSettings->getWorkDir());

  return executeCommand(pSettings);
}

int FileTaskSettings::GetBaseDirectory(CString& baseFile) {
  CString settings = myRunSettings->getServiceTaskSettingsPath();
  LOG.LogDebugFormat(L"Start resolve of base path: %s", settings);

  const DWORD sz = 65535;
  TCHAR buff[sz+1];
  TCHAR* pFile;
  
  int n = GetFullPathName(settings, sz, buff, &pFile);
  if (n <= 0 || n >= sz) {
    LOG.LogErrorFormat(L"Failed to resolve path: %s", settings);
    return 1;
  }

  if (pFile == NULL) {
    LOG.LogErrorFormat(L"Given path to settings should point to file: %s", settings);
    return 1;
  }

  //make path end before file name in buff
  *pFile = L'\0';
  pFile--;
  //remove trailing slash
  if (*pFile == L'\\' || *pFile == L'/') *pFile = L'\0';


  LOG.LogDebugFormat(L"Resolved base path: %s", buff);
  baseFile = buff;
  return 0;
}