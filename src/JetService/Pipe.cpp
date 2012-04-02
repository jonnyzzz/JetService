#include "StdAfx.h"
#include "Pipe.h"
#include "Logger.h"

const Logger LOG(L"Pipe");

Pipe::Pipe(void)
  : myReadHandle(NULL)
  , myWriteHandle(NULL)
{
  SECURITY_ATTRIBUTES attrs;
  ZeroMemory(&attrs, sizeof(attrs));
  attrs.nLength = sizeof(attrs);
  attrs.bInheritHandle = TRUE;
  attrs.lpSecurityDescriptor = NULL;
  
  const DWORD BUFF_SZ = 0; //let OS decide 
  if (0 == CreatePipe(&myReadHandle, &myWriteHandle, &attrs, BUFF_SZ)) {
    LOG.LogErrorFormat(L"Failed to create pipe. %s", LOG.GetLastError());
    myReadHandle = NULL;
    myWriteHandle = NULL;
    myIsValid = false;
    return;
  } 
  myIsValid = DuplicateHandleNonInheritable(myReadHandle) && DuplicateHandleNonInheritable(myWriteHandle);  
}

bool Pipe::DuplicateHandleNonInheritable(HANDLE& h) {
  HANDLE process = GetCurrentProcess();
  if (0 == DuplicateHandle(process, h, process, &h, 0, FALSE, DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE)) {   
    LOG.LogError(L"Fail to duplicate handle");
    SafeCloseHandleImpl(h);    
    return false;
  }
  return true;
}

Pipe::Pipe(const Pipe& pipe) {
  LOG.LogError(L"Pipe objects should not be copied via copy contructor");
}

Pipe::~Pipe(void)
{
  SafeCloseHandleImpl(myWriteHandle);
  SafeCloseHandleImpl(myReadHandle);
}

void Pipe::SafeCloseHandle(HANDLE handle) {
  if (handle == myReadHandle) {
    SafeCloseHandleImpl(myReadHandle);
    return;
  }
  if (handle == myWriteHandle) {
    SafeCloseHandleImpl(myWriteHandle);
    return;
  }
  LOG.LogWarn(L"Unknown handle");
  SafeCloseHandleImpl(handle);
}

void Pipe::SafeCloseHandleImpl(HANDLE& handle) {
  if (handle != NULL) {
    if (0 == CloseHandle(handle)) {
      LOG.LogWarn(L"Failed to close handle");
    }
    handle = NULL;
  }
}

bool Pipe::IsValid() const {
  return myIsValid;
}

void Pipe::MakeInvalid() {
  myIsValid = false;
}

HANDLE Pipe::GetReadHandle() const {
  return myReadHandle;
}

HANDLE Pipe::GetWriteHandle() const {
  return myWriteHandle;
}

void Pipe::MakeInheritable(HANDLE handle) {
  if (!IsValid()) return;
  if (0 == SetHandleInformation(handle, HANDLE_FLAG_INHERIT, TRUE)) {
    LOG.LogErrorFormat(L"Failed to set HANDLE_FLAG_INHERIT. %s", LOG.GetLastError());
    MakeInvalid();
  }
}

//////

ChildProcessHandle::ChildProcessHandle() {
}

ChildProcessHandle::~ChildProcessHandle() {
}

HANDLE ChildProcessHandle::GetChildProcessHandle() {
  HANDLE h = GetChildProcessHandleImpl();
  MakeInheritable(h);
  return h;
}

HANDLE ChildProcessHandle::GetHostProcessHandle() {
  //TODO: make non-inheritable
  HANDLE h = GetHostProcessHandleImpl();  
  return h;
}

void ChildProcessHandle::CloseChildProcessHandle() {
  SafeCloseHandle(GetChildProcessHandle());
}

void ChildProcessHandle::CloseHostProcessHandle() {
  SafeCloseHandle(GetHostProcessHandle());
}


//////

ChildProcessOutHandle::ChildProcessOutHandle() {  
}

ChildProcessOutHandle::~ChildProcessOutHandle() {
}

HANDLE ChildProcessOutHandle::GetChildProcessHandleImpl() const {
  return GetWriteHandle();
}

HANDLE ChildProcessOutHandle::GetHostProcessHandleImpl() const {
  return GetReadHandle();
}


///////

ChildProcessInHandle::ChildProcessInHandle() { 
}

ChildProcessInHandle::~ChildProcessInHandle() {
}

HANDLE ChildProcessInHandle::GetChildProcessHandleImpl() const {
  return GetReadHandle();
}

HANDLE ChildProcessInHandle::GetHostProcessHandleImpl() const {
  return GetWriteHandle();
}

