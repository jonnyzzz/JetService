#pragma once

class ServiceStatus;
class ServiceTask;

class ServiceEventContext
{
public:
  ServiceEventContext();
  virtual ~ServiceEventContext();

public:

  virtual ServiceStatus* GetServiceStatus() const = 0;
  virtual ServiceTask* GetServiceTask() const = 0;
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

private:
  ServiceStatus* myServiceStatus;
  ServiceTask* myTask;

};



