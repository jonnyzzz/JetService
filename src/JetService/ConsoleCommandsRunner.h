#pragma once

#include "Argz.h"
#include "ConsoleWriter.h"
#include "ConsoleAction.h"

class ConsoleCommandsRunner
{
public:
  ConsoleCommandsRunner(ConsoleWriter* writer, Argz* argz, ConsoleAction** commands);
  virtual ~ConsoleCommandsRunner();

public:

  int executeCommand();
  int usage();

private:
  ConsoleWriter* const myWriter;
  Argz* const myArgz;
  ConsoleAction** myActions; //NULL terminated array
};

