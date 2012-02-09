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
// landmapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "landmap.h"
#include "landmapDlg.h"
#include "DbfFile.h"
#include "DEMProcess.h"
#include "Sort.h"
#include <assert.h>
#include <math.h>

//#include "GridIO.h"



#include <time.h>
#include <direct.h>
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
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CLandmapDlg dialog

CLandmapDlg::CLandmapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLandmapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLandmapDlg)
	m_FileName = _T("");
	m_LLX = 0.0;
	m_LLY = 0.0;
	m_MissDataValue = 0.0f;
	m_PitDepth = 0.0;
	m_PitArea = 0;
	m_PitVolumn = 0.0;
	m_Row = 0;
	m_GridSize = 0.0f;
	m_Column = 0;
	m_WorkingDirectory = _T("");
	m_STATIC_CURRENTPROCESS = _T("");
	m_InvertedElev = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLandmapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLandmapDlg)
	DDX_Control(pDX, IDC_PROGRESS2, m_PROGRESS2);
	DDX_Control(pDX, IDC_PROGRESS1, m_PROGRESS1);
	DDX_Text(pDX, IDC_FILENAME, m_FileName);
	DDX_Text(pDX, IDC_MISSDATA, m_MissDataValue);
	DDX_Text(pDX, IDC_PITDEPTH, m_PitDepth);
	DDX_Text(pDX, IDC_PITAREA, m_PitArea);
	DDX_Text(pDX, IDC_ROW, m_Row);
	DDX_Text(pDX, IDC_GSIZE, m_GridSize);
	DDX_Text(pDX, IDC_COLUMN, m_Column);
	DDX_Text(pDX, IDC_EDIT_WORKING_DIRECTORY, m_WorkingDirectory);
	DDX_Text(pDX, IDC_STATIC_CURRENTPROCESS, m_STATIC_CURRENTPROCESS);
	DDX_Check(pDX, IDC_CHECK_INVERTED, m_InvertedElev);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLandmapDlg, CDialog)
	//{{AFX_MSG_MAP(CLandmapDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_USER_RECALC_IN_PROGRESS, OnRecalcInProgress1)
	ON_BN_CLICKED(IDC_BUTTON_DEMFILE, OnButtonDemfile)
	ON_BN_CLICKED(IDC_BUTTON_Cancel, OnBUTTONCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLandmapDlg message handlers

BOOL CLandmapDlg::OnInitDialog()
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
	

	m_Column = 0;
	m_Row = 0;
	m_FileName = "";
	m_GridSize = 10;
	m_PitArea = 10;
	m_PitDepth = 0.15;
	m_WorkingDirectory = "";
	m_MissDataValue = -9999;
	char full[_MAX_PATH];

	if( _fullpath( full, "Config.txt", _MAX_PATH ) != NULL )
	{



	

		//ifstream flConfig("Config.txt");
		//char* pszWorkingDirectory = new char[255];
		//flConfig>>pszWorkingDirectory;
		
		char* pszWorkingDirectory = new char[255];

				
		sFullCurrentPath = full;
		std::ifstream flConfig(full);
		flConfig.getline(pszWorkingDirectory,255);
		if(!strcmp(pszWorkingDirectory,"Config.txt"))
		{
		
			flConfig.getline(pszWorkingDirectory,255);

			m_WorkingDirectory = pszWorkingDirectory;
			flConfig>>m_Column;
		
			flConfig>>m_Row;;
		
			flConfig.getline(pszWorkingDirectory,255);
			flConfig.getline(pszWorkingDirectory,255);
			m_FileName = pszWorkingDirectory;
			flConfig>>m_GridSize;
		
			flConfig>>m_PitArea;

			flConfig>>m_PitDepth;


			flConfig>>m_MissDataValue;
			
		}
		delete pszWorkingDirectory;
	}

	bKillThread = false;
	m_InvertedElev = true;
	UpdateData(false);
	m_STATIC_CURRENTPROCESS = "Current Process:";

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLandmapDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLandmapDlg::OnPaint() 
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
HCURSOR CLandmapDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/*
UINT RUN_FLOW( LPVOID pParam )
{

		
	CDEMProcess	*pDEMProcess;
	long* pParams = (long*) pParam;
	pDEMProcess = (CDEMProcess	*)pParams[0];
	double* pfDEMGrid = (double*) pParams[1];
	short* piFlowGrid = (short*)pParams[2];
	pDEMProcess->FlowDir(pfDEMGrid, piFlowGrid);
    return 0;   // thread completed successfully
}


UINT RUN_FIXFLAT( LPVOID pParam )
{

		
	CDEMProcess	*pDEMProcess;
	long* pParams = (long*) pParam;
	pDEMProcess = (CDEMProcess	*)pParams[0];
	double* pfDEMGrid = (double*) pParams[1];
	short* piFlowGrid = (short*)pParams[2];
	pDEMProcess->FixFlat(pfDEMGrid, piFlowGrid);
    return 0;   // thread completed successfully
}
UINT RUN_CALCDREC( LPVOID pParam )
{

		
	CDEMProcess	*pDEMProcess;
	long* pParams = (long*) pParam;
	pDEMProcess = (CDEMProcess	*)pParams[0];
	double* pfDEMGrid = (double*) pParams[1];
	short* piFlowGrid = (short*)pParams[2];
	int* piDrecGrid = (int*)pParams[4];

	pDEMProcess->CalcDrec(piFlowGrid, piDrecGrid);
    return 0;   // thread completed successfully
}
UINT RUN_CIRCLEFLOW( LPVOID pParam )
{

		
	CDEMProcess	*pDEMProcess;
	long* pParams = (long*) pParam;
	pDEMProcess = (CDEMProcess	*)pParams[0];
	double* pfDEMGrid = (double*) pParams[1];
	short* piFlowGrid = (short*)pParams[2];
	int* piDrecGrid = (int*)pParams[4];

	pDEMProcess->CircleFlow(piFlowGrid, piDrecGrid);
    return 0;   // thread completed successfully
}

UINT RUN_CALCWATERSHED( LPVOID pParam )
{

		
	CDEMProcess	*pDEMProcess;
	long* pParams = (long*) pParam;
	pDEMProcess = (CDEMProcess	*)pParams[0];
	double* pfDEMGrid = (double*) pParams[1];
	short* piFlowGrid = (short*)pParams[2];
	int* piDrecGrid = (int*)pParams[4];
	int* piShedGrid = (int*) pParams[5];
	int* piDEMSort = (int*) pParams[6];

	pDEMProcess->CalcWaterShed(pfDEMGrid, piFlowGrid, piDrecGrid, piShedGrid, piDEMSort);
    return 0;   // thread completed successfully
}

UINT RUN_CALCUPSLOPEAREA( LPVOID pParam )
{

		
	CDEMProcess	*pDEMProcess;
	long* pParams = (long*) pParam;
	pDEMProcess = (CDEMProcess	*)pParams[0];
	double* pfDEMGrid = (double*) pParams[1];
	short* piFlowGrid = (short*)pParams[2];
	int* piDrecGrid = (int*)pParams[4];
	int* piShedGrid = (int*) pParams[5];
	int* piDEMSort = (int*) pParams[6];
	int* piUpSlpGrid = (int*) pParams[7];
	pDEMProcess->CalcUpSlopeArea(pfDEMGrid, piFlowGrid, piDrecGrid, piUpSlpGrid, piDEMSort);
    return 0;   // thread completed successfully
}

UINT RUN_VOLUME2FLOOD( LPVOID pParam )
{

		
	CDEMProcess	*pDEMProcess;
	long* pParams = (long*) pParam;
	pDEMProcess = (CDEMProcess	*)pParams[0];
	double* pfDEMGrid = (double*) pParams[1];
	short* piFlowGrid = (short*)pParams[2];
	int* piDrecGrid = (int*)pParams[4];
	int* piShedGrid = (int*) pParams[5];
	int* piDEMSort = (int*) pParams[6];
	int* piUpSlpGrid = (int*) pParams[7];
	LSMSTATVAR* pLsmStatVar = (LSMSTATVAR*) pParams[8];
	float* pfVol2FlGrid = (float*) pParams[9];
	
	double* pfMm2FlGrid = (double*) pParams[10];
	int* pfPArea = (int*) pParams[11];
	int* piShedWGrid = (int*) pParams[12];
	int iPitNo = *(int*) pParams[13];




	pDEMProcess->Volume2Flood(piDEMSort,pfDEMGrid, pLsmStatVar,
		pfVol2FlGrid, pfMm2FlGrid, pfPArea, piShedWGrid, piDEMSort, piDrecGrid, iPitNo, piFlowGrid);
    return 0;   // thread completed successfully
}

*/
LRESULT CLandmapDlg::OnRecalcInProgress1(WPARAM wParam, LPARAM lParam)
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





UINT RUN_PROCESS(LPVOID pParam)
{
	CString* sMessage = new CString;
	CPtrArray* pArray =  (CPtrArray*) pParam;

	CString m_WorkingDirectory = *((CString*) pArray->GetAt(0));
	CString m_FileName = *((CString*) pArray->GetAt(1));
	int m_Row = *((int*) pArray->GetAt(2));
	int m_Column = *((int*) pArray->GetAt(3));
	double m_GridSize = *((double*) pArray->GetAt(4));
	double m_MissDataValue = *((double*) pArray->GetAt(5));
	long m_PitArea = *((long*) pArray->GetAt(6));
	double m_PitDepth = *((double*) pArray->GetAt(7));
	double m_PitVolumn = *((double*) pArray->GetAt(8));
	
	bool* pbKillThread = ((bool*) pArray->GetAt(13));

	bool* pbInvertedElev = ((bool*) pArray->GetAt(14));
	
	if(*pbKillThread == true)
	{
		AfxEndThread(0);	
	}
	

	CString FullPathTest = *(CString*) pArray->GetAt(12);
	CLandmapDlg* dlgProgressBar = ((CLandmapDlg*) pArray->GetAt(9));
	
	CWnd* pWndNotifyProgress;

	HWND phWnd = (HWND) pArray->GetAt(9);
	pWndNotifyProgress = CWnd::FromHandle((HWND)pArray->GetAt(9));
	CProgressCtrl* pProgressBar = (CProgressCtrl*) pArray->GetAt(11);
	CProgressCtrl* pProgressBar2 =(CProgressCtrl*) pArray->GetAt(10);
	pProgressBar2->SetRange(0,6);
	pProgressBar2->SetStep(1);
	pProgressBar2->SetPos(0);

	pProgressBar->SetRange(0,26);
	pProgressBar->SetStep(1);
	pProgressBar->SetPos(0);
	

	/*
	plParameters[0] = (long) &m_WorkingDirectory;
	plParameters[1] = (long) & m_FileName;
	plParameters[2] = (long) & m_Row;
	plParameters[3] = (long) & m_Column;
	plParameters[4] = (long) & m_GridSize;
	plParameters[5] = (long) & mMissDataValue;
	plParameters[6] = (long) & m_PitArea;
	plParameters[7] = (long) & m_PitDepth;
	plParameters[8] = (long) & m_PitVolumn;
	*/

	double * pElev;
	pElev = 0;
	//CGridIO oGrid;
	//oGrid.ReadGrid(3,"d:\\nwgrd1","d:\\fixedgrid13");
	//clock_t start, finish;

	//CLOCKS_PER_SEC = 1000;
	//start = clock();
	



	//finLoc.


	

	CDbfFile	*pDbfFile;
	CDbfFile	*pOutDbfFile;
	CDEMProcess	*pDEMProcess;
	double		*pfTmpDEMGrid;
	double		*pfDEMGrid;
	double		*pfBottomUp;
	//double		*pfBottomUp1;
	short		*piFlowGrid;
	int			*piShedGrid;
	int			*piShedWGrid;
	int			*piUpSlpGrid;
	int			*piDrecGrid;
	int			*piDEMSort;
	//FILE		*hInpf;
	CString		sTmp;
	CString		sTmp1;
	int			i,  ii;
	
	//clock_t start, finish;
	//double duration;
	//start = clock();


	//*********************************
	//IGK
	//3 Arrays used in calculation of volume to flood, mm to flood, and pArea
	double		*pfVol2FlGrid;
	double		*pfMm2FlGrid;
	int		*pfPArea;

	//**********************************


	

	//AfxMessageBox(s);

	if ((pfDEMGrid = new double[m_Row*m_Column]) == NULL)
	{
		AfxMessageBox("Not enough memory for DEM grid!");
		exit(1);
	}

	sTmp = m_WorkingDirectory + m_FileName + ".dbf";


//**************************************************************

	*sMessage = "Opening input file...";
	pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);	


	int m_Col = m_Column;

	BOOL bValid = TRUE;

	char szBuffer [255];	

	CFile myFile(sTmp, CFile::modeRead );

	myFile.Seek(4,CFile::begin);
	myFile.Read(szBuffer,4);

	int* pShortInt = new int;
	*pShortInt = *(int*) &szBuffer;
	

	myFile.Seek(32,CFile::begin);
	int* nRowLocation = new int;
	*nRowLocation = -1;
	int* nColLocation = new int;
	*nColLocation = -1;
	int* nNumFields = new int;
	* nNumFields = 0;
	int* nElevLocation = new int;
	*nElevLocation = -1;
	int* nDdirLocation = new int;
	*nDdirLocation = -1;
	int* nDrecLocation = new int;
	*nDrecLocation = -1;
	int* nUpSlpLocation = new int;
	*nUpSlpLocation = -1;
	int* pnShedNowLocation = new int;
	*pnShedNowLocation = -1;
	

	int nFieldLength[35];

	for(int nLoop = 0;nLoop<100;nLoop++)
	{
		myFile.Read(szBuffer,32);
		if(szBuffer[0] !=13)
		{

			if(_stricmp(szBuffer,"elev")==0)
			{
				*nElevLocation = nLoop;
				*nColLocation = nLoop;
				//nLoop = 100;
			}


			nFieldLength[nLoop] = szBuffer[16];
		}
		else
		{
			nLoop = 100;
		}
	}
	
	if(*nElevLocation == -1)
	{
		AfxMessageBox("Could not locate elev field.");
		AfxEndThread(1);
	}

	myFile.Seek(8,CFile::begin);
	myFile.Read(szBuffer,2);

	short int* piFirstRecordLoc = new short int;
	*piFirstRecordLoc = *(short int*)&szBuffer ;
	
	myFile.Seek(10,CFile::begin);
	myFile.Read(szBuffer,2);

	short int* piRecordLength = new short int;
	*piRecordLength = *(short int*) &szBuffer;

	
	myFile.Seek(*piFirstRecordLoc + 
		(*pShortInt -1) * (*piRecordLength),CFile::begin);
	myFile.Read(szBuffer,*piRecordLength);
	
	char szNumBuffer[8] = {'0','0','0','0','0','0','0','0'};
	
	int nBitesOfSet = 0;



	//for(nLoop=0;nLoop<*nColLocation;nLoop++)
	//{
	//	nBitesOfSet = nBitesOfSet + nFieldLength[nLoop];
	//}

	int nIndex =0;
	//char* szP = new char[nFieldLength[*nColLocation]];
	//for(nLoop=0;nLoop<nFieldLength[*nColLocation];nLoop++)
	//{

	//	szP[nLoop] = szBuffer[nBitesOfSet +1 +nLoop];
			
		

	//}

	//char szBuffTemp[8] = {'0','0','0','0','0','0','0','0'};

	//for(nLoop = 0;nLoop<8;nLoop++)
	//{
	//	szBuffTemp[nLoop] = szNumBuffer[nLoop];

	//}
	//m_Col = (int)atof(szP);

	
	//nIndex = 0;
	//nBitesOfSet =0;
	//for(nLoop=0;nLoop<*nRowLocation;nLoop++)
	//{
	//	nBitesOfSet = nBitesOfSet + nFieldLength[nLoop];
	//}


	//delete szP;

	//szP = new char [nFieldLength[*nRowLocation]];
	//for(nLoop=0;nLoop<nFieldLength[*nRowLocation];nLoop++)
	//{

	//	szP[nLoop] = szBuffer[nBitesOfSet +1 +nLoop];
			


	//}
	
	//for(nLoop=0;nLoop<8;nLoop++)
	//{
	//	szBuffTemp[nLoop] = '0';
	//}

	//for(nLoop = 0;nLoop<8;nLoop++)
	//{
	//	szBuffTemp[nLoop] = szNumBuffer[nLoop];

	//}
	//m_Row = (int)atof(szP);
	//delete szP;




	//Read fields Elev, Drec, DDir, UpSLp from dbf table
	//***********************************************************
	double* pTemp = new double [m_Row*m_Col];
	//double* pdElev;
	int* pTemp1 = new int [m_Row*m_Col];
	short* pTemp2 = new short int [m_Row*m_Col];
	int* pTemp3= new int [m_Row*m_Col];
	//piShedNow = new int[m_Row*m_Col];

	nBitesOfSet =0;
	int nUpSlpBitesOfSet = 0;
	int nDrecBitesOfSet = 0;
	int nDdirBitesOfSet= 0;
	int nShedNowBitesOfSet = 0;

	for(int nLoop=0;nLoop<*nElevLocation;nLoop++)
	{
		nBitesOfSet = nBitesOfSet + nFieldLength[nLoop];
	}


	
	myFile.Seek(*piFirstRecordLoc,CFile::begin);
	
	char* szElevBuffer = new char [255];
	char* szDrecBuffer = new char [255];
	char* szDdirBuffer = new char [255];
	char* szUpSlpBuffer = new char [255];
	
	char* szShedNowBuffer = new char[255];

	for(int nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{

		myFile.Read(szBuffer,*piRecordLength);

		for(nIndex=0;nIndex<nFieldLength[*nElevLocation];nIndex++)
		{
			szElevBuffer[nIndex] = szBuffer[nBitesOfSet +1 +nIndex];

		}
		szElevBuffer[nIndex] = 0;
		pTemp[nLoop] = atof(szElevBuffer);


		szShedNowBuffer[nIndex] = 0;
		//piShedNow[nLoop] = atoi(szShedNowBuffer);
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}
	}
	

	for(int nLoop=0;nLoop<m_Row*m_Column;nLoop++)
	{	
		pfDEMGrid[nLoop] = pTemp[nLoop];
	}
	//piDrec = pTemp1;
	//piDdir = pTemp2;


	//piUpSlope = pTemp3;


	myFile.Close();		
	//piSeedType = new short int[m_Row*m_Col];

	//if(piSeedType ==NULL) 
	//{
	//	AfxMessageBox("Not enough memory for seed type array.");
	//	exit(0);
	//}

	//piChan_No = new int [m_Row*m_Col];

	//if(piChan_No ==NULL)
	//{
	//	AfxMessageBox("Not enough memory for Chan_No array.");
	//	exit(0);
	//}

	
	delete nUpSlpLocation;
	delete nDrecLocation;
	delete nDdirLocation;
	delete szUpSlpBuffer;
	delete szDdirBuffer;
	delete szDrecBuffer;
	delete piRecordLength;
	delete nRowLocation;
	delete nColLocation;
	delete pShortInt;
	delete piFirstRecordLoc;
	delete nElevLocation;
	delete nNumFields;
	delete szElevBuffer;
	delete szShedNowBuffer;
	delete pnShedNowLocation;
	delete pTemp;
	delete pTemp1;
	delete pTemp2;	
	delete pTemp3;



	







//********************************************************************88
































	//if ((hInpf = fopen(sTmp, "r")) == NULL)
	


	if(0)
	{
		AfxMessageBox("Unable open input data file");
		//exit(1);
		//delete pDEMProcess;
		//delete pDbfFile;
		//delete pfDEMGrid;
		//delete pfTmpDEMGrid;
		//delete piDEMSort;
		
		//pDEMProcess=0;
	}
	else
	{
		//fclose(hInpf);
	
		if((pDbfFile = new CDbfFile())== NULL)
		{
			AfxMessageBox("Not enough memory for the dbf files!");
			exit(1);
		}
		pDEMProcess = new CDEMProcess(m_Row, m_Column, m_GridSize);
		pDEMProcess->dMissingValue = m_MissDataValue;

		assert(m_FileName != "");
		
		if (m_FileName.Find(".") > 0)
			m_FileName.Delete(m_FileName.Find("."), 4);
		

		if ((pfTmpDEMGrid =  new double[m_Row*m_Column]) == NULL)
		{
			AfxMessageBox("Not enough memory for Tmp DEM grid!");
			exit(1);
		}
		if ((piDEMSort = new int[m_Row*m_Column]) == NULL)
		{
			AfxMessageBox("Not enough memory for sort grid!");
			exit(1);
		}

		//char full[_MAX_PATH];
		//_fullpath( full, "LandMap.exe", _MAX_PATH);
		//CString sPath = full;
		//int nLoc = sPath.Find("LandMap.exe");
		//sPath = sPath.Left(nLoc);

		std::ofstream flConfig(FullPathTest);
		char* pszWorkingDirectory = new char[255];
		flConfig<<"Config.txt"<<'\n';
		flConfig<<m_WorkingDirectory.GetBuffer(0);


		//_itoa(m_Column,pszWorkingDirectory,10);
		
		flConfig<<'\n' <<m_Column;
		//_itoa(m_Row,pszWorkingDirectory,10);

		flConfig<<'\n' << m_Row;
		//m_Row = atoi(pszWorkingDirectory);
		//flConfig.get(pszWorkingDirectory,255);
		//m_FileName = pszWorkingDirectory;
		flConfig<<'\n'<<m_FileName;
		//m_GridSize = atoi(pszWorkingDirectory);
		flConfig<<'\n'<<m_GridSize;
	//	m_PitArea = atoi(pszWorkingDirectory);
		flConfig<<'\n'<<m_PitArea;
		//m_PitDepth = atof(pszWorkingDirectory);

		flConfig<<'\n'<<m_PitDepth;
		//m_MissDataValue = atof(pszWorkingDirectory);

		flConfig<<'\n'<<m_MissDataValue<<'\n';
		delete pszWorkingDirectory;


		
		//dlgProgressBar->m_UserMessage ="Creating Grid...";
		////dlgProgressBar->UpdateData(false);
		//pDbfFile->Dbf2Grid(pfDEMGrid, hInpf, m_Row, m_Column);
		pProgressBar->StepIt();
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}
		//fclose (hInpf);
		//dlgProgressBar->m_ProgressBar1.StepIt();
		//dlgProgressBar->m_ProgressBar2.StepIt();
		
		int* pTempSort = new int[m_Row*m_Column];//temp array to hold index of records with missing elevation field
		int nMissingNum = 0;
		int nIndex = 0;
		int nIndex1 = 0;

		for (i = 0; i < m_Row * m_Column; i++)
		{
			if(!(pfDEMGrid[i]==m_MissDataValue))//if value is not missing
			{
				pfTmpDEMGrid[nIndex] = pfDEMGrid[i];
				piDEMSort[nIndex] = i;
				nIndex++;

			}
			else
			{
				pTempSort[nMissingNum] = i;
				nMissingNum++; //keep count of missing values

			}
		}

		
		QuickSort(pfTmpDEMGrid, m_Row * m_Column-nMissingNum, piDEMSort,true); //sort only non missing values
		free (pfTmpDEMGrid);
		nIndex = 0;
		for(i=m_Row*m_Column-nMissingNum;i<m_Row*m_Column;i++) //make the rest of pointers which are
			// pointing to the missing values, valid. 
			
		{
			piDEMSort[i] = pTempSort[nIndex];
			nIndex++;

		}
		delete(pTempSort);
		//dlgProgressBar->m_ProgressBar1.StepIt();
		//dlgProgressBar->m_ProgressBar2.StepIt();

		
		
		


		//***********************************************************************************
		//IGK
		if ((pfVol2FlGrid = new double [m_Row*m_Column])==NULL)
		{
			AfxMessageBox("Not enough memory for volume to flood grid!");
			exit(1);
		}

		if ((pfMm2FlGrid = new double [m_Row*m_Column])==NULL)
		{
			AfxMessageBox("Not enough memory for mm to flood to flood grid!");
			exit(1);
		}

		if ((pfPArea = new int [m_Row*m_Column])==NULL)
		{
			AfxMessageBox("Not enough memory for pArea grid!");
			exit(1);
		}
		//************************************************************************************


		if ((piFlowGrid = new short int [m_Row*m_Column]) == NULL)
		{
			AfxMessageBox("Not enough memory for flow direction grid!");
			exit(1);
		}



		//dlgProgressBar->m_UserMessage  = "Calculating flow direction...";
		////dlgProgressBar->UpdateData(false);

		//plParameters[0] = (long) pDEMProcess;
		//plParameters[1] = (long) pfDEMGrid;
		//plParameters[2] = (long) piFlowGrid;
		//plParameters[3] = (long) &dlgProgressBar;

		//AfxBeginThread(RUN_FLOW, plParameters);
		*sMessage = "Calculating flow direction...";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pDEMProcess->FlowDir(pfDEMGrid, piFlowGrid);
		pProgressBar->StepIt();
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}
		//dlgProgressBar->m_ProgressBar1.StepIt();


		//AfxBeginThread(RUN_FIXFLAT, plParameters);
		*sMessage = "Running FixFlat...";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pDEMProcess->FixFlat(pfDEMGrid, piFlowGrid);
		pProgressBar->StepIt();
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}
		//dlgProgressBar->m_ProgressBar1.StepIt();
		//dlgProgressBar->m_ProgressBar2.StepIt();

		if ((piDrecGrid = new int[m_Row*m_Column]) == NULL)
		{
			AfxMessageBox("Not enough memory for down slope record grid!");
			exit(1);
		}

		//plParameters[4] = (long) piDrecGrid;
		//AfxBeginThread(RUN_CALCDREC, plParameters);
		*sMessage = "Running CalcDrec...";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pDEMProcess->CalcDrec(piFlowGrid, piDrecGrid);
		pProgressBar->StepIt();
		//dlgProgressBar->m_ProgressBar1.StepIt();
		//dlgProgressBar->m_ProgressBar2.StepIt();
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}

		//AfxBeginThread(RUN_CIRCLEFLOW, plParameters);
		*sMessage = "Running CircleFlow...";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pDEMProcess->CircleFlow(piFlowGrid, piDrecGrid);
		pProgressBar->StepIt();
		//dlgProgressBar->m_ProgressBar1.StepIt();
		//dlgProgressBar->m_ProgressBar2.StepIt();
		



		if ((piShedGrid = new int [m_Row*m_Column]) == NULL)
		{
			AfxMessageBox("Not enough memory for watershed grid!");
			exit(1);
		}

		
		//plParameters[5] = (long)piShedGrid;
		//plParameters[6] = (long)piDEMSort;

		//AfxBeginThread(RUN_CALCWATERSHED, plParameters);
		*sMessage = "Calculating water sheds...";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pDEMProcess->CalcWaterShed(pfDEMGrid, piFlowGrid, piDrecGrid, piShedGrid, piDEMSort);
		pProgressBar->StepIt();		





		//dlgProgressBar->m_ProgressBar1.StepIt();
		//dlgProgressBar->m_ProgressBar2.StepIt();



		if ((piUpSlpGrid = new int [m_Row*m_Column]) == NULL)
		{
			AfxMessageBox("Not enough memory for up slope grid!");
			exit(1);
		}
		

		//dlgProgressBar->m_UserMessage = "Calculating upslope area...";
		////dlgProgressBar->UpdateData(false);

		
		//plParameters[7] = (long)piUpSlpGrid;
		//AfxBeginThread(RUN_CALCUPSLOPEAREA, plParameters);
		*sMessage = "Calculating upslope area...";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pDEMProcess->CalcUpSlopeArea(pfDEMGrid, piFlowGrid, piDrecGrid, piUpSlpGrid, piDEMSort);
		pProgressBar->StepIt();
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}
		//dlgProgressBar->m_ProgressBar1.StepIt();
		//dlgProgressBar->m_ProgressBar2.StepIt();


		CStringArray	*psDBFValue;
		psDBFValue = new CStringArray();
		//LSMPROCPARM	*pLsmProcParm;

		//if ((pLsmProcParm = new LSMPROCPARM) == NULL)
		//{
		//	AfxMessageBox("Not enough memory for default LSM parameters!");
		///	exit(1);
		//}
		//pLsmProcParm->fMaxVol = 10.0;
		//pLsmProcParm->iMaxArea = 10;
		//pLsmProcParm->fMaxDepth = (double)0.15;
		//pLsmProcParm->fMaxRunOff= 300.0;
		

		int		iPitNo = 0;
		LSMSTATVAR	*pLsmStatVar;
		LSMSTATVAR2	*pLsmStatVar2;

		if ((pfBottomUp = new double [m_Row*m_Column]) == NULL)
		{
			AfxMessageBox("Not enough memory for bottom up index grid!");
			exit(1);
		}
		/*
		if ((pfBottomUp1 = (double *) malloc ((m_Row * m_Column) * sizeof (double))) == NULL)
		{
			AfxMessageBox("Not enough memory for backup bottom up index grid!");
			exit(1);
		}*/
		if ((piShedWGrid = new int [m_Row*m_Column]) == NULL)
		{
			AfxMessageBox("Not enough memory for merge watershed grid!");
			exit(1);
		}
		
		int* piSortedTemp = new int [m_Row*m_Column];//temp array to hold index of records with missing elev field
		
		nIndex = 0;
		nMissingNum = 0;

		for (i = 0; i < m_Row * m_Column; i++)
		{
			piShedWGrid[i] = piShedGrid[i];

			if (piFlowGrid[i] == 5)
				iPitNo++;
			if(!(pfDEMGrid[i] ==m_MissDataValue)) //make sure that value for elevation is not missing
			{
			piShedWGrid[i] = piShedGrid[i];
			pfBottomUp[nIndex] = (double)(((double)piShedWGrid[i] * pow(10.0, 18.0)) +
							((double)pfDEMGrid[i] * pow(10.0, 12.0)) + 
							(1000000.0 - (double)piUpSlpGrid[i]));
			//pfBottomUp1[nIndex] = pfBottomUp[nIndex];
			piDEMSort[nIndex] = i;
			nIndex++;
			}
			else
			{
				piSortedTemp[nMissingNum] = i;//keep track of missing value records 
				nMissingNum ++;

			}

		}
		pProgressBar->StepIt();



		if ((pLsmStatVar = (LSMSTATVAR *) malloc (iPitNo * sizeof (LSMSTATVAR))) == NULL)
		{
			AfxMessageBox("Not enough memory for LSM statistic 1!");
			exit(1);
		}

		QuickSort(pfBottomUp, m_Row * m_Column - nMissingNum, piDEMSort);//sort only records that have valid values
		nIndex = 0;
		for(i = m_Row*m_Column-nMissingNum;i<m_Row*m_Column;i++)//make the rest of sorted array point to 
			//records that have missing values. 
		{
			piDEMSort[i] = piSortedTemp[nIndex];
			nIndex++;
		}
		delete piSortedTemp;
		//dlgProgressBar->m_ProgressBar1.StepIt();

		int nPercentage = 10;
		//dlgProgressBar->m_ProgressBar1.SetPos(0);
		//dlgProgressBar->m_ProgressBar1.SetRange(0,10);
		//dlgProgressBar->m_UserMessage = "Generating watershed statistics...";
		////dlgProgressBar->UpdateData(false);
		//UpdateData(false);
		iPitNo = 0;
		for (ii = 0; ii < m_Row * m_Column; ii++)
		{	
			if (ii ==int(((m_Row * m_Column)/100)*nPercentage))
			{
				//dlgProgressBar->m_ProgressBar1.StepIt();
				nPercentage = nPercentage + 10;
			}
			if (piFlowGrid[piDEMSort[ii]] == 5 && (!(pfDEMGrid[piDEMSort[ii]]==m_MissDataValue)))
			{
				pDEMProcess->ShedStat1(ii, piDEMSort, pfDEMGrid, piFlowGrid, piShedGrid, piShedWGrid, &pLsmStatVar[iPitNo]);

				if (pLsmStatVar[iPitNo].iShedArea > 0)
					pLsmStatVar[iPitNo].fVaratio = (double)(pLsmStatVar[iPitNo].fPitVol / pLsmStatVar[iPitNo].iShedArea * 1000.0);
				else
					pLsmStatVar[iPitNo].fVaratio = 0.0;
				if (pLsmStatVar[iPitNo].fVaratio > 999999.99)
					pLsmStatVar[iPitNo].fVaratio = (double)999999.99;

				iPitNo++;

			}
		}
		pProgressBar->StepIt();
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}
		int	*piShedOrd;
		double	*pfShed;
		if ((piShedOrd = new int [m_Row*m_Column]) == NULL)
		{
			AfxMessageBox("Not enough memory for watershed order index grid!");
			exit(1);
		}
		if ((pfShed = new double [m_Row*m_Column]) == NULL)
		{
			AfxMessageBox("Not enough memory for watershed order grid!");
			exit(1);
		}
		if ((pLsmStatVar2 = new LSMSTATVAR2) == NULL)
		{
			AfxMessageBox("Not enough memory for LSM statistic 2!");
			exit(1);
		}

		for (i = 0; i < iPitNo; i++)
		{
			pfShed[i] = (double)(((20000.0 - (double)pLsmStatVar[i].fPitElev) * pow(10.0, 10.0)) + (int)pLsmStatVar[i].fVaratio);
			piShedOrd[i] = i;
		}

		
		
		//************************************************************
		//IGK
		//calculate volume to flood

		

		//AfxBeginThread(RUN_VOLUME2FLOOD, plParameters);
		*sMessage = "Calculating Volume2Flood...";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pDEMProcess->Volume2Flood(piDEMSort,pfDEMGrid, pLsmStatVar,
			pfVol2FlGrid, pfMm2FlGrid, pfPArea, piShedWGrid, piDEMSort, piDrecGrid, iPitNo, piFlowGrid);
		pProgressBar->StepIt();		
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}


		//*******************************************************************
		QuickSort(pfShed, iPitNo, piShedOrd);
		


		int	iPitRemoved=0;



		//dlgProgressBar->m_ProgressBar1.SetPos(0);
		//dlgProgressBar->m_ProgressBar1.SetRange(0,100);



		// Currently variable iOutShed holds values for DrainsTo
		//because fox pro uses drainsto instead of OutShed, before going into the pit removal
		//function, we will copy values from iOutSHed into DrainsTo 
		int nTemp1 = 0;
		for(nTemp1=0;nTemp1<iPitNo;nTemp1++)
		{
			pLsmStatVar[nTemp1].iDrainsTo = pLsmStatVar[nTemp1].iOutShed;
			pLsmStatVar[nTemp1].iNextPit = 0;
		}

		
		*sMessage = "Running Lsm_PitR...";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pDEMProcess->Lsm_PitR(pfDEMGrid,piDEMSort,pLsmStatVar,piShedWGrid,piUpSlpGrid,iPitNo,piFlowGrid,
		piDrecGrid,m_PitArea,(double)m_PitDepth,dlgProgressBar, pWndNotifyProgress, sMessage);
		
		int nLargest = 0;
		int nLoop;
		




		//dlgProgressBar->m_ProgressBar2.StepIt();
		////dlgProgressBar->UpdateData(false);
		
		//IGK
		//int nLoop=0;
		// replace shed no with shednow

		for( nLoop = 0;nLoop<m_Row*m_Column;nLoop++)
		{
			piShedGrid[nLoop] = piShedWGrid[nLoop];

		}
		//***************************************************
		pProgressBar->StepIt();
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}

		//*************************************************************
		//IGK
		//Declare variables to hold pond statistics data, represents pond table
		LSMPONDSTATS* pPondStatVar;
		if((pPondStatVar = new LSMPONDSTATS[1000]) ==NULL)
		{
			AfxMessageBox("Not enough memory for pond statistics!");
			exit(1);
		}
		int nPondFileSize = 0;


		LSMSTATVAR* pFillFile;
		int nFillFileSize = 0;

		
		if((pFillFile = new LSMSTATVAR[1000])==NULL)
		{
			AfxMessageBox("Not enough memory for fill file!");
			exit(1);
		}

		
		CString sName;
		sName = "C++Vold.csv";
		CVoldFile vold(sName);
		sName = "C++Mold.csv";
		CVoldFile mold(sName);


		
		
		
		for(nLoop=0;nLoop<iPitNo;nLoop++)
		{
			pLsmStatVar[nLoop].fPreVol = 0.0;
		}
		for(nLoop=0;nLoop<m_Column*m_Row;nLoop++)
		{
			piShedGrid[nLoop] = piShedWGrid[nLoop];
		}
		
		
		
		*sMessage = "Running LowPitRemoval...";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);		
		pDEMProcess->LowPitRemoval( pLsmStatVar, &pPondStatVar, m_GridSize,
		iPitNo, pfDEMGrid,piDEMSort,  piShedOrd, pfShed,pfBottomUp, piShedWGrid, piUpSlpGrid,
		piDrecGrid, piFlowGrid, vold, nPondFileSize,pfVol2FlGrid,pfMm2FlGrid,pfPArea, piFlowGrid,
		pWndNotifyProgress,sMessage) ;















		delete pfShed;
		delete piShedOrd;
		CString s;
		s = "Fix Pit File";
		//AfxAfxMessageBox(s);
		*sMessage = "Running Fix_PitFile...";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pDEMProcess->Fix_PitFile(pLsmStatVar, pfDEMGrid, piShedWGrid, 
						  iPitNo, piShedGrid);
		pProgressBar->StepIt();	
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}

		s = "Reset DEM";
		//AfxAfxMessageBox(s);
		*sMessage = "Running Reset_DEM...";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pDEMProcess->Reset_Dem(vold,piFlowGrid,piDrecGrid,piUpSlpGrid, piShedWGrid, piShedGrid);
		pProgressBar->StepIt();		
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}






		*sMessage = "Running LSM_PitR8...";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pDEMProcess->LSM_PitR8(piShedGrid,pLsmStatVar,iPitNo, &pFillFile, nFillFileSize, pfDEMGrid,piShedWGrid,piUpSlpGrid, mold,
 			piFlowGrid, piDrecGrid,pfVol2FlGrid,pfMm2FlGrid, pPondStatVar, dlgProgressBar,pWndNotifyProgress,sMessage);
		pProgressBar->StepIt();		
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}



		int nStepNum=0;
		int nCounter1 = 0;
		char** pTempPointer=0;
		//CStrArray sArray;
		char* sStringTemp =new char[255];
		char* sString = 0;
		int nStringSize;
		//finish = clock();
		//double dd;
		//dd = (double)(finish - start)/CLOCKS_PER_SEC;
		/*
		ofstream file1("file1.txt");
		
		file1<<"SeqNUM,Flow, Drection, Upslope"<<endl;
		for(nLoop=0;nLoop<m_Row*m_Column;nLoop++)
		{
			file1<<nLoop+1<<","<<piFlowGrid[nLoop]<<","<<piDrecGrid[nLoop]<<","<<piUpSlpGrid[nLoop]<<endl;
		}
		
		file1.close();
		*/
		
		//char* p = new char [480000*2];
		char* pMissing = new char [m_Row*m_Column];
		for(nLoop=0;nLoop<m_Row*m_Column;nLoop++)
		{

			//char* pTemp = new char[2];
			if(pfDEMGrid[nLoop]==m_MissDataValue)
			{
				//strcpy(pTemp,"T");
				pMissing[nLoop] = 'T';

			}
			else
			{
				//strcpy(pTemp,"F");
				pMissing[nLoop] = 'F';
			}
			//pMissing[nLoop+1] = '\0';
			//pMissing[nLoop]  = pTemp;
			
		}
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}
		CString chrEdge = "T";
		CString chrFinal = "T";
		CString chrRemoved = "T";
		CString chrVisited = "T";
		CString sTempName;
		sTempName = m_WorkingDirectory;
		sTempName += m_FileName.GetAt(0);
		sTempName+=m_FileName.GetAt(1);
		sTempName+=m_FileName.GetAt(2);
		CString sTemp;
	
		
		sTemp = sTempName + "pit.dbf";
		*sMessage = "Saving "  + sTemp + " file..";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		if((pOutDbfFile = new CDbfFile(sTemp, OVERWRITE))==NULL)
		{
			AfxMessageBox("Not enough memory for pit dbf file!");
			exit(1);
		}

		
		pOutDbfFile->SetDBFColumn("ShedNo", 6, 0);
		pOutDbfFile->SetDBFColumnLogical("Edge",1,0);
		pOutDbfFile->SetDBFColumnLogical("Final",1,0);
		pOutDbfFile->SetDBFColumn("EndPit",6,0);
		pOutDbfFile->SetDBFColumn("ShedArea", 6, 0);
		pOutDbfFile->SetDBFColumn("PitRow", 5, 0);
		pOutDbfFile->SetDBFColumn("PitCol", 5, 0);
		pOutDbfFile->SetDBFColumn("PitRec", 9, 0);
		pOutDbfFile->SetDBFColumn("PitElev", 10, 4);
		pOutDbfFile->SetDBFColumn("PourElev", 10, 4);
		pOutDbfFile->SetDBFColumn("PreVol",8,3);
		pOutDbfFile->SetDBFColumn("PitVol", 8, 3);
		pOutDbfFile->SetDBFColumn("Varatio",8,2);
		pOutDbfFile->SetDBFColumn("PitArea", 6, 0);
		pOutDbfFile->SetDBFColumn("DrainsTo", 6, 0);
		pOutDbfFile->SetDBFColumn("NextPit",6,0);
		pOutDbfFile->SetDBFColumn("Becomes",6,0);
		pOutDbfFile->SetDBFColumnLogical ("Removed",1,0);
		pOutDbfFile->SetDBFColumn("InRow", 5, 0);
		pOutDbfFile->SetDBFColumn("InCol", 5, 0);
		pOutDbfFile->SetDBFColumn("InRec", 9, 0);
		pOutDbfFile->SetDBFColumn("InElev", 10, 4);
		pOutDbfFile->SetDBFColumn("OutRow", 5, 0);
		pOutDbfFile->SetDBFColumn("OutCol", 5, 0);
		pOutDbfFile->SetDBFColumn("OutRec", 9, 0);
		pOutDbfFile->SetDBFColumn("OutElev", 10, 4);
		pOutDbfFile->SetDBFColumn("Stage",9,0);
		pOutDbfFile->SetDBFColumnLogical("Visited",1,0);
		psDBFValue->RemoveAll();
		
		//dlgProgressBar->m_UserMessage.Format("Saving pit file..");
		//dlgProgressBar->m_ProgressBar1.SetRange(0,iPitNo);
		////dlgProgressBar->UpdateData(FALSE);




		//Declare all the variables used to write dbf files
		//CStringArray strArray;
		//CString strTempString;
		//strArray.SetSize(16);
		char* pChar = new char[255];
		double dDoubleValue = 0;

		pOutDbfFile->Initialize();
		for(nLoop = 0;nLoop <iPitNo;nLoop++)
		{
			//dlgProgressBar->m_ProgressBar1.StepIt();
			////dlgProgressBar->UpdateData();

			if(pLsmStatVar[nLoop].bEdge == true)
			{
				chrEdge = "T";
			}
			else
			{
				chrEdge = 'F';
			}

			if(pLsmStatVar[nLoop].bFinal ==true)
			{
				chrFinal = "T";
			}
			else
			{
				chrFinal = "F";
			}

			if(pLsmStatVar[nLoop].bRemoved == true)
			{
				chrRemoved = "T";
			}
			else
			{
				chrRemoved = "F";
			}

			if(pLsmStatVar[nLoop].bVisited ==true)
			{
				chrVisited = "T";
			}
			else
			{
				chrVisited = "F";
			}
			
			/*
			nStringSize = sprintf(sStringTemp ,%i,%s",
				


		
				pLsmStatVar[nLoop].iStage,
				chrVisited);
				sString = new char[nStringSize+1];
				strcpy(sString,sStringTemp);
				//sArray.Add(sString);
				pTempPointer[nLoop] = sString;
			*/
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iThisShed);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,0,(void*) & dDoubleValue);
			sprintf(pChar,"%s","F");
			pOutDbfFile->WriteRecord(nLoop,1,pChar);
			sprintf(pChar,"%s",chrFinal);
			pOutDbfFile->WriteRecord(nLoop,2,pChar);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iEndPit);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,3,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iShedArea);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,4,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iPitRow);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,5,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iPitCol);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,6,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iPitRec);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,7,(void*) & dDoubleValue);
			sprintf(pChar,"%.4f",pLsmStatVar[nLoop].fPitElev);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,8,(void*) & dDoubleValue);
			sprintf(pChar,"%.4f",pLsmStatVar[nLoop].fPourElev);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,9,(void*) & dDoubleValue);
			sprintf(pChar,"%.3f",pLsmStatVar[nLoop].fPreVol );
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,10,(void*) & dDoubleValue);
			sprintf(pChar,"%.4f",pLsmStatVar[nLoop].fPitVol);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,11,(void*) & dDoubleValue);
			sprintf(pChar,"%.2f",pLsmStatVar[nLoop].fVaratio );
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,12,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iPitArea );
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,13,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iDrainsTo);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,14,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iNextPit);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,15,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iBecomes);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,16,(void*) & dDoubleValue);
			sprintf(pChar,"%s",chrRemoved);
			pOutDbfFile->WriteRecord(nLoop,17,pChar);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iInRow);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,18,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iInColumn);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,19,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iInRec);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,20,(void*) & dDoubleValue);
			sprintf(pChar,"%.4f",pLsmStatVar[nLoop].fInElev);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,21,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iOutRow);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,22,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iOutColumn);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,23,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iOutRec);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,24,(void*) & dDoubleValue);
			sprintf(pChar,"%.4f",pLsmStatVar[nLoop].fOutElev);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,25,(void*) & dDoubleValue);
			sprintf(pChar,"%i",pLsmStatVar[nLoop].iStage);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,26,(void*) & dDoubleValue);
			sprintf(pChar,"%s",chrVisited);
			pOutDbfFile->WriteRecord(nLoop,27,pChar);
		}
		pOutDbfFile->Close();
		//sArray.RemoveAll();
		free (pLsmStatVar);  
		delete pOutDbfFile;

		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}


		CFile* pFile = new CFile(sTemp,CFile::modeWrite);
		pFile->Seek(29,CFile::begin);
		char* pCodePage = new char[1];
		*pCodePage = 30;
		pFile->Write(pCodePage,1);
		pFile->Close();
		delete pFile;
		pProgressBar->StepIt();
		//fill dbf file

		pTempPointer = new char*[nFillFileSize];
		sTemp = sTempName + "fill.dbf";
		*sMessage = "Saving "  + sTemp + " file..";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pOutDbfFile = new CDbfFile(sTemp, OVERWRITE);
		//dlgProgressBar->m_ProgressBar1.SetRange(0,nFillFileSize);
		//dlgProgressBar->UpdateData(FALSE);
		
		pOutDbfFile->SetDBFColumn("ShedNo", 9, 0);
		pOutDbfFile->SetDBFColumnLogical("Edge",1,0);
		pOutDbfFile->SetDBFColumnLogical("Final",1,0);
		pOutDbfFile->SetDBFColumn("EndPit",6,0);
		pOutDbfFile->SetDBFColumn("ShedArea", 6, 0);
		pOutDbfFile->SetDBFColumn("PitRow", 5, 0);
		pOutDbfFile->SetDBFColumn("PitCol", 5, 0);
		pOutDbfFile->SetDBFColumn("PitRec", 9, 0);
		pOutDbfFile->SetDBFColumn("PitElev", 10, 4);
		pOutDbfFile->SetDBFColumn("PourElev", 10, 4);
		pOutDbfFile->SetDBFColumn("PreVol",8,3);
		pOutDbfFile->SetDBFColumn("PitVol", 8, 3);
		pOutDbfFile->SetDBFColumn("Varatio",8,2);
		pOutDbfFile->SetDBFColumn("PitArea", 6, 0);
		pOutDbfFile->SetDBFColumn("DrainsTo", 6, 0);
		pOutDbfFile->SetDBFColumn("NextPit",6,0);
		pOutDbfFile->SetDBFColumn("Becomes",6,0);
		pOutDbfFile->SetDBFColumnLogical("Removed",1,0);
		pOutDbfFile->SetDBFColumn("InRow", 5, 0);
		pOutDbfFile->SetDBFColumn("InCol", 5, 0);
		pOutDbfFile->SetDBFColumn("InRec", 9, 0);
		pOutDbfFile->SetDBFColumn("InElev", 10, 4);
		pOutDbfFile->SetDBFColumn("OutRow", 5, 0);
		pOutDbfFile->SetDBFColumn("OutCol", 5, 0);
		pOutDbfFile->SetDBFColumn("OutRec", 9, 0);
		pOutDbfFile->SetDBFColumn("OutElev", 10, 4);
		pOutDbfFile->SetDBFColumn("Stage",9,0);
		pOutDbfFile->SetDBFColumnLogical("Visited",1,0);
		psDBFValue->RemoveAll();
		

		for(nLoop=0;nLoop<nFillFileSize;nLoop++)
		{

			if(pFillFile[nLoop].bEdge==true)
			{
				chrEdge = "T";
			}
			else
			{
				chrEdge = "F";
			}

			if(pFillFile[nLoop].bRemoved==true)
			{
				chrRemoved = "T";

			}
			else
			{
				chrRemoved = "F";
			}
			
			if(pFillFile[nLoop].bFinal ==true)
			{
				chrFinal = "T";
			}
			else
			{
				chrFinal = "F";
			}

			if(pFillFile[nLoop].bVisited==true)
			{
				chrVisited = "T";
			}
			else
			{
				chrVisited = "F";
			}



			nStringSize = sprintf(sStringTemp,"%i,%s,%s,%i,%i,%i,%i,%i,%.4f,%.4f,%.3f,%.4f,%.2f,%i,%i,%i,%i,%s,%i,%i,%i,%.4f,%i,%i,%i,%.4f,%i,%s",
			pFillFile[nLoop].iThisShed,
			chrEdge,
			chrFinal,
			pFillFile[nLoop].iEndPit,
			pFillFile[nLoop].iShedArea,
			pFillFile[nLoop].iPitRow,
			pFillFile[nLoop].iPitCol,
			pFillFile[nLoop].iPitRec,
			pFillFile[nLoop].fPitElev,
			pFillFile[nLoop].fPourElev,
			pFillFile[nLoop].fPreVol,
			pFillFile[nLoop].fPitVol,
			pFillFile[nLoop].fVaratio,
			pFillFile[nLoop].iPitArea,
			pFillFile[nLoop].iDrainsTo,
			pFillFile[nLoop].iNextPit,
			pFillFile[nLoop].iBecomes,
			chrRemoved,
			pFillFile[nLoop].iInRow,
			pFillFile[nLoop].iInColumn,
			pFillFile[nLoop].iInRec,
			pFillFile[nLoop].fInElev,
			pFillFile[nLoop].iOutRow,
			pFillFile[nLoop].iOutColumn,
			pFillFile[nLoop].iOutRec,
			pFillFile[nLoop].fOutElev,
			pFillFile[nLoop].iStage,
			chrVisited);
			//delete sString;
			sString = new char[nStringSize+1];
			strcpy(sString,sStringTemp);
			//sArray.Add(sString);
			pTempPointer[nLoop] = sString;
			//dlgProgressBar->m_ProgressBar1.StepIt();
			//dlgProgressBar->UpdateData(FALSE);
		}
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}
		pOutDbfFile->AddDBF3(pTempPointer,",",nFillFileSize);
		delete (pOutDbfFile);
		//sArray.RemoveAll();
		delete(pFillFile);
		pFile = new CFile(sTemp,CFile::modeWrite);
		pFile->Seek(29,CFile::begin);
		pFile->Write(pCodePage,1);
		pFile->Close();
		delete pFile;

		for(nLoop=0;nLoop<nFillFileSize;nLoop++)
		{
			delete pTempPointer[nLoop];
		}
		delete pTempPointer;
		pProgressBar->StepIt();
		//pond dbf file



		sTemp = sTempName + "pond.dbf";
		*sMessage = "Saving "  + sTemp + " file..";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pOutDbfFile = new CDbfFile(sTemp, OVERWRITE);
		pTempPointer = new char*[nPondFileSize];
		
		pOutDbfFile->SetDBFColumn("ShedNo", 6, 0);
		pOutDbfFile->SetDBFColumnLogical("Edge",1,0);
		pOutDbfFile->SetDBFColumnLogical("Final",1,0);
		pOutDbfFile->SetDBFColumn("EndPit",6,0);
		pOutDbfFile->SetDBFColumn("ShedArea", 6, 0);
		pOutDbfFile->SetDBFColumn("PitRow", 5, 0);
		pOutDbfFile->SetDBFColumn("PitCol", 5, 0);
		pOutDbfFile->SetDBFColumn("PitRec", 9, 0);
		pOutDbfFile->SetDBFColumn("PitElev", 10, 4);
		pOutDbfFile->SetDBFColumn("PourElev", 10, 4);
		pOutDbfFile->SetDBFColumn("PreVol",8,3);
		pOutDbfFile->SetDBFColumn("PitVol", 8, 3);
		pOutDbfFile->SetDBFColumn("Varatio",8,2);
		pOutDbfFile->SetDBFColumn("PitArea", 6, 0);
		pOutDbfFile->SetDBFColumn("DrainsTo", 6, 0);
		pOutDbfFile->SetDBFColumn("NextPit",6,0);
		pOutDbfFile->SetDBFColumn("Becomes",6,0);
		pOutDbfFile->SetDBFColumnLogical("Removed",1,0);
		pOutDbfFile->SetDBFColumn("InRow", 5, 0);
		pOutDbfFile->SetDBFColumn("InCol", 5, 0);
		pOutDbfFile->SetDBFColumn("InRec", 9, 0);
		pOutDbfFile->SetDBFColumn("InElev", 10, 4);
		pOutDbfFile->SetDBFColumn("OutRow", 5, 0);
		pOutDbfFile->SetDBFColumn("OutCol", 5, 0);
		pOutDbfFile->SetDBFColumn("OutRec", 9, 0);
		pOutDbfFile->SetDBFColumn("OutElev", 10, 4);
		pOutDbfFile->SetDBFColumn("Stage",9,0);
		pOutDbfFile->SetDBFColumnLogical("Visited",1,0);
		psDBFValue->RemoveAll();
		
		
		//dlgProgressBar->m_UserMessage = "Saving pond dbf file...";

		//dlgProgressBar->m_ProgressBar1.SetRange(0,nPondFileSize);
		//dlgProgressBar->m_ProgressBar2.StepIt();
		//dlgProgressBar->UpdateData(FALSE);
		nCounter1 = nStepNum;
		for(nLoop=0;nLoop<nPondFileSize;nLoop++)
		{

			//dlgProgressBar->m_ProgressBar1.StepIt();
			//dlgProgressBar->UpdateData(false);

			

			if(pPondStatVar[nLoop].bEdge == true)
			{
				chrEdge = "T";
			}
			else
			{
				chrEdge = "F";
			}

			
			if(pPondStatVar[nLoop].bFinal==true)
			{
				chrFinal = "T";
			}
			else
			{
				chrFinal = "F";
			}
			
			if(pPondStatVar[nLoop].bRemoved==true)
			{
				chrRemoved = "T";
			}
			else
			{
				chrRemoved = "F";
			}

			if(pPondStatVar[nLoop].bVisited==true)
			{
				chrVisited = "T";
			}
			else
			{
				chrVisited = "F";
			}


			nStringSize = sprintf(sStringTemp,"%i,%s,%s,%i,%i,%i,%i,%i,%.4f,%.4f,%.3f,%.4f,%.2f,%i,%i,%i,%i,%s,%i,%i,%i,%.4f,%i,%i,%i,%.4f,%i,%s",
			pPondStatVar[nLoop].iThisShed,
			chrEdge,
			chrFinal,
			pPondStatVar[nLoop].iEndPit,
			pPondStatVar[nLoop].iShedArea,
			pPondStatVar[nLoop].iPitRow,
			pPondStatVar[nLoop].iPitCol,
			pPondStatVar[nLoop].iPitRec,
			pPondStatVar[nLoop].fPitElev,
			pPondStatVar[nLoop].fPourElev,
			pPondStatVar[nLoop].fPreVol,
			pPondStatVar[nLoop].fPitVol,
			pPondStatVar[nLoop].fVaratio,
			pPondStatVar[nLoop].iPitArea,
			pPondStatVar[nLoop].iDrainsTo,
			pPondStatVar[nLoop].iNextPit,
			pPondStatVar[nLoop].iBecomes,
			chrRemoved,
			pPondStatVar[nLoop].iInRow,
			pPondStatVar[nLoop].iInColumn,
			pPondStatVar[nLoop].iInRec,
			pPondStatVar[nLoop].fInElev,
			pPondStatVar[nLoop].iOutRow,
			pPondStatVar[nLoop].iOutColumn,
			pPondStatVar[nLoop].iOutRec,
			pPondStatVar[nLoop].fOutElev,
			pPondStatVar[nLoop].iStage,
			chrVisited);
			//delete sString;
			sString = new char[nStringSize+1];
			strcpy(sString,sStringTemp);
			//sArray.Add(sString);
			pTempPointer[nLoop] = sString;
			
		}
		pOutDbfFile->AddDBF3(pTempPointer,",",nPondFileSize);
		delete (pOutDbfFile);
		//sArray.RemoveAll();
		delete(pPondStatVar);
		pFile = new CFile(sTemp,CFile::modeWrite);
		pFile->Seek(29,CFile::begin);
		pFile->Write(pCodePage,1);
		pFile->Close();
		delete pFile;
		for(nLoop=0;nLoop<nPondFileSize;nLoop++)
		{
			delete pTempPointer[nLoop];
		}
		delete pTempPointer;

		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}

		//vold dbf file
		sTemp = sTempName + "vold.dbf";
		*sMessage = "Saving "  + sTemp + " file..";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pOutDbfFile = new CDbfFile(sTemp, OVERWRITE);

		pOutDbfFile->SetDBFColumn("SeqNo", 9, 0);
		pOutDbfFile->SetDBFColumn("Ddir",2,0);
		pOutDbfFile->SetDBFColumn("Drec",9,0);
		pOutDbfFile->SetDBFColumn("UpSlope",9,0);
		pOutDbfFile->SetDBFColumn("ShedNow", 6, 0);
		pOutDbfFile->SetDBFColumn("Stage", 9, 0);
		pOutDbfFile->SetDBFColumn("Urec", 12, 0);
		pOutDbfFile->SetDBFColumn("Ds_Area", 12, 0);
		pOutDbfFile->SetDBFColumn("Varatio", 6, 1);
		psDBFValue->RemoveAll();
		
		//dlgProgressBar->m_UserMessage = "Saving vold dbf";
		//dlgProgressBar->UpdateData(FALSE);
		pOutDbfFile->Initialize ();
		for(nLoop=0;nLoop<vold.GetSize();nLoop++)
		{
			
			sprintf(pChar,"%i",vold.GetSeqNo(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,0,(void*) & dDoubleValue);

			sprintf(pChar,"%i",vold.GetDdir(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,1,(void*) & dDoubleValue);

			sprintf(pChar,"%i",vold.GetDrec(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,2,(void*) & dDoubleValue);

			sprintf(pChar,"%i",vold.GetUpSlop(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,3,(void*) & dDoubleValue);

			sprintf(pChar,"%i",vold.GetShedNow(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,4,(void*) & dDoubleValue);

			sprintf(pChar,"%i",vold.GetStage(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,5,(void*) & dDoubleValue);

			sprintf(pChar,"%i",vold.GetUrec(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,6,(void*) & dDoubleValue);

			sprintf(pChar,"%i",vold.GetDs_Area(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,7,(void*) & dDoubleValue);

			sprintf(pChar,"%.1f",vold.GetVaration(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,8,(void*) & dDoubleValue);

			
		}
		pOutDbfFile->Close();
		delete (pOutDbfFile);
		vold.RemoveAll();
		
		pFile = new CFile(sTemp,CFile::modeWrite);
		pFile->Seek(29,CFile::begin);
		pFile->Write(pCodePage,1);
		pFile->Close();
		delete pFile;
		pProgressBar->StepIt();
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}

		//mold dbf file

		sTemp = sTempName + "mold.dbf";
		*sMessage = "Saving "  + sTemp + " file..";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		pOutDbfFile = new CDbfFile(sTemp, OVERWRITE);

		
		pOutDbfFile->SetDBFColumn("SeqNo", 9, 0);
		pOutDbfFile->SetDBFColumn("Ddir",2,0);
		pOutDbfFile->SetDBFColumn("Drec",9,0);
		pOutDbfFile->SetDBFColumn("UpSlope",9,0);
		pOutDbfFile->SetDBFColumn("ShedNow", 6, 0);
		pOutDbfFile->SetDBFColumn("Stage", 9, 0);
		pOutDbfFile->SetDBFColumn("Urec", 12, 0);
		pOutDbfFile->SetDBFColumn("Ds_Area", 12, 0);
		pOutDbfFile->SetDBFColumn("Varatio", 6, 1);
		psDBFValue->RemoveAll();
		
		//dlgProgressBar->m_UserMessage = "Saving mold dbf";
		//dlgProgressBar->UpdateData(FALSE);
		pOutDbfFile->Initialize();
		for(nLoop=0;nLoop<mold.GetSize();nLoop++)
		{
			sprintf(pChar,"%i",mold.GetSeqNo(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,0,(void*) & dDoubleValue);

			sprintf(pChar,"%i",mold.GetDdir(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,1,(void*) & dDoubleValue);

			sprintf(pChar,"%i",mold.GetDrec(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,2,(void*) & dDoubleValue);

			sprintf(pChar,"%i",mold.GetUpSlop(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,3,(void*) & dDoubleValue);

			sprintf(pChar,"%i",mold.GetShedNow(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,4,(void*) & dDoubleValue);

			sprintf(pChar,"%i",mold.GetStage(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,5,(void*) & dDoubleValue);

			sprintf(pChar,"%i",mold.GetUrec(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,6,(void*) & dDoubleValue);

			sprintf(pChar,"%i",mold.GetDs_Area(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,7,(void*) & dDoubleValue);

			sprintf(pChar,"%.1f",mold.GetVaration(nLoop));
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,8,(void*) & dDoubleValue);
			
		}
		pOutDbfFile->Close();
		delete (pOutDbfFile);
		mold.RemoveAll();
		pFile = new CFile(sTemp,CFile::modeWrite);
		pFile->Seek(29,CFile::begin);
		pFile->Write(pCodePage,1);
		pFile->Close();
		delete pFile;

		pProgressBar->StepIt();
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}

		//DEM dbf file
		
		//dlgProgressBar->m_UserMessage = "Saving dem dbf...";
		//dlgProgressBar->UpdateData(false);


		sTemp = sTempName + "dem.dbf";
		*sMessage = "Saving "  + sTemp + " file..";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
		if((pOutDbfFile = new CDbfFile(sTemp, OVERWRITE))==NULL)
		{
			AfxMessageBox("Not enough memory for DEM dbf file output!");
			exit(1);
		}
		pOutDbfFile->SetDBFColumn("SeqNo",9,0);
		pOutDbfFile->SetDBFColumn("Row",5,0);
		pOutDbfFile->SetDBFColumn("Col",5,0);
		pOutDbfFile->SetDBFColumn("Elev",10,4);
		pOutDbfFile->SetDBFColumn("Ddir",2,0);
		pOutDbfFile->SetDBFColumn("Drec",9,0);
		pOutDbfFile->SetDBFColumn("UpSlope",9,0);
		pOutDbfFile->SetDBFColumn("ShedNo",6,0);
		pOutDbfFile->SetDBFColumn("ShedNow",6,0);
		//pOutDbfFile->SetDBFColumnLogical("Channel",1,0);
		//pOutDbfFile->SetDBFColumnLogical("Chanhead",1,0);
		pOutDbfFile->SetDBFColumnLogical("Missing",1,0);
		pOutDbfFile->SetDBFColumnLogical("Edge",1,0);
		pOutDbfFile->SetDBFColumn("Vol2Fl",11,1);
		pOutDbfFile->SetDBFColumn("Mm2Fl",11,4);
		pOutDbfFile->SetDBFColumn("PArea",7,0);



		CString sFalse = "F";
		nStepNum = (m_Column*m_Row) /10;
		nCounter1 = nStepNum;
		//dlgProgressBar->m_ProgressBar1.SetRange(0,10);
		//dlgProgressBar->m_ProgressBar1.SetPos(0);
		//dlgProgressBar->UpdateData(false);
		int n =  nStepNum;
		pOutDbfFile->Initialize();

		for(nLoop=0;nLoop<m_Column*m_Row;nLoop++)
		{
			/*
			
			if(nLoop == n)
			{
				//dlgProgressBar->m_ProgressBar1.StepIt();
				//dlgProgressBar->UpdateData(FALSE);
				n = n + nStepNum;
			}
			*/
			sprintf(pChar,"%i",nLoop+1);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,0,(void*) & dDoubleValue);


			sprintf(pChar,"%i",nLoop/m_Column+1);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,1,(void*) & dDoubleValue);


			sprintf(pChar,"%i",nLoop%m_Column+1);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,2,(void*) & dDoubleValue);


			sprintf(pChar,"%.4f",pfDEMGrid[nLoop]);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,3,(void*) & dDoubleValue);


			sprintf(pChar,"%i",piFlowGrid[nLoop]);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,4,(void*) & dDoubleValue);
			
			sprintf(pChar,"%i",piDrecGrid[nLoop]);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,5,(void*) & dDoubleValue);

			
			
			sprintf(pChar,"%i",piUpSlpGrid[nLoop]);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,6,(void*) & dDoubleValue);
			
		
			sprintf(pChar,"%i",piShedGrid[nLoop]);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,7,(void*) & dDoubleValue);
			
		
			sprintf(pChar,"%i",piShedWGrid[nLoop]);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,8,(void*) & dDoubleValue);
			

			//sprintf(pChar,"%s",sFalse);
			//pOutDbfFile->WriteRecord(nLoop,9,pChar);

			
		
			//sprintf(pChar,"%s",sFalse);
			//pOutDbfFile->WriteRecord(nLoop,10,pChar);
			
		
			sprintf(pChar,"%c",pMissing[nLoop]);
			pOutDbfFile->WriteRecord(nLoop,9,pChar);
			
		
			sprintf(pChar,"%s",sFalse);
			pOutDbfFile->WriteRecord(nLoop,10,pChar);
			

			sprintf(pChar,"%.1f",pfVol2FlGrid[nLoop]);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,11,(void*) & dDoubleValue);
			

			sprintf(pChar,"%.4f",pfMm2FlGrid[nLoop]);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,12,(void*) & dDoubleValue);
			
			sprintf(pChar,"%i",pfPArea[nLoop]);
			dDoubleValue = atof(pChar);
			pOutDbfFile->WriteRecord(nLoop,13,(void*) & dDoubleValue);		


			
		}
		pOutDbfFile->Close();
		if(*pbKillThread == true)
		{
			AfxEndThread(0);	
		}
		delete pOutDbfFile;
		//delete pMissing;
		pFile = new CFile(sTemp,CFile::modeWrite);
		pFile->Seek(29,CFile::begin);
		pFile->Write(pCodePage,1);
		pFile->Close();
		delete pFile;
		free (piDrecGrid);
		free (piFlowGrid);
		free (piShedGrid);
		free (piShedWGrid);
		free (piUpSlpGrid);

		free (pLsmStatVar2);
		//free (pLsmProcParm);

		//free (pfShed);
		//free (piShedOrd);

		
		//dlgProgressBar->m_ProgressBar2.StepIt();
		//dlgProgressBar->m_ProgressBar1.StepIt();

		//dlgProgressBar->UpdateData(FALSE);
		




		if(*pbInvertedElev == true)
		{









		
			pProgressBar->StepIt();
			double *pInvertedElev = new double[m_Row*m_Column];
			if(pInvertedElev ==NULL)
			{
				AfxMessageBox("Not enough memory to created inverted elevation array.");
				exit(0);
			}
			if(*pbKillThread == true)
			{
				AfxEndThread(0);	
			}
			double *fHighest = new double;
			*fHighest = 0.000;
			short int* pInvertedFlow = new short int[m_Row*m_Column];
			if(pInvertedFlow ==NULL)
			{
				AfxMessageBox("Not enough memory to create inverted flow array.");
				exit(0);

			}

			int* pInvertedDrec = new int[m_Row*m_Column];
			if(pInvertedDrec ==NULL)
			{
				AfxMessageBox("Not enough memory to create inverted flow array.");
				exit(0);

			}

			int* pInvertedUpSlp = new int [m_Row*m_Column];
			if(pInvertedUpSlp == NULL)
			{
				AfxMessageBox("Not enough memory to create inverted UpSlope array.");
				exit(0);
			}
			


			int* pInvertedShedNo = new int[m_Row*m_Column];
			if(pInvertedShedNo ==NULL)
			{
				AfxMessageBox("Not enough memory to create inverted ShedNo array.");
				exit(0);
			}

			int* pInvertedShedNow = new int[m_Row*m_Column];
			if(pInvertedShedNow ==NULL)
			{
				AfxMessageBox("Not enough memory to create inverted ShedNo array.");
				exit(0);
			}

			float* pInvertedVol2Fld = new float [m_Row*m_Column];
			if(pInvertedVol2Fld ==NULL)
			{
				AfxMessageBox("Not enough memory to create inverted volume too flood array.");
				exit(0);
			}


			double* pInvertedMm2Fld = new double [m_Row*m_Column];
			if(pInvertedMm2Fld ==NULL)
			{
				AfxMessageBox("Not enough memory to create inverted Mm too flood array.");
				exit(0);
			}

			int* pInvertedPArea = new int [m_Row*m_Column];
			if(pInvertedPArea == NULL) 
			{
				AfxMessageBox("Not enough memory to create inverted PArea array.");
				exit(0);
			}


			for(nLoop=0;nLoop<m_Row*m_Column;nLoop++)
			{
				if(*fHighest<pfDEMGrid[nLoop])
				{
					*fHighest = pfDEMGrid[nLoop];
				}
			}
			
			for(nLoop=0;nLoop<m_Row*m_Column;nLoop++)
			{
				if(!(pfDEMGrid[nLoop] ==m_MissDataValue))
				{
					pInvertedElev[nLoop] = *fHighest - pfDEMGrid[nLoop];
				}
				else
				{
					pInvertedElev[nLoop] = m_MissDataValue;
				}
			}
			delete fHighest;
			free (pfDEMGrid);
			//dlgProgressBar->m_UserMessage = "Calculating Flow direction...";
			//dlgProgressBar->UpdateData(false);

			*sMessage = "Running inverted flowdir...";
			pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
			pDEMProcess->FlowDir(pInvertedElev, pInvertedFlow);
			if(*pbKillThread == true)
			{
				AfxEndThread(0);	
			}
			*sMessage = "Running inverted FixFlat...";
			pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
			//dlgProgressBar->m_UserMessage = "Fixing Flat....";
			//dlgProgressBar->UpdateData(false);
			pDEMProcess->FixFlat(pInvertedElev,pInvertedFlow);
			if(*pbKillThread == true)
			{
				AfxEndThread(0);	
			}
			*sMessage = "Running inverted CalcDrec...";
			pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
			//dlgProgressBar->m_UserMessage = "Calculating Drection....";
			//dlgProgressBar->UpdateData(false);
			pDEMProcess->CalcDrec(pInvertedFlow,pInvertedDrec);
			if(*pbKillThread == true)
			{
				AfxEndThread(0);	
			}
			//dlgProgressBar->m_UserMessage = "Fixing circle flow....";
			//dlgProgressBar->UpdateData(false);

			*sMessage = "Running inverted CircleFlow...";
			pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
			pDEMProcess->CircleFlow(pInvertedFlow,pInvertedDrec);
			pProgressBar->StepIt();
			if ((pfTmpDEMGrid = new double [m_Row*m_Column]) == NULL)
			{
				AfxMessageBox("Not enough memory for Tmp DEM grid!");
				exit(1);
			}

			nMissingNum = 0;
			nIndex = 0;

			for (i = 0; i < m_Row * m_Column; i++)
			{
				if(!(pInvertedElev[i]==m_MissDataValue))//if value is not missing
				{
					pfTmpDEMGrid[nIndex] = pInvertedElev[i];
					piDEMSort[nIndex] = i;
					nIndex++;

				}
				else
				{
					pTempSort[nMissingNum] = i;
					nMissingNum++; //keep count of missing values

				}
			}
			if(*pbKillThread == true)
			{
				AfxEndThread(0);	
			}
			
			QuickSort(pfTmpDEMGrid, m_Row * m_Column-nMissingNum, piDEMSort,true); //sort only non missing values
			delete pfTmpDEMGrid;
			nIndex = 0;
			for(i=m_Row*m_Column-nMissingNum;i<m_Row*m_Column;i++) //make the rest of pointers which are
				// pointing to the missing values, valid. 
				
			{
				piDEMSort[i] = pTempSort[nIndex];
				nIndex++;

			}
			*sMessage = "Running inverted CalcWaterShed...";
			pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
			
			//dlgProgressBar->m_UserMessage = "Calculating water shed....";
			//dlgProgressBar->UpdateData(false);
			pDEMProcess->CalcWaterShed(pInvertedElev,pInvertedFlow,pInvertedDrec,pInvertedShedNo,piDEMSort);
			pProgressBar->StepIt();		
			
			if(*pbKillThread == true)
			{
				AfxEndThread(0);	
			}


			//dlgProgressBar->m_UserMessage = "Calculating upslope area...";
			//dlgProgressBar->UpdateData(false);

			*sMessage = "Running inverted CalcUpSlopeArea...";
			pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);

			pDEMProcess->CalcUpSlopeArea(pInvertedElev,pInvertedFlow,pInvertedDrec,pInvertedUpSlp,piDEMSort);
			pProgressBar->StepIt();		

			int nInvertedPitNo = 0;

			piSortedTemp = new int [m_Row*m_Column];//temp array to hold index of records with missing elev field
			
			nIndex = 0;
			nMissingNum = 0;

			for (i = 0; i < m_Row * m_Column; i++)
			{
				pInvertedShedNow[i] = pInvertedShedNo[i];

				if (pInvertedFlow[i] == 5)
					nInvertedPitNo++;
				if(!(pInvertedElev[i] ==m_MissDataValue)) //make sure that value for elevation is not missing
				{
				pInvertedShedNow[i] = pInvertedShedNo[i];
				pfBottomUp[nIndex] = (double)(((double)pInvertedShedNow[i] * pow(10.0, 18.0)) +
								((double)pInvertedElev[i] * pow(10.0, 12.0)) + 
								(1000000.0 - (double)pInvertedUpSlp[i]));
				//pfBottomUp1[nIndex] = pfBottomUp[nIndex];
				piDEMSort[nIndex] = i;
				nIndex++;
				}
				else
				{
					piSortedTemp[nMissingNum] = i;//keep track of missing value records 
					nMissingNum ++;

				}

			}
			pProgressBar->StepIt();
			//free (pfBottomUp1);
			if(*pbKillThread == true)
			{
				AfxEndThread(0);	
			}
			LSMSTATVAR* pInvertedLsmStatVar = 0;
			if ((pInvertedLsmStatVar = new LSMSTATVAR [nInvertedPitNo]) == NULL)
			{
				AfxMessageBox("Not enough memory for LSM statistic 1!");
				exit(1);
			}	
			
			for(nLoop = 0;nLoop<nInvertedPitNo;nLoop++)
			{
				pInvertedLsmStatVar[nLoop].bEdge = false;
				pInvertedLsmStatVar[nLoop].bFinal  = false;
				pInvertedLsmStatVar[nLoop].bRemoved  = false;
				pInvertedLsmStatVar[nLoop].bVisited  = false;
				pInvertedLsmStatVar[nLoop].iNextPit =0;
				pInvertedLsmStatVar[nLoop].iBecomes = 0;
				pInvertedLsmStatVar[nLoop].iStage = 0;
				pInvertedLsmStatVar[nLoop].fPreVol = 0;
				pInvertedLsmStatVar[nLoop].iDrainsTo = 0;
				pInvertedLsmStatVar[nLoop].iEndPit = 0;


			}
			if(*pbKillThread == true)
			{
				AfxEndThread(0);	
			}


			QuickSort(pfBottomUp, m_Row * m_Column - nMissingNum, piDEMSort);//sort only records that have valid values
			free (pfBottomUp);
			nIndex = 0;
			for(i = m_Row*m_Column-nMissingNum;i<m_Row*m_Column;i++)//make the rest of sorted array point to 
				//records that have missing values. 
			{
				piDEMSort[i] = piSortedTemp[nIndex];
				nIndex++;
			}
			delete piSortedTemp;
			//dlgProgressBar->m_ProgressBar1.StepIt();

			nPercentage = 10;
			//dlgProgressBar->m_ProgressBar1.SetPos(0);
			//dlgProgressBar->m_ProgressBar1.SetRange(0,10);
			//dlgProgressBar->m_UserMessage = "Generating watershed statistics...";
			//dlgProgressBar->UpdateData(false);
			//UpdateData(false);
			nInvertedPitNo = 0;
			for (ii = 0; ii < m_Row * m_Column; ii++)
			{	
				if (ii ==int(((m_Row * m_Column)/100)*nPercentage))
				{
					//dlgProgressBar->m_ProgressBar1.StepIt();
					nPercentage = nPercentage + 10;
				}
				if (pInvertedFlow[piDEMSort[ii]] == 5 && (!(pInvertedElev[piDEMSort[ii]]==m_MissDataValue)))
				{
					pDEMProcess->ShedStat1(ii, piDEMSort, pInvertedElev, pInvertedFlow, 
						pInvertedShedNo, pInvertedShedNow, &pInvertedLsmStatVar[nInvertedPitNo]);

					if (pInvertedLsmStatVar[nInvertedPitNo].iShedArea > 0)
						pInvertedLsmStatVar[nInvertedPitNo].fVaratio = 
						(double)(pInvertedLsmStatVar[nInvertedPitNo].fPitVol / 
						pInvertedLsmStatVar[nInvertedPitNo].iShedArea * 1000.0);
					else
						pInvertedLsmStatVar[nInvertedPitNo].fVaratio = 0.0;
					if (pInvertedLsmStatVar[nInvertedPitNo].fVaratio > 999999.99)
						pInvertedLsmStatVar[nInvertedPitNo].fVaratio = (double)999999.99;

					nInvertedPitNo++;

				}
			}


			*sMessage = "Running inverted Lsm_PitR...";
			pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);


			pDEMProcess->Lsm_PitR(pInvertedElev,piDEMSort,pInvertedLsmStatVar,pInvertedShedNow,pInvertedUpSlp,nInvertedPitNo,pInvertedFlow,
			pInvertedDrec,m_PitArea,(double)m_PitDepth,dlgProgressBar,pWndNotifyProgress,sMessage);
			pProgressBar->StepIt();
			//dlgProgressBar->m_ProgressBar2.StepIt();

			if(*pbKillThread == true)
			{
				AfxEndThread(0);	
			}
			//DEM dbf file


			//dlgProgressBar->m_UserMessage = "Saving idem dbf...";
			//dlgProgressBar->UpdateData(false);
			sTemp =  sTempName + "i" + "dem.dbf";
			*sMessage = "Saving "  + sTemp + " file..";
			pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
			if((pOutDbfFile = new CDbfFile(sTemp, OVERWRITE))==NULL)
			{
				AfxMessageBox("Not enough memory for DEM dbf file output!");
				exit(1);
			}
			pOutDbfFile->SetDBFColumn("SeqNo",9,0);
			pOutDbfFile->SetDBFColumn("Row",5,0);
			pOutDbfFile->SetDBFColumn("Col",5,0);
			pOutDbfFile->SetDBFColumn("Elev",10,4);
			pOutDbfFile->SetDBFColumn("Ddir",2,0);
			pOutDbfFile->SetDBFColumn("Drec",9,0);
			pOutDbfFile->SetDBFColumn("UpSlope",9,0);
			pOutDbfFile->SetDBFColumn("ShedNo",6,0);
			pOutDbfFile->SetDBFColumn("ShedNow",6,0);
			//pOutDbfFile->SetDBFColumnLogical("Channel",1,0);
			//pOutDbfFile->SetDBFColumnLogical("Chanhead",1,0);
			pOutDbfFile->SetDBFColumnLogical("Missing",1,0);
			pOutDbfFile->SetDBFColumnLogical("Edge",1,0);
			pOutDbfFile->SetDBFColumn("Vol2Fl",9,1);
			pOutDbfFile->SetDBFColumn("Mm2Fl",11,4);
			pOutDbfFile->SetDBFColumn("PArea",7,0);
			sFalse = "F";
			nStepNum = (m_Column*m_Row) /10;
			nCounter1 = nStepNum;
			//dlgProgressBar->m_ProgressBar1.SetRange(0,10);
			//dlgProgressBar->m_ProgressBar1.SetPos(0);
			//dlgProgressBar->UpdateData(false);
			n =  nStepNum;
			pOutDbfFile->Initialize();

			for(nLoop=0;nLoop<m_Column*m_Row;nLoop++)
			{
				/*
				
				if(nLoop == n)
				{
					//dlgProgressBar->m_ProgressBar1.StepIt();
					//dlgProgressBar->UpdateData(FALSE);
					n = n + nStepNum;
				}
				*/

				sprintf(pChar,"%i",nLoop+1);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,0,(void*) & dDoubleValue);


				sprintf(pChar,"%i",nLoop/m_Column+1);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,1,(void*) & dDoubleValue);


				sprintf(pChar,"%i",nLoop%m_Column+1);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,2,(void*) & dDoubleValue);


				sprintf(pChar,"%.4f",pInvertedElev[nLoop]);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,3,(void*) & dDoubleValue);


				sprintf(pChar,"%i",pInvertedFlow[nLoop]);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,4,(void*) & dDoubleValue);
				
				sprintf(pChar,"%i",pInvertedDrec[nLoop]);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,5,(void*) & dDoubleValue);

				
				
				sprintf(pChar,"%i",pInvertedUpSlp[nLoop]);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,6,(void*) & dDoubleValue);
				
			
				sprintf(pChar,"%i",pInvertedShedNo[nLoop]);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,7,(void*) & dDoubleValue);
				
			
				sprintf(pChar,"%i",pInvertedShedNow[nLoop]);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,8,(void*) & dDoubleValue);
				

				//sprintf(pChar,"%s",sFalse);
				//pOutDbfFile->WriteRecord(nLoop,9,pChar);

				
			
				//sprintf(pChar,"%s",sFalse);
				//pOutDbfFile->WriteRecord(nLoop,10,pChar);
				
			
				sprintf(pChar,"%c",pMissing[nLoop]);
				pOutDbfFile->WriteRecord(nLoop,9,pChar);
				
			
				sprintf(pChar,"%s",sFalse);
				pOutDbfFile->WriteRecord(nLoop,10,pChar);
				

				sprintf(pChar,"%.1f",pfVol2FlGrid[nLoop]);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,11,(void*) & dDoubleValue);
				

				sprintf(pChar,"%.4f",pfMm2FlGrid[nLoop]);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,12,(void*) & dDoubleValue);
				
				sprintf(pChar,"%i",pfPArea[nLoop]);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,13,(void*) & dDoubleValue);		


				
			}
			pOutDbfFile->Close();

			delete pOutDbfFile;
			if(*pbKillThread == true)
			{
				AfxEndThread(0);	
			}
			pFile = new CFile(sTemp,CFile::modeWrite);
			pFile->Seek(29,CFile::begin);
			pFile->Write(pCodePage,1);
			pFile->Close();
			delete pFile;
			pProgressBar->StepIt();




			sTemp =  sTempName  + "i" + "pit.dbf";
			*sMessage = "Saving "  + sTemp + " file..";
			pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,0,(long) sMessage);
			if((pOutDbfFile = new CDbfFile(sTemp, OVERWRITE))==NULL)
			{
				AfxMessageBox("Not enough memory for pit dbf file!");
				exit(1);
			}

			
			pOutDbfFile->SetDBFColumn("ShedNo", 6, 0);
			pOutDbfFile->SetDBFColumnLogical("Edge",1,0);
			pOutDbfFile->SetDBFColumnLogical("Final",1,0);
			pOutDbfFile->SetDBFColumn("EndPit",6,0);
			pOutDbfFile->SetDBFColumn("ShedArea", 6, 0);
			pOutDbfFile->SetDBFColumn("PitRow", 5, 0);
			pOutDbfFile->SetDBFColumn("PitCol", 5, 0);
			pOutDbfFile->SetDBFColumn("PitRec", 9, 0);
			pOutDbfFile->SetDBFColumn("PitElev", 10, 4);
			pOutDbfFile->SetDBFColumn("PourElev", 10, 4);
			pOutDbfFile->SetDBFColumn("PreVol",8,3);
			pOutDbfFile->SetDBFColumn("PitVol", 8, 3);
			pOutDbfFile->SetDBFColumn("Varatio",8,2);
			pOutDbfFile->SetDBFColumn("PitArea", 6, 0);




			pOutDbfFile->SetDBFColumn("DrainsTo", 6, 0);
			pOutDbfFile->SetDBFColumn("NextPit",6,0);
			pOutDbfFile->SetDBFColumn("Becomes",6,0);
			pOutDbfFile->SetDBFColumnLogical ("Removed",1,0);
			pOutDbfFile->SetDBFColumn("InRow", 5, 0);
			pOutDbfFile->SetDBFColumn("InCol", 5, 0);
			pOutDbfFile->SetDBFColumn("InRec", 9, 0);
			pOutDbfFile->SetDBFColumn("InElev", 10, 4);
			pOutDbfFile->SetDBFColumn("OutRow", 5, 0);
			pOutDbfFile->SetDBFColumn("OutCol", 5, 0);
			pOutDbfFile->SetDBFColumn("OutRec", 9, 0);
			pOutDbfFile->SetDBFColumn("OutElev", 10, 4);
			pOutDbfFile->SetDBFColumn("Stage",9,0);
			pOutDbfFile->SetDBFColumnLogical("Visited",1,0);
			psDBFValue->RemoveAll();
			
			//dlgProgressBar->m_UserMessage.Format("Saving ipit file..");
			//dlgProgressBar->m_ProgressBar1.SetRange(0,nInvertedPitNo);
			//dlgProgressBar->UpdateData(FALSE);




			//Declare all the variables used to write dbf files


			pOutDbfFile->Initialize();
			for(nLoop = 0;nLoop <nInvertedPitNo;nLoop++)
			{
				//dlgProgressBar->m_ProgressBar1.StepIt();
				//dlgProgressBar->UpdateData();

				if(pInvertedLsmStatVar[nLoop].bEdge == true)
				{
					chrEdge = "T";
				}
				else
				{
					chrEdge = 'F';
				}

				if(pInvertedLsmStatVar[nLoop].bFinal ==true)
				{
					chrFinal = "T";
				}
				else
				{
					chrFinal = "F";
				}

				if(pInvertedLsmStatVar[nLoop].bRemoved == true)
				{
					chrRemoved = "T";
				}
				else
				{
					chrRemoved = "F";
				}

				if(pInvertedLsmStatVar[nLoop].bVisited ==true)
				{
					chrVisited = "T";
				}
				else
				{
					chrVisited = "F";
				}
				
				/*
				nStringSize = sprintf(sStringTemp ,%i,%s",
					


			
					pLsmStatVar[nLoop].iStage,
					chrVisited);
					sString = new char[nStringSize+1];
					strcpy(sString,sStringTemp);
					//sArray.Add(sString);
					pTempPointer[nLoop] = sString;
				*/
				
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iThisShed);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,0,(void*) & dDoubleValue);
				sprintf(pChar,"%s","F");
				pOutDbfFile->WriteRecord(nLoop,1,pChar);
				sprintf(pChar,"%s",chrFinal);
				pOutDbfFile->WriteRecord(nLoop,2,pChar);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iEndPit);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,3,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iShedArea);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,4,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iPitRow);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,5,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iPitCol);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,6,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iPitRec);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,7,(void*) & dDoubleValue);
				sprintf(pChar,"%.4f",pInvertedLsmStatVar[nLoop].fPitElev);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,8,(void*) & dDoubleValue);
				sprintf(pChar,"%.4f",pInvertedLsmStatVar[nLoop].fPourElev);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,9,(void*) & dDoubleValue);
				sprintf(pChar,"%.3f",pInvertedLsmStatVar[nLoop].fPreVol );
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,10,(void*) & dDoubleValue);
				sprintf(pChar,"%.4f",pInvertedLsmStatVar[nLoop].fPitVol);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,11,(void*) & dDoubleValue);
				sprintf(pChar,"%.2f",pInvertedLsmStatVar[nLoop].fVaratio );
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,12,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iPitArea );
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,13,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iDrainsTo);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,14,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iNextPit);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,15,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iBecomes);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,16,(void*) & dDoubleValue);
				sprintf(pChar,"%s",chrRemoved);
				pOutDbfFile->WriteRecord(nLoop,17,pChar);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iInRow);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,18,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iInColumn);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,19,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iInRec);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,20,(void*) & dDoubleValue);
				sprintf(pChar,"%.4f",pInvertedLsmStatVar[nLoop].fInElev);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,21,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iOutRow);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,22,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iOutColumn);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,23,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iOutRec);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,24,(void*) & dDoubleValue);
				sprintf(pChar,"%.4f",pInvertedLsmStatVar[nLoop].fOutElev);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,25,(void*) & dDoubleValue);
				sprintf(pChar,"%i",pInvertedLsmStatVar[nLoop].iStage);
				dDoubleValue = atof(pChar);
				pOutDbfFile->WriteRecord(nLoop,26,(void*) & dDoubleValue);
				sprintf(pChar,"%s",chrVisited);
				pOutDbfFile->WriteRecord(nLoop,27,pChar);
			}
			pOutDbfFile->Close();
			//sArray.RemoveAll();
			delete pOutDbfFile;




			pFile = new CFile(sTemp,CFile::modeWrite);
			pFile->Seek(29,CFile::begin);
			pFile->Write(pCodePage,1);
			pFile->Close();
			delete pFile;
			delete pCodePage;
			pProgressBar->StepIt();
			//dlgProgressBar->m_ProgressBar2.StepIt();
			//dlgProgressBar->UpdateData(false);






			
			//free (pfBottomUp1);
			//free (pfBottomUp);
			free (piDEMSort);
			delete (pDEMProcess);
			delete (pDbfFile);
			delete (psDBFValue);
			

			delete sStringTemp;
			
			delete pInvertedPArea;
			delete pInvertedElev;
			delete pInvertedFlow;
			delete pInvertedDrec;
			delete pInvertedShedNo;
			delete pInvertedShedNow;
			delete pInvertedVol2Fld;
			delete pInvertedMm2Fld;
			delete pInvertedUpSlp;
			
			delete pMissing;
			free (pInvertedLsmStatVar);
			//delete pfTmpDEMGrid;
		}

		delete pChar;


		//*****************************

		free (pfVol2FlGrid);
		free (pfMm2FlGrid);
		free (pfPArea);
		//******************************

		//finish = clock();
		//duration = (double)(finish - start) / CLOCKS_PER_SEC;
		//CString str;
		//str.Format("Time: %.1f seconds.",duration);
		//AfxMessageBox(str);
		
		//CDialog::OnOK();
		//delete sString;
		pProgressBar->SetPos(26);
		*sMessage = "Calculation Complete!";
		pWndNotifyProgress->PostMessage(WM_USER_RECALC_IN_PROGRESS,1,(long) sMessage);
	}




	delete pArray;

	return 0;

}




double* CLandmapDlg::ReadDbf(CString sTemp)
{
	int m_Col = m_Column;

	BOOL bValid = TRUE;

	char szBuffer [255];	

	CFile myFile(sTemp, CFile::modeRead );

	myFile.Seek(4,CFile::begin);
	myFile.Read(szBuffer,4);

	int* pShortInt = new int;
	*pShortInt = *(int*) &szBuffer;
	

	myFile.Seek(32,CFile::begin);
	int* nRowLocation = new int;
	*nRowLocation = -1;
	int* nColLocation = new int;
	*nColLocation = -1;
	int* nNumFields = new int;
	* nNumFields = 0;
	int* nElevLocation = new int;
	*nElevLocation = -1;
	int* nDdirLocation = new int;
	*nDdirLocation = -1;
	int* nDrecLocation = new int;
	*nDrecLocation = -1;
	int* nUpSlpLocation = new int;
	*nUpSlpLocation = -1;
	int* pnShedNowLocation = new int;
	*pnShedNowLocation = -1;
	

	int nFieldLength[35];

	for(int nLoop = 0;nLoop<100;nLoop++)
	{
		myFile.Read(szBuffer,32);
		if(szBuffer[0] !=13)
		{

			if(_stricmp(szBuffer,"elev")==0)
			{
				*nElevLocation = nLoop;
				nLoop = 100;
			}


			nFieldLength[nLoop] = szBuffer[16];
		}
		else
		{
			nLoop = 100;
		}
	}
	
	

	myFile.Seek(8,CFile::begin);
	myFile.Read(szBuffer,2);

	short int* piFirstRecordLoc = new short int;
	*piFirstRecordLoc = *(short int*)&szBuffer ;
	
	myFile.Seek(10,CFile::begin);
	myFile.Read(szBuffer,2);

	short int* piRecordLength = new short int;
	*piRecordLength = *(short int*) &szBuffer;

	
	myFile.Seek(*piFirstRecordLoc + 
		(*pShortInt -1) * (*piRecordLength),CFile::begin);
	myFile.Read(szBuffer,*piRecordLength);
	
	char szNumBuffer[8] = {'0','0','0','0','0','0','0','0'};
	
	int nBitesOfSet = 0;

	for(int nLoop=0;nLoop<*nColLocation;nLoop++)
	{
		nBitesOfSet = nBitesOfSet + nFieldLength[nLoop];
	}
	int nIndex =0;
	char* szP = new char[nFieldLength[*nColLocation]];
	for(int nLoop=0;nLoop<nFieldLength[*nColLocation];nLoop++)
	{

		szP[nLoop] = szBuffer[nBitesOfSet +1 +nLoop];
			
		

	}
	
	char szBuffTemp[8] = {'0','0','0','0','0','0','0','0'};

	for(int nLoop = 0;nLoop<8;nLoop++)
	{
		szBuffTemp[nLoop] = szNumBuffer[nLoop];

	}
	m_Col = (int)atof(szP);

	
	nIndex = 0;
	nBitesOfSet =0;
	for(int nLoop=0;nLoop<*nRowLocation;nLoop++)
	{
		nBitesOfSet = nBitesOfSet + nFieldLength[nLoop];
	}
	delete szP;
	szP = new char [nFieldLength[*nRowLocation]];
	for(int nLoop=0;nLoop<nFieldLength[*nRowLocation];nLoop++)
	{

		szP[nLoop] = szBuffer[nBitesOfSet +1 +nLoop];
			


	}
	
	for(int nLoop=0;nLoop<8;nLoop++)
	{
		szBuffTemp[nLoop] = '0';
	}

	for(int nLoop = 0;nLoop<8;nLoop++)
	{
		szBuffTemp[nLoop] = szNumBuffer[nLoop];

	}
	m_Row = (int)atof(szP);
	delete szP;




	//Read fields Elev, Drec, DDir, UpSLp from dbf table
	//***********************************************************
	double* pTemp = new double [m_Row*m_Col];
	double* pdElev;
	int* pTemp1 = new int [m_Row*m_Col];
	short* pTemp2 = new short int [m_Row*m_Col];
	int* pTemp3= new int [m_Row*m_Col];
	//piShedNow = new int[m_Row*m_Col];

	nBitesOfSet =0;
	int nUpSlpBitesOfSet = 0;
	int nDrecBitesOfSet = 0;
	int nDdirBitesOfSet= 0;
	int nShedNowBitesOfSet = 0;

	for(int nLoop=0;nLoop<*nElevLocation;nLoop++)
	{
		nBitesOfSet = nBitesOfSet + nFieldLength[nLoop];
	}
	for(int nLoop=0;nLoop<*nDrecLocation;nLoop++)
	{
		nDrecBitesOfSet = nDrecBitesOfSet + nFieldLength[nLoop];
	}	
	for(int nLoop=0;nLoop<*nDdirLocation;nLoop++)
	{
		nDdirBitesOfSet = nDdirBitesOfSet + nFieldLength[nLoop];
	}	
	for(int nLoop=0;nLoop<*nUpSlpLocation;nLoop++)
	{
		nUpSlpBitesOfSet = nUpSlpBitesOfSet + nFieldLength[nLoop];
	}		
	for(int nLoop=0;nLoop<*pnShedNowLocation;nLoop++)
	{
		nShedNowBitesOfSet = nShedNowBitesOfSet + nFieldLength[nLoop];
	}

	
	myFile.Seek(*piFirstRecordLoc,CFile::begin);
	
	char* szElevBuffer = new char [255];
	char* szDrecBuffer = new char [255];
	char* szDdirBuffer = new char [255];
	char* szUpSlpBuffer = new char [255];
	
	char* szShedNowBuffer = new char[255];

	for(int nLoop=0;nLoop<m_Row*m_Col;nLoop++)
	{

		myFile.Read(szBuffer,*piRecordLength);

		for(int nIndex=0;nIndex<nFieldLength[*nElevLocation];nIndex++)
		{
			szElevBuffer[nIndex] = szBuffer[nBitesOfSet +1 +nIndex];

		}
		szElevBuffer[nIndex] = 0;
		pTemp[nLoop] = atof(szElevBuffer);

		for(int nIndex=0;nIndex<nFieldLength[*nDrecLocation];nIndex++)
		{
			szDrecBuffer[nIndex] = szBuffer[nDrecBitesOfSet +1 +nIndex];

		}
		szDrecBuffer[nIndex] = 0;
		pTemp1[nLoop] = atoi(szDrecBuffer);

		for(int nIndex=0;nIndex<nFieldLength[*nDdirLocation];nIndex++)
		{
			szDdirBuffer[nIndex] = szBuffer[nDdirBitesOfSet +1 +nIndex];

		}
		
		szDdirBuffer[nIndex] = 0;
		pTemp2[nLoop] = atoi(szDdirBuffer);

		for(int nIndex=0;nIndex<nFieldLength[*nUpSlpLocation];nIndex++)
		{
			szUpSlpBuffer[nIndex] = szBuffer[nUpSlpBitesOfSet +1 +nIndex];

		}
		szUpSlpBuffer[nIndex] = 0;
		pTemp3[nLoop] = atoi(szUpSlpBuffer);

		for(int nIndex=0;nIndex<nFieldLength[*pnShedNowLocation];nIndex++)
		{
			szShedNowBuffer[nIndex] = szBuffer[nShedNowBitesOfSet +1 +nIndex];

		}
		szShedNowBuffer[nIndex] = 0;
		//piShedNow[nLoop] = atoi(szShedNowBuffer);

	}
	

	pdElev = pTemp;
	//piDrec = pTemp1;
	//piDdir = pTemp2;


	//piUpSlope = pTemp3;


	myFile.Close();
	
	//piSeedType = new short int[m_Row*m_Col];

	//if(piSeedType ==NULL) 
	//{
	//	AfxMessageBox("Not enough memory for seed type array.");
	//	exit(0);
	//}

	//piChan_No = new int [m_Row*m_Col];

	//if(piChan_No ==NULL)
	//{
	//	AfxMessageBox("Not enough memory for Chan_No array.");
	//	exit(0);
	//}


	delete nUpSlpLocation;
	delete nDrecLocation;
	delete nDdirLocation;
	delete szUpSlpBuffer;
	delete szDdirBuffer;
	delete szDrecBuffer;
	delete piRecordLength;
	delete nRowLocation;
	delete nColLocation;
	delete pShortInt;
	delete piFirstRecordLoc;
	delete nElevLocation;
	delete nNumFields;
	delete szElevBuffer;
	delete szShedNowBuffer;
	delete pnShedNowLocation;
	return pdElev;
}



void CLandmapDlg::OnOK() 
{
	bKillThread = false;
	CPtrArray* pPointerArray = new CPtrArray;
	//CProgressBarDialog		dlgProgressBar(this);
	UpdateData();
	if(m_WorkingDirectory == "")
	{
		AfxMessageBox("Please select a valid elevation file.");
	}
	else
	{
		if(m_FileName == "")
		{
			AfxMessageBox("Please select a valid elevation file.");
		}

		else
		{
			pPointerArray->Add(&m_WorkingDirectory);//0
			pPointerArray->Add( & m_FileName);//1
			pPointerArray->Add(& m_Row);//2
			pPointerArray->Add(& m_Column);//3
			pPointerArray->Add(& m_GridSize);//4
			pPointerArray->Add( & m_MissDataValue);//5
			pPointerArray->Add( & m_PitArea);//6
			pPointerArray->Add(& m_PitDepth);//7
			pPointerArray->Add(& m_PitVolumn);//8


			
			pPointerArray->Add((this)->m_hWnd);//9
			
			pPointerArray->Add(&m_PROGRESS1);//10
			pPointerArray->Add(&m_PROGRESS2);//11
			
			pPointerArray->Add(&sFullCurrentPath);

			pPointerArray->Add(&bKillThread);
			pPointerArray->Add(&m_InvertedElev);//14
			AfxBeginThread(RUN_PROCESS, pPointerArray);





		}

	}




}

void CLandmapDlg::OnButtonDemfile() 
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
					
					m_WorkingDirectory = m_FileName.Left(nLoop22 +1);
					nLoop22 = 0;
				}
			}
				
			//Create a 3 character prefix for the name
			CString s;
			s = m_FileName;
			s.MakeUpper();
			if(s.Find("ELEV") == -1  )
			{
				AfxMessageBox("You must select file named XXXelev.dbf");
			}
			else
			{
				str3CharName = m_FileName.Left(m_FileName.GetLength() - 4);
				str3CharName = str3CharName.Right(7);
				m_FileName = str3CharName;
			UpdateData(FALSE);
			}
		}
	}
	// TODO: Add your control notification handler code here
	
	delete dlgFile;






	// TODO: Add your control notification handler code here
	
}

void CLandmapDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CLandmapDlg::OnBUTTONCancel() 
{
	// TODO: Add your control notification handler code here
	bKillThread = true;
}
