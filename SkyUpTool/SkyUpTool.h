
// SkyUpTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSkyUpToolApp: 
// �йش����ʵ�֣������ SkyUpTool.cpp
//

class CSkyUpToolApp : public CWinApp
{
public:
	CSkyUpToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSkyUpToolApp theApp;