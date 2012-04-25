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

class SimpleUserSettings : public UserSettings {
public:
  SimpleUserSettings();
  virtual ~SimpleUserSettings();

public:
  virtual CString getUserName() const;
  virtual CString getDomain() const;
  virtual CString getPassword() const;

public:
  void setUserName(const CString& name);
  void setDomain(const CString& name);
  void setPassword(const CString& pwd);

private:
  CString myUserName;
  CString myDomain;
  CString myPassword;
};


class SimpleCreateServiceSettings : public CreateServiceSettings, public SimpleUserSettings {
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
  void setAutostart(bool autostart);
  void setRunAsSystem(bool runAsSystem);

private:
  const ServiceSettings* const myBase;
  CString myServiceExecutableCommand;
  bool myAutostart;
  bool myRunAsSystem;
};

class SimpleExecutionSettings : public ExecutionSettings {
public:
  SimpleExecutionSettings();  
  virtual ~SimpleExecutionSettings();

public:
  virtual CString getWorkDir() const;
  virtual CString getProgramPath() const;
  virtual CString getProgramArguments() const;

public:
  void setWorkDir(const CString& workdir);
  void setProgramPath(const CString& path);
  void setProgramArguments(const CString& argz);

private:
  CString myWorkDir;
  CString myProgramPath;
  CString myArguments;
};

class SimpleServiceTaskSettings : public ServiceTaskSettings, public SimpleExecutionSettings {
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
  virtual const ExecutionSettings* getStopCommand() const;

  virtual const std::vector<CString>& getEnvironmentVariablesToOverride() const;

public:
  void setTerminateWaitTimeoutMillis(long timeout); 
  void setStopCommand(const ExecutionSettings* command);
  void addEnvironmentVariableToOverride(const CString& name);

private:
  const ServiceSettings* const myBase;
  SimpleExecutionSettings myStopCommand;
  bool myUseStopCommand;
  long myStopTimeout;
  std::vector<CString> myEnvironmentToOverride;
};

