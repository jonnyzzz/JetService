#pragma once
#include "Argz.h"

class Command {
public:
  Command();
  virtual ~Command();

public:
  virtual int executeCommand() = 0;

};

