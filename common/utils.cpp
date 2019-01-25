#include "stdafx.h"
#include "utils.h"
#include "exception.h"
#include "md5.hh"
#include "crc32.h"
#include <string>
#include <stdio.h>

using namespace std;
#define DBG_TAG "utils"

string common::getMd5(const string &strFileName)
{
    if (strFileName.empty())
        THROW(ERR_ARG);
  
    // win must use rb mode
    FILE *fp = fopen(strFileName.c_str(), "rb");
    if (fp == NULL)
        THROW_SYSERR();

    MD5 md5;
    md5.update(fp);
    md5.finalize();

    char* ptr = md5.hex_digest();
    string result = ptr;
    
    delete[] ptr;
    return result;
}

uint32_t common::getCrc32(uint8_t *buf, int32_t len)
{
    return clsCrc32().getCrc32(buf, len);
}

int64_t common::getFileSize(const string &strFileName)
{
    uint64_t len;

    // win must use rb mode
    FILE *fp = fopen(strFileName.c_str(), "rb");
    if (fp == NULL)
        THROW_SYSERR();

    fseek(fp, 0L, SEEK_END);
    len = ftell(fp);
    fclose(fp);

    return len;
}