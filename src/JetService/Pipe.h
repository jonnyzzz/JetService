#pragma once


class Pipe
{
public:
  Pipe();
  virtual ~Pipe(void);

private:
  Pipe(const Pipe& pipe);

public:
  bool IsValid() const;

protected:
  HANDLE GetReadHandle() const;
  HANDLE GetWriteHandle() const; 

  void MakeInvalid();
  void MakeInheritable(HANDLE handle);

  void SafeCloseHandle(HANDLE h);

private:
  static void SafeCloseHandleImpl(HANDLE& h);
  

private:
  HANDLE myReadHandle;
  HANDLE myWriteHandle;
  bool myIsValid;
};

class ChildProcessHandle : public Pipe {
public:
  ChildProcessHandle();
  virtual ~ChildProcessHandle();

public:
  HANDLE GetChildProcessHandle();
  HANDLE GetHostProcessHandle();

protected:
  virtual HANDLE GetChildProcessHandleImpl() const = 0;
  virtual HANDLE GetHostProcessHandleImpl() const = 0;

public:
  void CloseChildProcessHandle();
  void CloseHostProcessHandle();

};


///represents child process stdout stderr handle
///thus current process is about to _read_ from it.
class ChildProcessOutHandle : public ChildProcessHandle {
public:
  ChildProcessOutHandle();
  virtual ~ChildProcessOutHandle();

public:
  virtual HANDLE GetChildProcessHandleImpl() const;
  virtual HANDLE GetHostProcessHandleImpl() const;

};


///represents child process stdin handle
///thus current process is about to _write_ to it.
class ChildProcessInHandle : public ChildProcessHandle {
public:
  ChildProcessInHandle();
  virtual ~ChildProcessInHandle();

protected:
  virtual HANDLE GetChildProcessHandleImpl() const;
  virtual HANDLE GetHostProcessHandleImpl() const;

};

