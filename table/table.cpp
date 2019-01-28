// table.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "table.h"
#include "sky_up_info.h"
#include "exception.h"
#include "utils.h"
#include <string>
#include <cstddef>
#include <string.h>
#include <stdlib.h>

using namespace std;

#define DBG_TAG "table"

// This is the constructor of a class that has been exported.
// see table.h for the class definition
clsTable::clsTable()
{
}

clsTable::~clsTable()
{
    if (m_upInfo) {
        delete m_upInfo;
        m_upInfo = nullptr;
    }

    if (m_tsFp != NULL) {
        fclose(m_tsFp);
        m_tsFp = nullptr;
    }

    if (m_upFp != NULL) {
        fclose(m_upFp);
        m_upFp = nullptr;
    }
}

clsTable::clsTable(const clsUpInfo *info) :
    m_upInfo(nullptr), m_progress(0), m_upFp(nullptr), m_tsFp(nullptr), m_upInfoTsCnt(0), m_upDataTsCnt(0)
{
    memset(m_upInfoSection, 0, sizeof(m_upInfoSection));
    memset(m_upDataSection, 0, sizeof(m_upDataSection));

    if (!info)
        THROW(ERR_ARG);

    clsTable::~clsTable();
    m_upInfo = new clsUpInfo(*info);
    
    m_upFp = fopen(m_upInfo->inFileName().c_str(), "rb");
    if (m_upFp == NULL)
        THROW_SYSERR();

    m_tsFp = fopen(m_upInfo->outFileName().c_str(), "wb+");
    if (m_tsFp == NULL)
        THROW_SYSERR();
}

uint32_t clsTable::generateTsFile()
{
    uint32_t len = 0;

    uint32_t info_section_len = createUpInfoSection();

    uint32_t num = m_upDataSectionNum();
    uint32_t step = num / 100;

    if (step <= 0) {
        logErr("step <= 0");
    }   

    for (uint32_t i = 0; i < num; i++) {
        /* Note: 4K * 256 = 1M, every 1M insert one up info section */
        if (0 == i % 256) {
            // start section 4bytes + 0
            upInfo2Ts(info_section_len + 1);
        }

        upData2Ts(createUpDataSection(i, num - 1) + 1);

        if (0 == i % step) {
            progress(i / step);
        }
    }

    progress(100);

    return len;
}

/*
* @brief 不区分大小端，串行化处理
*/
uint32_t clsTable::createUpInfoSection()
{
    sky_up_info_section_t *p = (sky_up_info_section_t *)m_upInfoSection;
    uint32_t pos = 0;
    
    //table_id
    m_upInfoSection[pos++] = m_upInfo->infoTid();
    //len
    m_upInfoSection[pos++] = 0xff;
    m_upInfoSection[pos++] = 0xff;
    //unused1
    m_upInfoSection[pos++] = 0xff;

    // get file name from pathname
    string strFileName = m_upInfo->inFileName();
    size_t found = strFileName.find_last_of("\\");
    strncpy((char *)&m_upInfoSection[pos], strFileName.substr(found + 1).c_str(), sizeof(p->updateFileName));
    pos += sizeof(p->updateFileName);

    // get md5 
    try {
        strncpy((char *)&m_upInfoSection[pos], common::getMd5(strFileName).c_str(), sizeof(p->md5));
        //strncpy((char *)&m_upInfoSection[pos], "md5", sizeof(p->md5));
        pos += sizeof(p->md5);

        string md5((char *)p->md5, sizeof(p->md5));
        logDbg("%s", md5.c_str());
    }
    catch (exception& e) {
        logErr("get md5: %s", e.what());
        exit(-1);
    }

    strncpy((char *)&m_upInfoSection[pos], m_upInfo->manufactureId().c_str(), sizeof(p->manufactureId));
    pos += sizeof(p->manufactureId);
    strncpy((char *)&m_upInfoSection[pos], m_upInfo->hardVer().c_str(), sizeof(p->hardVer));
    pos += sizeof(p->hardVer);
    strncpy((char *)&m_upInfoSection[pos], m_upInfo->hardMod().c_str(), sizeof(p->hardMol));
    pos += sizeof(p->hardMol);
    strncpy((char *)&m_upInfoSection[pos], m_upInfo->softVer().c_str(), sizeof(p->softVer));
    pos += sizeof(p->softVer);
    strncpy((char *)&m_upInfoSection[pos], m_upInfo->softMod().c_str(), sizeof(p->softMol));
    pos += sizeof(p->softMol);
    strncpy((char *)&m_upInfoSection[pos], m_upInfo->softDependVer().c_str(), sizeof(p->softDependVer));
    pos += sizeof(p->softDependVer);
    strncpy((char *)&m_upInfoSection[pos], m_upInfo->startSN().c_str(), sizeof(p->startSN));
    pos += sizeof(p->startSN);
    strncpy((char *)&m_upInfoSection[pos], m_upInfo->endSN().c_str(), sizeof(p->endSN));
    pos += sizeof(p->endSN);
    m_upInfoSection[pos++] = ((m_upInfo->mandatory() ? 1 : 0) << 7) | 0x7f;
    m_upInfoSection[pos++] = 0xff;
	m_upInfoSection[pos++] = (m_upInfo->dataPid() >> 8) & 0xff; //0xff;
	m_upInfoSection[pos++] = m_upInfo->dataPid() & 0xff; //0xff;

    // -3（tab_id + len） + 4(crc32)
    uint8_t h = (pos - 3 + 4) >> 8;
    m_upInfoSection[1] = h & 0x0f;
    m_upInfoSection[2] = (pos - 3 + 4) & 0xff;

    uint32_t crc32 = common::getCrc32(m_upInfoSection, pos);
    //logDbg("0x%08x", crc32);
    m_upInfoSection[pos++] = (crc32 >> 24) & 0xff;
    m_upInfoSection[pos++] = (crc32 >> 16) & 0xff;
    m_upInfoSection[pos++] = (crc32 >> 8) & 0xff;
    m_upInfoSection[pos++] = crc32 & 0xff;
    
    if (common::getCrc32(m_upInfoSection, pos) != 0)
        logErr("crc32 err!");

    return pos;
}

void clsTable::upInfo2Ts(uint32_t section_len)
{
    uint8_t ts_package[188];
    uint32_t offset = 0;
    uint32_t cnt = section_len / (188 - 4);
    
    cnt += (section_len % (188 - 4) ? 1 : 0);
    --section_len;

    for (uint32_t i = 0; i < cnt; i++) {
        uint8_t pos = 0;

        ts_package[pos++] = 0x47;
        ts_package[pos++] = (i == 0 ? 0x40 : 0) | ((m_upInfo->infoPid() >> 8) & 0x1f);
        ts_package[pos++] = m_upInfo->infoPid() & 0xff;
        ts_package[pos++] = 0x10 | (m_upInfoTsCnt++ & 0x0f);

        // start section's first ts head = 4bytes + 0(1byte)
        if (i == 0)
            ts_package[pos++] = 0;

        size_t len;
        if (section_len - offset >= 188U - pos) {
            len = 188 - pos;
        }
        else {
            len = section_len - offset;
        }

        memcpy(&ts_package[pos], &m_upInfoSection[offset], len);
        pos += len;
        offset += len;

        if (pos < 188)
            memset(&ts_package[pos], 0xff, 188 - pos);

        size_t wr = fwrite(ts_package, 1, sizeof(ts_package), m_tsFp);
        if (wr != sizeof(ts_package))
            logErr("write num:%u err", wr);
    }
}

/*
* @brief 不区分大小端，串行化处理
*/
uint32_t clsTable::createUpDataSection(uint32_t cur_section, uint32_t last_section)
{
    sky_up_data_section_head_t *p = (sky_up_data_section_head_t *)m_upDataSection;
    uint32_t pos = 0;
    
    //table_id
    m_upDataSection[pos++] = m_upInfo->dataTid();
    //len
    m_upDataSection[pos++] = 0xff;
    m_upDataSection[pos++] = 0xff;
    //unused1
    m_upDataSection[pos++] = 0xff;

    m_upDataSection[pos++] = (cur_section >> 24) & 0xff;
    m_upDataSection[pos++] = (cur_section >> 16) & 0xff;
    m_upDataSection[pos++] = (cur_section >> 8) & 0xff;
    m_upDataSection[pos++] = cur_section & 0xff;

    m_upDataSection[pos++] = (last_section >> 24) & 0xff;
    m_upDataSection[pos++] = (last_section >> 16) & 0xff;
    m_upDataSection[pos++] = (last_section >> 8) & 0xff;
    m_upDataSection[pos++] = last_section & 0xff;

    size_t rdlen = fread(&m_upDataSection[pos], 1, max_section_size - pos - 4, m_upFp);

    if (rdlen != max_section_size - pos - 4)
        logDbg("read last section from file.");
    pos += rdlen;

    // -3（tab_id + len） + 4(crc32)
    uint8_t h = (pos - 3 + 4) >> 8;
    m_upDataSection[1] = h & 0x0f;
    m_upDataSection[2] = (pos - 3 + 4) & 0xff;

    uint32_t crc32 = common::getCrc32(m_upDataSection, pos);
    //logDbg("0x%08x", crc32);
    m_upDataSection[pos++] = (crc32 >> 24) & 0xff;
    m_upDataSection[pos++] = (crc32 >> 16) & 0xff;
    m_upDataSection[pos++] = (crc32 >> 8) & 0xff;
    m_upDataSection[pos++] = crc32 & 0xff;

    if (common::getCrc32(m_upDataSection, pos) != 0)
        logErr("crc32 err!");

    return pos;
}

void clsTable::upData2Ts(uint32_t section_len)
{
    uint8_t ts_package[188];
    uint32_t offset = 0;
    uint32_t cnt = section_len / (188 - 4);

    cnt += (section_len % (188 - 4) ? 1 : 0);
    --section_len;

    for (uint32_t i = 0; i < cnt; i++) {
        uint8_t pos = 0;

        ts_package[pos++] = 0x47;
        ts_package[pos++] = (i == 0 ? 0x40 : 0) | ((m_upInfo->dataPid() >> 8) & 0x1f);
        ts_package[pos++] = m_upInfo->dataPid() & 0xff;
        ts_package[pos++] = 0x10 | (m_upDataTsCnt++ & 0x0f);

        // start section's first ts head = 4bytes + 0(1byte)
        if (i == 0)
            ts_package[pos++] = 0;

        size_t len;
        if (section_len - offset >= 188U - pos) {
            len = 188 - pos;
        }
        else {
            len = section_len - offset;
        }

        memcpy(&ts_package[pos], &m_upDataSection[offset], len);
        pos += len;
        offset += len;

        if (pos < 188)
            memset(&ts_package[pos], 0xff, 188 - pos);

        size_t wr = fwrite(ts_package, 1, sizeof(ts_package), m_tsFp);
        if (wr != sizeof(ts_package))
            logErr("write num:%u err", wr);
    }
}

uint32_t clsTable::m_upDataSectionNum()
{
    int64_t len;

    try {
        len = common::getFileSize(m_upInfo->inFileName());
    }
    catch(exception& e) {
        logErr("%s", e.what());
        exit(-1);
    }

    /*crc32 = 4*/
    uint32_t num = (uint32_t)len / (max_section_size - sizeof(sky_up_data_section_head_t) - 4);
    num += (len % (max_section_size - sizeof(sky_up_data_section_head_t) - 4) ? 1 : 0);

    logDbg("up data section num:%u", num);

    return num;
}