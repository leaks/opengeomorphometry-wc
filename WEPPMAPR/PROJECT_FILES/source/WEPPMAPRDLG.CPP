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
// WeppMaprDlg.cpp : implementation file
//

	
#include "stdafx.h"
#include "WeppMapr.h"
#include "WeppMaprDlg.h"
//#include "Sort.h"
//#include <Time.h>
//#include <Math.h>
//#include<fstream.h>
//#include "DbfFile.h"
#include "WeppProcess.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif











/////////////////////////////////////////////////////////////////////////////
// CWeppMaprDlg dialog

CWeppMaprDlg::CWeppMaprDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWeppMaprDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWeppMaprDlg)
	m_FileName = _T("");
	m_Size = 0;
	m_Upslope = 0;
	m_MissingData = 0.0;
	m_Message = _T("");
	m_Directory = _T("");
	m_MaxSegLength = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CWeppMaprDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWeppMaprDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_ProgressCtrl);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_FileName);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_Size);
	DDX_Text(pDX, IDC_EDIT_UPSLOPE, m_Upslope);
	DDX_Text(pDX, IDC_EDIT_MISSINGDATA, m_MissingData);
	DDX_Text(pDX, IDC_MESSAGE, m_Message);
	DDX_Text(pDX, IDC_EDIT_DIRECTORY, m_Directory);
	DDX_Text(pDX, IDC_EDIT_MAXSEGLENGTH, m_MaxSegLength);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWeppMaprDlg, CDialog)
	//{{AFX_MSG_MAP(CWeppMaprDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER_RECALC_IN_PROGRESS, OnRecalcInProgress1)
	ON_BN_CLICKED(IDC_BUTTON_DEMFILE, OnButtonDemfile)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeppMaprDlg message handlers

BOOL CWeppMaprDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);
	
	m_MissingData = -9999;
	m_Upslope = 300;
	m_Size = 10;
	m_MaxSegLength = 200;
	UpdateData(false);

	char*  s = new char[255];
	char full[_MAX_PATH];
	if( _fullpath( full, "Param.txt", _MAX_PATH ) != NULL )
	{
		CString sPath = full;
		sFullPathParam = full;
		int nLoc = sPath.Find("Param.txt");
		sPath = sPath.Left(nLoc);
		std::ifstream ParamFile(sPath + "Param.txt");
		
		ParamFile.getline(s,255);
		if(strcmp(s,"Param.txt")==0)
		{
			ParamFile.getline(s,255);
			m_Directory= s;
			ParamFile.getline(s,255);
			m_FileName = s;
			ParamFile>>m_MissingData;
			ParamFile>>m_Size;
			ParamFile>>m_Upslope;
			ParamFile>>m_MaxSegLength;

		}
		else
		{

		}
	}

	UpdateData(false);

	//TEMP

	UpdateData(false);
	
	//Hold address of CWeppProcess object
	nProcessPointer = 0;
	pPointerArray = 0;
	delete s;

	//piShedNo=0;
	//piShedSide  = 0;


	//pWeppProcess=0;
	//temp end
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWeppMaprDlg::OnPaint() 
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
HCURSOR CWeppMaprDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CWeppMaprDlg::OnRecalcInProgress1(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox("Here");
	m_Message = *(CString*) lParam;
	UpdateData(false);
	if(wParam ==1)
	{
		delete (CString*) lParam;
	}
	return 0;
}





UINT RUN_PROCESS(LPVOID pParam)
{

	
	CPtrArray* pArray = (CPtrArray*) pParam;
	CWeppProcess* pWeppProcess = (CWeppProcess*)pArray->GetAt(0);	
	CWnd* pWndNotifyProgress;

	HWND phWnd = (HWND) pArray->GetAt(1);
	pWndNotifyProgress = CWnd::FromHandle((HWND)pArray->GetAt(1));
	CProgressCtrl* pProgressCtrl = (CProgressCtrl*) pArray->GetAt(2);
	double* m_MissingData = (double*) pArray->GetAt(3);
	CString* m_Directory = (CString*) pArray->GetAt(4);
	CString* m_FileName = (CString*) pArray->GetAt(5);
	int* m_Size = (int*) pArray->GetAt(6);
	int* m_Upslope = (int*) pArray->GetAt(7);
	

	CString sFullPathParam = *(CString*) pArray->GetAt(8);
	int m_MaxSegLength = *(int*) pArray->GetAt(9);
	//bool* pbStopProcess = (bool*) pArray->GetAt(10);


	pWeppProcess->RunProcess(*m_MissingData, *m_Directory,*m_FileName,*m_Size,*m_Upslope, 
		pWndNotifyProgress, pProgressCtrl, sFullPathParam, m_MaxSegLength,pArray );
	


	return 0;
}


CWeppMaprDlg::~CWeppMaprDlg()
{
	delete (CWeppProcess*) nProcessPointer;
	delete pPointerArray;
}






void CWeppMaprDlg::OnOK() 
{
	UpdateData(true);

	bStopProcess = false;
	if(m_FileName == "")
	{
		AfxMessageBox("You must provide input file name");
	}
	else
	{
		if(m_Directory == "")
		{
			AfxMessageBox("You must provide working directory name");

		}
		else
		{

			delete (CWeppProcess*) nProcessPointer;
			CWeppProcess* pWeppProcess = new CWeppProcess;
			nProcessPointer = (long) pWeppProcess;

			delete pPointerArray;
			pPointerArray = new CPtrArray;
			//CProgressBarDialog		dlgProgressBar(this);
			
			UpdateData();
			pPointerArray->Add(pWeppProcess);//0
			pPointerArray->Add((this)->m_hWnd);//1
			pPointerArray->Add(& m_ProgressCtrl);//2
			pPointerArray->Add(&m_MissingData);//3
			pPointerArray->Add(&m_Directory);//4
			pPointerArray->Add(&m_FileName);//5
			pPointerArray->Add(&m_Size);//6
			pPointerArray->Add(&m_Upslope);//7

			pPointerArray->Add(&sFullPathParam);//8
			pPointerArray->Add(&m_MaxSegLength);//9
			pPointerArray->Add(&bStopProcess);//10


			AfxBeginThread(RUN_PROCESS, pPointerArray);





		}

	}




	// TODO: Add extra validation here
	
	


	//CDialog::OnOK();
}




void CWeppMaprDlg::OnButtonDemfile() 
{

	UpdateData(true);
	CString sFileName;
	CString sFullFileName;
	CString str3CharName;
	CFileDialog* dlgFile = new CFileDialog(TRUE,"","",FALSE,"Database Files (*.dbf)|*.dbf");
	if(dlgFile->DoModal())
	{
		sFileName =  dlgFile->GetFileName() ;
		if(sFileName.GetLength() >0)
		{
			sFullFileName = dlgFile->GetPathName();
			m_FileName = sFullFileName;
			
			//Create a directory name
			int nLoop22=0;
			for(nLoop22=m_FileName.GetLength() -1;nLoop22>0;nLoop22--)
			{
				if((m_FileName.GetAt(nLoop22) =='\\') || (m_FileName.GetAt(nLoop22) =='//'))
				{
					
					m_Directory = m_FileName.Left(nLoop22 +1);
					nLoop22 = 0;
				}
			}
				
			//Create a 3 character prefix for the name
			str3CharName = m_FileName.Left(m_FileName.GetLength() - 7);
			str3CharName = str3CharName.Right(3);
			m_FileName = str3CharName;
			UpdateData(FALSE);
		}
	}
	// TODO: Add your control notification handler code here
	
	delete dlgFile;












	// TODO: Add your control notification handler code here
	
}

void CWeppMaprDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	bStopProcess = true;
}
