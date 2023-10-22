#include "Windows.h"
// LauncherDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Launcher.h"
#include "LauncherDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <iostream>
#include <sstream>
#include <string>


using std::string;
using std::cout;
using std::wcout;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLauncherDlg dialog



CLauncherDlg::CLauncherDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LAUNCHER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, WindowSizeCombo, windowSizeComboBox);
	DDX_Control(pDX, ArchitectureCombo, architectureComboBox);
	DDX_Control(pDX, ConfigurationCombo, configurationComboBox);
	DDX_Control(pDX, ClassInstantiateCombo, classInstantiateComboBox);
	DDX_Control(pDX, FullscreenCheck, fullscreenBtn);
	DDX_Control(pDX, ProjectPathBrowser, projectPath);

}

BEGIN_MESSAGE_MAP(CLauncherDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(StartBtn, &CLauncherDlg::OnBnClickedStartbtn)
	ON_BN_CLICKED(CancelBtn, &CLauncherDlg::OnBnClickedCancelbtn)
	ON_CBN_SELCHANGE(WindowSizeCombo, &CLauncherDlg::OnCbnSelchangeWindowsizecombo)
END_MESSAGE_MAP()


// CLauncherDlg message handlers

BOOL CLauncherDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	windowSizeComboBox.SetCurSel(0);
	architectureComboBox.SetCurSel(0);
	configurationComboBox.SetCurSel(0);
	classInstantiateComboBox.SetCurSel(0);
	projectPath.SetWindowTextW(L"C:\\Users\\juanc\\Documents\\UAD\\4to Cuatrimestre\\Programación III\\uad_progra3\\uad_progra3");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLauncherDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

	CStatic* pPCBox = (CStatic*)(GetDlgItem(ImageDlg));
	CRect rect;
	pPCBox->GetClientRect(rect);
	CClientDC dc(pPCBox);
	CImage image;
	image.Load(L"seppuku.jpg");
	CBitmap bitmap;
	bitmap.Attach(image.Detach());
	CDC memoryDC;
	memoryDC.CreateCompatibleDC(&dc);
	memoryDC.SelectObject(bitmap);
	BITMAP bmp;
	bitmap.GetBitmap(&bmp);
	dc.SetStretchBltMode(COLORONCOLOR);
	dc.StretchBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memoryDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CLauncherDlg::OnBnClickedStartbtn()
{
	// TODO: Add your control notification handler code here

	CString cPath;
	CString cArch;
	CString cSize;
	CString cConfig;
	CString cClass;
	
	projectPath.GetWindowTextW(cPath);
	architectureComboBox.GetLBText(architectureComboBox.GetCurSel(), cArch);
	windowSizeComboBox.GetLBText(windowSizeComboBox.GetCurSel(), cSize);
	configurationComboBox.GetLBText(configurationComboBox.GetCurSel(), cConfig);
	classInstantiateComboBox.GetLBText(classInstantiateComboBox.GetCurSel(), cClass);

	isFullscreen = fullscreenBtn.GetCheck() == BST_CHECKED;

	std::wstring wProjectPath(cPath.GetString());
	wProjectPath += L"\\Binaries\\Exe\\";

	std::wstring wArch(cArch.GetString());
	wArch += L"\\";

	std::wstring wConfig(cConfig.GetString());
	wConfig += L"\\";

	std::wstring wExePath(wProjectPath + wArch + wConfig + L"uad_progra3.exe");

	std::wstring wSize(cSize.GetString());
	std::wstring wWidth;
	std::wstring wX;
	std::wstring wHeight;
	std::wstringstream wResolution(wSize);
	wResolution >> wWidth >> wX >> wHeight;

	std::wstring wClass(cClass.GetString());
	
	std::wstring wFullScrn;
	if (isFullscreen)
	{
		wFullScrn = L"True";
	}
	else
	{
		wFullScrn = L"False";
	}

	std::wstring wCommand = wExePath + L" " + wWidth + L" " + wHeight + L" " + wClass + L" " + wFullScrn;

	LPWSTR commandLine = (LPWSTR)wCommand.c_str();

	STARTUPINFO startUp;
	PROCESS_INFORMATION processInfo;
	SECURITY_ATTRIBUTES securityAttributes = { sizeof(SECURITY_ATTRIBUTES), NULL, FALSE };

	ZeroMemory(&startUp, sizeof(startUp));
	ZeroMemory(&processInfo, sizeof(processInfo));

	startUp.cb = sizeof(startUp);


	if (!CreateProcessW(NULL, commandLine, &securityAttributes, &securityAttributes, false, 0, NULL, NULL, &startUp, &processInfo))
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return;
	}
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);
}


void CLauncherDlg::OnBnClickedCancelbtn()
{
	// TODO: Add your control notification handler code here
	exit(0);
}


void CLauncherDlg::OnCbnSelchangeWindowsizecombo()
{
	// TODO: Add your control notification handler code here


	//CComboBox comboBox;


	//CString cSize;
	//wchar_t wSize;

	//comboBox.GetLBText(WindowSizeCombo, cSize);
	//comboBox.GetLBText(WindowSizeCombo, &wSize);

	//cout << cSize;
	//wcout << wSize;


	//cout << "";
}
