#pragma once

#include "ServiceSettings.h"

class SimpleServiceSettings : public ServiceSettings
{
public:
  SimpleServiceSettings(const CString& name, const CString& dname, const CString& descr, const CString& mySettingsFilePath);
  virtual ~SimpleServiceSettings();

public:  
  virtual CString getServiceName() const;
  virtual CString getServiceDisplayName() const;
  virtual CString getServiceDescription() const;
  virtual CString getServiceTaskSettingsPath() const;

private:
  const CString myName;
  const CString myDName;
  const CString myDescription;
  const CString mySettingsFilePath;
};



class SimpleCreateServiceSettings : public CreateServiceSettings {
public:
  SimpleCreateServiceSettings(const ServiceSettings* baseSettings, const CString& serviceCommand);
  virtual ~SimpleCreateServiceSettings();

public:
  virtual CString getServiceName() const;
  virtual CString getServiceDisplayName() const;
  virtual CString getServiceDescription() const;
  virtual CString getServiceTaskSettingsPath() const;
  virtual bool isAutostart() const;
  virtual bool runAsSystem() const;
  virtual CString getUserName() const;
  virtual CString getDomain() const;
  virtual CString getPassword() const;
  virtual CString getServiceExecutable() const;

public:
  void setUserName(const CString& name);
  void setDomain(const CString& name);
  void setPassword(const CString& pwd);
  void setAutostart(bool autostart);
  void setRunAsSystem(bool runAsSystem);

private:
  const ServiceSettings* const myBase;
  CString myServiceExecutableCommand;
  CString myUserName;
  CString myDomain;
  CString myPassword;
  bool myAutostart;
  bool myRunAsSystem;
};


class SimpleServiceTaskSettings : public ServiceTaskSettings {
public:
  SimpleServiceTaskSettings(const ServiceSettings* baseSettings);
  virtual ~SimpleServiceTaskSettings();

public:
 
  virtual CString getServiceName() const;
  virtual CString getServiceDisplayName() const;
  virtual CString getServiceDescription() const;
  virtual CString getServiceTaskSettingsPath() const;

  virtual CString getWorkDir() const;
  virtual CString getProgramPath() const;
  virtual CString getProgramArguments() const;

  virtual long getTerminateWaitTimeoutMilliseconds() const;

public:
  void setWorkDir(const CString& workdir);
  void setProgramPath(const CString& path);
  void setProgramArguments(const CString& argz);
  void setTerminateWaitTimeoutMillis(long timeout); 


private:
  const ServiceSettings* const myBase;
  CString myWorkDir;
  CString myProgramPath;
  CString myArguments;
  long myStopTimeout;
};

