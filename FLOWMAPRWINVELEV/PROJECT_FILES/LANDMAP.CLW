; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CLandmapDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "landmap.h"

ClassCount=4
Class1=CLandmapApp
Class2=CLandmapDlg
Class3=CAboutDlg

ResourceCount=5
Resource1=IDD_LANDMAP_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CProgressBarDialog
Resource4=IDD_PROGRESSBARDIALOG_DIALOG
Resource5=IDD_DIALOG1

[CLS:CLandmapApp]
Type=0
HeaderFile=landmap.h
ImplementationFile=landmap.cpp
Filter=N

[CLS:CLandmapDlg]
Type=0
HeaderFile=landmapDlg.h
ImplementationFile=landmapDlg.cpp
Filter=D
LastObject=IDC_CHECK_INVERTED
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=landmapDlg.h
ImplementationFile=landmapDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_LANDMAP_DIALOG]
Type=1
Class=CLandmapDlg
ControlCount=27
Control1=IDC_FILENAME,edit,1484849280
Control2=IDC_EDIT_WORKING_DIRECTORY,edit,1484849280
Control3=IDC_ROW,edit,1350631552
Control4=IDC_COLUMN,edit,1350631552
Control5=IDC_GSIZE,edit,1350631552
Control6=IDC_MISSDATA,edit,1350631552
Control7=IDC_PITAREA,edit,1350631552
Control8=IDC_PITDEPTH,edit,1350631552
Control9=IDOK,button,1342242817
Control10=IDCANCEL,button,1342242816
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342308352
Control15=IDC_STATIC,static,1342308352
Control16=IDC_STATIC,static,1342308352
Control17=IDC_STATIC,static,1342308352
Control18=IDC_STATIC,static,1342308352
Control19=IDC_STATIC,static,1342308353
Control20=IDC_STATIC,static,1342308352
Control21=IDC_PROGRESS1,msctls_progress32,1082130432
Control22=IDC_PROGRESS2,msctls_progress32,1350565888
Control23=IDC_STATIC_CURRENTPROCESS,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_BUTTON_DEMFILE,button,1342242816
Control26=IDC_BUTTON_Cancel,button,1342242816
Control27=IDC_CHECK_INVERTED,button,1342242819

[DLG:IDD_PROGRESSBARDIALOG_DIALOG]
Type=1
Class=CProgressBarDialog
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PROGRESS_BAR2,msctls_progress32,1350565888
Control4=IDC_PROGRESS_BAR1,msctls_progress32,1350565888
Control5=IDC_CURRENT_PROCESS,static,1342308352
Control6=IDC_OVERALL_PROGRESS,static,1342308352
Control7=IDC_USER_MESSAGE,static,1342308352

[CLS:CProgressBarDialog]
Type=0
HeaderFile=ProgressBarDialog.h
ImplementationFile=ProgressBarDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_OVERALL_PROGRESS

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

