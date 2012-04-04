#pragma once
#include "command.h"


class LSAPolicyRight {
private:
  ACCESS_MASK myRight;
  const CString myName;
  LSAPolicyRight(CString name, ACCESS_MASK mask) : myName(name), myRight(mask) {}
  
public:
  operator ACCESS_MASK() const { return myRight; }
  operator CString() const { return myName;}

  static const LSAPolicyRight ADD_PRIVILEGE;
};


class LSAPolicyCommand : public Command
{
public:
  LSAPolicyCommand(const LSAPolicyRight& right);
  virtual ~LSAPolicyCommand();

public:
  virtual int executeCommand();
  virtual int executeCommand(LSA_HANDLE lsa) = 0;

private:
  const LSAPolicyRight& myRight;
};

