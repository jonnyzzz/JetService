#pragma once


class StatusValue {
public:
  StatusValue(const CString& name, DWORD state, bool isFinal = false, DWORD waitHint = -1);  
  virtual ~StatusValue();

public:
  const CString& GetName() const;
  DWORD GetState() const;  
  DWORD GetWaitHint() const;
  bool IsRepeating() const;
  bool IsFinalEvent() const;

private:
  const CString myName;
  const DWORD myState;
  const DWORD myWaitHint;
  const bool myIsFinal;

public:
  static const StatusValue* STARTING;
  static const StatusValue* RUNNING;
  static const StatusValue* STOPPING;
  static const StatusValue* STOPPED;

  //TODO: And currently not supported events:
  // SERVICE_CONTINUE_PENDING
  // SERCICE_PAUSE_PENDING
  // SERCICE_PAUSED  
};

