//////////////////////////////////////////////////////////////////////////////////////
//This file is part of open-geomorphometry.
//
//    open-geomorphometry is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    open-geomorphometry is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with open-geomorphometry.  If not, see <http://www.gnu.org/licenses/>.
//
//////////////////////////////////////////////////////////////////////////////////////
//
// formDlg.cpp : implementation file
//

#include "stdafx.h"
#include "form.h"
#include "formDlg.h"

#include "FormProcess.h"
#include<fstream>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

LRESULT CFormDlg::OnRecalcInProgress1(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox("Here");
	m_CURRENT_STATUS = *(CString*) lParam;
	UpdateData(false);
	if(wParam ==1)
	{
		delete (CString*) lParam;
	}
	return 0;
}
CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormDlg dialog
void CFormDlg::OnUpdateStatus(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox("Here");
	m_CURRENT_STATUS = *(CString*) lParam;
	UpdateData(false);
	if(wParam ==1)
	{
		delete (CString*) lParam;
	}
	//return 0;
}

CFormDlg::CFormDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFormDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFormDlg)
	m_EDIT_DEM = _T("");
	m_EDIT_DSAREA = 0;
	m_EDIT_SIZE = 0;
	m_EDIT_UPSLOPEAREA = 0;
	m_RADIO1 = -1;
	m_CURRENT_STATUS = _T("");
	m_OVERALL_STATUS = _T("");
	m_MissingValue = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFormDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormDlg)
	DDX_Control(pDX, IDC_PROGRESS_OVERALL_PROCESS, m_OVERALL_PROGRESS);
	DDX_Control(pDX, IDC_PROGRESS_CURRENT_PROCESS, m_CURRENT_PROGRESS);
	DDX_Text(pDX, IDC_EDIT_DEM, m_EDIT_DEM);
	DDX_Text(pDX, IDC_EDIT_DSAREA, m_EDIT_DSAREA);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_EDIT_SIZE);
	DDX_Text(pDX, IDC_EDIT_UPSLOPEAREA, m_EDIT_UPSLOPEAREA);
	DDX_Radio(pDX, IDC_RADIO1, m_RADIO1);
	DDX_Text(pDX, IDC_STATIC_CURRENT_PROCESS, m_CURRENT_STATUS);
	DDX_Text(pDX, IDC_STATIC_OVERALL_PROCESS, m_OVERALL_STATUS);
	DDX_Text(pDX, IDC_EDIT_MISSINGVALUE, m_MissingValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFormDlg, CDialog)
	//{{AFX_MSG_MAP(CFormDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DEM, OnButtonDem)
	ON_MESSAGE(WM_USER_RECALC_IN_PROGRESS, OnRecalcInProgress1)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormDlg message handlers

BOOL CFormDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
	
	
	//m_EDIT_DEM = "c:\\copy of m1b\\m1bdem.dbf";
	//m_EDIT_DSAREA = 300;
	//m_EDIT_SIZE = 10;
	//m_EDIT_UPSLOPEAREA = 300;
	
	m_MissingValue = -9999;

	m_EDIT_DEM = "";
	m_EDIT_DSAREA = 300;
	m_EDIT_SIZE = 10;
	m_EDIT_UPSLOPEAREA = 300;
	m_OVERALL_STATUS = "Overall progress: ";
	m_CURRENT_STATUS = "Current process:";

	//Set it to 0 so we can call delete on it;
	pFormProcess = 0;
	m_RADIO1 = 0;

	UpdateData(false);

	
	
	char*  s = new char[255];
	char full[_MAX_PATH];
	if( _fullpath( full, "FormParam.txt", _MAX_PATH ) != NULL )
	{
		CString sPath = full;
		sFullPathParam = full;
		int nLoc = sPath.Find("FormParam.txt");
		sPath = sPath.Left(nLoc);
		std::ifstream ParamFile(sPath + "FormParam.txt");
		
		ParamFile.getline(s,255);
		if(strcmp(s,"FormParam.txt")==0)
		{
			ParamFile.getline(s,255);
			strDirectory= s;
			ParamFile.getline(s,255);
			m_EDIT_DEM = s;
			ParamFile.getline(s,255);
			str3CharName = s;
			ParamFile>>m_EDIT_DSAREA;
			ParamFile>>m_EDIT_SIZE;
			ParamFile>>m_EDIT_UPSLOPEAREA;
			ParamFile>>m_RADIO1;
			ParamFile>>m_MissingValue;
		}
		else
		{

		}
	}



	UpdateData(false);
	
	pFormProcess = 0;
	pPointerArray = 0;

	bKillThread = false;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFormDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFormDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFormDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFormDlg::OnButtonDem() 
{
	UpdateData(true);
	// TODO: Add your control notification handler code here
	CString sFileName;
	CString sFullFileName;
	CFileDialog* dlgFile = new CFileDialog(TRUE,"","",FALSE,"Database Files (*.dbf)|*.dbf");
	if(dlgFile->DoModal())
	{
		sFileName =  dlgFile->GetFileName() ;
		sFullFileName = dlgFile->GetPathName();
		m_EDIT_DEM = sFullFileName;
		
		//Create a directory name
		int nLoop22=0;
		for(nLoop22=m_EDIT_DEM.GetLength() -1;nLoop22>0;nLoop22--)
		{
			if((m_EDIT_DEM.GetAt(nLoop22) =='\\') || (m_EDIT_DEM.GetAt(nLoop22) =='//'))
			{
				strDirectory = m_EDIT_DEM.Left(nLoop22 +1);
				nLoop22 = 0;
			}
		}
		
		//Create a 3 character prefix for the name
		str3CharName = m_EDIT_DEM.Left(m_EDIT_DEM.GetLength() - 7);
		str3CharName = str3CharName.Right(3);
			
		
		
		
		
		if(sFileName.GetLength() >0)
		{
			UpdateData(FALSE);
		}
	}
	// TODO: Add your control notification handler code here
	
	delete dlgFile;	
}

UINT PROCESS1( LPVOID pParam )
{
	CPtrArray* pArray =  (CPtrArray*) pParam;

	CFormProcess* pFormProcess = (CFormProcess*) pArray->GetAt(0);

	pFormProcess->PROCESS1(pArray);
	
	
    return 0;   // thread completed successfully
}

UINT PROCESS2( LPVOID pParam )
{
	CPtrArray* pArray =  (CPtrArray*) pParam;

	CFormProcess* pFormProcess = (CFormProcess*) pArray->GetAt(0);

	pFormProcess->PROCESS2(pArray);
	
	
    return 0;   // thread completed successfully
}
UINT PROCESS3( LPVOID pParam )
{
	CPtrArray* pArray =  (CPtrArray*) pParam;

	CFormProcess* pFormProcess = (CFormProcess*) pArray->GetAt(0);

	pFormProcess->PROCESS3(pArray);
	
	
    return 0;   // thread completed successfully
}
void CFormDlg::OnOK() 
{
	m_OVERALL_PROGRESS.SetPos(0);
	m_OVERALL_PROGRESS.SetStep(1);
	m_OVERALL_PROGRESS.SetRange(0,5);

	bKillThread = false;
	UpdateData(true);


	
	// TODO: Add extra validation here
	delete pFormProcess;
	delete pPointerArray;
	pFormProcess = new CFormProcess;
	pPointerArray = new CPtrArray;
	int nLoop22=0;
	for(nLoop22=m_EDIT_DEM.GetLength() -1;nLoop22>0;nLoop22--)
	{
		if((m_EDIT_DEM.GetAt(nLoop22) =='\\') || (m_EDIT_DEM.GetAt(nLoop22) =='//'))
		{
			strDirectory = m_EDIT_DEM.Left(nLoop22 +1);
			nLoop22 = 0;
		}
	}


	str3CharName = m_EDIT_DEM.Left(m_EDIT_DEM.GetLength() - 7);
	str3CharName = str3CharName.Right(3);
	

	
	std::ofstream fParamFile(sFullPathParam);

	fParamFile<<"FormParam.txt"<<'\n';
	fParamFile<<strDirectory<<'\n';
	fParamFile<<m_EDIT_DEM<<'\n';
	fParamFile<<str3CharName <<'\n';
	fParamFile<<m_EDIT_DSAREA<<'\n';
	fParamFile<<m_EDIT_SIZE<<'\n';
	fParamFile<<m_EDIT_UPSLOPEAREA<<'\n';
	fParamFile<<m_RADIO1<<'\n';
	fParamFile<<m_MissingValue<<'\n';
	fParamFile.close();
	
	



	bool bValid = false;
	if(m_EDIT_DEM.GetLength()>0) 
	{
		if(m_EDIT_SIZE >0) 
		{
			if(m_EDIT_UPSLOPEAREA>0)
			{
				if(m_EDIT_DSAREA>0)
				{
					bValid = true;

					//Index 0
					pPointerArray->Add(pFormProcess);
					
					//Index 1
					pPointerArray->Add(&m_EDIT_DEM);

					//Index 2
					pPointerArray->Add(&m_EDIT_SIZE);

					//Index 3
					pPointerArray->Add(&m_EDIT_UPSLOPEAREA);
					
					//Index 4
					pPointerArray->Add(&m_EDIT_DSAREA);

					//Index 5
					pPointerArray->Add(&m_CURRENT_PROGRESS);

					//index 6
					pPointerArray->Add(&m_OVERALL_PROGRESS);


					//Index 7
					pPointerArray->Add(&m_CURRENT_STATUS);

					//Index 8
					pPointerArray->Add(&m_OVERALL_STATUS);

					//Index 9 - window handle.
					pPointerArray->Add((this)->m_hWnd);

					//Index 10
					pPointerArray->Add(&str3CharName);

					//Index 11
					pPointerArray->Add(&strDirectory);
					
					//Index12
					pPointerArray->Add(&bKillThread);


					//Index 13
					pPointerArray->Add(&m_MissingValue);

					pFormProcess->SetVariables(pPointerArray);


					switch(m_RADIO1)
					{
					case 0:
						AfxBeginThread(PROCESS1,pPointerArray);
						break;
					case 1:
						AfxBeginThread(PROCESS2,pPointerArray);
						break;
					case 2:
						AfxBeginThread(PROCESS3,pPointerArray);
						break;
					}


				}
				else
				{
					AfxMessageBox("You must provide DS_Area value.");
				}
			}
			else
			{
				AfxMessageBox("You must provide upslope value.");
			}
		}
		else
		{
			AfxMessageBox("You must provide grid size.");
		}
	}
	else
	{
		AfxMessageBox("You must provide DEM file.");
	}

	if(bValid ==false)
	{
		delete pPointerArray;
		pPointerArray = 0;

		delete pFormProcess;
		pFormProcess = 0;
	}


	//CDialog::OnOK();
}


void CFormDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	bKillThread = true;
	//delete pFormProcess;
	//delete pPointerArray;
	//CDialog::OnCancel();
	
}


void CFormDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	delete pFormProcess;
	delete pPointerArray;
	bKillThread = true;
	exit(1);
	
}
