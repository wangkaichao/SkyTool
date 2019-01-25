#pragma once

#include <stdint.h>

class clsCrc32 {
public:
    enum { POLYNOMIAL = 0x04c11db7 };

    clsCrc32();
    ~clsCrc32();

    uint32_t getCrc32(uint8_t *buf, int32_t len);

private:
    uint32_t  m_table[256];
};