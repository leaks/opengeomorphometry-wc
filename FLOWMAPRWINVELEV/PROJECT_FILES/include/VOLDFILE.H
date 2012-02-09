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
#include<fstream>

//Class represents Mold and Vold tables
//Use its functions to add and access values
//It will grow as needed automaticaly.



struct    VoldFileStructure
{
	int		iSeqNo;
	int		iDdir;
	int     iDrec;
	int     iUpslope;
	int     iShedNow;
	int     iStage;
	int     iUrec;
	int     iDs_Area;
	double   iVaration;
};





class CVoldFile
{
public:
	void RemoveAll();
	CVoldFile(CString sFileName);
	~CVoldFile();
	void AddNew(int iSeqNoNew,int iDdirNew, int iDrecNew, int iUpSlopeNew, int iShedNowNew, 
		int iStageNew, int iUrecNew = -1, int iDs_AreaNew = -1, double iVarationNew = -1);
	int GetSeqNo(int nIndex);
	int GetDdir(int nIndex);
	int GetDrec(int nIndex);
	int GetUpSlop(int nIndex);
	int GetShedNow(int nIndex);
	int GetStage(int nIndex);
	int GetUrec(int nIndex);
	int GetDs_Area(int nIndex);
	double GetVaration(int nIndex);

	void RemoveAt(int nIndex);
	int GetSize();
private:
	VoldFileStructure* pFile;
	int nNumberOfRecords;
	int nArraySize;

protected:
	CString sFileName;
};