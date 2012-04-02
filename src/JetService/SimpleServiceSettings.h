#pragma once

#include "ServiceSettings.h"

class SimpleServiceSettings : public ServiceSettings
{
public:
  SimpleServiceSettings(const CString& name, const CString& dname, const CString& descr);
  virtual ~SimpleServiceSettings();

public:  
  virtual CString getServiceName() const;
  virtual CString getServiceDisplayName() const;
  virtual CString getServiceDescription() const;

private:
  const CString myName;
  const CString myDName;
  const CString myDescription;
};



class SimpleCreateServiceSettings : public CreateServiceSettings {
public:
  SimpleCreateServiceSettings(const RunServiceSettings* baseSettings, const CString& serviceCommand);
  virtual ~SimpleCreateServiceSettings();

public:
  virtual CString getServiceName() const;
  virtual CString getServiceDisplayName() const;
  virtual CString getServiceDescription() const;
  virtual CString getServiceTaskSettingsPath() const;
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
  const RunServiceSettings* myBase;
  const CString myServiceExecutableCommand;
  CString myUserName;
  CString myPassword;
  bool myAutostart;
  bool myRunAsSystem;
};


class SimpleRunServiceSettings : public RunServiceSettings {
public:
  SimpleRunServiceSettings(const ServiceSettings* baseSettings, const CString& taskPath);
  virtual ~SimpleRunServiceSettings();
public:
  virtual CString getServiceName() const;
  virtual CString getServiceDisplayName() const;
  virtual CString getServiceDescription() const;
  virtual CString getServiceTaskSettingsPath() const;
private:
  const ServiceSettings* const myBase;
  const CString myTaskPath;
};



class SimpleServiceTaskSettings : public ServiceTaskSettings {
public:
  SimpleServiceTaskSettings(const RunServiceSettings* baseSettings, 
                            const CString& workDir, 
                            const CString& programPath, 
                            const CString& arguments);
  virtual ~SimpleServiceTaskSettings();

public:
 
  virtual CString getServiceName() const;
  virtual CString getServiceDisplayName() const;
  virtual CString getServiceDescription() const;
  virtual CString getServiceTaskSettingsPath() const;

  virtual CString getWorkDir() const;
  virtual CString getProgramPath() const;
  virtual CString GetProgramArguments() const;

private:
  const RunServiceSettings* const myBase;
  const CString myWorkDir;
  const CString myProgramPath;
  const CString myArguments;
};

