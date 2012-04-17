#pragma once

#include "Command.h"
#include "ServiceSettings.h"

class LogonUserMode {
private:
  const DWORD myMode;
  const CString myName;
  LogonUserMode(CString name, DWORD mode) : myName(name), myMode(mode) {}
public:
  operator DWORD() const { return myMode; }
  operator const CString&() const { return myName; }

  static const LogonUserMode NETWORK;
  static const LogonUserMode AS_SERVICE;
};


class LogonUserCommand : public Command
{
public:
  LogonUserCommand(const UserSettings* settings, const LogonUserMode& mode);
  virtual ~LogonUserCommand();

public:
  virtual int executeCommand();
  virtual int executeCommand(HANDLE userToken) = 0;

private:
  const UserSettings* const mySettings;
  const LogonUserMode& myMode;
};

