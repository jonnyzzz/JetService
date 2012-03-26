#pragma once

class ServiceSettings {
public:
  virtual ~ServiceSettings() {}

public:
  virtual CString getServiceName() const = 0;
  virtual CString getServiceDisplayName() const = 0;
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
