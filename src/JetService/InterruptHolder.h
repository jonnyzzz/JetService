#pragma once

class InterruptHolder {
public:
  InterruptHolder();
  virtual ~InterruptHolder();

public:
  bool IsInterrupted() const;
  void Interrupt();

private:
  volatile bool myStopFlag;
};


