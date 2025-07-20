
// Duilib_UIA_MFC_CallerDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "Duilib_UIA_MFC_Caller.h"
#include "Duilib_UIA_MFC_CallerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef BOOL (*Ptr_AttachWindowFunc)(HWND hwnd);
typedef BOOL(*Ptr_DetachWindowFunc)(HWND hwnd);
HMODULE hModule = nullptr;
Ptr_AttachWindowFunc AttachWindow = nullptr;
Ptr_DetachWindowFunc DetachWindow = nullptr;


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDuilibUIAMFCCallerDlg 对话框



CDuilibUIAMFCCallerDlg::CDuilibUIAMFCCallerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DUILIB_UIA_MFC_CALLER_DIALOG, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDuilibUIAMFCCallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_HWND, HwndEdit);
	DDX_Control(pDX, IDC_CHECK_ATTACH, AttachCheckBox);
	DDX_Control(pDX, IDC_STATIC_IMAGE, TargetImage);
}

BEGIN_MESSAGE_MAP(CDuilibUIAMFCCallerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CDuilibUIAMFCCallerDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_ATTACH, &CDuilibUIAMFCCallerDlg::OnBnClickedCheckAttach)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CDuilibUIAMFCCallerDlg 消息处理程序

BOOL CDuilibUIAMFCCallerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetWindowText(_T("Duilib UIA-Proxy Installer"));

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL); // 初始化GDI+


	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	m_hNewIcon = AfxGetApp()->LoadIconW(IDI_ICON_BLANK);
	m_hSpyIcon = AfxGetApp()->LoadIconW(IDI_ICON_TARGET);

	this->TargetImage.ModifyStyle(0x0, SS_ICON | SS_CENTERIMAGE);
	this->TargetImage.SetIcon(m_hSpyIcon);

	m_hTargetCursor = AfxGetApp()->LoadCursorW(IDC_CRS_TARGET);

	hModule = GetModuleHandle(_T("duilib_uia_proxy.dll"));

	if (hModule != nullptr)
	{
		AttachWindow = (Ptr_AttachWindowFunc)GetProcAddress(hModule, "AttachWindow");
		DetachWindow = (Ptr_DetachWindowFunc)GetProcAddress(hModule, "DetachWindow");
		if (AttachWindow == nullptr || DetachWindow == nullptr)
		{
			AfxMessageBox(_T("无法加载 duilib_uia_proxy.dll 中的函数"));
			return FALSE;
		}
	}
	else
	{
		AfxMessageBox(_T("无法加载 duilib_uia_proxy.dll"));
		return FALSE;
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDuilibUIAMFCCallerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDuilibUIAMFCCallerDlg::OnPaint()
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
HCURSOR CDuilibUIAMFCCallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDuilibUIAMFCCallerDlg::OnBnClickedCancel()
{
	this->AttachCheckBox.SetCheck(BST_UNCHECKED);
	CDialogEx::OnCancel();
}

void CDuilibUIAMFCCallerDlg::OnBnClickedCheckAttach()
{

	CString strHwnd;
	this->HwndEdit.GetWindowText(strHwnd);
	if (strHwnd.IsEmpty())
	{
		AfxMessageBox(_T("请先输入窗口句柄"));
		this->AttachCheckBox.SetCheck(BST_UNCHECKED);
		return;
	}
	HWND hwnd = (HWND)_tcstol(strHwnd, nullptr, 16);
	if (hwnd == nullptr)
	{
		AfxMessageBox(_T("无效的窗口句柄"));
		this->AttachCheckBox.SetCheck(BST_UNCHECKED);
		return;
	}
	if (this->AttachCheckBox.GetCheck() == BST_CHECKED)
	{
		if (AttachWindow == nullptr) {
			AfxMessageBox(_T("无法调用AttachWindow"));
		}
		else {
			BOOL done = AttachWindow(hwnd);
			if (done) {
				CString strMsg;
				strMsg.Format(_T("已成功附加到窗口句柄: %08X"), (INT)(UINT_PTR)hwnd);
				AfxMessageBox(strMsg);
			}
			else {
				AfxMessageBox(_T("附加窗口失败"));
			}
		}
	}
	else
	{
		if (DetachWindow == nullptr) {
			AfxMessageBox(_T("无法调用DetachWindow"));
		}
		else {
			BOOL done = DetachWindow(hwnd);
			if (done) {
				CString strMsg;
				strMsg.Format(_T("已成功从窗口句柄: %08X 中分离"), (INT)(UINT_PTR)hwnd);
				AfxMessageBox(strMsg);
			}
			else {
				AfxMessageBox(_T("分离窗口失败"));
			}
		}
	}

}

void CDuilibUIAMFCCallerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rtTarget;
	TargetImage.GetWindowRect(&rtTarget);
	ScreenToClient(&rtTarget);
	if (rtTarget.PtInRect(point))
	{
		TargetImage.SetIcon(m_hNewIcon);
		::SetCursor(m_hTargetCursor);
		::SetCapture(m_hWnd);
		m_bIsDragging = TRUE;
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CDuilibUIAMFCCallerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	TargetImage.SetIcon(m_hSpyIcon);
	ReleaseCapture();
	m_bIsDragging = FALSE;

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CDuilibUIAMFCCallerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	POINT ptMouse;
	RECT rt;
	HWND hWnd;

	if (m_bIsDragging && GetCursorPos(&ptMouse))
	{
		if ((hWnd = ::WindowFromPoint(ptMouse)) && ::GetWindowRect(hWnd, &rt))
		{
			Gdiplus::Graphics graphics(::GetDesktopWindow());
			Gdiplus::Pen pen(Gdiplus::Color(255, 0, 255), 5);
			graphics.DrawRectangle(
				&pen,
				rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top);
		
			CString strHwnd;
			strHwnd.Format(_T("%08X"), (UINT_PTR)hWnd);
			this->HwndEdit.SetWindowText(strHwnd);
		
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}
