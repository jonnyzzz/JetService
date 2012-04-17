#pragma once
#include "CreateServiceSettingsAction.h"


class CreateServiceCheckAccountAction : public CreateServiceSettingsAction
{
public:
  CreateServiceCheckAccountAction(const CString& commandName);
  virtual ~CreateServiceCheckAccountAction(void);

public:
  virtual int ExecuteAction(const Argz* argz, const CreateServiceSettings* settings, const ServiceTaskSettings* task);

private:
  int TryAddExecuteAsService(const Argz* argz, const CreateServiceSettings* settings);

protected:
  virtual CString GetUsageString();
  
private:
  static const CString KEY_CHECK_USER_ACCOUNT;
  static const CString KEY_GIVE_USER_LOGON;
};

