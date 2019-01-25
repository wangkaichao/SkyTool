
// SkyUpToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SkyUpTool.h"
#include "SkyUpToolDlg.h"
#include "afxdialogex.h"
#include "Comm.h"
#include "sky_up_info.h"
#include "table.h"
#include <string>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSkyUpToolDlg 对话框



CSkyUpToolDlg::CSkyUpToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSkyUpToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_table = nullptr;
}
CSkyUpToolDlg::~CSkyUpToolDlg()
{
    if (m_table != nullptr) {
        delete m_table;
        m_table = nullptr;
    }
}

/*UINT AFX_CDECL CreateTsThread(void *param)
{
    CSkyUpToolDlg *mainDlg = (CSkyUpToolDlg *)param;
    mainDlg->pTable->generateTsFile();

    return 0;
}*/

bool CSkyUpToolDlg::initCtl()
{
    LPCTSTR modulation[3] = { "QAM 64", "QAM 128", "QAM 256" };

    //升级文本只能选择，不能手动输入
    m_inFileEdt.EnableWindow(0);

    m_manufactureIdEdt.SetWindowTextA("0");
    m_hardVerEdt.SetWindowTextA("0");
    m_hardModEdt.SetWindowTextA("0");
    m_softVerEdt.SetWindowTextA("0");
    m_softModEdt.SetWindowTextA("0");
    m_softDependVerEdt.SetWindowTextA("0");

    m_startSnEdt.SetWindowTextA("0");
    m_endSnEdt.SetWindowTextA("0");
    OnBnClickedCheck3();

    m_infoPidEdt.SetWindowTextA("7001");
    m_dataPidEdt.SetWindowTextA("7000");
    m_dataTidEdt.SetWindowTextA("144");

    m_netIdEdt.SetWindowTextA("1");
    m_nitVerNumEdt.SetWindowTextA("1");
    m_tsIdEdt.SetWindowTextA("1");
    m_serviceIdEdt.SetWindowTextA("1");
    m_pmtPidEdt.SetWindowTextA("170");
    m_serviceTypeEdt.SetWindowTextA("144");
    OnBnClickedCheck1();

    m_freqEdt.SetWindowTextA("0");
    m_symbolRateEdt.SetWindowTextA("6875");
    for (int i = 0; i < sizeof(modulation) / sizeof(INT8 *); ++i)
        m_modulationComb.AddString(modulation[i]);
    m_modulationComb.SetCurSel(0);

    m_freqEdt.EnableWindow(0);
    m_symbolRateEdt.EnableWindow(0);
    m_modulationComb.EnableWindow(0);

    m_upCtl.SetRange(0, PROGRESS_SIZE);
    m_upCtl.SetStep(4);
    m_upCtl.SetPos(0);

    return true;
}

void CSkyUpToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

    //文件选项组
    DDX_Control(pDX, IDC_EDIT1, m_inFileEdt);
    DDX_Control(pDX, IDC_BUTTON1, m_inFileBtn);
    DDX_Control(pDX, IDC_EDIT2, m_outFileEdt);
    DDX_Control(pDX, IDC_BUTTON2, m_outFileBtn);
    DDX_Control(pDX, IDC_EDIT3, m_manufactureIdEdt);
    DDX_Control(pDX, IDC_EDIT4, m_hardVerEdt);
    DDX_Control(pDX, IDC_EDIT5, m_hardModEdt);
    DDX_Control(pDX, IDC_EDIT6, m_softVerEdt);
    DDX_Control(pDX, IDC_EDIT7, m_softModEdt);
    DDX_Control(pDX, IDC_EDIT8, m_softDependVerEdt);

    //打包选项组
    DDX_Control(pDX, IDC_CHECK1, m_psiSiBtn);
    DDX_Control(pDX, IDC_CHECK2, m_nullPackageBtn);
    DDX_Control(pDX, IDC_EDIT9, m_infoPidEdt);
    DDX_Control(pDX, IDC_EDIT10, m_dataPidEdt);
    DDX_Control(pDX, IDC_EDIT11, m_dataTidEdt);
    DDX_Control(pDX, IDC_EDIT14, m_netIdEdt);
    DDX_Control(pDX, IDC_EDIT15, m_nitVerNumEdt);
    DDX_Control(pDX, IDC_EDIT16, m_tsIdEdt);
    DDX_Control(pDX, IDC_EDIT17, m_serviceIdEdt);
    DDX_Control(pDX, IDC_EDIT18, m_pmtPidEdt);
    DDX_Control(pDX, IDC_EDIT19, m_serviceTypeEdt);
    DDX_Control(pDX, IDC_EDIT12, m_freqEdt);
    DDX_Control(pDX, IDC_EDIT13, m_symbolRateEdt);
    DDX_Control(pDX, IDC_COMBO1, m_modulationComb);

    //升级选项组
    DDX_Control(pDX, IDC_CHECK3, m_allSnBtn);
    DDX_Control(pDX, IDC_EDIT20, m_startSnEdt);
    DDX_Control(pDX, IDC_EDIT21, m_endSnEdt);
    DDX_Control(pDX, IDC_CHECK4, m_mandatoryBtn);

    //制作升级流进度
    DDX_Control(pDX, IDC_PROGRESS1, m_upCtl);
    DDX_Control(pDX, IDC_BUTTON3, m_upBtn);
    DDX_Control(pDX, IDC_STATIC_UP, m_upStatus);

    //退出
    DDX_Control(pDX, IDC_BUTTON4, m_exitBtn);
}

// 屏蔽客户输入esc键或enter键退出程序  
BOOL CSkyUpToolDlg::PreTranslateMessage(MSG* pMsg)
{
    // TODO: 在此添加专用代码和/或调用基类  
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)// 屏蔽esc键  
    {
        return TRUE;// 不作任何操作  
    }
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)// 屏蔽enter键  
    {
        return TRUE;// 不作任何处理  
    }
    return CDialogEx::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CSkyUpToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_CHECK3, &CSkyUpToolDlg::OnBnClickedCheck3)
    ON_BN_CLICKED(IDC_CHECK4, &CSkyUpToolDlg::OnBnClickedCheck4)
    ON_BN_CLICKED(IDC_CHECK1, &CSkyUpToolDlg::OnBnClickedCheck1)
    ON_BN_CLICKED(IDC_CHECK2, &CSkyUpToolDlg::OnBnClickedCheck2)
    ON_BN_CLICKED(IDC_BUTTON1, &CSkyUpToolDlg::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CSkyUpToolDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON3, &CSkyUpToolDlg::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CSkyUpToolDlg::OnBnClickedButton4)
    ON_WM_TIMER()
END_MESSAGE_MAP()


// CSkyUpToolDlg 消息处理程序

BOOL CSkyUpToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
#if USE_DEBUG
    Comm::InitConsoleWindow();
    logDbg("str = %s\n ", "Debug output goes to terminal\n");
#endif

    initCtl();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSkyUpToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSkyUpToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSkyUpToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//升级所有序列号复选框
void CSkyUpToolDlg::OnBnClickedCheck3()
{
    // TODO: Add your control notification handler code here
    BOOL enable = m_allSnBtn.GetCheck() ? 0 : 1;
    m_startSnEdt.EnableWindow(enable);
    m_endSnEdt.EnableWindow(enable);
}

//强制升级复选框
void CSkyUpToolDlg::OnBnClickedCheck4()
{
    // TODO: Add your control notification handler code here
    //AfxMessageBox("强制升级");
}

//生成psi/si表复选框
void CSkyUpToolDlg::OnBnClickedCheck1()
{
    // TODO: Add your control notification handler code here
    //AfxMessageBox("psi/si");
    BOOL enable = m_psiSiBtn.GetCheck();
    m_netIdEdt.EnableWindow(enable);
    m_nitVerNumEdt.EnableWindow(enable);
    m_tsIdEdt.EnableWindow(enable);
    m_serviceIdEdt.EnableWindow(enable);
    m_pmtPidEdt.EnableWindow(enable);
    m_serviceTypeEdt.EnableWindow(enable);
}

//插入空包复选框
void CSkyUpToolDlg::OnBnClickedCheck2()
{
    // TODO: Add your control notification handler code here
    //AfxMessageBox("null package");
}

//点击浏览按钮（指定输入文件）
void CSkyUpToolDlg::OnBnClickedButton1()
{
    // TODO: Add your control notification handler code here
    //AfxMessageBox("指定输入文件");
    BOOL isOpen = TRUE; //TRUE打开(FALSE保存)  
    CString filter = "文件(*.zip)|*.zip|所有文件(*.*)|*.*|";
    CFileDialog fileDlg(isOpen, _T("zip"), NULL, 0, filter, this);

    if (fileDlg.DoModal() == IDOK) {
        CString filename = fileDlg.GetPathName();
        m_inFileEdt.SetWindowText(filename);
    }
}

//点击浏览按钮（指定输出文件）
void CSkyUpToolDlg::OnBnClickedButton2()
{
    // TODO: Add your control notification handler code here
    //AfxMessageBox("指定输出文件");
    BOOL isOpen = TRUE; //TRUE打开(FALSE保存)  
    CString filter = "文件(*.ts)|*.ts|所有文件(*.*)|*.*|";
    CFileDialog fileDlg(isOpen, _T("ts"), NULL, 0, filter, this);

    if (fileDlg.DoModal() == IDOK) {
        CString filename = fileDlg.GetPathName();
        m_outFileEdt.SetWindowText(filename);
    }
}

//点击生产（TS流文件）
void CSkyUpToolDlg::OnBnClickedButton3()
{
    // TODO: Add your control notification handler code here
    //AfxMessageBox("生成");
    //传递数据
    clsUpInfo upInfo;
    CString cstr;
    
    //文件选项
    m_inFileEdt.GetWindowTextA(cstr);
    if (cstr.IsEmpty()) {
        AfxMessageBox("输入文件为空！");
        return;
    }
    upInfo.inFileName(cstr.GetString());

    m_outFileEdt.GetWindowTextA(cstr);
    if (cstr.IsEmpty()) {
        AfxMessageBox("升级流文件为空！");
        return;
    }
    if (fopen(cstr.GetString(), "rb") != NULL) {
        
        // 1-> ok 2->cancel
        if (AfxMessageBox("升级流文件已存在，是否替换！", MB_OKCANCEL | MB_ICONQUESTION) == 2)
            return;
    }
    upInfo.outFileName(cstr.GetString());

    m_manufactureIdEdt.GetWindowTextA(cstr);
    if (cstr.IsEmpty()) {
        AfxMessageBox("厂商ID为空！");
        return;
    }
    upInfo.manufactureId(cstr.GetString());

    m_hardVerEdt.GetWindowTextA(cstr);
    if (cstr.IsEmpty()) {
        AfxMessageBox("硬件版本为空！");
        return;
    }
    upInfo.hardVer(cstr.GetString());

    m_hardModEdt.GetWindowTextA(cstr);
    if (cstr.IsEmpty()) {
        AfxMessageBox("硬件型号为空！");
        return;
    }
    upInfo.hardMod(cstr.GetString());

    m_softVerEdt.GetWindowTextA(cstr);
    if (cstr.IsEmpty()) {
        AfxMessageBox("软件版本为空！");
        return;
    }
    upInfo.softVer(cstr.GetString());

    m_softModEdt.GetWindowTextA(cstr);
    if (cstr.IsEmpty()) {
        AfxMessageBox("软件型号不能为空！");
        return;
    }
    upInfo.softMod(cstr.GetString());

    m_softDependVerEdt.GetWindowTextA(cstr);
    if (cstr.IsEmpty()) {
        AfxMessageBox("软件依赖版本不能为空！");
        return;
    }
    upInfo.softDependVer(cstr.GetString());

    //升级选型
    if (m_allSnBtn.GetCheck()) {
        upInfo.startSN("0");
        upInfo.endSN("0");
    }
    else {
        m_startSnEdt.GetWindowTextA(cstr);
        if (cstr.IsEmpty()) {
            AfxMessageBox("起始序列号不能为空！");
            return;
        }
        upInfo.startSN(cstr.GetString());

        m_endSnEdt.GetWindowTextA(cstr);
        if (cstr.IsEmpty()) {
            AfxMessageBox("结束序列号不能为空！");
            return;
        }
        upInfo.endSN(cstr.GetString());
    }

    upInfo.mandatory(m_mandatoryBtn.GetCheck() ? true : false);

    //打包选项
    m_infoPidEdt.GetWindowTextA(cstr);
    if (cstr.IsEmpty()) {
        AfxMessageBox("引导PID不能为空！");
        return;
    }
    upInfo.infoPid(cstr.GetString());

    m_dataPidEdt.GetWindowTextA(cstr);
    if (cstr.IsEmpty()) {
        AfxMessageBox("升级PID不能为空！");
        return;
    }
    upInfo.dataPid(cstr.GetString());

    m_dataTidEdt.GetWindowTextA(cstr);
    if (cstr.IsEmpty()) {
        AfxMessageBox("升级TID不能为空！");
        return;
    }
    upInfo.dataTid(cstr.GetString());
    upInfo.infoTid(cstr.GetString());

    upInfo.log();
    
    m_table = new clsTable(&upInfo);
    SetTimer(UP_TIMER_ID, 200, NULL);

    SetCapture();
    ::SetCursor(::LoadCursor(NULL, IDC_WAIT));
    m_upBtn.EnableWindow(0);
    m_upStatus.SetWindowTextA("生成中...");

    //启动线程
    //AfxBeginThread((AFX_THREADPROC)CreateTsThread, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0);
    AfxBeginThread([](void *param)->UINT {
        CSkyUpToolDlg *mainDlg = (CSkyUpToolDlg *)param;
        mainDlg->m_table->generateTsFile();
        return 0;

    }, (LPVOID)this, THREAD_PRIORITY_ABOVE_NORMAL, 0, 0);
}

//点击退出
void CSkyUpToolDlg::OnBnClickedButton4()
{
    // TODO: Add your control notification handler code here
    //AfxMessageBox("退出");
    AfxGetMainWnd()->PostMessage(WM_QUIT, 0, 0);
}

//主窗口定时器，画进度条
void CSkyUpToolDlg::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: Add your message handler code here and/or call default
    if (nIDEvent == UP_TIMER_ID) {
        uint32_t pos = m_table->progress();
        //logDbg("pos=%d", pos);
        m_upCtl.SetPos(pos);

        if (pos == PROGRESS_SIZE) {
            delete m_table;
            m_table = nullptr;

            KillTimer(UP_TIMER_ID);
            m_upStatus.SetWindowTextA("生成完成");
            m_upBtn.EnableWindow(1);
            ::SetCursor(::LoadCursor(NULL, IDC_ARROW));
            ReleaseCapture();
        }
    }
    
    CDialogEx::OnTimer(nIDEvent);
}
