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

class CreateServiceSettings : public ServiceSettings {
public:
  virtual ~CreateServiceSettings() {}
public:
  virtual bool isAutostart() const = 0;
  virtual bool runAsSystem() const = 0;
  virtual CString getUserName() const = 0;
  virtual CString getPassword() const = 0;
  virtual CString getServiceExecutable() const = 0;
};


class ServiceTaskSettings : public ServiceSettings {
public:
  virtual ~ServiceTaskSettings() {}

public:

  virtual CString getWorkDir() const = 0;
  virtual CString getProgramPath() const = 0;
  virtual CString GetProgramArguments() const = 0;
};

