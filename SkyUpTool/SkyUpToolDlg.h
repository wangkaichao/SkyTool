
// SkyUpToolDlg.h : 头文件
//

#pragma once
#pragma comment(lib, "table")

class clsTable;

// CSkyUpToolDlg 对话框
class CSkyUpToolDlg : public CDialogEx
{
// 构造
public:
	CSkyUpToolDlg(CWnd* pParent = NULL);	// 标准构造函数
    ~CSkyUpToolDlg();
    //friend UINT __cdecl CreateTsThread(void *param);

    // dll打包类
    clsTable *m_table;

// 对话框数据
	enum { IDD = IDD_SKYUPTOOL_DIALOG };
    enum { UP_TIMER_ID = 1 };
    enum { PROGRESS_SIZE = 100};

private:
    //文件选项
    CEdit m_inFileEdt;
    CButton m_inFileBtn;
    CEdit m_outFileEdt;
    CButton m_outFileBtn;
    CEdit m_manufactureIdEdt;
    CEdit m_hardVerEdt;
    CEdit m_hardModEdt;
    CEdit m_softVerEdt;
    CEdit m_softModEdt;
    CEdit m_softDependVerEdt;

    //打包选项
    CButton m_psiSiBtn;
    CButton m_nullPackageBtn;
    CEdit m_infoPidEdt;
    CEdit m_dataPidEdt;
    CEdit m_dataTidEdt;
    CEdit m_netIdEdt;
    CEdit m_nitVerNumEdt;
    CEdit m_tsIdEdt;
    CEdit m_serviceIdEdt;
    CEdit m_pmtPidEdt;
    CEdit m_serviceTypeEdt;
    CEdit m_freqEdt;
    CEdit m_symbolRateEdt;
    CComboBox m_modulationComb;

    //升级选项
    CButton m_allSnBtn;
    CEdit m_startSnEdt;
    CEdit m_endSnEdt;
    CButton m_mandatoryBtn;

    //制作升级流进度
    CProgressCtrl m_upCtl;
    CButton m_upBtn;
    CStatic m_upStatus;
    CButton m_exitBtn;

    bool initCtl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    virtual BOOL PreTranslateMessage(MSG* pMsg);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnBnClickedCheck3();
    afx_msg void OnBnClickedCheck4();
    afx_msg void OnBnClickedCheck1();
    afx_msg void OnBnClickedCheck2();
    afx_msg void OnBnClickedButton1();
    afx_msg void OnBnClickedButton2();
    afx_msg void OnBnClickedButton3();
    afx_msg void OnBnClickedButton4();
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};
