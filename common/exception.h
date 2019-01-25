//-*-c++-*-
#pragma once
#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <string>
#include <stdexcept>
#include <string.h>

#define THROW(err) do {					\
    throw std::runtime_error(err.err_string.c_str());	\
  } while(0)

typedef struct {
  int err_code;
  std::string err_string;
}_exception_t;

#define THROW_SYSERR() do {			\
    char syserr[128];				\
    strerror_s(syserr, 128, errno);		\
    throw std::runtime_error(syserr);		\
  } while(0)


#define DEFINE_ERROR(name, code, str) static _exception_t name = {code, str};
DEFINE_ERROR(ERR_ARG, 1, "argument invalid")
DEFINE_ERROR(ERR_ADDR, 2, "fopen failed")
DEFINE_ERROR(ERR_MEM, 3, "memory invalid")
DEFINE_ERROR(ERR_EVADD, 4, "add event failed")
DEFINE_ERROR(ERR_EVBASE, 5, "create event base failed")
DEFINE_ERROR(ERR_BUSY, 6, "resource busy")
DEFINE_ERROR(ERR_CONNECT, 7, "connect failed")
DEFINE_ERROR(ERR_TIMEOUT, 8, "timeout")
DEFINE_ERROR(ERR_ACCEPT, 9, "accept error");
DEFINE_ERROR(ERR_FOPEN, 10, "open file failed");
DEFINE_ERROR(ERR_INVALID, 11, "resource invalid");
DEFINE_ERROR(ERR_EPIPE, 12, "broken pipe");
DEFINE_ERROR(ERR_DENY, 13, "permission deny");

#endif
