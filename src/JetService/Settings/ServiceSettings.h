#pragma once

class ServiceSettings {
public:
  virtual ~ServiceSettings() {}

public:
  virtual CString getServiceName() const = 0;
  virtual CString getServiceDisplayName() const = 0;
  virtual CString getServiceDescription() const = 0;
  
  virtual CString getServiceTaskSettingsPath() const = 0;
};

class UserSettings {
public:
  virtual ~UserSettings() {};

public:
  virtual CString getUserName() const = 0;
  virtual CString getDomain() const = 0;
  virtual CString getPassword() const = 0;
};  

class CreateServiceSettings : public ServiceSettings, public UserSettings {
public:
  virtual ~CreateServiceSettings() {}
public:
  virtual bool isAutostart() const = 0;
  virtual bool runAsSystem() const = 0;  
  virtual CString getServiceExecutable() const = 0;
};

class ExecutionSettings {
public:
  virtual ~ExecutionSettings() {}

public:
  virtual CString getWorkDir() const = 0;
  virtual CString getProgramPath() const = 0;
  virtual CString getProgramArguments() const = 0;
};


class ServiceTaskSettings : public ServiceSettings, public ExecutionSettings {
public:
  virtual ~ServiceTaskSettings() {}

public:
  virtual long getTerminateWaitTimeoutMilliseconds() const = 0;
  virtual const ExecutionSettings* getStopCommand() const = 0;

  ///retuns names of Environment variables that must be reset to system-wide value
  ///due to TW-21240 and kb821761 we likely to override those variables explicitly
  virtual const std::vector<CString>& getEnvironmentVariablesToOverride() const = 0;
};


