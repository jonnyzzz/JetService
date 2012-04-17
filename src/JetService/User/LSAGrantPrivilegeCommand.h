#pragma once

#include "LSAUserCommand.h"


class LSAGrantPrivilegeCommand : public LSAUserCommand
{
public:
  LSAGrantPrivilegeCommand(const UserSettings* settings);
  virtual ~LSAGrantPrivilegeCommand();

public:
  virtual int executeCommand(LSA_HANDLE lsa, PSID sid);

protected:
  static void InitializeUnicodeString(LSA_UNICODE_STRING& x, LPWSTR value);
};

