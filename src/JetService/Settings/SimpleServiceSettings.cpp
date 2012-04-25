#include "StdAfx.h"
#include "SimpleServiceSettings.h"


SimpleServiceSettings::SimpleServiceSettings(const CString& name, const CString& dname, const CString& descr, const CString& path)
  : myName(name)
  , myDName(dname)
  , myDescription(descr)
  , mySettingsFilePath(path)
{
}


SimpleServiceSettings::~SimpleServiceSettings(void)
{
}


CString SimpleServiceSettings::getServiceName() const {
  return myName;
}

CString SimpleServiceSettings::getServiceDescription() const {
  return myDescription;
}

CString SimpleServiceSettings::getServiceDisplayName() const {
  return myDName;
}

CString SimpleServiceSettings::getServiceTaskSettingsPath() const {
  return mySettingsFilePath;
}


////////////////////////////////////////

SimpleUserSettings::SimpleUserSettings() 
  : myUserName(L"")
  , myDomain(L".")
  , myPassword(L"") {
}

SimpleUserSettings::~SimpleUserSettings() {
}

CString SimpleUserSettings::getUserName() const {
  return myUserName;
}

CString SimpleUserSettings::getDomain() const {
  return myDomain;
}

CString SimpleUserSettings::getPassword() const {
  return myPassword;
}

void SimpleUserSettings::setUserName(const CString& name) {
  myUserName = name;
}

void SimpleUserSettings::setDomain(const CString& domain) {
  myDomain = domain;
}

void SimpleUserSettings::setPassword(const CString& pwd) {
  myPassword = pwd;
}


////////////////////////////////////////

SimpleCreateServiceSettings::SimpleCreateServiceSettings(const ServiceSettings* baseSettings, const CString& serviceCommand)
  : myBase(baseSettings)
  , myAutostart(true)
  , myRunAsSystem(false)
  , myServiceExecutableCommand(serviceCommand) {
}

SimpleCreateServiceSettings::~SimpleCreateServiceSettings(){
}

CString SimpleCreateServiceSettings::getServiceName() const {
  return myBase->getServiceName();
}

CString SimpleCreateServiceSettings::getServiceDisplayName() const {
  return myBase->getServiceDisplayName();
}

CString SimpleCreateServiceSettings::getServiceDescription() const {
  return myBase->getServiceDescription();
}

CString SimpleCreateServiceSettings::getServiceTaskSettingsPath() const {
  return myBase->getServiceTaskSettingsPath();
}

bool SimpleCreateServiceSettings::isAutostart() const {
  return myAutostart;
}

bool SimpleCreateServiceSettings::runAsSystem() const {
  return myRunAsSystem;
}

CString SimpleCreateServiceSettings::getUserName() const {
  return SimpleUserSettings::getUserName();
}

CString SimpleCreateServiceSettings::getDomain() const {
  return SimpleUserSettings::getDomain();
}

CString SimpleCreateServiceSettings::getPassword() const {
  return SimpleUserSettings::getPassword();
}

CString SimpleCreateServiceSettings::getServiceExecutable() const {
  return myServiceExecutableCommand;
}

void SimpleCreateServiceSettings::setAutostart(bool autostart) {
  myAutostart = autostart;
}

void SimpleCreateServiceSettings::setRunAsSystem(bool runAsSystem) {
  myRunAsSystem = runAsSystem;
}

///////////////////
SimpleExecutionSettings::SimpleExecutionSettings()
  : myWorkDir(L"")
  , myProgramPath(L"")
  , myArguments(L"") {
}


SimpleExecutionSettings::~SimpleExecutionSettings() {
}

CString SimpleExecutionSettings::getWorkDir() const {
  return myWorkDir;
}

CString SimpleExecutionSettings::getProgramPath() const {
  return myProgramPath;
}

CString SimpleExecutionSettings::getProgramArguments() const {
  return myArguments;
}
void SimpleExecutionSettings::setWorkDir(const CString& workdir) {
  myWorkDir = workdir;
}

void SimpleExecutionSettings::setProgramPath(const CString& path) {
  myProgramPath = path;
}

void SimpleExecutionSettings::setProgramArguments(const CString& argz) {
  myArguments = argz;
}

///////////////////

SimpleServiceTaskSettings::SimpleServiceTaskSettings(const ServiceSettings* baseSettings)
  : myBase(baseSettings)
  , myStopTimeout(0)  
  , myUseStopCommand(false){
}

SimpleServiceTaskSettings::~SimpleServiceTaskSettings() {
}


CString SimpleServiceTaskSettings::getServiceName() const {
  return myBase->getServiceName();
} 

CString SimpleServiceTaskSettings::getServiceDisplayName() const {
  return myBase->getServiceDisplayName();
}

CString SimpleServiceTaskSettings::getServiceDescription() const {
  return myBase->getServiceDescription();
}

CString SimpleServiceTaskSettings::getServiceTaskSettingsPath() const {
  return myBase->getServiceTaskSettingsPath();
}

CString SimpleServiceTaskSettings::getWorkDir() const {
  return SimpleExecutionSettings::getWorkDir();
}

CString SimpleServiceTaskSettings::getProgramPath() const {
  return SimpleExecutionSettings::getProgramPath();
}

CString SimpleServiceTaskSettings::getProgramArguments() const {
  return SimpleExecutionSettings::getProgramArguments();
}

long SimpleServiceTaskSettings::getTerminateWaitTimeoutMilliseconds() const {
  return myStopTimeout;
}

const ExecutionSettings* SimpleServiceTaskSettings::getStopCommand() const {
  return myUseStopCommand ? &myStopCommand : NULL;
}

void SimpleServiceTaskSettings::setStopCommand(const ExecutionSettings* command) {
  if (command == NULL) {
    myUseStopCommand = false;
  } else {
    myUseStopCommand = true;
    myStopCommand.setProgramArguments(command->getProgramArguments());
    myStopCommand.setProgramPath(command->getProgramPath());
    myStopCommand.setWorkDir(command->getWorkDir());
  }
}

void SimpleServiceTaskSettings::setTerminateWaitTimeoutMillis(long timeout) {
  myStopTimeout = max(0, timeout);
}


const std::vector<CString>& SimpleServiceTaskSettings::getEnvironmentVariablesToOverride() const {
  return myEnvironmentToOverride;
}

void SimpleServiceTaskSettings::addEnvironmentVariableToOverride(const CString& name) {
  myEnvironmentToOverride.push_back(name);
}
