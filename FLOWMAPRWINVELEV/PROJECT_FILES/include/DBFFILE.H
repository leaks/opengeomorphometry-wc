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
// DbfFile.h: interface for the CDbfFile class.
//
//////////////////////////////////////////////////////////////////////

#include "StrArray.h"
#if !defined(AFX_DBFFILE_H__459C4BD1_6AAC_11D4_A967_0000B434B8D3__INCLUDED_)
#define AFX_DBFFILE_H__459C4BD1_6AAC_11D4_A967_0000B434B8D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef USE_DBMALLOC
#include <dbmalloc.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char uchar;


// max's for the DBF file
#define MAX_DBFCOL_LEN		10
#define MAX_DBFCOLS			255
#define MAX_DBF_RECSIZE		4000

typedef enum
{
	APPEND,
	OVERWRITE,
	KEEP
} ACCESSMODE;

typedef unsigned char uchar;

#define TRIM_DBF_WHITESPACE

#define DISABLE_MULTIPATCH_MEASURE

/************************************************************************/
/*                             SHP Support.                             */
/************************************************************************/
typedef enum 
{
	FTLogical,
	FTString,
	FTInteger,
	FTDouble,
	FTInvalid
}	DBFFieldType;

#ifdef __cplusplus
}
#endif


/************************************************************************/
/*                             DBF Support.                             */
/************************************************************************/
typedef	struct
{
	FILE	*fp;

	int     nRecords;

	int		nRecordLength;
	int		nHeaderLength;
	int		nFields;
	int		*panFieldOffset;
	int		*panFieldSize;
	int		*panFieldDecimals;
	char	*pachFieldType;

	char	*pszHeader;

	int		nCurrentRecord;
	int		bCurrentRecordModified;
	char	*pszCurrentRecord;
    
	int		bNoHeader;
	int		bUpdated;
} DBFInfo;

typedef DBFInfo * DBFHandle;

/* dBase file structure */
struct DB_HDR
{
  unsigned char      magic_no;
  unsigned char      year;
  unsigned char      month;
  unsigned char      date;
  long      record_no;
  unsigned short     header_byte;
  unsigned short     record_byte;
  unsigned short     reserved1;
  unsigned char      flag;
  unsigned char      encrypt_flag;
  unsigned char      res_multi[12];
  unsigned char      mdx_flag;
  unsigned char      lang_id;
  unsigned short     reserved2;
};

struct DB_FIELD
{
  char      name[11];
  char      type;
  long      reserved;
  char      length;
  char      decimal;
  short     reserved1;
  char      work_id;
  char      reserved2[10];
  char      ind_flag;
};


class CDbfFile  
{
public:
	CFileFind ShapeFile;
	DBFHandle hDBF;
	int         AddField(CStringArray* strField);

				CDbfFile();
				CDbfFile(CString FileName, ACCESSMODE AppendOverwriteKeep);
	virtual		~CDbfFile();

	int			Dbf2Grid(double *Grid, FILE *Inpf, int Row, int Column);

	int			DBFAddRecord(CStringArray* sDBFFieldValList);
	int			DBFAddRecord(DBFHandle DBF, CStringArray* sDBFFieldValList);
	int			DBFSetColumn(CStringArray* sColNameList);
	int			AddDBF(CStringArray* AttValueList);
	int			AddDBF(CStringArray* AttValueList, CString Delimiter);
    int			AddDBF3(char** psDBFAttList,CString sDelimiter, int nSize);
	int			AddDBF4(char* psDBFAttList,CString sDelimiter, int nSize);
	int			SetDBFColumn(CStringArray* DBFAttList);
	int			SetDBFColumn(CString DBFAttItem);
	int			SetDBFColumn(CString Name, int Width);
	int			SetDBFColumn(CString Name, int Size, int Precision);
	int         SetDBFColumnLogical(CString sName, int iSize, int iPrecision);
	BOOL		ValidateDBF(void);
protected:

	void		*SfRealloc(void * pMem, int nNewSize);

	int			DBFAddField(DBFHandle psDBF, const char * pszFieldName, 
							DBFFieldType eType, int nWidth, int nDecimals);
	void		DBFClose(DBFHandle psDBF);
	void		*DBFReadAttribute(DBFHandle psDBF, int hEntity, int iField, char chReqType);
	double		DBFReadDoubleAttribute(DBFHandle psDBF, int iRecord, int iField);
	int			DBFReadIntegerAttribute(DBFHandle psDBF, int iRecord, int iField);
	const char	*DBFReadStringAttribute(DBFHandle psDBF, int iRecord, int iField);
	int			DBFWriteAttribute(DBFHandle psDBF, int hEntity, int iField, void * pValue);
	int			DBFWriteIntegerAttribute(DBFHandle psDBF, int iRecord, int iField, int nValue);
	int			DBFWriteHeader(DBFHandle psDBF);
	void		DBFFlushRecord(DBFHandle psDBF);
	DBFHandle	DBFCreate();
	DBFHandle	DBFOpen(const char * pszAccess);
	DBFFieldType DBFGetFieldInfo(DBFHandle psDBF, int iField, char * pszFieldName,
								int * pnWidth, int * pnDecimals);
	int			DBFGetFieldCount(DBFHandle psDBF);
	int			DBFGetRecordCount(DBFHandle psDBF);
	int			DBFWriteStringAttribute(DBFHandle psDBF, int iRecord, int iField,
										const char * pszValue);
	int			DBFWriteDoubleAttribute(DBFHandle psDBF, int iRecord, int iField, double dValue);
	

public:
	int WriteRecord(int hEntity,int iField, void *pValue);
	void Close();
	void Initialize();

	INT*		_piPartType;
	CString		_szFileName;
	CString		_szDBFName;
	ACCESSMODE	_eMode;
	BOOL		_bInit;
	CStringArray	_psDBFAttList;
};

#endif // !defined(AFX_DBFFILE_H__459C4BD1_6AAC_11D4_A967_0000B434B8D3__INCLUDED_)
