#include "StdAfx.h"
#include "SimpleServiceSettings.h"


SimpleServiceSettings::SimpleServiceSettings(const CString& name, const CString& dname, const CString& descr)
  : myName(name)
  , myDName(dname)
  , myDescription(descr)
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

CString SimpleServiceSettings::getServiceDisplayName() const{
  return myDName;
}



////////////////////////////////////////


SimpleCreateServiceSettings::SimpleCreateServiceSettings(const RunServiceSettings* baseSettings, const CString& serviceCommand)
  : myBase(baseSettings)
  , myUserName(L"")
  , myPassword(L"")
  , myAutostart(true)
  , myRunAsSystem(false)
  , myServiceExecutableCommand(serviceCommand) {
}

SimpleCreateServiceSettings::~SimpleCreateServiceSettings(){
}

CString SimpleCreateServiceSettings::getServiceName() const {
  return myBase->getServiceName();
}

CString SimpleCreateServiceSettings::getServiceDisplayName() const{
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

bool SimpleCreateServiceSettings::runAsSystem() const{
  return myRunAsSystem;
}

CString SimpleCreateServiceSettings::getUserName() const{
  return myUserName;
}

CString SimpleCreateServiceSettings::getPassword() const {
  return myPassword;
}

CString SimpleCreateServiceSettings::getServiceExecutable() const {
  return myServiceExecutableCommand;
}

void SimpleCreateServiceSettings::setUserName(const CString& name) {
  myUserName = name;
}

void SimpleCreateServiceSettings::setPassword(const CString& pwd) {
  myPassword = pwd;
}

void SimpleCreateServiceSettings::setAutostart(bool autostart) {
  myAutostart = autostart;
}

void SimpleCreateServiceSettings::setRunAsSystem(bool runAsSystem) {
  myRunAsSystem = runAsSystem;
}

/////////



SimpleRunServiceSettings::SimpleRunServiceSettings(const ServiceSettings* base, const CString& taskPath)
  : myBase(base)
  , myTaskPath(taskPath){
}

SimpleRunServiceSettings::~SimpleRunServiceSettings() {
}

CString SimpleRunServiceSettings::getServiceName() const {
  return myBase->getServiceName();
}

CString SimpleRunServiceSettings::getServiceDisplayName() const {
  return myBase->getServiceDisplayName();
}

CString SimpleRunServiceSettings::getServiceDescription() const {
  return myBase->getServiceDescription();
}


CString SimpleRunServiceSettings::getServiceTaskSettingsPath() const {
  return myTaskPath;
}


///////////////////

SimpleServiceTaskSettings::SimpleServiceTaskSettings(const RunServiceSettings* baseSettings, 
                            const CString& workDir, 
                            const CString& programPath, 
                            const CString& arguments)
  : myBase(baseSettings)
  , myWorkDir(workDir)
  , myProgramPath(programPath)
  , myArguments(arguments) {
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
  return myWorkDir;
}

CString SimpleServiceTaskSettings::getProgramPath() const {
  return myProgramPath;
}

CString SimpleServiceTaskSettings::GetProgramArguments() const {
  return myArguments;
}



