#pragma once

template<typename For>
class InstanceCount
{
public:
  InstanceCount(void) : myId(++myLastId) {}
  virtual ~InstanceCount(void) {}

public:
  int GetInstanceId() { return myId; }

private:
  static int myLastId;
  const int myId;
};

template<typename For>
int InstanceCount<For>::myLastId = 0;

