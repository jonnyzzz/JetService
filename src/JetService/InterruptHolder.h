#pragma once

class InterruptHolder {
public:
  InterruptHolder(const InterruptHolder* parent = NULL);
  virtual ~InterruptHolder();

public:
  bool IsInterrupted() const;
  void Interrupt();
  CString IsInterruptedString() const;

private:
  const InterruptHolder* const myParent;
  volatile bool myStopFlag;
};


