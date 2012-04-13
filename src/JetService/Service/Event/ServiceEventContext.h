#pragma once

class ServiceStatus;
class ServiceTask;
class ServiceSettings;

class ServiceEventContext
{
public:
  ServiceEventContext();
  virtual ~ServiceEventContext();

public:

  virtual ServiceStatus* GetServiceStatus() const = 0;
  virtual ServiceTask* GetServiceTask() const = 0;
  virtual const ServiceSettings* GetServiceSettings() const = 0;
  bool IsValid() const;

};


class ServiceEventContextEx : public ServiceEventContext {
public: 
  ServiceEventContextEx();
  virtual ~ServiceEventContextEx();

public:
  virtual ServiceStatus* GetServiceStatus() const;
  void SetServiceStatus(ServiceStatus* status);

  virtual ServiceTask* GetServiceTask() const;
  void SetServiceTask(ServiceTask* task);

  virtual const ServiceSettings* GetServiceSettings() const;
  void SetServiceSettings(const ServiceSettings* settings);

private:
  const ServiceSettings* mySettings;
  ServiceStatus* myServiceStatus;
  ServiceTask* myTask;

};



