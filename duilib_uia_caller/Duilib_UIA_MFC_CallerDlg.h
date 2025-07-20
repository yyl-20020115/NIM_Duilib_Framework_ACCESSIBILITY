
// Duilib_UIA_MFC_CallerDlg.h: 头文件
//

#pragma once


// CDuilibUIAMFCCallerDlg 对话框
class CDuilibUIAMFCCallerDlg : public CDialogEx
{
// 构造
public:
	CDuilibUIAMFCCallerDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DUILIB_UIA_MFC_CALLER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HICON m_hNewIcon;
	HICON m_hSpyIcon;
	HCURSOR m_hTargetCursor;
	BOOL m_bIsDragging = FALSE;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit HwndEdit;
	CButton AttachCheckBox;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckAttach();
	CStatic TargetImage;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
