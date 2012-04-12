#pragma once

class InterruptHolder {
public:
  InterruptHolder(InterruptHolder* parent = NULL);
  virtual ~InterruptHolder();

public:
  bool IsInterrupted() const;
  void Interrupt();
  CString IsInterruptedString() const;

private:
  InterruptHolder* const myParent;
  volatile bool myStopFlag;
};


