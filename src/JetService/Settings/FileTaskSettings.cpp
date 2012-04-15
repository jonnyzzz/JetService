#include "StdAfx.h"
#include "FileTaskSettings.h"
#include "SimpleServiceSettings.h"
#include "rapidxml\rapidxml.hpp"
#include "Logger.h"

using namespace rapidxml;
const Logger LOG(L"FileTaskSettings");


FileTaskSettings::FileTaskSettings(const ServiceSettings* runSettings)
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

  CString baseDir;
  SimpleServiceTaskSettings settings(myRunSettings);


  int ret = GetBaseDirectory(baseDir);
  if (ret != 0) return ret;
  ret = parseProgramDir(baseDir, execnode, &settings, baseDir);
  if (ret != 0) return ret;
  ret = parseProgramPath(baseDir, execnode, &settings);
  if (ret != 0) return ret;
  ret = parseProgramArgs(baseDir, execnode, &settings);
  if (ret != 0) return ret;
  ret = parseProgramStopMethod(baseDir, execnode, &settings);
  if (ret != 0) return ret;

  

  const ServiceTaskSettings* pSettings = &settings;

  LOG.LogInfoFormat(L"Program to execute:   %s", pSettings->getProgramPath());
  LOG.LogInfoFormat(L"Program arguments:    %s", pSettings->getProgramArguments());
  LOG.LogInfoFormat(L"Program work dir:     %s", pSettings->getWorkDir());
  LOG.LogInfoFormat(L"Program stop timeout: %ld", pSettings->getTerminateWaitTimeoutMilliseconds());

  return executeCommand(pSettings);
}

int FileTaskSettings::parseProgramStopMethod(CString baseDir, rapidxml::xml_node<TCHAR>* execution, SimpleServiceTaskSettings* settings) {
  //defaults
  settings->setTerminateWaitTimeoutMillis(2 * 60 * 1000);
  settings->setStopCommand(NULL);

  xml_node<TCHAR>* termination = execution->first_node(L"termination");
  if (termination == NULL) {
    LOG.LogDebug(L"No termination action is specified in <jetservice>/<execution>/<termination>. Will use default termination");    
    return 0;
  }

  int ret = parseProgramStopTimeout(termination, settings);
  if (ret != 0) return ret;

  xml_node<TCHAR>* stopExe = termination->first_node(L"execution");
  if (stopExe == NULL) {
    LOG.LogDebug(L"No stop command is specified in <jetservice>/<execution>/<termination>/<execution>");
    return 0;
  }

  SimpleExecutionSettings stopSettings;
  ret = parseProgramPath(baseDir, stopExe, &stopSettings);
  if (ret != 0) return ret;
  ret = parseProgramDir(baseDir, stopExe, &stopSettings, settings->getWorkDir());
  if (ret != 0) return ret;
  ret = parseProgramArgs(baseDir, stopExe, &stopSettings);
  if (ret != 0) return ret;

  settings->setStopCommand(&stopSettings);

  return 0;
}

int FileTaskSettings::parseProgramStopTimeout(rapidxml::xml_node<TCHAR>* node, SimpleServiceTaskSettings* settings) {
  xml_attribute<TCHAR>* attr = node->first_attribute(L"timeout");
  if (attr == NULL) {
    LOG.LogDebug(L"No termination timeout is specified in <jetservice>/<execution>/<termination> 'timeout' attribute. Will use default termination of 2 minutes");
    return 0;
  }

  CString value;
  value.Append(attr->value(), attr->value_size());
  
  LOG.LogDebugFormat(L"Program stop timeout from settings: %s", value);
  value = value.Trim();

  if (value.GetLength() == 0) {
    LOG.LogError(L"timout value is empty");
    return 1;
  }

  if (value == L"0") {
    settings->setTerminateWaitTimeoutMillis(0);
    return 0;
  }

  LOG.LogDebugFormat(L"Parsing timeout: %s", value);
  double timeout = _wtof(value);
  if (timeout <= 0) {
    LOG.LogErrorFormat(L"Failed to parse timeout value: %s", value);
    return 1;
  }

  LOG.LogDebugFormat(L"Parsed value: %f", timeout);
  settings->setTerminateWaitTimeoutMillis((long)(timeout * 1000 * 60));
  return 0;
}


int FileTaskSettings::parseProgramPath(CString baseDir, rapidxml::xml_node<TCHAR>* execnode, SimpleExecutionSettings* settings) {  
  CString program = L"";

  xml_node<TCHAR>* nameNode = execnode->first_node(L"program");
  if (nameNode == NULL) {
    LOG.LogError(L"Failed to find <jetservice>/<execution>/<program> element in settings");      
    return 1;
  } 

  program = nodeText(nameNode);
  LOG.LogDebugFormat(L"Program to execute from settings: %s", program);
  
  if (program.GetLength() == 0) {
    LOG.LogError(L"Program to execute was not specified");
    return 1;
  }

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

  settings->setProgramPath(program);
  return 0;
}

int FileTaskSettings::parseProgramArgs(CString baseDir, rapidxml::xml_node<TCHAR>* execnode, SimpleExecutionSettings* settings) {
  CString argz = L"";
  
  xml_node<TCHAR>* nameNode = execnode->first_node(L"arguments");
  if (nameNode == NULL) {
    LOG.LogDebug(L"Failed to find <jetservice>/<execution>/<arguments> element in settings");            
  } else {
    argz = nodeText(nameNode);
  }

  LOG.LogDebugFormat(L"Program arguments from settings:  %s", argz);
  settings->setProgramArguments(argz);
  return 0;
}

int FileTaskSettings::parseProgramDir(CString baseDir, rapidxml::xml_node<TCHAR>* execnode, SimpleExecutionSettings* settings, CString def) {    
  CString workDir;

  xml_node<TCHAR>* nameNode = execnode->first_node(L"workdir");
  if (nameNode == NULL) {
    LOG.LogDebug(L"Failed to find <jetservice>/<execution>/<workdir> element in settings");      
    workDir = def;
  } else {
    workDir = nodeText(nameNode);
    LOG.LogDebugFormat(L"Program work dir from settings:   %s", workDir);
  
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

  settings->setWorkDir(workDir);
  return 0;
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

