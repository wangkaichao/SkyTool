#include "stdafx.h"
#include "Comm.h"
#include <io.h>
#include <Fcntl.h>

Comm::Comm()
{
}

Comm::~Comm()
{
}

void Comm::GetCurrentPath(CString &strPathName)
{
  GetModuleFileName(NULL, strPathName.GetBuffer(256), 256);
  strPathName.ReleaseBuffer(256);
  int nPos = strPathName.ReverseFind('\\');
  strPathName = strPathName.Left(nPos + 1);
}

void Comm::InitConsoleWindow()
{
  AllocConsole();
  HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
  int hCrt = _open_osfhandle((long)handle, _O_TEXT);
  FILE * hf = _fdopen(hCrt, "w");
  *stdout = *hf;
}

	