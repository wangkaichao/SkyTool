#pragma once
#pragma warning( disable: 4251 )

#include <stdint.h>
#include <string>

#ifdef TABLE_EXPORTS
#define UP_INFO_API __declspec(dllexport)
#else
#define UP_INFO_API __declspec(dllimport)
#endif

class UP_INFO_API clsUpInfo
{
public:
    clsUpInfo();
    ~clsUpInfo();
    //使用默认拷贝构造函数 不需要深度拷贝
    /*explicit clsUpInfo(const clsUpInfo &info) {
        m_strInFileName = info.m_strInFileName;
        m_strOutFileName = info.m_strOutFileName;
        m_strInfoPid = info.m_strInfoPid;
        m_strInfoTid = info.m_strInfoTid;
        m_strDataPid = info.m_strDataPid;
        m_strDataTid = info.m_strDataTid;
        m_strManufactureId = info.m_strManufactureId;
        m_strHardVer = info.m_strHardVer;
        m_strHardMod = info.m_strHardMod;
        m_strSoftVer = info.m_strSoftVer;
        m_strSoftMod = info.m_strSoftMod;
        m_strSoftDependVer = info.m_strSoftDependVer;
        m_strStartSN = info.m_strStartSN;
        m_strEndSN = info.m_strEndSN;
        m_bMandatory = info.m_bMandatory;
    }*/

    void log();

    void inFileName(std::string filename) { m_strInFileName = filename; }
    void outFileName(std::string filename) { m_strOutFileName = filename; }
    void infoPid(std::string pid) { m_strInfoPid = pid; }
    void infoPid(uint16_t pid);
    void infoTid(std::string tid) { m_strInfoTid = tid; }
    void infoTid(uint8_t tid);
    void dataPid(std::string pid) { m_strDataPid = pid; }
    void dataPid(uint16_t pid);
    void dataTid(std::string tid) { m_strDataTid = tid; }
    void dataTid(uint8_t tid);
    void manufactureId(std::string id) { m_strManufactureId = id; }
    void hardVer(std::string ver) { m_strHardVer = ver; }
    void hardMod(std::string mod) { m_strHardMod = mod; }
    void softVer(std::string ver) { m_strSoftVer = ver; }
    void softMod(std::string mod) { m_strSoftMod = mod; }
    void softDependVer(std::string depend) { m_strSoftDependVer = depend; }
    void startSN(std::string sn) { m_strStartSN = sn; }
    void endSN(std::string sn) { m_strEndSN = sn; }
    void mandatory(bool mandatory) { m_bMandatory = mandatory; }

    std::string inFileName() const { return m_strInFileName; }
    std::string outFileName() const { return m_strOutFileName; }
    std::string infoPid() const { return m_strInfoPid; }
    uint16_t infoPid() { return std::stoi(m_strInfoPid); };
    std::string infoTid() const { return m_strInfoTid; }
    uint8_t infoTid() { return std::stoi(m_strInfoTid); };
    std::string dataPid() const { return m_strDataPid; }
    uint16_t dataPid() { return std::stoi(m_strDataPid); };
    std::string dataTid() const { return m_strDataTid; }
    uint8_t dataTid() { return std::stoi(m_strDataTid); };
    std::string manufactureId() const { return m_strManufactureId; }
    std::string hardVer() const { return m_strHardVer; }
    std::string hardMod() const { return m_strHardMod; }
    std::string softVer() const { return m_strSoftVer; }
    std::string softMod() const { return m_strSoftMod; }
    std::string softDependVer() const { return m_strSoftDependVer; }
    std::string startSN() const { return m_strStartSN; }
    std::string endSN() const { return m_strEndSN; }
    bool mandatory() const { return m_bMandatory; }

private:
    std::string m_strInFileName;
    std::string m_strOutFileName;
    std::string m_strInfoPid;
    std::string m_strInfoTid;
    std::string m_strDataPid;
    std::string m_strDataTid;
    std::string m_strManufactureId;
    std::string m_strHardVer;
    std::string m_strHardMod;
    std::string m_strSoftVer;
    std::string m_strSoftMod;
    std::string m_strSoftDependVer;
    std::string m_strStartSN;
    std::string m_strEndSN;
    bool   m_bMandatory;
};

