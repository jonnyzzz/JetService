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

  virtual int executeCommnad(const CreateServiceSettings* settings);
  virtual int executeCommnad(const ServiceSettings* settings);

private:
  CString loadSettingsXml();

private:
  const CString myConfigFile;
};

