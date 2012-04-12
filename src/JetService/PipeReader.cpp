#include "StdAfx.h"
#include "PipeReader.h"

#include "Logger.h"

const Logger LOG(L"PipeReader");


ReaderCallback::ReaderCallback() {
}

ReaderCallback::~ReaderCallback() {
}

/////

DWORD WINAPI PipeReaderThreadProc(LPVOID lpParameter) {
  return ((PipeReader*)lpParameter)->ThreadProc();  
}

PipeReader::PipeReader(ReaderCallback* readerCallback, HANDLE pipe, InterruptHolder* interrupt)
  : InterruptHolder(interrupt)
  , myReaderCallback(readerCallback)
  , myPipe(pipe)  
  , myThread(NULL)
  , myThreadId(0)
{
  myThread = CreateThread(NULL, 0, &PipeReaderThreadProc, this, 0, &myThreadId);
  if (myThread == NULL) {
    LOG.LogErrorFormat(L"Failed to start pipe read thread. %s", LOG.GetLastError());    
  }
}

PipeReader::~PipeReader(void)
{
  LOG.LogDebug(L"PipeReader::~PipeReader => inheritor object is disposed!");
  WaitForExit();
}

void PipeReader::WaitForExit() {
  if (myThread == NULL) return;
  Interrupt();
  if (WAIT_OBJECT_0 == WaitForSingleObject(myThread, 500)) {
    //Thread has gone
    return;
  }
  //Thread may have beed locked on IO
  CancelIo(myPipe);
  CancelSynchronousIo(myThread);
  if (WAIT_OBJECT_0 != WaitForSingleObject(myThread, 1500)) {
    LOG.LogWarn(L"Failed to wait for pipe reader thread to exit");
  }
}

DWORD PipeReader::ThreadProc() {
  if (IsInterrupted()) return 0;

  const DWORD SZ = 65536;  
  char buff[SZ+1];
  TCHAR tbuff[2*SZ+4];

  CString buffer;
  while (!IsInterrupted()) {
    Sleep(100);
    DWORD buffSize = 0;
    if (0 == PeekNamedPipe(myPipe, NULL, 0, NULL, &buffSize, NULL)) {
      LOG.LogDebugFormat(L"Failed to read from pipe. %s, interrupted=%s", LOG.GetLastError(), IsInterruptedString());
      return 1;
    }

    if (buffSize <= 0) continue; //nothig to read now. Will try avoid blocking
    buffSize = min(SZ, buffSize);
    
    if (0 == ReadFile(myPipe, buff, buffSize, &buffSize, NULL)) {
      //ERROR_IO_PENDING does not mean a error
      //ERROR_MORE_DATA mans we fetch the rest via PeekNamedPipe on next iteration
      if (GetLastError() != ERROR_IO_PENDING && GetLastError() != ERROR_MORE_DATA)  {
        LOG.LogDebugFormat(L"Failed to read from pipe. %s, interrupted=%s", LOG.GetLastError(), IsInterruptedString());
        return 1;
      }
    }

    if (buffSize <= 0) {
      LOG.LogDebug(L"Nothing is read from pipe. EOF detected.");
      return 0;
    }
    
    //NOTE: here could be an issue with UTF-8 symbols that encoded with more than 1 byte
    //NOTE: such symbols may not be fully read from the pipe and thus may not 
    //NOTE: by coverted or even lead the function to fail.
    int n = MultiByteToWideChar(GetConsoleOutputCP(), 0, buff, buffSize, tbuff, sizeof(tbuff));
    if (n == 0) {
      LOG.LogWarnFormat(L"Failed to read process output text. %s", LOG.GetLastError());
      buffer.Append(L"?");
    } else {      
      buffer.Append(tbuff, n);
    }

    ProcessBuffer(buffer);
  }

  return 0;
}


void PipeReader::ProcessNextPortion(const CString& text) {
  myReaderCallback->ProcessNextPortion(text);
}

void PipeReader::ProcessBuffer(CString& buffer) {
  bool processed = true;
  while(processed && buffer.GetLength() > 0) {
    processed = false;

    int newLine = buffer.FindOneOf(L"\n");
    if (newLine >= 0) {
      CString sub = buffer.Left(newLine).TrimRight();
      ProcessNextPortion(sub);
      buffer.Delete(0, newLine+1);
      processed = true;
    } else if (buffer.GetLength() > 4096) {
      ProcessNextPortion(buffer);
      buffer.Delete(0, buffer.GetLength());
      processed = true;
    }
  }
}


