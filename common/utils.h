#pragma once

#include <stdint.h>
#include <stdio.h>
#include <string>

#define logErr(...) do {				\
    printf("[%s %s:%d]", DBG_TAG, __FUNCTION__, __LINE__);            \
    printf(__VA_ARGS__);				\
    printf("\n");					\
    } while(0)

#define logDbg(...) do {				\
    printf("[%s %s:%d]", DBG_TAG, __FUNCTION__, __LINE__);            \
    printf(__VA_ARGS__);				\
    printf("\n");					\
    } while(0)

namespace common {
  /*
   * caculate md5
   */
    std::string getMd5(const std::string &strFileName);

    uint32_t getCrc32(uint8_t *buf, int32_t len);

    int64_t getFileSize(const std::string &strFileName);
}
