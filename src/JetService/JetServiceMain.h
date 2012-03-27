#pragma once;

class ServiceAction;
class ServiceSettings;
class Argz;

template<int N> 
VOID WINAPI JetServiceMain(DWORD dwArgc, LPTSTR *lpszArgv) {
  JetServiceState<N>::myState->JetServiceMain(JetServiceState<N>::myParam1, JetServiceState<N>::myParam2, dwArgc, lpszArgv);  
}

template<int N> 
struct JetServiceState {
  static ServiceAction* myState;
  static const Argz* myParam1;
  static const ServiceSettings* myParam2;

  static bool setState(int n, ServiceAction* state, const Argz* param1, const ServiceSettings* param2) {
    if (n > N) return false;
    if (N == n) {
      myState = state;
      myParam1 = param1;
      myParam2 = param2;
      return true;
    }
    return JetServiceState<N-1>::setState(n, state, param1, param2);
  }
  static LPSERVICE_MAIN_FUNCTION getFunction(int n) {
    if (n > N) return NULL;
    if (N == n) return &JetServiceMain<N>;          
    return JetServiceState<N-1>::getFunction(n);
  }
};

template<int N>
ServiceAction* JetServiceState<N>::myState = NULL;
template<int N>

const Argz* JetServiceState<N>::myParam1 = NULL;
template<int N>
const ServiceSettings* JetServiceState<N>::myParam2 = NULL;

template<>
struct JetServiceState<0> { 
  static LPSERVICE_MAIN_FUNCTION getFunction(int n) { return NULL; }
  static bool setState(int n, ServiceAction* state, const Argz* param1, const ServiceSettings* param2) { return false; }
};

