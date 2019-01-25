#pragma once

#include <stdio.h>

#define USE_DEBUG	(1)
#if USE_DEBUG

#define logErr(...) do {\
  printf("[%s:%d]", __FUNCTION__, __LINE__); \
  printf(__VA_ARGS__); \
  printf("\n"); \
} while (0)

#define logDbg(...) do {\
  printf("[%s:%d]", __FUNCTION__, __LINE__); \
  printf(__VA_ARGS__); \
  printf("\n"); \
} while (0)

#else

#define logErr(...)	
#define logDbg(...) 

#endif

class Comm
{
public:
  Comm();
  ~Comm();
  static void GetCurrentPath(CString &strPathName);
  static void InitConsoleWindow();
};
