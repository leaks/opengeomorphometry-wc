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
// GridReadWriteUtilityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GridReadWriteUtility.h"
#include "GridReadWriteUtilityDlg.h"


#include "DbfFile.h"

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
// CGridReadWriteUtilityDlg dialog

CGridReadWriteUtilityDlg::CGridReadWriteUtilityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGridReadWriteUtilityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGridReadWriteUtilityDlg)
	m_RADIO_BUTTON = -1;
	m_EDIT_INPUT = _T("");
	m_EDIT_OUTPUT = _T("");
	m_NumCols = 0;
	m_MissingData = 0.0;
	m_NumRows = 0;
	m_X = 0.0;
	m_Y = 0.0;
	m_CellSize = 0;
	m_FIELDWIDTH = 0;
	m_FIELDPRECISION = 0;
	m_FIELDNAME = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGridReadWriteUtilityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridReadWriteUtilityDlg)
	DDX_Control(pDX, IDC_COMBO_FIELDTYPE, m_FIELDTYPE);
	DDX_Control(pDX, IDC_COMBO_FIELDS, m_ComboBox);
	DDX_Control(pDX, IDC_PROGRESS1, m_PROGRESS_BAR);
	DDX_Radio(pDX, IDC_RADIO1, m_RADIO_BUTTON);
	DDX_Text(pDX, IDC_EDIT_INPUT, m_EDIT_INPUT);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_EDIT_OUTPUT);
	DDX_Text(pDX, IDC_EDIT_COLUMNS, m_NumCols);
	DDX_Text(pDX, IDC_EDIT_MISSINGDATA, m_MissingData);
	DDX_Text(pDX, IDC_EDIT_ROWS, m_NumRows);
	DDX_Text(pDX, IDC_EDIT_X, m_X);
	DDX_Text(pDX, IDC_EDIT_Y, m_Y);
	DDX_Text(pDX, IDC_EDIT_CELLSIZE, m_CellSize);
	DDX_Text(pDX, IDC_EDIT_FIELD_WIDTH, m_FIELDWIDTH);
	DDX_Text(pDX, IDC_EDIT_PRECISION, m_FIELDPRECISION);
	DDX_Text(pDX, IDC_EDIT_FIELDNAME, m_FIELDNAME);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGridReadWriteUtilityDlg, CDialog)
	//{{AFX_MSG_MAP(CGridReadWriteUtilityDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INPUT, OnButtonInput)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, OnButtonOutput)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_EN_CHANGE(IDC_EDIT_ROWS, OnChangeEditRows)
	ON_EN_CHANGE(IDC_EDIT_COLUMNS, OnChangeEditColumns)
	ON_EN_CHANGE(IDC_EDIT_X, OnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, OnChangeEditY)
	ON_EN_CHANGE(IDC_EDIT_CELLSIZE, OnChangeEditCellsize)
	ON_EN_CHANGE(IDC_EDIT_MISSINGDATA, OnChangeEditMissingdata)
	ON_BN_CLICKED(IDC_BUTTON_LOADHEADER, OnButtonLoadheader)
	ON_EN_CHANGE(IDC_EDIT_FIELD_WIDTH, OnChangeEditFieldWidth)
	ON_CBN_EDITCHANGE(IDC_COMBO_FIELDTYPE, OnEditchangeComboFieldtype)
	ON_EN_CHANGE(IDC_EDIT_PRECISION, OnChangeEditPrecision)
	ON_EN_CHANGE(IDC_EDIT_FIELDNAME, OnChangeEditFieldname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridReadWriteUtilityDlg message handlers

BOOL CGridReadWriteUtilityDlg::OnInitDialog()
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
	
	m_RADIO_BUTTON= 1;
	m_MissingData = -9999.0;


	m_FIELDPRECISION = 4;
	m_FIELDWIDTH = 12;

	m_FIELDTYPE.AddString("Number");
	m_FIELDTYPE.AddString("String");
	m_FIELDTYPE.AddString("Boolean");

	m_FIELDNAME = "Elev";

	m_FIELDTYPE.SetCurSel(0);
	// TODO: Add extra initialization here
	UpdateData(false);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGridReadWriteUtilityDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGridReadWriteUtilityDlg::OnPaint() 
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
HCURSOR CGridReadWriteUtilityDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGridReadWriteUtilityDlg::OnOK() 
{
	// TODO: Add extra validation here

	CFile oInputFile;
	int nLoc = 0;
	char sBuffer[255];

	int nStart;
	char sBuffer2[255];

	int nLoop=0;

	int nIndex = 0;
	
	int nLoop2 = 0;
	UpdateData(false);


	bool bMissing = false;














	if(sInputFileName == "")
	{
		AfxMessageBox("You must provide input file.");
		bMissing = true;
	}
	if(sOutputFileName == "")
	{
		AfxMessageBox("You must provide output file.");
		bMissing = true;
	}






















	bool bKillThread = false;	

	if(bMissing == false)
	{
		if(m_RADIO_BUTTON == 1 ) 
		{






			if(bMissing == false)
			{
				if(oInputFile.Open(sInputFileName,CFile::modeRead ) != 0)
				{

					

					oInputFile.Read(sBuffer,255);
					CString s = sBuffer;
					

					
					//Find and read ncols information

					nLoc = s.Find("ncols");
					if(nLoc == -1)
					{
						AfxMessageBox("ncols not found in header file.");
						exit(0);
					}
					for (nLoop=nLoc + 6;nLoop<s.GetLength();nLoop++)
					{
						if(s.GetAt(nLoop) != 32)
						{
							for(nLoop2=nLoop;nLoop2<s.GetLength();nLoop2++)
							{
								if(s.GetAt(nLoop2) != 13)
								{
									sBuffer2[nIndex] = s.GetAt(nLoop2);
									nIndex++;
								}
								else
								{
									sBuffer2[nIndex] = 12;
									nLoop2 = s.GetLength();
									nLoop = s.GetLength();
								}
							}
						}
					}
					nCols = atoi(sBuffer2);



					







					

					//Find and read nrows information

					nLoc = s.Find("nrows");
					nIndex = 0;
					if(nLoc == -1)
					{
						AfxMessageBox("nrowss not found in header file.");
						exit(0);
					}
					for (nLoop=nLoc + 6;nLoop<s.GetLength();nLoop++)
					{
						if(s.GetAt(nLoop) != 32)
						{
							for(nLoop2=nLoop;nLoop2<s.GetLength();nLoop2++)
							{
								if(s.GetAt(nLoop2) != 13)
								{
									sBuffer2[nIndex] = s.GetAt(nLoop2);
									nIndex++;
								}
								else
								{
									sBuffer2[nIndex] = 12;
									nLoop2 = s.GetLength();
									nLoop = s.GetLength();
								}
							}
						}
					}
					nRows = atoi(sBuffer2);


					//Find and read nodata_value information

					nLoc = s.Find("NODATA_value");
					nIndex = 0;
					if(nLoc == -1)
					{
						AfxMessageBox("NODATA_value not found in header file.");
						exit(0);
					}
					for (nLoop=nLoc + 13;nLoop<s.GetLength();nLoop++)
					{
						if(s.GetAt(nLoop) != 32)
						{
							for(nLoop2=nLoop;nLoop2<s.GetLength();nLoop2++)
							{
								if(s.GetAt(nLoop2) != 13)
								{
									sBuffer2[nIndex] = s.GetAt(nLoop2);
									nIndex++;
								}
								else
								{
									sBuffer2[nIndex] = 12;
									nLoop2 = s.GetLength();
									nLoop = s.GetLength();
								}
							}
						}
					}
					NoDataValue = atof(sBuffer2);






					//Find and read byteorder information

					nLoc = s.Find("byteorder");
					nIndex = 0;
					if(nLoc == -1)
					{
						AfxMessageBox("byteorder not found in header file.");
						exit(0);
					}
					for (nLoop=nLoc + 10;nLoop<s.GetLength();nLoop++)
					{
						if(s.GetAt(nLoop) != 32)
						{
							for(nLoop2=nLoop;nLoop2<s.GetLength();nLoop2++)
							{
								if(s.GetAt(nLoop2) != 13)
								{
									sBuffer2[nIndex] = s.GetAt(nLoop2);
									nIndex++;
								}
								else
								{
									sBuffer2[nIndex] = 12;
									nLoop2 = s.GetLength();
									nLoop = s.GetLength();
								}
							}
						}
					}
					sByteOrder = sBuffer2;



					double* pVal = new double [nRows*nCols];
					GetDlgItem(IDC_STATIC_STATUS)->SetWindowText("Running...");
					


					oInputFile.Close();

					oInputFile.Open(sInputFileName.Left(sInputFileName.GetLength() - 3) + "flt",CFile::modeRead);
					


				
					CDbfFile oOutputFile;
					

					if(m_FIELDTYPE.GetCurSel() == 0)
					{

						if(m_FIELDWIDTH > 4) 
						{
							if(m_FIELDPRECISION > 0) 
							{
								//CDbfFile oOutputFile;
								oOutputFile.AddField(m_FIELDNAME,'D', m_FIELDWIDTH,m_FIELDPRECISION);
								
								double dVal;

								oOutputFile.SetInitialFileSize(nRows*nCols);

								for(nLoop=0;nLoop<nRows*nCols;nLoop++)
								{
									oInputFile.Read(sBuffer,4);

									//sBuffer2[0] = sBuffer[3];
									//sBuffer2[1] = sBuffer[2];
									//sBuffer2[2] = sBuffer[1];
									//sBuffer2[3] = sBuffer[0];
									
									pVal[nLoop]  = *((float*) sBuffer);

								}
								for (nLoop=0;nLoop<nRows*nCols;nLoop++)
								{
									oOutputFile.AddRecord();
									dVal = pVal[nLoop];
									oOutputFile.SetValue(0,nLoop, dVal);

								}
							}
							else
							{
								//CDbfFile oOutputFile;
								oOutputFile.AddField(m_FIELDNAME,'I', m_FIELDWIDTH,0);
								
								int nVal;

								oOutputFile.SetInitialFileSize(nRows*nCols);

								for(nLoop=0;nLoop<nRows*nCols;nLoop++)
								{
									oInputFile.Read(sBuffer,4);

									//sBuffer2[0] = sBuffer[3];
									//sBuffer2[1] = sBuffer[2];
									//sBuffer2[2] = sBuffer[1];
									//sBuffer2[3] = sBuffer[0];
									
									pVal[nLoop]  = *((float*) sBuffer);

								}
								for (nLoop=0;nLoop<nRows*nCols;nLoop++)
								{
									oOutputFile.AddRecord();
									nVal = pVal[nLoop];
									oOutputFile.SetValue(0,nLoop, nVal);

								}
							}
						}
						else
						{
							if(m_FIELDPRECISION > 0)
							{
								
								oOutputFile.AddField(m_FIELDNAME,'F', m_FIELDWIDTH,m_FIELDPRECISION);
								
								float nVal;

								oOutputFile.SetInitialFileSize(nRows*nCols);

								for(nLoop=0;nLoop<nRows*nCols;nLoop++)
								{
									oInputFile.Read(sBuffer,4);

									//sBuffer2[0] = sBuffer[3];
									//sBuffer2[1] = sBuffer[2];
									//sBuffer2[2] = sBuffer[1];
									//sBuffer2[3] = sBuffer[0];
									
									pVal[nLoop]  = *((float*) sBuffer);

								}
								for (nLoop=0;nLoop<nRows*nCols;nLoop++)
								{
									oOutputFile.AddRecord();
									nVal = pVal[nLoop];
									oOutputFile.SetValue(0,nLoop, nVal);

								}
							}
							else
							{
								//CDbfFile oOutputFile;
								oOutputFile.AddField(m_FIELDNAME,'S', m_FIELDWIDTH,0);
								
								short int nVal;

								oOutputFile.SetInitialFileSize(nRows*nCols);

								for(nLoop=0;nLoop<nRows*nCols;nLoop++)
								{
									oInputFile.Read(sBuffer,4);

									//sBuffer2[0] = sBuffer[3];
									//sBuffer2[1] = sBuffer[2];
									//sBuffer2[2] = sBuffer[1];
									//sBuffer2[3] = sBuffer[0];
									
									pVal[nLoop]  = *((float*) sBuffer);

								}
								for (nLoop=0;nLoop<nRows*nCols;nLoop++)
								{
									oOutputFile.AddRecord();
									nVal = pVal[nLoop];
									oOutputFile.SetValue(0,nLoop, nVal);

								}
							}
						}

					}







					//CDbfFile oOutputFile;


					GetDlgItem(IDC_STATIC_STATUS)->EnableWindow(true);

					oOutputFile.SaveToFile(sOutputFileName,&m_PROGRESS_BAR,&bKillThread);
					GetDlgItem(IDC_STATIC_STATUS)->SetWindowText("Done!");			


					UpdateData(true);

					delete pVal;

					int nValue = m_RADIO_BUTTON;
				}
				else
				{
					AfxMessageBox("Could not open input file.");
					

				}
			}
		}
		else
		{
			if(m_CellSize == 0)
			{
				AfxMessageBox("You must provide Cell Size.");
				bMissing = true;
			}


			if(m_NumRows == 0)
			{
				AfxMessageBox("You must provide Number of rows.");
				bMissing = true;
			}



			
			if(m_NumCols ==0)
			{
				AfxMessageBox("You must provide Number of columns.");
				bMissing = true;
			}

			if(m_X == 0)
			{
				AfxMessageBox("You must provide X Coordinate.");
				bMissing = true;
			}


			if(m_Y == 0)
			{
				AfxMessageBox("You must provide Y Coordinate.");
				bMissing = true;
			}


			if(m_MissingData == 0)
			{
				AfxMessageBox("You must provide Missing Data value.");
				bMissing = true;
			}

			if(m_ComboBox.GetCurSel() !=CB_ERR && bMissing == false)
			{
				GetDlgItem(IDC_STATIC_STATUS)->SetWindowText("Running...");	
				CDbfFile oInputFile;
				oInputFile.OpenFile(sInputFileName,&m_PROGRESS_BAR,&bKillThread);
				
				CFile oOutputFile;
				oOutputFile.Open(sOutputFileName,CFile::modeCreate | CFile::modeWrite);
				

				strcpy(sBuffer,"ncols         ");
				oOutputFile.Write(sBuffer,14);
				sprintf(sBuffer,"%d",m_NumCols);
				oOutputFile.Write(sBuffer,strlen(sBuffer));
				sBuffer[0] = 13;
				sBuffer[1] = 10;
				oOutputFile.Write(sBuffer,2);

				strcpy(sBuffer,"nrows           ");
				oOutputFile.Write(sBuffer,14);
				sprintf(sBuffer,"%d",m_NumRows);
				oOutputFile.Write(sBuffer,strlen(sBuffer));
				sBuffer[0] = 13;
				sBuffer[1] = 10;
				oOutputFile.Write(sBuffer,2);
				


				strcpy(sBuffer,"xllcorner     ");
				oOutputFile.Write(sBuffer,14);
				sprintf(sBuffer,"%.4f",m_X);
				oOutputFile.Write(sBuffer,strlen(sBuffer));
				sBuffer[0] = 13;
				sBuffer[1] = 10;
				oOutputFile.Write(sBuffer,2);

				strcpy(sBuffer,"yllcorner     ");
				oOutputFile.Write(sBuffer,14);
				sprintf(sBuffer,"%.4f",m_Y);
				oOutputFile.Write(sBuffer,strlen(sBuffer));
				sBuffer[0] = 13;
				sBuffer[1] = 10;
				oOutputFile.Write(sBuffer,2);

				strcpy(sBuffer,"cellsize      ");
				oOutputFile.Write(sBuffer,14);
				sprintf(sBuffer,"%.d",m_CellSize);
				oOutputFile.Write(sBuffer,strlen(sBuffer));
				sBuffer[0] = 13;
				sBuffer[1] = 10;
				oOutputFile.Write(sBuffer,2);

				strcpy(sBuffer,"NODATA_value  ");
				oOutputFile.Write(sBuffer,14);
				sprintf(sBuffer,"%.1f",m_MissingData);
				oOutputFile.Write(sBuffer,strlen(sBuffer));
				sBuffer[0] = 13;
				sBuffer[1] = 10;
				oOutputFile.Write(sBuffer,2);

				strcpy(sBuffer,"byteorder     LSBFIRST");
				oOutputFile.Write(sBuffer,22);
				sBuffer[0] = 13;
				sBuffer[1] = 10;
				oOutputFile.Write(sBuffer,2);


				//Done with the header now we can write the binary data file.


				oOutputFile.Close();

				oOutputFile.Open(sOutputFileName.Left(sOutputFileName.GetLength() - 3) + "flt",CFile::modeCreate | CFile::modeWrite);
				
				if(oInputFile.GetNumRecords() != m_NumCols*m_NumRows)
				{
					AfxMessageBox("Total number of rows and columns does not equal total number of records.");
				}
				
				int nFieldLoc = m_ComboBox.GetItemData(m_ComboBox.GetCurSel());
				float* nVal2 = new float;


				if(oInputFile.ReturnType(nFieldLoc) == 'D' )
				{
					for(int nLoop=0;nLoop<oInputFile.GetNumRecords();nLoop++)
					{
						*nVal2 =  *(double*)oInputFile.ReturnValue(nFieldLoc,nLoop);

						oOutputFile.Write(nVal2,4);

					}
				}
				if(oInputFile.ReturnType(nFieldLoc) == 'F' )
				{
					for(int nLoop=0;nLoop<oInputFile.GetNumRecords();nLoop++)
					{
						*nVal2 =  *(float*)oInputFile.ReturnValue(nFieldLoc,nLoop);

						oOutputFile.Write(nVal2,4);

					}
				}

				if( oInputFile.ReturnType(nFieldLoc) == 'I')
				{
					for(int nLoop=0;nLoop<oInputFile.GetNumRecords();nLoop++)
					{
						*nVal2 =  *(int*)oInputFile.ReturnValue(nFieldLoc,nLoop);

						oOutputFile.Write(nVal2,4);

					}
				}
				if( oInputFile.ReturnType(nFieldLoc) == 'S')
				{
					for(int nLoop=0;nLoop<oInputFile.GetNumRecords();nLoop++)
					{
						*nVal2 =  *(short int*)oInputFile.ReturnValue(nFieldLoc,nLoop);

						oOutputFile.Write(nVal2,4);

					}
				}

				delete nVal2;
				GetDlgItem(IDC_STATIC_STATUS)->SetWindowText("Done!");	
			}
			
			else
			{
				AfxMessageBox("You must select input field.");

			}
			


		}
	}

	
	//CDialog::OnOK();
}

void CGridReadWriteUtilityDlg::OnButtonInput() 
{
	// TODO: Add your control notification handler code here
	
	if(m_RADIO_BUTTON == 1)
	{
		CFileDialog* dlgFile = new CFileDialog(TRUE,"","",FALSE,"Header Files (*.hdr)|*.hdr");
		if(dlgFile->DoModal() == IDOK)
		{
			sInputFileName = dlgFile->GetPathName();
			
		}
		delete dlgFile;
	}
	else
	{
		CFileDialog* dlgFile = new CFileDialog(TRUE,"","",FALSE,"FoxPro Tables (*.dbf)|*.dbf");
		if(dlgFile->DoModal() == IDOK)
		{
			sInputFileName = dlgFile->GetPathName();
			PopulateFields();
			
		}
		delete dlgFile;
	}
	
	m_EDIT_INPUT = sInputFileName;
	UpdateData(false);
}

void CGridReadWriteUtilityDlg::OnButtonOutput() 
{
	// TODO: Add your control notification handler code here
	
	if(m_RADIO_BUTTON == 1)
	{
		CFileDialog* dlgFile = new CFileDialog(FALSE,"","",FALSE,"FOX Pro DBF Files (*.dbf)|*.dbf");
		if(dlgFile->DoModal()==IDOK)
		{
			sOutputFileName = dlgFile->GetPathName();

		}
		delete dlgFile;
	}
	else
	{
		CFileDialog* dlgFile = new CFileDialog(FALSE,"","",FALSE,"Header Files (*.hdr)|*.hdr");
		if(dlgFile->DoModal()==IDOK)
		{
			sOutputFileName = dlgFile->GetPathName();
		}
		delete dlgFile;
	}
	m_EDIT_OUTPUT = sOutputFileName;
	UpdateData(false);
}

void CGridReadWriteUtilityDlg::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(true);
	UpdateData(true);
	

	
	InableGridFields();
}

void CGridReadWriteUtilityDlg::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	InableGridFields();
}

void CGridReadWriteUtilityDlg::PopulateFields()
{
	char szBuffer[255];
	bool bReturnValue = false;
	CFile myFile;

	short int nEachFieldLength[255];
	short int nEachFieldDecimal[255];

	char szFieldType[255];
	
	int nSizeOfFile = 0;

	int nNumOfRows = 0;
	CString pColumnName[255]; 
	int nResult = 0;

	int nNumOfColumns = 0;
	nResult = myFile.Open(sInputFileName,CFile::modeRead);
	if(nResult!=0)
	{

		int** pColumnLocation = new int* [255];

		myFile.Seek(4,CFile::begin);
		myFile.Read(szBuffer,4);

		int* pNumRecInFile = new int;
		*pNumRecInFile = *(int*) &szBuffer;
		
		nSizeOfFile = *pNumRecInFile;
		nNumOfRows = nSizeOfFile;

		/*
		if(pProgressBar !=0)
		{
			pProgressBar->SetRange32(0,*pNumRecInFile);
			pProgressBar->SetStep(1);
			pProgressBar->SetPos(0);
		}
		*/
		int* pShortInt = pNumRecInFile;
		myFile.Seek(32,CFile::begin);
		int nLoop = 0;

		for (nLoop = 0;nLoop<255;nLoop++)
		{
			pColumnLocation[nLoop] = new int;
			*pColumnLocation[nLoop] = -1;
		}


		
		int nFieldLength[35];

		for(nLoop = 0;nLoop<255;nLoop++)
		{
			myFile.Read(szBuffer,32);
			if(szBuffer[0] !=13)
			{

				nFieldLength[nLoop] = szBuffer[16];
				pColumnName[nNumOfColumns] = szBuffer;
				nEachFieldLength[nNumOfColumns] = szBuffer[16];
				nEachFieldDecimal[nNumOfColumns] = szBuffer[17];
				szFieldType[nNumOfColumns] = szBuffer[11];

				*pColumnLocation[nNumOfColumns] = nLoop;
				nNumOfColumns++;

			}
			else
			{
				nLoop = 255;
			}
		}

		m_ComboBox.ResetContent();
		int nLoop5=0;
		for (nLoop=0;nLoop<nNumOfColumns;nLoop++)
		{
			if(szFieldType[nLoop] !='L')
			{
				m_ComboBox.AddString(pColumnName[nLoop]);
				m_ComboBox.SetItemData(nLoop5,nLoop);
				nLoop5++;
			}
		}


		for (nLoop = 0;nLoop<255;nLoop++)
		{
			delete pColumnLocation[nLoop] ;
		}

	}

}

void CGridReadWriteUtilityDlg::InableGridFields()
{
	UpdateData(true);
	if(m_RADIO_BUTTON == 0) 
	{
		GetDlgItem(IDC_EDIT_ROWS)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_COLUMNS)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_X)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_Y)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_MISSINGDATA)->EnableWindow(true);
		GetDlgItem(IDC_COMBO_FIELDS)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_CELLSIZE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_LOADHEADER)->EnableWindow(true);		
		GetDlgItem(IDC_COMBO_FIELDTYPE)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_FIELD_WIDTH)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_PRECISION)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_FIELDNAME)->EnableWindow(false);		


			
		
		//this->GetDlgControl(IDC_EDIT_ROWS)->EnableWindow();

	}
	else
	{
		GetDlgItem(IDC_EDIT_ROWS)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_COLUMNS)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_X)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_Y)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_MISSINGDATA)->EnableWindow(false);
		GetDlgItem(IDC_COMBO_FIELDS)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_CELLSIZE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_LOADHEADER)->EnableWindow(false);	
		GetDlgItem(IDC_COMBO_FIELDTYPE)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_FIELD_WIDTH)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_PRECISION)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_FIELDNAME)->EnableWindow(true);
	}

}

void CGridReadWriteUtilityDlg::OnChangeEditRows() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CGridReadWriteUtilityDlg::OnChangeEditColumns() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CGridReadWriteUtilityDlg::OnChangeEditX() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CGridReadWriteUtilityDlg::OnChangeEditY() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CGridReadWriteUtilityDlg::OnChangeEditCellsize() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);

}

void CGridReadWriteUtilityDlg::OnChangeEditMissingdata() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CGridReadWriteUtilityDlg::OnButtonLoadheader() 
{
	// TODO: Add your control notification handler code here
	
	CFile oInputFile;
	int nLoc = 0;
	char sBuffer[255];

	int nStart;
	char sBuffer2[255];

	int nLoop=0;

	int nIndex = 0;
	
	int nLoop2 = 0;
	UpdateData(false);

	
	CFileDialog* dlgFile = new CFileDialog(TRUE,"","",FALSE,"Header files (*.hdr)|*.hdr");
	if(dlgFile->DoModal() == IDOK)
	{
		CFile oInputFile;
		sInputFileName = dlgFile->GetPathName();
		
		if(oInputFile.Open(sInputFileName,CFile::modeRead ) != 0)
		{

			

			oInputFile.Read(sBuffer,255);
			CString s = sBuffer;
			//Find and read ncols information

			nLoc = s.Find("ncols");
			if(nLoc == -1)
			{
				AfxMessageBox("ncols not found in header file.");
				exit(0);
			}
			for (nLoop=nLoc + 6;nLoop<s.GetLength();nLoop++)
			{
				if(s.GetAt(nLoop) != 32)
				{
					for(nLoop2=nLoop;nLoop2<s.GetLength();nLoop2++)
					{
						if(s.GetAt(nLoop2) != 13)
						{
							sBuffer2[nIndex] = s.GetAt(nLoop2);
							nIndex++;
						}
						else
						{
							sBuffer2[nIndex] = 12;
							nLoop2 = s.GetLength();
							nLoop = s.GetLength();
						}
					}
				}
			}
			nCols = atoi(sBuffer2);



			m_NumCols = nCols;







			

			//Find and read nrows information

			nLoc = s.Find("nrows");
			nIndex = 0;
			if(nLoc == -1)
			{
				AfxMessageBox("nrowss not found in header file.");
				exit(0);
			}
			for (nLoop=nLoc + 6;nLoop<s.GetLength();nLoop++)
			{
				if(s.GetAt(nLoop) != 32)
				{
					for(nLoop2=nLoop;nLoop2<s.GetLength();nLoop2++)
					{
						if(s.GetAt(nLoop2) != 13)
						{
							sBuffer2[nIndex] = s.GetAt(nLoop2);
							nIndex++;
						}
						else
						{
							sBuffer2[nIndex] = 12;
							nLoop2 = s.GetLength();
							nLoop = s.GetLength();
						}
					}
				}
			}
			nRows = atoi(sBuffer2);

			m_NumRows = nRows;
			//Find and read nodata_value information

			nLoc = s.Find("NODATA_value");
			nIndex = 0;
			if(nLoc == -1)
			{
				AfxMessageBox("NODATA_value not found in header file.");
				exit(0);
			}
			for (nLoop=nLoc + 13;nLoop<s.GetLength();nLoop++)
			{
				if(s.GetAt(nLoop) != 32)
				{
					for(nLoop2=nLoop;nLoop2<s.GetLength();nLoop2++)
					{
						if(s.GetAt(nLoop2) != 13)
						{
							sBuffer2[nIndex] = s.GetAt(nLoop2);
							nIndex++;
						}
						else
						{
							sBuffer2[nIndex] = 12;
							nLoop2 = s.GetLength();
							nLoop = s.GetLength();
						}
					}
				}
			}
			NoDataValue = atof(sBuffer2);
			m_MissingData = NoDataValue;

			//Find and read X information
			

			nLoc = s.Find("xllcorner");
			nIndex = 0;
			if(nLoc == -1)
			{
				AfxMessageBox("xllcorner not found in header file.");
				exit(0);
			}
			for (nLoop=nLoc + 10;nLoop<s.GetLength();nLoop++)
			{
				if(s.GetAt(nLoop) != 32)
				{
					for(nLoop2=nLoop;nLoop2<s.GetLength();nLoop2++)
					{
						if(s.GetAt(nLoop2) != 13)
						{
							sBuffer2[nIndex] = s.GetAt(nLoop2);
							nIndex++;
						}
						else
						{
							sBuffer2[nIndex] = 12;
							nLoop2 = s.GetLength();
							nLoop = s.GetLength();
						}
					}
				}
			}
			m_X = atof(sBuffer2);
			

			//Find and read Y information

			nLoc = s.Find("yllcorner");
			nIndex = 0;
			if(nLoc == -1)
			{
				AfxMessageBox("yllcorner not found in header file.");
				exit(0);
			}
			for (nLoop=nLoc + 10;nLoop<s.GetLength();nLoop++)
			{
				if(s.GetAt(nLoop) != 32)
				{
					for(nLoop2=nLoop;nLoop2<s.GetLength();nLoop2++)
					{
						if(s.GetAt(nLoop2) != 13)
						{
							sBuffer2[nIndex] = s.GetAt(nLoop2);
							nIndex++;
						}
						else
						{
							sBuffer2[nIndex] = 12;
							nLoop2 = s.GetLength();
							nLoop = s.GetLength();
						}
					}
				}
			}
			m_Y = atof(sBuffer2);


			//Find and read CellSize information

			nLoc = s.Find("cellsize");
			nIndex = 0;
			if(nLoc == -1)
			{
				AfxMessageBox("yllcorner not found in header file.");
				exit(0);
			}
			for (nLoop=nLoc + 9;nLoop<s.GetLength();nLoop++)
			{
				if(s.GetAt(nLoop) != 32)
				{
					for(nLoop2=nLoop;nLoop2<s.GetLength();nLoop2++)
					{
						if(s.GetAt(nLoop2) != 13)
						{
							sBuffer2[nIndex] = s.GetAt(nLoop2);
							nIndex++;
						}
						else
						{
							sBuffer2[nIndex] = 12;
							nLoop2 = s.GetLength();
							nLoop = s.GetLength();
						}
					}
				}
			}
			m_CellSize = atof(sBuffer2);

			//Find and read byteorder information

			nLoc = s.Find("byteorder");
			nIndex = 0;
			if(nLoc == -1)
			{
				AfxMessageBox("byteorder not found in header file.");
				exit(0);
			}
			for (nLoop=nLoc + 10;nLoop<s.GetLength();nLoop++)
			{
				if(s.GetAt(nLoop) != 32)
				{
					for(nLoop2=nLoop;nLoop2<s.GetLength();nLoop2++)
					{
						if(s.GetAt(nLoop2) != 13)
						{
							sBuffer2[nIndex] = s.GetAt(nLoop2);
							nIndex++;
						}
						else
						{
							sBuffer2[nIndex] = 12;
							nLoop2 = s.GetLength();
							nLoop = s.GetLength();
						}
					}
				}
			}
			sByteOrder = sBuffer2;
			
			UpdateData(false);

		}
	}

}

void CGridReadWriteUtilityDlg::OnChangeEditFieldWidth() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CGridReadWriteUtilityDlg::OnEditchangeComboFieldtype() 
{
		// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CGridReadWriteUtilityDlg::OnChangeEditPrecision() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CGridReadWriteUtilityDlg::OnChangeEditFieldname() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);
}
