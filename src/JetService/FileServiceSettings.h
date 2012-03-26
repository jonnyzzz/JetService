#pragma once

#include "Command.h"
#include "ServiceSettings.h"
#include "CreateServiceCommand.h"

class FileServiceSettings : public Command
{
public:
  FileServiceSettings(const CString& configFilePath);
  virtual ~FileServiceSettings(void);

public:
  virtual int executeCommand();

  
  virtual int executeCommand(const ServiceSettings* settings) = 0;

private:
  CString loadSettingsXml();

private:
  const CString myConfigFile;
};

