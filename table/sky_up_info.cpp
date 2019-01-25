#include "stdafx.h"
#include "sky_up_info.h"
#include <iostream>
#include <stdio.h>

using namespace std;

clsUpInfo::clsUpInfo()
{
}

clsUpInfo::~clsUpInfo()
{
}


void clsUpInfo::infoPid(UINT16 pid)
{
    char buf[16];
    sprintf(buf, "%d", pid);

    m_strInfoPid = buf; 
}
void clsUpInfo::infoTid(uint8_t tid) 
{
    char buf[16];
    sprintf(buf, "%d", tid);
    m_strInfoTid = buf;
}

void clsUpInfo::dataPid(UINT16 pid)
{
    char buf[16];
    sprintf(buf, "%d", pid);
    m_strDataPid = buf;
}

void clsUpInfo::dataTid(uint8_t tid)
{
    char buf[16];
    sprintf(buf, "%d", tid);
    m_strDataTid = buf;
}

void clsUpInfo::log()
{
    cout << "{" << endl;
    cout << "\t m_strInFileName" << " : " << m_strInFileName << endl;
    cout << "\t m_strOutFileName" << " : " << m_strOutFileName << endl;
    cout << "\t m_strInfoPid" << " : " << m_strInfoPid << endl;
    cout << "\t m_strInfoTid" << " : " << m_strInfoTid << endl;
    cout << "\t m_strDataPid" << " : " << m_strDataPid << endl;
    cout << "\t m_strDataTid" << " : " << m_strDataTid << endl;
    cout << "\t m_strManufactureId" << " : " << m_strManufactureId << endl;
    cout << "\t m_strHardVer" << " : " << m_strHardVer << endl;
    cout << "\t m_strHardMod" << " : " << m_strHardMod << endl;
    cout << "\t m_strSoftVer" << " : " << m_strSoftVer << endl;
    cout << "\t m_strSoftMod" << " : " << m_strSoftMod << endl;
    cout << "\t m_strSoftDependVer" << " : " << m_strSoftDependVer << endl;
    cout << "\t m_strStartSN" << " : " << m_strStartSN << endl;
    cout << "\t m_strEndSN" << " : " << m_strEndSN << endl;
    cout << "\t m_bMandatory" << " : " << (m_bMandatory ? "true" : "false") << endl;
    cout << "}" << endl;
}