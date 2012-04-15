#pragma once;

#include "InstanceCount.h"

template<typename For, int N>
struct ConsoleControlHandler {
  static For* myAction;
  static bool SetAction(int n, For* action) {
    if (n == N) myAction = action;
    if (n > N) return false;
    return ConsoleControlHandler<For,N-1>::SetAction(n, action);
  }

  static PHANDLER_ROUTINE GetFunction(int n) {
    if (n == N) return &ConsoleControlHandler_HandlerRoutine<For,N>;
    if (n > N) return NULL;
    return ConsoleControlHandler<For, N-1>::GetFunction(n);
  }
};

template<typename For>
struct ConsoleControlHandler<For, 0> {
  static bool SetAction(int n, For* action) { return false; }
  static PHANDLER_ROUTINE GetFunction(int n) { return NULL; }
};

template<typename For, int N>
For* ConsoleControlHandler<For, N>::myAction = NULL;

template<typename For, int N>
BOOL WINAPI ConsoleControlHandler_HandlerRoutine(DWORD dwCtrlType) {
  return ConsoleControlHandler<For,N>::myAction->ConsoleControlHandler(dwCtrlType);
}

template<typename For>
class DelegateConsoleControlHandler : private InstanceCount<For> {
private:
  typedef ConsoleControlHandler<For, 8> Handler;
public:
  PHANDLER_ROUTINE GetConsoleControlHandlerFunction() {    
    Handler::SetAction(GetInstanceId(), static_cast<For*>(this));
    return Handler::GetFunction(GetInstanceId());
  }
};
