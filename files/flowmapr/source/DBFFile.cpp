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
/*
 * mod: shapefile.cpp
 * hdr: shapefile.h
 * cls: CSHAPEFILE
 *
 * inf: Handles the creation of shape file. The main routine is a
 *		public domain C library created by Frank Warmerdam.
 *
 * mtd: 
 * 
 * var:
 *
 * ver: 99.04.26		bcl/gismo		created.
 */

// DbfFile.cpp: implementation of the CDbfFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "landmap.h"
#include "DbfFile.h"
#include <math.h>
#include <limits.h>
#include <assert.h>


#include "StrArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


typedef unsigned char uchar;

#if UINT_MAX == 65535
typedef long      int32;
#else
typedef int	      int32;
#endif

#ifndef FALSE
#  define FALSE		0
#  define TRUE		1
#endif

#define ByteCopy( a, b, c )	memcpy( b, a, c )
#ifndef MAX
#  define MIN(a,b)      ((a<b) ? a : b)
#  define MAX(a,b)      ((a>b) ? a : b)
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDbfFile::CDbfFile()
{
}


CDbfFile::CDbfFile(CString szFileName, ACCESSMODE eMode)
{
	_szFileName = szFileName;
	_eMode = eMode;
	_bInit = true;
}

CDbfFile::~CDbfFile()
{

}


int CDbfFile::Dbf2Grid(double *pfGrid, FILE *hDBFf, int iRow, int iColumn)
{
	// Function to populate grid array with data from  .dbf file
	// DEM grid will replace 3 row arrays(row1z,row2z,row3z) and will eliminate the need to
	// constanly load and unload data related to dem.

	int		i, j, l;
	int		iRet=0;
	

	int nMissingStart = 0;
	int nMissingLength = 0;
	/* Routine to read the DBF file */
	struct	DB_HDR db_hdr;
	struct	DB_FIELD db_field;

	char	temp[600],
			temp1[10];


	int	startField=0;
	int lengthField;

	/* read the DBF header */
	l = fread((char *)&db_hdr, sizeof(struct DB_HDR), 1, hDBFf);

	if (l != 1) 
	{
		AfxMessageBox("Error reading input file.");
		exit(0);
	}
		//return 1;
	db_hdr.header_byte = db_hdr.header_byte - 32;

	j = 1;
	/* we need to check out where is the "lblrecno" field */
	while (db_hdr.header_byte > 31) 
	{
		l = fread((DB_FIELD *)&db_field, sizeof(struct DB_FIELD), 1, hDBFf);
		if (l != 1) 
			return 1;
	
		if (_stricmp(db_field.name, "elev") == 0) 
		{
			startField = j;
			lengthField = (int)(db_field.length);
		}
		j = j + (int)(db_field.length);
		db_hdr.header_byte = db_hdr.header_byte - 32;
	}

	/* could not find the "lblrecno" field */
	if (startField == 0) 
		return 1;

	/* skip the field terminator */
	l = fread((char *)temp, sizeof(char), db_hdr.header_byte, hDBFf);
	if (l != db_hdr.header_byte) 
		return 1;
	//for (i = 0; i < db_hdr.record_no; i++) {
	/* read all the record and extract the information from elevation field */
	for (i = 0; i < iRow; i++)
	{

		for (j = 0; j < iColumn; j++)
		{
			l = fread((char *)temp, sizeof(char), db_hdr.record_byte, hDBFf);
			if (l != db_hdr.record_byte) 
				return 0;
			temp[db_hdr.record_byte] = '\0';
			strncpy(temp1, &temp[startField], lengthField);
			temp1[lengthField] = '\0';
			*(pfGrid+((i*iColumn)+j)) = (double)atof(temp1);
			
			
		}
	}

	return iRet;
}


static int 	bBigEndian;


/*
 * mtd:	ADDSHAPE
 *
 * inf:	Create shape file and associated dBase file. If shape object is NULL, no shape file
 *		will be created.
 *
 * arg:	pShpTbl - shape table contain the shape objects
 *		psDBFAttList - dBase field values array
 *		sDelimiter - the delimiter of the dBase field values
 *		iProjStatus - projection flag
 *		dRefLongit - central longitude of UTM/TTM projection
 *
 * ret:	1 - problem.
 *		0 - successful.
 *
 * ver:	99.05.11		bcl/gismo		created.
 */







int CDbfFile::AddDBF4(char* psDBFAttList,CString sDelimiter, int nSize)
//used when strings separated by '\0'
{
	DBFHandle	hDBF;

	int			kk;
	CFileFind	ShapeFile;
	CString		sTmp;
	CStringArray	*psDBFAtt;
	psDBFAtt = new CStringArray();

	if (_bInit)
	{
		_bInit = false;
		ValidateDBF();
		DBFSetColumn(&_psDBFAttList);
	}
    // Open/Create the database.						
	hDBF = DBFOpen("r+b" );
	if( hDBF == NULL )
	{
		printf("DBFOpen failed: %s\n", _szDBFName);
		return 1;
	}
	
	int nStrStart = 0;
	for (kk = 0; kk < nSize; kk++)
	{
		psDBFAtt->RemoveAll();
		sTmp = "";
		while(psDBFAttList[nStrStart]!='\0')
		{
			sTmp = sTmp + psDBFAttList[nStrStart++];
		}
		nStrStart++;
		while (sTmp.Find(sDelimiter) != -1)
		{
			psDBFAtt->Add(sTmp.Left(sTmp.Find(sDelimiter)));
			sTmp.Delete(0, (sTmp.Find(sDelimiter) + 1));
		}
		if (!sTmp.IsEmpty())
			psDBFAtt->Add(sTmp);
		DBFAddRecord(hDBF, psDBFAtt);

	}

	psDBFAtt->RemoveAll();

/* -------------------------------------------------------------------- */
/*      Close and cleanup.                                              */
/* -------------------------------------------------------------------- */
	DBFClose( hDBF );
	delete(psDBFAtt);
	return 0;
}









int CDbfFile::AddDBF3(char** psDBFAttList,CString sDelimiter, int nSize) 
//used when psDBFAttList is not separated by '\0'
{
	DBFHandle	hDBF;

	int			kk;
	CFileFind	ShapeFile;
	CString		sTmp;
	CStringArray	*psDBFAtt;
	psDBFAtt = new CStringArray();

	if (_bInit)
	{
		_bInit = false;
		ValidateDBF();
		DBFSetColumn(&_psDBFAttList);
	}
    // Open/Create the database.						
	hDBF = DBFOpen("r+b" );
	if( hDBF == NULL )
	{
		printf("DBFOpen failed: %s\n", _szDBFName);
		return 1;
	}

	for (kk = 0; kk < nSize; kk++)
	{
		psDBFAtt->RemoveAll();
		sTmp = psDBFAttList[kk];
		while (sTmp.Find(sDelimiter) != -1)
		{
			psDBFAtt->Add(sTmp.Left(sTmp.Find(sDelimiter)));
			sTmp.Delete(0, (sTmp.Find(sDelimiter) + 1));
		}
		if (!sTmp.IsEmpty())
			psDBFAtt->Add(sTmp);
		DBFAddRecord(hDBF, psDBFAtt);
	}

	psDBFAtt->RemoveAll();

/* -------------------------------------------------------------------- */
/*      Close and cleanup.                                              */
/* -------------------------------------------------------------------- */
	DBFClose( hDBF );
	delete(psDBFAtt);
	return 0;
}




int CDbfFile::AddDBF(CStringArray* psDBFAttList,CString sDelimiter)
{
	DBFHandle	hDBF;

	int			kk;
	CFileFind	ShapeFile;
	CString		sTmp;
	CStringArray	*psDBFAtt;
	psDBFAtt = new CStringArray();

	if (_bInit)
	{
		_bInit = false;
		ValidateDBF();
		DBFSetColumn(&_psDBFAttList);
	}
    // Open/Create the database.						
	hDBF = DBFOpen("r+b" );
	if( hDBF == NULL )
	{
		printf("DBFOpen failed: %s\n", _szDBFName);
		return 1;
	}

	for (kk = 0; kk < psDBFAttList->GetSize(); kk++)
	{
		psDBFAtt->RemoveAll();
		sTmp = psDBFAttList->GetAt(kk);
		while (sTmp.Find(sDelimiter) != -1)
		{
			psDBFAtt->Add(sTmp.Left(sTmp.Find(sDelimiter)));
			sTmp.Delete(0, (sTmp.Find(sDelimiter) + 1));
		}
		if (!sTmp.IsEmpty())
			psDBFAtt->Add(sTmp);
		DBFAddRecord(hDBF, psDBFAtt);
	}

	psDBFAtt->RemoveAll();

/* -------------------------------------------------------------------- */
/*      Close and cleanup.                                              */
/* -------------------------------------------------------------------- */
	DBFClose( hDBF );
	delete(psDBFAtt);
	return 0;
}



/*
 * mtd:	ADDSHAPE
 *
 * inf:	Create shape file and associated dBase file. If shape object is NULL, no shape file
 *		will be created.
 *
 * arg:	pzSrcShape - source shape object
 *		psDBFAttList - dBase field values array
 *		iProjStatus - projection flag
 *		dRefLongit - central longitude of UTM/TTM projection
 *
 * ret:	1 - problem.
 *		0 - successful.
 *
 * ver:	99.05.11		bcl/gismo		created.
 */

int CDbfFile::AddDBF(CStringArray* psDBFAttList)
{
	CFileFind	ShapeFile;

	if (_bInit)
	{
		_bInit = false;
		ValidateDBF();
		DBFSetColumn(&_psDBFAttList);
	}

	DBFAddRecord(psDBFAttList);

	_psDBFAttList.RemoveAll();


	return 0;
}




/*
 * mtd:	SETDBFCOLUMN
 *
 * inf:	Set the dBase file column definition.
 *
 * arg:	psDBFAttList - column definition in CStringArray.
 *					   valid definition: -s <name of string column> <width of column>
 *					 					 -n <name of numeric column> <size of number digit> <precision>
 *
 * ret:	1 - problem.
 *		0 - successful.
 *
 * ver:	99.05.20		bcl/gismo		created.
 */

int CDbfFile::SetDBFColumn(CStringArray* psDBFAttList)
{
	_psDBFAttList.Append(*psDBFAttList);
	return 0;
}



int CDbfFile::SetDBFColumn(CString sName, int iWidth)
{
	CString	sTmp;

	sTmp.Format("-s %s %d",sName,iWidth);
	_psDBFAttList.Add(sTmp);

	return (0);
}



int CDbfFile::SetDBFColumn(CString sName, int iSize, int iPrecision)
{
	CString	sTmp;

	sTmp.Format("-n %s %d %d",sName,iSize,iPrecision);
	_psDBFAttList.Add(sTmp);

	return (0);
}

int CDbfFile::SetDBFColumnLogical(CString sName, int iSize, int iPrecision)
{
	CString	sTmp;

	sTmp.Format("-l %s %d %d",sName,iSize,iPrecision);
	_psDBFAttList.Add(sTmp);

	return (0);
}


/*
 * mtd: VALIDATEDBF
 *
 * inf: Ensures that the passed parameters meet the criteria
 *		for a DBF file.
 *
 * arg: none.
 *
 * ret: none.
 *
 * ver: 99.06.06		rjb/mit		created.
 */

BOOL CDbfFile::ValidateDBF(void)
{
	int		i,j;
	int		iUniqueCnt;
	BOOL	bValid = TRUE;
	CString	sTmp;
	char	sInt[5];

	// make sure no dup's occur in column list
	for (i=0; i < _psDBFAttList.GetUpperBound(); i++)
	{
		for (j=0; j < _psDBFAttList.GetUpperBound(); j++)
		{
			iUniqueCnt = 0;
			if ( (_psDBFAttList[i] == _psDBFAttList[j]) && (j != i) )
			{
				if (_psDBFAttList[j].GetLength() >= MAX_DBFCOL_LEN)
				{	
					itoa(iUniqueCnt++,sInt,10);
					sTmp = _psDBFAttList[j].Left(MAX_DBFCOL_LEN - 2) + sInt;
					_psDBFAttList[j] = sTmp ;
				}
				bValid = FALSE;
			}
		}
	}

	// get rid of any columns exceeding the max
	if (_psDBFAttList.GetUpperBound() > MAX_DBFCOLS)
	{
		_psDBFAttList.RemoveAt(MAX_DBFCOLS+1,(_psDBFAttList.GetUpperBound()-MAX_DBFCOLS));
		bValid = FALSE;
	}

	return (bValid);
}




/*
 * mtd:	SETDBFCOLUMN
 *
 * inf:	Set the dBase file column definition.
 *
 * arg:	sAttName - individual column definition in CString.
 *				   valid definition: -s <name of string column> <width of column>
 *									 -n <name of numeric column> <size of number digit> <precision>
 *
 * ret:	1 - problem.
 *		0 - successful.
 *
 * ver:	99.05.20		bcl/gismo		created.
 */

int CDbfFile::SetDBFColumn(CString sAttName)
{
	_psDBFAttList.Add(sAttName);
	return 0;
}



/*
 * mtd:	DBFADDRECORD
 *
 * inf:	Add one record to an existing DBF file.
 *
 * arg:	iNumField - counter of number of DBF fields
 *		psDBFFieldValue - array of DBF field value as string
 *
 * ret: 0 - if all is well
 *		1 - if not successful
 *
 * ver:	99.05.03	bcl/gismo	created.
 */

int CDbfFile::DBFAddRecord(DBFHandle hDBF, CStringArray* psDBFFieldValList)
{
	int			i;
	int			iRecord;
	int			iNumField;

	if (psDBFFieldValList == NULL)
		iNumField = 0;
	else
		iNumField = psDBFFieldValList->GetSize();
	/*
	
	// Do we have the correct number of arguments?			
	if( DBFGetFieldCount( hDBF ) != iNumField )
	{
		CString sMsg;
		sMsg.Format("Received %d field(s), but require %d field(s).",iNumField,DBFGetFieldCount(hDBF));
		printf("%s\n", sMsg);

		int		i, iWidth, iDecimals;
		char	psName[257];

		for (i=0; i<DBFGetFieldCount(hDBF); i++)
		{
			DBFGetFieldInfo(hDBF,i,psName,&iWidth,&iDecimals);
			sMsg.Format("%d of %d) FieldName: %s    Width: %d    Decimals: %d",i+1,DBFGetFieldCount(hDBF),psName,iWidth,iDecimals);
			printf("%s\n",sMsg);
		}
		return 1;
	}
	*/
	iRecord = DBFGetRecordCount( hDBF );

	
/* -------------------------------------------------------------------- */
/*	Loop assigning the new field values.				*/
/* -------------------------------------------------------------------- */
	for( i = 0; i < DBFGetFieldCount(hDBF); i++ )
	{
		if( DBFGetFieldInfo( hDBF, i, NULL, NULL, NULL ) == FTString )
			DBFWriteStringAttribute(hDBF, iRecord, i, psDBFFieldValList->GetAt(i) );
		else
			DBFWriteDoubleAttribute(hDBF, iRecord, i, atof(psDBFFieldValList->GetAt(i)) );
	}

	return 0;
}


/*
 * mtd:	DBFADDRECORD
 *
 * inf:	Add one record to an existing DBF file.
 *
 * arg:	iNumField - counter of number of DBF fields
 *		psDBFFieldValue - array of DBF field value as string
 *
 * ret: 0 - if all is well
 *		1 - if not successful
 *
 * ver:	99.05.03	bcl/gismo	created.
 */

int CDbfFile::DBFAddRecord(CStringArray* psDBFFieldValList)
{
    DBFHandle	hDBF;
	int			i;
	int			iRecord;
	int			iNumField;

	if (psDBFFieldValList == NULL)
		iNumField = 0;
	else
		iNumField = psDBFFieldValList->GetSize();

    // Open/Create the database.						
	hDBF = DBFOpen("r+b" );
	if( hDBF == NULL )
	{
		printf("DBFOpen failed: %s", _szDBFName);
		return 1;
	}
    
	// Do we have the correct number of arguments?			
	if( DBFGetFieldCount( hDBF ) != iNumField )
	{
		CString sMsg;
		sMsg.Format("Received %d field(s), but require %d field(s).",iNumField,DBFGetFieldCount(hDBF));
		printf("%s\n",sMsg);

		int		i, iWidth, iDecimals;
		char	psName[257];

		for (i=0; i<DBFGetFieldCount(hDBF); i++)
		{
			DBFGetFieldInfo(hDBF,i,psName,&iWidth,&iDecimals);
			sMsg.Format("%d of %d) FieldName: %s    Width: %d    Decimals: %d",i+1,DBFGetFieldCount(hDBF),psName,iWidth,iDecimals);
			printf("%s\n",sMsg);
		}
		return 1;
	}

	iRecord = DBFGetRecordCount( hDBF );

/* -------------------------------------------------------------------- */
/*	Loop assigning the new field values.				*/
/* -------------------------------------------------------------------- */
	for( i = 0; i < DBFGetFieldCount(hDBF); i++ )
	{
		if( DBFGetFieldInfo( hDBF, i, NULL, NULL, NULL ) == FTString )
			DBFWriteStringAttribute(hDBF, iRecord, i, psDBFFieldValList->GetAt(i) );
		else
			DBFWriteDoubleAttribute(hDBF, iRecord, i, atof(psDBFFieldValList->GetAt(i)) );
	}

/* -------------------------------------------------------------------- */
/*      Close and cleanup.                                              */
/* -------------------------------------------------------------------- */
	DBFClose( hDBF );

	return 0;
}



/*
 * mtd:	DBFSETCOLUMN
 *
 * inf:	Create the DBF file.
 *
 * arg:	iNumField - counter of number of field
 *		psDBFFields - array of field name and definition as string
 *
 * ret: 0 - if all is well
 *		1 - if not successful
 *
 * ver:	99.05.03	bcl/gismo	created.
 */

int CDbfFile::DBFSetColumn(CStringArray* psDBFColNameList)
{
    DBFHandle	hDBF;
	INT			i;
	INT			j;
	INT			k;
	INT			iStringLength;
	INT			iNumField;
	//CString		szPolyId;
	CString		szAttItem;
	CFileFind	DBFFile;
	
	if (_szDBFName.IsEmpty())
	{
		_szDBFName = _szFileName.Left(_szFileName.Find('.'));
		_szDBFName.Insert(_szFileName.GetLength(), ".dbf");
	}

	if ((DBFFile.FindFile(_szDBFName)) && (_eMode == KEEP))
	{
		printf("DBF file already exist");
		return 1;
	}
	else if ((DBFFile.FindFile(_szDBFName) && (_eMode == OVERWRITE)) ||
			(!DBFFile.FindFile(_szDBFName)))
	{
		hDBF = DBFCreate();
		if( hDBF == NULL )
		{
	  		printf( "DBFCreate(%s) failed.\n", _szDBFName );
			return 1;
		}
    
/*		if (_szFileName.Find('\\') > 0)
		{
			szPolyId = _szFileName.Right(_szFileName.GetLength()-_szFileName.ReverseFind('\\')-1);
			szPolyId = szPolyId.Left(8) + "_";
		}
		else
			szPolyId = _szFileName.Left(8) + "_";

		if( DBFAddField( hDBF, szPolyId, FTDouble, 11, 0) == -1 )
		{
			printf( "DBFAddField(%s,FTDouble,8,0) failed.\n", szPolyId);
			return 1;
		}
		if (szPolyId.GetLength() > 8)
			szPolyId = szPolyId + "i";
		else
			szPolyId = szPolyId + "id";
		if( DBFAddField( hDBF, szPolyId, FTDouble, 11, 0) == -1 )
		{
			printf( "DBFAddField(%s,FTDouble,8,0) failed.\n", szPolyId);
			return 1;
		} */
/* -------------------------------------------------------------------- */
/*	Loop over the field definitions adding new fields.	       	*/
/* -------------------------------------------------------------------- */
		iNumField = psDBFColNameList->GetSize();
		for( i = 0; i < iNumField; i++ )
		{
			szAttItem = psDBFColNameList->GetAt(i);
			if( strcmp(szAttItem.Left(2),"-s") == 0 )
			{
				szAttItem.Delete(0, 3);
				iStringLength = szAttItem.GetLength();
				k = szAttItem.Find(' ');
				ASSERT (k > 0);
				if( DBFAddField( hDBF, szAttItem.Left(k), FTString,
											atoi(szAttItem.Right(iStringLength-k-1)), 0 ) == -1 )
				{
					printf( "DBFAddField(%s,FTString,%d,0) failed.\n",
							psDBFColNameList->GetAt(i+1), atoi(psDBFColNameList->GetAt(i)) );
					return 1;
				}
			}
			else if( strcmp(szAttItem.Left(2),"-n") == 0 )
			{
				szAttItem.Delete(0, 3);
				iStringLength = szAttItem.GetLength();
				j = szAttItem.Find(' ');
				ASSERT (j > 0);
				k = szAttItem.ReverseFind(' ');
				ASSERT (k > 0);
				if( DBFAddField( hDBF, szAttItem.Left(j), FTDouble, atoi(szAttItem.Mid(j+1, k-j-1)), 
											atoi(szAttItem.Right(iStringLength-k-1)) ) == -1 )
				{
					printf( "DBFAddField(%s,FTDouble,%d,%d) failed.\n",
							psDBFColNameList->GetAt(i+1), atoi(psDBFColNameList->GetAt(i)), atoi(psDBFColNameList->GetAt(i+3)) );
					return 1;
				}
			}
			else
			{
				if(strcmp(szAttItem.Left(2),"-l") == 0)
				{

					szAttItem.Delete(0, 3);
					iStringLength = szAttItem.GetLength();
					k = szAttItem.Find(' ');
					ASSERT (k > 0);
					if( DBFAddField( hDBF, szAttItem.Left(k), FTLogical,
												atoi(szAttItem.Right(iStringLength-k-1)), 0 ) == -1 )
					{
						printf( "DBFAddField(%s,FTString,%d,0) failed.\n",
								psDBFColNameList->GetAt(i+1), atoi(psDBFColNameList->GetAt(i)) );
						return 1;
					}
				}
				else
				{
					printf( "Field definition incomplete, or unrecognised:%s\n", psDBFColNameList->GetAt(i) );
					exit( 3 );
				}
			}
		}
		DBFClose( hDBF );
	}

	return 0;

}



/*
 * mtd:	SWAPWORD
 *
 * inf:	swap a 2, 4 or 8 byte word.
 *
 * arg:	length - length of the word
 *		wordP - word to swap
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

static void	SwapWord( int length, void * wordP )

{
    int		i;
    uchar	temp;

    for( i=0; i < length/2; i++ )
    {
		temp = ((uchar *) wordP)[i];
		((uchar *)wordP)[i] = ((uchar *) wordP)[length-i-1];
		((uchar *) wordP)[length-i-1] = temp;
    }
}



/*
 * mtd:	SFREALLOC
 *
 * inf:	A realloc cover function that will access a NULL pointer as
 *      a valid input.
 *
 * arg:	pMem - pointer to memory location
 *		nNewSize - new memory allocate size
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

void *CDbfFile::SfRealloc( void * pMem, int nNewSize )

{
    if( pMem == NULL )
		return( (void *) malloc(nNewSize) );
    else
		return( (void *) realloc(pMem,nNewSize) );
}



/*
 * mtd:	DBFOPEN
 *
 * inf:	Open a .dbf file.
 *
 * arg:	pszFilename - DBF filename
 *		pszAccess - DBF file access type
 *
 * ret:	DBFHandle
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

DBFHandle CDbfFile::DBFOpen(const char * pszAccess )

{
    DBFHandle	psDBF;
    uchar		*pabyBuf;
    int			nFields, nRecords, nHeadLen, nRecLen, iField;

/* -------------------------------------------------------------------- */
/*      We only allow the access strings "rb" and "r+".                  */
/* -------------------------------------------------------------------- */
    if( strcmp(pszAccess,"r") != 0 && strcmp(pszAccess,"r+") != 0 
        && strcmp(pszAccess,"rb") != 0 && strcmp(pszAccess,"r+b") != 0 )
        return( NULL );
    
/* -------------------------------------------------------------------- */
/*	Ensure the extension is converted to dbf or DBF if it is 	*/
/*	currently .shp or .shx.						*/    
/* -------------------------------------------------------------------- */
//    pszDBFFilename = (char *) malloc(strlen(pszFilename)+1);
//    strcpy( pszDBFFilename, pszFilename );
    
//    if( strcmp(pszFilename+strlen(pszFilename)-4,".shp")
//        || strcmp(pszFilename+strlen(pszFilename)-4,".shx") )
//    {
//        strcpy( pszDBFFilename+strlen(pszDBFFilename)-4, ".dbf");
//    }
//    else if( strcmp(pszFilename+strlen(pszFilename)-4,".SHP")
//             || strcmp(pszFilename+strlen(pszFilename)-4,".SHX") )
//    {
//        strcpy( pszDBFFilename+strlen(pszDBFFilename)-4, ".DBF");
//    }

/* -------------------------------------------------------------------- */
/*      Open the file.                                                  */
/* -------------------------------------------------------------------- */
    psDBF = (DBFHandle) calloc( 1, sizeof(DBFInfo) );
    psDBF->fp = fopen(_szDBFName, pszAccess );
    if( psDBF->fp == NULL )
        return( NULL );

    psDBF->bNoHeader = FALSE;
    psDBF->nCurrentRecord = -1;
    psDBF->bCurrentRecordModified = FALSE;

//    free( pszDBFFilename );

/* -------------------------------------------------------------------- */
/*  Read Table Header info                                              */
/* -------------------------------------------------------------------- */
    pabyBuf = (uchar *) malloc(500);
    fread( pabyBuf, 32, 1, psDBF->fp );

    psDBF->nRecords = nRecords = 
     pabyBuf[4] + pabyBuf[5]*256 + pabyBuf[6]*256*256 + pabyBuf[7]*256*256*256;

    psDBF->nHeaderLength = nHeadLen = pabyBuf[8] + pabyBuf[9]*256;
    psDBF->nRecordLength = nRecLen = pabyBuf[10] + pabyBuf[11]*256;
    
    psDBF->nFields = nFields = (nHeadLen - 32) / 32;

    psDBF->pszCurrentRecord = (char *) malloc(nRecLen);

/* -------------------------------------------------------------------- */
/*  Read in Field Definitions                                           */
/* -------------------------------------------------------------------- */
    
    pabyBuf = (uchar *) SfRealloc(pabyBuf,nHeadLen);
    psDBF->pszHeader = (char *) pabyBuf;

    fseek( psDBF->fp, 32, 0 );
    fread( pabyBuf, nHeadLen, 1, psDBF->fp );

    psDBF->panFieldOffset = (int *) malloc(sizeof(int) * nFields);
    psDBF->panFieldSize = (int *) malloc(sizeof(int) * nFields);
    psDBF->panFieldDecimals = (int *) malloc(sizeof(int) * nFields);
    psDBF->pachFieldType = (char *) malloc(sizeof(char) * nFields);

    for( iField = 0; iField < nFields; iField++ )
    {
		uchar		*pabyFInfo;

		pabyFInfo = pabyBuf+iField*32;

		if( pabyFInfo[11] == 'N' )
		{
			psDBF->panFieldSize[iField] = pabyFInfo[16];
			psDBF->panFieldDecimals[iField] = pabyFInfo[17];
		}
		else
		{
			psDBF->panFieldSize[iField] = pabyFInfo[16] + pabyFInfo[17]*256;
			psDBF->panFieldDecimals[iField] = 0;
		}

		psDBF->pachFieldType[iField] = (char) pabyFInfo[11];
		if( iField == 0 )
			psDBF->panFieldOffset[iField] = 1;
		else
			psDBF->panFieldOffset[iField] = 
				psDBF->panFieldOffset[iField-1] + psDBF->panFieldSize[iField-1];
	}

    return( psDBF );
}

/*
 * mtd:	DBFCLOSE
 *
 * inf:	close the DBF file
 *
 * arg:	psDBF - handle of DBF file
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

void	CDbfFile::DBFClose(DBFHandle psDBF)
{
/* -------------------------------------------------------------------- */
/*      Write out header if not already written.                        */
/* -------------------------------------------------------------------- */
    if( psDBF->bNoHeader )
        DBFWriteHeader( psDBF );

    DBFFlushRecord( psDBF );

/* -------------------------------------------------------------------- */
/*      Update last access date, and number of records if we have	*/
/*	write access.                					*/
/* -------------------------------------------------------------------- */
    if( psDBF->bUpdated )
    {
		uchar	abyFileHeader[32];

		fseek( psDBF->fp, 0, 0 );
		fread( abyFileHeader, 32, 1, psDBF->fp );

		abyFileHeader[1] = 95;			/* YY */
		abyFileHeader[2] = 7;			/* MM */
		abyFileHeader[3] = 26;			/* DD */

		abyFileHeader[4] = psDBF->nRecords % 256;
		abyFileHeader[5] = (psDBF->nRecords/256) % 256;
		abyFileHeader[6] = (psDBF->nRecords/(256*256)) % 256;
		abyFileHeader[7] = (psDBF->nRecords/(256*256*256)) % 256;

		fseek( psDBF->fp, 0, 0 );
		fwrite( abyFileHeader, 32, 1, psDBF->fp );
    }

/* -------------------------------------------------------------------- */
/*      Close, and free resources.                                      */
/* -------------------------------------------------------------------- */
    fclose( psDBF->fp );

    if( psDBF->panFieldOffset != NULL )
    {
        free( psDBF->panFieldOffset );
        free( psDBF->panFieldSize );
        free( psDBF->panFieldDecimals );
        free( psDBF->pachFieldType );
    }

    free( psDBF->pszHeader );
    free( psDBF->pszCurrentRecord );

    free( psDBF );
}

/*
 * mtd:	DBFCREATE
 *
 * inf:	Create a new .dbf file.
 *
 * arg:	pszFilename - pointer to DBF file
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

DBFHandle CDbfFile::DBFCreate()
{
    DBFHandle	psDBF;
    FILE	*fp;
    char	*pszBasename;
	int		i;

    pszBasename = (char *) malloc(strlen(_szFileName)+5);
    strcpy( pszBasename, _szFileName );
    for( i = strlen(pszBasename)-1; 
		i > 0 && pszBasename[i] != '.' && pszBasename[i] != '/'
	    && pszBasename[i] != '\\';
		i-- ) {}

    if( pszBasename[i] == '.' )
        pszBasename[i] = '\0';

	_szDBFName = pszBasename;
	_szDBFName = _szDBFName + ".dbf";
    free( pszBasename );

//    pszFullname = (char *) malloc(strlen(pszBasename) + 5);
//    sprintf( pszFullname, "%s.dbf", pszBasename );

/* -------------------------------------------------------------------- */
/*      Create the file.                                                */
/* -------------------------------------------------------------------- */
    fp = fopen(_szDBFName, "wb" );
    if( fp == NULL )
        return( NULL );

    fputc( 0, fp );
    fclose( fp );

    fp = fopen(_szDBFName, "rb+" );
    if( fp == NULL )
        return( NULL );

/* -------------------------------------------------------------------- */
/*	Create the info structure.					*/
/* -------------------------------------------------------------------- */
    psDBF = (DBFHandle) malloc(sizeof(DBFInfo));

    psDBF->fp = fp;
    psDBF->nRecords = 0;
    psDBF->nFields = 0;
    psDBF->nRecordLength = 1;
    psDBF->nHeaderLength = 33;
    
    psDBF->panFieldOffset = NULL;
    psDBF->panFieldSize = NULL;
    psDBF->panFieldDecimals = NULL;
    psDBF->pachFieldType = NULL;
    psDBF->pszHeader = NULL;

    psDBF->nCurrentRecord = -1;
    psDBF->bCurrentRecordModified = FALSE;
    psDBF->pszCurrentRecord = NULL;

    psDBF->bNoHeader = TRUE;

    return( psDBF );
}

/*
 * mtd:	DBFADDFIELD
 *
 * inf:	Add a field to a newly created .dbf file before any records
 *      are written.
 * arg:	psDBF - handle to DBF file
 *		pszFieldName - pointer to DBF field name
 *		eType - DBF field type
 *		nWidth - width of the data field
 *		nDecimals - decimal place for the field
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

int	CDbfFile::DBFAddField(DBFHandle psDBF, const char * pszFieldName, 
		    DBFFieldType eType, int nWidth, int nDecimals )

{
    char	*pszFInfo;
    int		i;

/*
 * Truncate the width of CHAR field to 254
 */
	if ((nWidth > 254) && (eType == FTString))
		nWidth = 254;

/* -------------------------------------------------------------------- */
/*      Do some checking to ensure we can add records to this file.     */
/* -------------------------------------------------------------------- */
    if( psDBF->nRecords > 0 )
        return( -1 );

    if( !psDBF->bNoHeader )
        return( -1 );

    if( eType != FTDouble && nDecimals != 0 )
        return( -1 );

/* -------------------------------------------------------------------- */
/*      SfRealloc all the arrays larger to hold the additional field      */
/*      information.                                                    */
/* -------------------------------------------------------------------- */
    psDBF->nFields++;

    psDBF->panFieldOffset = (int *) 
      SfRealloc( psDBF->panFieldOffset, sizeof(int) * psDBF->nFields );

    psDBF->panFieldSize = (int *) 
      SfRealloc( psDBF->panFieldSize, sizeof(int) * psDBF->nFields );

    psDBF->panFieldDecimals = (int *) 
      SfRealloc( psDBF->panFieldDecimals, sizeof(int) * psDBF->nFields );

    psDBF->pachFieldType = (char *) 
      SfRealloc( psDBF->pachFieldType, sizeof(char) * psDBF->nFields );

/* -------------------------------------------------------------------- */
/*      Assign the new field information fields.                        */
/* -------------------------------------------------------------------- */
    psDBF->panFieldOffset[psDBF->nFields-1] = psDBF->nRecordLength;
    psDBF->nRecordLength += nWidth;
    psDBF->panFieldSize[psDBF->nFields-1] = nWidth;
    psDBF->panFieldDecimals[psDBF->nFields-1] = nDecimals;

    if( eType == FTString )
        psDBF->pachFieldType[psDBF->nFields-1] = 'C';
    else
	{
		if (eType ==FTDouble)
		{
			psDBF->pachFieldType[psDBF->nFields-1] = 'N';
		}
		else
		{
			psDBF->pachFieldType[psDBF->nFields-1] = 'L';
		}
	}
        

/* -------------------------------------------------------------------- */
/*      Extend the required header information.                         */
/* -------------------------------------------------------------------- */
    psDBF->nHeaderLength += 32;
    psDBF->bUpdated = FALSE;

    psDBF->pszHeader = (char *) SfRealloc(psDBF->pszHeader,psDBF->nFields*32);

    pszFInfo = psDBF->pszHeader + 32 * (psDBF->nFields-1);

    for( i = 0; i < 32; i++ )
        pszFInfo[i] = '\0';

    if( strlen(pszFieldName) < 10 )
        strncpy( pszFInfo, pszFieldName, strlen(pszFieldName));
    else
        strncpy( pszFInfo, pszFieldName, 10);

    pszFInfo[11] = psDBF->pachFieldType[psDBF->nFields-1];

    if( eType == FTString )
    {
        pszFInfo[16] = nWidth % 256;
        pszFInfo[17] = nWidth / 256;
    }
    else
    {
        pszFInfo[16] = nWidth;
        pszFInfo[17] = nDecimals;
    }
    
/* -------------------------------------------------------------------- */
/*      Make the current record buffer appropriately larger.            */
/* -------------------------------------------------------------------- */
    psDBF->pszCurrentRecord = (char *) SfRealloc(psDBF->pszCurrentRecord,
					       psDBF->nRecordLength);

    return( psDBF->nFields-1 );
}

/*
 * mtd:	DBFREADATTRIBUTE
 *
 * inf:	Read one of the attribute fields of a record.
 *
 * arg:	psDBF - handle to DBF file
 *		hEntity - entity number
 *		iField - field number
 *		chReqType - request type
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

void *CDbfFile::DBFReadAttribute(DBFHandle psDBF, int hEntity, int iField,
                              char chReqType )

{
    int	    nRecordOffset;
    uchar	*pabyRec;
    void	*pReturnField = NULL;

    static double dDoubleField;
    static char * pszStringField = NULL;
    static int	nStringFieldLen = 0;

/* -------------------------------------------------------------------- */
/*	Have we read the record?					*/
/* -------------------------------------------------------------------- */
    if( hEntity < 0 || hEntity >= psDBF->nRecords )
        return( NULL );

    if( psDBF->nCurrentRecord != hEntity )
    {
		DBFFlushRecord( psDBF );

		nRecordOffset = psDBF->nRecordLength * hEntity + psDBF->nHeaderLength;

		fseek( psDBF->fp, nRecordOffset, 0 );
		fread( psDBF->pszCurrentRecord, psDBF->nRecordLength, 1, psDBF->fp );

		psDBF->nCurrentRecord = hEntity;
    }

    pabyRec = (uchar *) psDBF->pszCurrentRecord;

/* -------------------------------------------------------------------- */
/*	Ensure our field buffer is large enough to hold this buffer.	*/
/* -------------------------------------------------------------------- */
    if( psDBF->panFieldSize[iField]+1 > nStringFieldLen )
    {
		nStringFieldLen = psDBF->panFieldSize[iField]*2 + 10;
		pszStringField = (char *) SfRealloc(pszStringField,nStringFieldLen);
    }

/* -------------------------------------------------------------------- */
/*	Extract the requested field.					*/
/* -------------------------------------------------------------------- */
    strncpy( pszStringField, (char *)pabyRec+psDBF->panFieldOffset[iField],
	     psDBF->panFieldSize[iField] );
    pszStringField[psDBF->panFieldSize[iField]] = '\0';

    pReturnField = pszStringField;

/* -------------------------------------------------------------------- */
/*      Decode the field.                                               */
/* -------------------------------------------------------------------- */
    if( chReqType == 'N' )
    {
        sscanf( pszStringField, "%lf", &dDoubleField );

		pReturnField = &dDoubleField;
    }

/* -------------------------------------------------------------------- */
/*      Should we trim white space off the string attribute value?      */
/* -------------------------------------------------------------------- */
#ifdef TRIM_DBF_WHITESPACE
    else
    {
        char	*pchSrc, *pchDst;

        pchDst = pchSrc = pszStringField;
        while( *pchSrc == ' ' )
            pchSrc++;

        while( *pchSrc != '\0' )
            *(pchDst++) = *(pchSrc++);
        *pchDst = '\0';

        while( *(--pchDst) == ' ' && pchDst != pszStringField )
            *pchDst = '\0';

    }
#endif
    
    return( pReturnField );
}

/*
 * mtd: DBFREADINTATTRIBUTE
 *
 * inf:	Read an integer attribute.
 *
 * arg:	psDBF - handle to DBF file
 *		iRecord - record number
 *		iField - field number
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

int	CDbfFile::DBFReadIntegerAttribute( DBFHandle psDBF, int iRecord, int iField )

{
    double	*pdValue;

    pdValue = (double *) DBFReadAttribute( psDBF, iRecord, iField, 'N' );

    return( (int) *pdValue );
}

/*
 * mtd:	DBFREADDOUBLEATTRIBUTE
 *
 * inf:	Read a double attribute.
 *
 * arg:	psDBF - handle to DBF file
 *		iRecord - record number
 *		iField - field number
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

double	CDbfFile::DBFReadDoubleAttribute( DBFHandle psDBF, int iRecord, int iField )

{
    double	*pdValue;

    pdValue = (double *) DBFReadAttribute( psDBF, iRecord, iField, 'N' );

    return( *pdValue );
}

/*
 * mtd:	DBFREADSTRINGATTRIBUTE
 *
 * inf:	Read a string attribute.
 *
 * arg:	psDBF - handle to DBF file
 *		iRecord - record number
 *		iField - field number
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

const char *CDbfFile::DBFReadStringAttribute( DBFHandle psDBF, int iRecord, int iField )

{
    return( (const char *) DBFReadAttribute( psDBF, iRecord, iField, 'C' ) );
}

/*
 * mtd:	DBFGETFIELDCOUNT
 *
 * inf:	Return the number of fields in this table.
 *
 * arg:	psDBF - handle to DBF file
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

int	CDbfFile::DBFGetFieldCount( DBFHandle psDBF )

{
    return( psDBF->nFields );
}

/*
 * mtd:	DBFGETRECORDCOUNT
 *
 * inf:	Return the number of records in this table.
 *
 * arg:	psDBF - handle to DBF file
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

int	CDbfFile::DBFGetRecordCount( DBFHandle psDBF )

{
    return( psDBF->nRecords );
}

/*
 * mtd: DBFGETFIELDINFO
 *
 * inf:	Return any requested information about the field.
 *
 * arg:	psDBF - handle to DBF file
 *		iField - field number
 *		pszFieldName - DBF field name
 *		pnWidth - field width
 *		pnDecimals - number of decimal place
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

DBFFieldType CDbfFile::DBFGetFieldInfo( DBFHandle psDBF, int iField, char * pszFieldName,
			      int * pnWidth, int * pnDecimals )

{
    if( iField < 0 || iField >= psDBF->nFields )
        return( FTInvalid );

    if( pnWidth != NULL )
        *pnWidth = psDBF->panFieldSize[iField];

    if( pnDecimals != NULL )
        *pnDecimals = psDBF->panFieldDecimals[iField];

    if( pszFieldName != NULL )
    {
		int	i;

		strncpy( pszFieldName, (char *) psDBF->pszHeader+iField*32, 11 );
		pszFieldName[11] = '\0';
		for( i = 10; i > 0 && pszFieldName[i] == ' '; i-- )
			pszFieldName[i] = '\0';
    }

    if( psDBF->pachFieldType[iField] == 'N' 
        || psDBF->pachFieldType[iField] == 'D' )
    {
		if( psDBF->panFieldDecimals[iField] > 0 )
			return( FTDouble );
		else
			return( FTInteger );
	}
    else
    {
		return( FTString );
    }
}

/*
 * mtd:	DBFWRITEATTRIBUTE
 *
 * inf:	Write an attribute record to the file.
 *
 * arg:	psDBF - handle to DBF file
 *		hEntity - DBF entity number
 *		iField - field number
 *		pValue - pointer to the field value
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

int CDbfFile::DBFWriteAttribute(DBFHandle psDBF, int hEntity, int iField,
			     void * pValue )
{
    int	    nRecordOffset, i, j;
    uchar	*pabyRec;
    char	szSField[40], szFormat[12];

/* -------------------------------------------------------------------- */
/*	Is this a valid record?						*/
/* -------------------------------------------------------------------- */
    if( hEntity < 0 || hEntity > psDBF->nRecords )
        return( FALSE );

    if( psDBF->bNoHeader )
        DBFWriteHeader(psDBF);

/* -------------------------------------------------------------------- */
/*      Is this a brand new record?                                     */
/* -------------------------------------------------------------------- */
    if( hEntity == psDBF->nRecords )
    {
		DBFFlushRecord( psDBF );

		psDBF->nRecords++;
		for( i = 0; i < psDBF->nRecordLength; i++ )
			psDBF->pszCurrentRecord[i] = ' ';

		psDBF->nCurrentRecord = hEntity;
    }

/* -------------------------------------------------------------------- */
/*      Is this an existing record, but different than the last one     */
/*      we accessed?                                                    */
/* -------------------------------------------------------------------- */
    if( psDBF->nCurrentRecord != hEntity )
    {
		DBFFlushRecord( psDBF );

		nRecordOffset = psDBF->nRecordLength * hEntity + psDBF->nHeaderLength;

		fseek( psDBF->fp, nRecordOffset, 0 );
		fread( psDBF->pszCurrentRecord, psDBF->nRecordLength, 1, psDBF->fp );

		psDBF->nCurrentRecord = hEntity;
    }

    pabyRec = (uchar *) psDBF->pszCurrentRecord;

/* -------------------------------------------------------------------- */
/*      Assign all the record fields.                                   */
/* -------------------------------------------------------------------- */
    switch( psDBF->pachFieldType[iField] )
    {
		case 'D':
		case 'N':
		if( psDBF->panFieldDecimals[iField] == 0 )
		{
			sprintf( szFormat, "%%%dd", psDBF->panFieldSize[iField] );
			sprintf(szSField, szFormat, (int) *((double *) pValue) );
			if( (int) strlen(szSField) > psDBF->panFieldSize[iField] )
				szSField[psDBF->panFieldSize[iField]] = '\0';
			strncpy((char *) (pabyRec+psDBF->panFieldOffset[iField]),
				szSField, strlen(szSField) );
		}
		else
		{
			sprintf( szFormat, "%%%d.%df", 
		    psDBF->panFieldSize[iField],
		    psDBF->panFieldDecimals[iField] );
			sprintf(szSField, szFormat, *((double *) pValue) );
			if( (int) strlen(szSField) >  psDBF->panFieldSize[iField] )
				szSField[psDBF->panFieldSize[iField]] = '\0';
			strncpy((char *) (pabyRec+psDBF->panFieldOffset[iField]),
		    szSField, strlen(szSField) );
		}
		break;

		default:
		if( (int) strlen((char *) pValue) > psDBF->panFieldSize[iField] )
		{
			int iben = strlen((char *) pValue);
			char szTemp[255];
			strncpy(szTemp, (char *) pValue, 253);
			szTemp[253] = '\0';
			strcat(szTemp, "*");
			strcpy((char *) pValue, szTemp);
			j = psDBF->panFieldSize[iField];
		}
		else
			j = strlen((char *) pValue);

		strncpy((char *) (pabyRec+psDBF->panFieldOffset[iField]),
				(char *) pValue, j );
		break;
    }

    psDBF->bCurrentRecordModified = TRUE;
    psDBF->bUpdated = TRUE;

    return( TRUE );
}

/*
 * mtd:	DBFWRITEDOUBLEATTRIBUTE
 *
 * inf:	Write a double attribute.
 *
 * arg:	psDBF - handle to DBF file
 *		iRecord - DBF record number
 *		iField - field number
 *		dValue - field value as double
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

int CDbfFile::DBFWriteDoubleAttribute( DBFHandle psDBF, int iRecord, int iField,
			     double dValue )

{
    return( DBFWriteAttribute( psDBF, iRecord, iField, (void *) &dValue ) );
}

/*
 * mtd:	DBFWRITEINTEGERATTRIBUTE
 *
 * inf:	Write a integer attribute.
 *
 * arg:	psDBF - handle to DBF file
 *		iRecord - DBF record number
 *		iField - field number
 *		nValue - field value as integer
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

int CDbfFile::DBFWriteIntegerAttribute( DBFHandle psDBF, int iRecord, int iField,
			      int nValue )

{
    double	dValue = nValue;

    return( DBFWriteAttribute( psDBF, iRecord, iField, (void *) &dValue ) );
}

/*
 * mtd:	DBFWRITESTRINGATTRIBUTE
 *
 * inf:	Write a string attribute.
 *
 * arg:	psDBF - handle to DBF file
 *		iRecord - DBF record number
 *		iField - field number
 *		pszValue - pointer to field value
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

/************************************************************************/
/*                      DBFWriteStringAttribute()                       */
/*                                                                      */
/*      Write a string attribute.                                       */
/************************************************************************/

int CDbfFile::DBFWriteStringAttribute( DBFHandle psDBF, int iRecord, int iField,
			     const char * pszValue )
{
    return( DBFWriteAttribute( psDBF, iRecord, iField, (void *) pszValue) );
}

/*
 * mtd:	DBFWRITEHEADER
 *
 * inf:	This is called to write out the file header, and field
 *      descriptions before writing any actual data records.  This
 *      also computes all the DBFDataSet field offset/size/decimals
 *      and so forth values.
 *
 * arg:	psDBF - handle to DBF file
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

int CDbfFile::DBFWriteHeader(DBFHandle psDBF)
{
    uchar	abyHeader[32];
    int		i;

    if( !psDBF->bNoHeader )
        return 1;

    psDBF->bNoHeader = FALSE;

/* -------------------------------------------------------------------- */
/*	Initialize the file header information.				*/
/* -------------------------------------------------------------------- */
    for( i = 0; i < 32; i++ )
        abyHeader[i] = 0;

    abyHeader[0] = 0x03;		/* memo field? - just copying 	*/

    /* date updated on close, record count preset at zero */

    abyHeader[8] = psDBF->nHeaderLength % 256;
    abyHeader[9] = psDBF->nHeaderLength / 256;
    
    abyHeader[10] = psDBF->nRecordLength % 256;
    abyHeader[11] = psDBF->nRecordLength / 256;

/* -------------------------------------------------------------------- */
/*      Write the initial 32 byte file header, and all the field        */
/*      descriptions.                                     		*/
/* -------------------------------------------------------------------- */
    fseek( psDBF->fp, 0, 0 );
    fwrite( abyHeader, 32, 1, psDBF->fp );
    fwrite( psDBF->pszHeader, 32, psDBF->nFields, psDBF->fp );

/* -------------------------------------------------------------------- */
/*      Write out the newline character if there is room for it.        */
/* -------------------------------------------------------------------- */
    if( psDBF->nHeaderLength > 32*psDBF->nFields + 32 )
    {
        char	cNewline;

        cNewline = 0x0d;
        fwrite( &cNewline, 1, 1, psDBF->fp );
    }
	return 0;
}

/*
 * mtd:	DBFFLUSHRECORD
 *
 * inf:	Write out the current record if there is one.
 *
 * arg:	psDBF - handle to DBF file
 *
 * ret:
 *
 * ver:	99.05.04		bcl/gismo	modified (SHPLib public domain software)
 */

void CDbfFile::DBFFlushRecord( DBFHandle psDBF )

{
    int		nRecordOffset;

    if( psDBF->bCurrentRecordModified && psDBF->nCurrentRecord > -1 )
    {
		psDBF->bCurrentRecordModified = FALSE;

		nRecordOffset = psDBF->nRecordLength * psDBF->nCurrentRecord 
						+ psDBF->nHeaderLength;

		fseek( psDBF->fp, nRecordOffset, 0 );
		fwrite( psDBF->pszCurrentRecord, psDBF->nRecordLength, 1, psDBF->fp );
    }
}


int CDbfFile::AddField(CStringArray* psDBFFieldValList)
{
	int			i;
	int			iRecord;
	int			iNumField;

	if (psDBFFieldValList == NULL)
		iNumField = 0;
	else
		iNumField = psDBFFieldValList->GetSize();

	// Do we have the correct number of arguments?			
	if( DBFGetFieldCount( hDBF ) != iNumField )
	{
		CString sMsg;
		sMsg.Format("Received %d field(s), but require %d field(s).",iNumField,DBFGetFieldCount(hDBF));
		printf("%s\n", sMsg);

		int		i, iWidth, iDecimals;
		char	psName[257];

		for (i=0; i<DBFGetFieldCount(hDBF); i++)
		{
			DBFGetFieldInfo(hDBF,i,psName,&iWidth,&iDecimals);
			sMsg.Format("%d of %d) FieldName: %s    Width: %d    Decimals: %d",i+1,DBFGetFieldCount(hDBF),psName,iWidth,iDecimals);
			printf("%s\n",sMsg);
		}
		return 1;
	}

	iRecord = DBFGetRecordCount( hDBF );

	
/* -------------------------------------------------------------------- */
/*	Loop assigning the new field values.				*/
/* -------------------------------------------------------------------- */
	for( i = 0; i < DBFGetFieldCount(hDBF); i++ )
	{
		if( DBFGetFieldInfo( hDBF, i, NULL, NULL, NULL ) == FTString )
			DBFWriteStringAttribute(hDBF, iRecord, i, psDBFFieldValList->GetAt(i) );
		else
			DBFWriteDoubleAttribute(hDBF, iRecord, i, atof(psDBFFieldValList->GetAt(i)) );
	}

	return 0;

}

void CDbfFile::Initialize()
{
	if(_bInit) 
	{
		DBFSetColumn(&_psDBFAttList);
		ValidateDBF();
		_bInit = false;
		
	}
	hDBF = DBFOpen("r+b" );
	if( hDBF == NULL )
	{
		printf("DBFOpen failed: %s\n", _szDBFName);
		exit(1);
	}

}

void CDbfFile::Close()
{
	DBFClose( hDBF );

}

int CDbfFile::WriteRecord(int hEntity, int iField, void *pValue)
{
    int	    nRecordOffset, i, j;
    uchar	*pabyRec;
    char	szSField[40], szFormat[12];

/* -------------------------------------------------------------------- */
/*	Is this a valid record?						*/
/* -------------------------------------------------------------------- */
    if( hEntity < 0 || hEntity > hDBF->nRecords )
        return( FALSE );

    if( hDBF->bNoHeader )
        DBFWriteHeader(hDBF);

/* -------------------------------------------------------------------- */
/*      Is this a brand new record?                                     */
/* -------------------------------------------------------------------- */
    if( hEntity == hDBF->nRecords )
    {
		DBFFlushRecord( hDBF );

		hDBF->nRecords++;
		for( i = 0; i < hDBF->nRecordLength; i++ )
			hDBF->pszCurrentRecord[i] = ' ';

		hDBF->nCurrentRecord = hEntity;
    }

/* -------------------------------------------------------------------- */
/*      Is this an existing record, but different than the last one     */
/*      we accessed?                                                    */
/* -------------------------------------------------------------------- */
    if( hDBF->nCurrentRecord != hEntity )
    {
		DBFFlushRecord( hDBF );

		nRecordOffset = hDBF->nRecordLength * hEntity + hDBF->nHeaderLength;

		fseek( hDBF->fp, nRecordOffset, 0 );
		fread( hDBF->pszCurrentRecord, hDBF->nRecordLength, 1, hDBF->fp );

		hDBF->nCurrentRecord = hEntity;
    }

    pabyRec = (uchar *) hDBF->pszCurrentRecord;

/* -------------------------------------------------------------------- */
/*      Assign all the record fields.                                   */
/* -------------------------------------------------------------------- */
    switch( hDBF->pachFieldType[iField] )
    {
		case 'D':
		case 'N':
		if( hDBF->panFieldDecimals[iField] == 0 )
		{
			sprintf( szFormat, "%%%dd", hDBF->panFieldSize[iField] );
			sprintf(szSField, szFormat, (int) *((double *) pValue) );
			if( (int) strlen(szSField) > hDBF->panFieldSize[iField] )
				szSField[hDBF->panFieldSize[iField]] = '\0';
			strncpy((char *) (pabyRec+hDBF->panFieldOffset[iField]),
				szSField, strlen(szSField) );
		}
		else
		{
			sprintf( szFormat, "%%%d.%df", 
		    hDBF->panFieldSize[iField],
		    hDBF->panFieldDecimals[iField] );
			sprintf(szSField, szFormat, *((double *) pValue) );
			if( (int) strlen(szSField) >  hDBF->panFieldSize[iField] )
				szSField[hDBF->panFieldSize[iField]] = '\0';
			strncpy((char *) (pabyRec+hDBF->panFieldOffset[iField]),
		    szSField, strlen(szSField) );
		}
		break;

		default:
		if( (int) strlen((char *) pValue) > hDBF->panFieldSize[iField] )
		{
			int iben = strlen((char *) pValue);
			char szTemp[255];
			strncpy(szTemp, (char *) pValue, 253);
			szTemp[253] = '\0';
			strcat(szTemp, "*");
			strcpy((char *) pValue, szTemp);
			j = hDBF->panFieldSize[iField];
		}
		else
			j = strlen((char *) pValue);

		strncpy((char *) (pabyRec+hDBF->panFieldOffset[iField]),
				(char *) pValue, j );
		break;
    }

    hDBF->bCurrentRecordModified = TRUE;
    hDBF->bUpdated = TRUE;

    return( TRUE );

}
