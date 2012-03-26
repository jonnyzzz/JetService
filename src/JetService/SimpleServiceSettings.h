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

