#pragma once

#include "ServiceSettings.h"

class SimpleServiceSettings : public ServiceSettings
{
public:
  SimpleServiceSettings(const CString& name, const CString& descr);
  virtual ~SimpleServiceSettings();

public:  
  virtual CString getServiceName() const;
  virtual CString getServiceDisplayName() const;

private:
  const CString myName;
  const CString myDescription;
};



class SimpleCreateServiceSettings : public CreateServiceSettings {
public:
  SimpleCreateServiceSettings(const ServiceSettings* baseSettings, const CString& serviceCommand);
  virtual ~SimpleCreateServiceSettings();

public:
  virtual CString getServiceName() const;
  virtual CString getServiceDisplayName() const;
  virtual bool isAutostart() const;
  virtual bool runAsSystem() const;
  virtual CString getUserName() const;
  virtual CString getPassword() const;
  virtual CString getServiceExecutable() const;

public:
  void setUserName(const CString& name);
  void setPassword(const CString& pwd);
  void setAutostart(bool autostart);
  void setRunAsSystem(bool runAsSystem);

private:
  const ServiceSettings* myBase;
  const CString myServiceExecutableCommand;
  CString myUserName;
  CString myPassword;
  bool myAutostart;
  bool myRunAsSystem;
};