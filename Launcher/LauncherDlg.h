
// LauncherDlg.h : header file
//

#pragma once


// CLauncherDlg dialog
class CLauncherDlg : public CDialogEx
{
// Construction
public:
	CLauncherDlg(CWnd* pParent = nullptr);	// standard constructor
	
	CComboBox windowSizeComboBox;
	CComboBox architectureComboBox;
	CComboBox configurationComboBox;
	CComboBox classInstantiateComboBox;
	CButton fullscreenBtn;
	CMFCEditBrowseCtrl projectPath;

	bool isFullscreen;


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAUNCHER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartbtn();
	afx_msg void OnBnClickedCancelbtn();
	afx_msg void OnCbnSelchangeWindowsizecombo();
};
