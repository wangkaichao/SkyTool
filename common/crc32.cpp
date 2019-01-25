#include "stdafx.h"
#include "crc32.h"

clsCrc32::clsCrc32()
{
    uint16_t i, j;

    uint32_t crc_accum;

    for (i = 0; i < 256; i++) {
        crc_accum = (i << 24);

        for (j = 0; j < 8; j++) {
            if (crc_accum & 0x80000000L)
                crc_accum = (crc_accum << 1) ^ POLYNOMIAL;
            else
                crc_accum = (crc_accum << 1);
        }

        m_table[i] = crc_accum;
    }
}

clsCrc32::~clsCrc32() {}

uint32_t clsCrc32::getCrc32(uint8_t *buf, int32_t len)
{
    uint32_t crc = 0xffffffff;

    while (len--) {
        crc = (crc << 8) ^ m_table[((crc >> 24) ^ *buf++) & 0xff];
    }

    return crc;
}