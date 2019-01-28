#include <stdint.h>
#include <stdio.h>

#ifdef TABLE_EXPORTS
#define TABLE_API __declspec(dllexport)
#else
#define TABLE_API __declspec(dllimport)
#endif

class clsUpInfo;

// This class is exported from the table.dll
class TABLE_API clsTable {

public:
    static const uint8_t ts_sync_byte = 0x47;
    static const uint32_t max_section_size = 1 << 12;

    /*
    * @brief 不区分大小端，串行化处理， 前3个字节是PSI标准，固定
    */
    typedef struct {
        uint32_t table_id : 8;            ///< 0
        uint32_t section_len : 16;        ///< 1,2
        uint32_t unused1 : 8;             ///< 3
        uint8_t updateFileName[128];      ///< 4-131
        uint8_t md5[32];                  ///< 132-163
        uint8_t manufactureId[16];        ///< 164-179
        uint8_t hardVer[16];              ///< 180-195
        uint8_t hardMol[16];              ///< 196-211
        uint8_t softVer[16];              ///< 212-227
        uint8_t softMol[16];              ///< 228-243
        uint8_t softDependVer[16];        ///< 244-259
        uint8_t startSN[32];              ///< 260-291
        uint8_t endSN[32];                ///< 292-323
        uint32_t mandatory : 1;           ///< 324-327
        uint32_t unused2 : 15;
		uint32_t upPid : 16;
        uint32_t crc32;                   ///< 328-331
    } sky_up_info_section_t;

    /*
    * @brief 不区分大小端，串行化处理，前3个字节是PSI标准，固定
    */
	typedef struct {
		uint32_t table_id : 8;              ///< 0
        uint32_t section_len : 16;          ///< 1,2
        uint32_t unused1 : 8;               ///< 3
		uint32_t section_num;               ///< 4-7
		uint32_t last_section_num;          ///< 8-11
	} sky_up_data_section_head_t;

public:
    // TODO: add your methods here.
    ~clsTable();
    clsTable(const clsUpInfo *info);

    uint32_t generateTsFile();

    // max progress is 100
    uint32_t progress() const { return m_progress; }

private:
    clsTable(void);
    void progress(uint32_t pos) { m_progress = pos; }

    uint32_t createUpInfoSection();
    void upInfo2Ts(uint32_t section_len);
    uint32_t createUpDataSection(uint32_t cur_section, uint32_t last_section);
    void upData2Ts(uint32_t section_len);
    uint32_t m_upDataSectionNum();

    // ui data -> clsUpInfo
    clsUpInfo *m_upInfo;

    // contrl ts file write
    FILE *m_tsFp;
    FILE *m_upFp;

    uint32_t m_progress;
    
    // upgrade info section 
    uint8_t m_upInfoSection[sizeof(sky_up_info_section_t)];
    uint64_t m_upInfoTsCnt;

    // upgrade data section
    uint8_t m_upDataSection[max_section_size];
    uint64_t m_upDataTsCnt;
};
