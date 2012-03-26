#include "StdAfx.h"
#include "SimpleServiceSettings.h"


SimpleServiceSettings::SimpleServiceSettings(const CString& name, const CString& descr)
  : myName(name)
  , myDescription(descr)
{
}


SimpleServiceSettings::~SimpleServiceSettings(void)
{
}


CString SimpleServiceSettings::getServiceName() const {
  return myName;
}

CString SimpleServiceSettings::getServiceDisplayName() const{
  return myDescription;
}



////////////////////////////////////////


SimpleCreateServiceSettings::SimpleCreateServiceSettings(const ServiceSettings* baseSettings)
  : myBase(baseSettings),
  myUserName(L""),
  myPassword(L""),
  myAutostart(true), 
  myRunAsSystem(false) {
}

SimpleCreateServiceSettings::~SimpleCreateServiceSettings(){
}

CString SimpleCreateServiceSettings::getServiceName() const {
  return myBase->getServiceName();
}

CString SimpleCreateServiceSettings::getServiceDisplayName() const{
  return myBase->getServiceDisplayName();
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



