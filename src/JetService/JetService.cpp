// JetService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Command.h"


class C2 : public Command {
public: 
  C2(Argz* a) : Command() {}
  virtual ~C2() {}
public:
  virtual int executeCommand() { return 0;}
};

int _tmain(int argc, _TCHAR* argv[])
{
  Argz az(argc, argv);

  C2 z(&az);


  wprintf(L"Started JetService\r\n");

  SC_HANDLE handler = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE);

  wprintf(L"Service openned\r\n");

  if (handler != NULL) {
    CloseServiceHandle(handler);
  }
  
	return 0;
}

