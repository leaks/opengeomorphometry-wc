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
// facetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "facet.h"
#include "facetDlg.h"
#include <afxdlgs.h>
#include "FacetProcess.h"
#include <afxtempl.h>
#include <afxcoll.h>
//#include "DbfFile.h"

#include <fstream>
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
	//ON_UPDATE_COMMAND_UI(ID_PERCENT_DONE, OnUpdatePercentDone)

END_MESSAGE_MAP()




LRESULT CFacetDlg::OnRecalcInProgress1(WPARAM wParam, LPARAM lParam)
{
	//AfxMessageBox("Here");
	m_STATIC_CURRENTPROCESS = *(CString*) lParam;
	UpdateData(false);
	if(wParam ==1)
	{
		delete (CString*) lParam;
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CFacetDlg dialog

CFacetDlg::~CFacetDlg()
{
	//delete pFacetProcess;
}

CFacetDlg::CFacetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFacetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFacetDlg)
	m_EDIT_DEM = _T("");
	m_EDIT_FUZZY_ATTRIBUTE_FILE = _T("");
	m_EDIT_FUZZY_FACETS = _T("");
	m_RADIO_1 = 0;
	m_EDIT_RELZFILE = _T("");
	m_EDIT_FORMFILE = _T("");
	m_STATIC_CURRENTPROCESS = _T("");
	m_EDIT_ZONEFILE = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFacetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFacetDlg)
	DDX_Control(pDX, IDC_BUTTON_ZONE, m_BUTON_ZONE);
	DDX_Control(pDX, IDOK, m_BUTTON_OK);
	DDX_Control(pDX, IDC_BUTTON_FUZZY_FACETS, m_BUTTON_FUZZY_FACETS);
	DDX_Control(pDX, IDC_BUTTON_FUZZY_ATTRIBUTES_FILE, m_BUTTON_FUZZY_ATTRIBUTE_FILE);
	DDX_Control(pDX, IDC_PROGRESS_BAR_OVERALL, m_PROGRESS_BAR2);
	DDX_Control(pDX, IDC_PROGRESS_BAR, m_PROGRESS_BAR1);
	DDX_Control(pDX, IDC_BUTTON_DEMFILE, m_BUTTON_DEMFILE);
	DDX_Text(pDX, IDC_EDIT_DEM, m_EDIT_DEM);
	DDX_Text(pDX, IDC_EDIT_FUZZY_ATTRIBUTES_FILE, m_EDIT_FUZZY_ATTRIBUTE_FILE);
	DDX_Text(pDX, IDC_EDIT_FUZZY_FACETS, m_EDIT_FUZZY_FACETS);
	DDX_Radio(pDX, IDC_RADIO_1, m_RADIO_1);
	DDX_Text(pDX, IDC_EDIT_RELZFILE, m_EDIT_RELZFILE);
	DDX_Text(pDX, IDC_EDIT_FORMFILE, m_EDIT_FORMFILE);
	DDX_Text(pDX, IDC_STATIC_CURRENTPROCESS, m_STATIC_CURRENTPROCESS);
	DDX_Text(pDX, IDC_EDIT_ZONEFILE, m_EDIT_ZONEFILE);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFacetDlg, CDialog)
	//{{AFX_MSG_MAP(CFacetDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_DEMFILE, OnButtonDemfile)
	ON_BN_CLICKED(IDC_BUTTON_FUZZY_ATTRIBUTES_FILE, OnButtonFuzzyAttributesFile)
	ON_BN_CLICKED(IDC_BUTTON_FUZZY_FACETS, OnButtonFuzzyFacets)
	ON_BN_CLICKED(IDC_BUTTON_RELZFILE, OnButtonRelzfile)
	ON_BN_CLICKED(IDC_BUTTON_FORMFILE, OnButtonFormfile)
	ON_BN_CLICKED(IDC_BUTTON_ZONE, OnButtonZone)
	ON_MESSAGE(WM_USER_RECALC_IN_PROGRESS, OnRecalcInProgress1)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFacetDlg message handlers

BOOL CFacetDlg::OnInitDialog()
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



	m_PROGRESS_BAR1.SetRange(0,100);
	m_PROGRESS_BAR1.SetPos(0);
	m_PROGRESS_BAR1.SetStep(1);
	m_PROGRESS_BAR2.SetRange(0,100);	
	m_PROGRESS_BAR2.SetPos(0);	
	m_PROGRESS_BAR2.SetStep(1);
	
	m_RADIO_1 = 0;
	
	//pFacetProcess = new CFacetProcess;
	
	/*Remove this for release*/
	//m_EDIT_DEM = "c:\\m1b\\m1bdem.DBF";
	//m_EDIT_FUZZY_ATTRIBUTE_FILE = "c:\\m1b\\LM3ARULE.DBF";
	//m_EDIT_FUZZY_FACETS = "c:\\m1b\\LM3CRULE.DBF";
	//m_EDIT_RELZFILE = "c:\\m1b\\m1brelz.DBF";
	//m_EDIT_FORMFILE = "c:\\m1b\\m1bform.dbf";
	//m_EDIT_ZONEFILE = "c:\\m1b\\m1bzone.dbf";
	m_STATIC_CURRENTPROCESS = "Current Process:";
	pFacetProcess = new CFacetProcess;
	pPointerArray = 0;
	UpdateData(FALSE);

	
	char*  s = new char[255];
	char full[_MAX_PATH];
	if( _fullpath( full, "FacetParam.txt", _MAX_PATH ) != NULL )
	{
		CString sPath = full;
		sFullPathParam = full;
		int nLoc = sPath.Find("FacetParam.txt");
		sPath = sPath.Left(nLoc);
		std::ifstream ParamFile(sPath + "FacetParam.txt");
		
		ParamFile.getline(s,255);
		if(strcmp(s,"FacetParam.txt")==0)
		{
			ParamFile.getline(s,255);
			strDirectory= s;
			ParamFile.getline(s,255);
			m_EDIT_DEM = s;
			ParamFile.getline(s,255);
			str3CharName = s;
			ParamFile.getline(s,255);
			m_EDIT_FORMFILE= s;
			ParamFile.getline(s,255);
			m_EDIT_FUZZY_ATTRIBUTE_FILE= s;
			ParamFile.getline(s,255);
			m_EDIT_FUZZY_FACETS= s;
			ParamFile.getline(s,255);
			m_EDIT_RELZFILE= s;
			ParamFile.getline(s,255);
			m_EDIT_ZONEFILE= s;
			ParamFile>>m_RADIO_1;
		}
		else
		{

		}
	}
	




	UpdateData(FALSE);










	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFacetDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFacetDlg::OnPaint() 
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
HCURSOR CFacetDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
UINT LSM_FUZA_Proc( LPVOID pParam )
{
	CPtrArray* pArray =  (CPtrArray*) pParam;
	//CArray* pArray = (CArray*) pParam;
	//CProgressCtrl* pProgressBar = (CProgressCtrl*) (long)p[0];
	//CFacetProcess* pFacetProcess = (CFacetProcess*) p[1];
	//int n = 0;
	//CArray* pArray2 = (CArray* ) p;
	//CProgressCtrl* pProgressBar = (CProgressCtrl*) pArray->GetAt(0);
	CFacetProcess* pFacetProcess = (CFacetProcess*) pArray->GetAt(5);
		//CDbfFile* pDBFFile = (CDbfFile*) pArray->GetAt(6);
		//CString* strP = (CString*)pArray->GetAt(3);
		//CString FileName = *strP;
	//pDBFFile->OpenFile(FileName,(CProgressCtrl*)pArray->GetAt(0));
	//exit(0);
	//DBFFile.OpenFile(m_EDIT_FUZZY_ATTRIBUTE_FILE,&m_PROGRESS_BAR1);
	pFacetProcess->LSM_FUZA(pArray);
	pFacetProcess->LSM_FUZC(pArray);
	pFacetProcess->LSM_MAXC(pArray);

	//AfxMessageBox("Exiting");
    return 0;   // thread completed successfully
}



UINT CALC_LSM_Proc( LPVOID pParam )
{
	CPtrArray* pArray =  (CPtrArray*) pParam;

	CFacetProcess* pFacetProcess = (CFacetProcess*) pArray->GetAt(5);

	pFacetProcess->CALC_LSM(pArray);


    return 0;   // thread completed successfully
}


UINT CALC_DSS_Proc( LPVOID pParam )
{
	CPtrArray* pArray =  (CPtrArray*) pParam;

	CFacetProcess* pFacetProcess = (CFacetProcess*) pArray->GetAt(5);

	pFacetProcess->CALC_DSS(pArray);


    return 0;   // thread completed successfully
}

void CFacetDlg::OnOK() 
{
	delete pFacetProcess;
	delete pPointerArray;
	pFacetProcess = new CFacetProcess;
	
	


	UpdateData(TRUE);







	std::ofstream fParamFile(sFullPathParam);

	fParamFile<<"FacetParam.txt"<<'\n';
	fParamFile<<strDirectory<<'\n';
	fParamFile<<m_EDIT_DEM<<'\n';
	fParamFile<<str3CharName <<'\n';
	fParamFile<<m_EDIT_FORMFILE<<'\n';
	fParamFile<<m_EDIT_FUZZY_ATTRIBUTE_FILE<<'\n';
	fParamFile<<m_EDIT_FUZZY_FACETS<<'\n';
	fParamFile<<m_EDIT_RELZFILE<<'\n';

	fParamFile<<m_EDIT_ZONEFILE<<'\n';
	fParamFile<<m_RADIO_1<<'\n';
	fParamFile.close();
	










	bKillThread = false;
	

	//CDbfFile* DBFFile = new CDbfFile;

	
	//int nColumnField2 = DBFFile.FindField("Field2");
	//DBFFile.~CDbfFile();

	//exit(0);
	

	m_PROGRESS_BAR2.SetPos(0);
	m_PROGRESS_BAR2.SetRange(0,11);
	m_PROGRESS_BAR2.SetStep(1);
	pPointerArray = new CPtrArray;



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





	//Parameter 1 - Progress Bar
	pPointerArray->Add(&m_PROGRESS_BAR1);
	//pPointerArray->Add( pFacetProcess);
	
	//Parameter 2 - ARule Structure
	//pPointerArray->Add(pARule);
	pPointerArray->Add( &m_PROGRESS_BAR2);
	if(m_EDIT_FORMFILE!="")
	{
		if(m_EDIT_RELZFILE!="")
			if(m_EDIT_DEM != "")

			{
				//Parameter 3 - DEM file name
				pPointerArray->Add(&m_EDIT_DEM );

				if(m_EDIT_FUZZY_ATTRIBUTE_FILE !="" ||(m_EDIT_FUZZY_ATTRIBUTE_FILE ==""
					&& m_RADIO_1 ==1))
				{
					//Parameter 4 - Fuzzy Attribute File
					pPointerArray->Add(&m_EDIT_FUZZY_ATTRIBUTE_FILE);
					if(m_EDIT_FUZZY_FACETS !="" ||(m_EDIT_FUZZY_FACETS ==""
						&& m_RADIO_1 ==1))
					{
						//Parameter 5 - Fuzzy Facets File
						pPointerArray->Add(&m_EDIT_FUZZY_FACETS);

						//Parameter 6 - FacetProcess object
						pPointerArray->Add(pFacetProcess);

						//Parameter 7 - The dbf file containing the data for relative relief and quinn wetness index
						
						pPointerArray->Add(&m_EDIT_RELZFILE);

						//Parameter 8 - Form File dbf
						pPointerArray->Add(&m_EDIT_FORMFILE);

						//Parameter 9
						pPointerArray->Add(&m_STATIC_CURRENTPROCESS);

						//Parameter 10
						pPointerArray->Add((this)->m_hWnd);
						//Parameter 11
						pPointerArray->Add(&m_EDIT_ZONEFILE);
						

						sGeoFileName = m_EDIT_ZONEFILE.Left(m_EDIT_ZONEFILE.GetLength() -8);
						sGeoFileName = sGeoFileName + "geo.dbf";
						
						//Parameter 12
						pPointerArray->Add(&sGeoFileName);
						

						//Parameter 13
						pPointerArray->Add(&strDirectory);


						//Parameter 14
						pPointerArray->Add(&str3CharName);


						//Parameter 15
						pPointerArray->Add(&bKillThread);
						switch(m_RADIO_1)
						{

						case 0:
							AfxBeginThread(LSM_FUZA_Proc, pPointerArray);
							break;
						case 1:
							AfxBeginThread(CALC_DSS_Proc,pPointerArray);
							break;
						case 2:
							AfxBeginThread(CALC_LSM_Proc,pPointerArray);
							break;
						}


						
					}
					else
					{
						MessageBox("Please select a valid fuzzy facets file.","DEM file not selected.");
					}
				}
				else
				{
					MessageBox("Please select a valid fuzzy attribute file.","DEM file not selected.");
				}
			}
			else
			{
				MessageBox("Please select a valid DEM file.","DEM file not selected.");
			}
		else
		{
			MessageBox("Please select a validThe dbf file containing the data for relative relief and quinn wetness index.");
		}
	}
	else
	{
		MessageBox("Please select a valid Form dbf file.","Form file not selected");
	}
	//MessageBox("Here","");
	//CDialog::OnOK();
	//delete []pPointerArray;
	//delete pFacetProcess;


}

void CFacetDlg::OnButtonDemfile() 
{
	UpdateData(true);
	CString sFileName;
	CString sFullFileName;
	CFileDialog* dlgFile = new CFileDialog(TRUE,"","",FALSE,"Database Files (*.dbf)|*.dbf");
	if(dlgFile->DoModal())
	{
		sFileName =  dlgFile->GetFileName() ;
		if(sFileName.GetLength() >0)
		{
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
			
			//m_EDIT_FUZZY_ATTRIBUTE_FILE = "c:\\m1b\\LM3ARULE.DBF";
			//m_EDIT_FUZZY_FACETS = "c:\\m1b\\LM3CRULE.DBF";
			m_EDIT_RELZFILE = strDirectory + str3CharName + "relz.dbf";
			m_EDIT_FORMFILE = strDirectory + str3CharName + "form.dbf";
			m_EDIT_ZONEFILE = strDirectory + str3CharName + "zone.dbf";		
			
			
			
			
			
			UpdateData(FALSE);
		}
	}
	// TODO: Add your control notification handler code here
	
	delete dlgFile;
	
}

void CFacetDlg::OnButtonFuzzyAttributesFile() 
{
	UpdateData(true);
	// TODO: Add your control notification handler code here
	CString sFileName;
	CString sFullFileName;
	CFileDialog* dlgFile = new CFileDialog(TRUE,"","",FALSE,"Database Files (*.dbf)|*.dbf");
	
	if(dlgFile->DoModal())
	{

		sFileName =  dlgFile->GetFileName() ;
		if(sFileName.GetLength()>0)
		{
			sFullFileName = dlgFile->GetPathName();
			m_EDIT_FUZZY_ATTRIBUTE_FILE = sFullFileName;
			UpdateData(FALSE);
		}
	}
	// TODO: Add your control notification handler code here
	
	delete dlgFile;	
}

void CFacetDlg::OnButtonFuzzyFacets() 
{
	UpdateData(true);
	// TODO: Add your control notification handler code here
	CString sFileName;
	CString sFullFileName;
	CFileDialog* dlgFile = new CFileDialog(TRUE,"","",FALSE,"Database Files (*.dbf)|*.dbf");
	
	if(dlgFile->DoModal())
	{
		sFileName =  dlgFile->GetFileName() ;
		if(sFileName.GetLength()>0)
		{
			sFullFileName = dlgFile->GetPathName();
			m_EDIT_FUZZY_FACETS= sFullFileName;
			UpdateData(FALSE);
		}
	}
	// TODO: Add your control notification handler code here
	
	delete dlgFile;		
}



void CFacetDlg::OnButtonRelzfile() 
{
	UpdateData(true);
	CString sFileName;
	CString sFullFileName;
	CFileDialog* dlgFile = new CFileDialog(TRUE,"","",FALSE,"Database Files (*.dbf)|*.dbf");
	if(dlgFile->DoModal())
	{
		sFileName =  dlgFile->GetFileName() ;
		sFullFileName = dlgFile->GetPathName();
		m_EDIT_RELZFILE = sFullFileName;
		UpdateData(FALSE);
	}
	
	delete dlgFile;	
}

void CFacetDlg::OnButtonFormfile() 
{
	CString sFileName;
	CString sFullFileName;
	CFileDialog* dlgFile = new CFileDialog(TRUE,"","",FALSE,"Database Files (*.dbf)|*.dbf");
	if(dlgFile->DoModal())
	{
		sFileName =  dlgFile->GetFileName() ;
		sFullFileName = dlgFile->GetPathName();
		m_EDIT_FORMFILE = sFullFileName;
		UpdateData(FALSE);
	}
	
	delete dlgFile;	
	
}

void CFacetDlg::CUpdateData(BOOL bValue)
{
	UpdateData(bValue);

}

void CFacetDlg::OnButtonZone() 
{
	CString sFileName;
	CString sFullFileName;
	CFileDialog* dlgFile = new CFileDialog(TRUE,"","",FALSE,"Database Files (*.dbf)|*.dbf");
	if(dlgFile->DoModal())
	{
		sFileName =  dlgFile->GetFileName() ;
		sFullFileName = dlgFile->GetPathName();
		m_EDIT_ZONEFILE = sFullFileName;
		UpdateData(FALSE);
	}
	
	delete dlgFile;	
	
}

void CFacetDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	//CDialog::OnCancel();
	bKillThread = true;
}

void CFacetDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	delete pPointerArray;
	delete pFacetProcess;
	bKillThread = true;
	exit(1);
}
