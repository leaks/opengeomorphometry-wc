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
//		LandmapThread.cpp
//
//		Author: M Harrison mharrison@niagararesearch.org
//
/////////////////////////////////////////////////////////////////////////////////////

#include "LandmapThread.h"

/////////////////////////////////////////////////////////////////////////////////////
//
//		Constructors
//
/////////////////////////////////////////////////////////////////////////////////////
LandmapThread::LandmapThread()
{
	m_dMissDataValue = 0.0;
	m_dPitDepth = 0.15;
	m_lPitArea = 10;
	m_dPitVolume = 0.0;
	m_bInvertedElev = true;
}

LandmapThread::~LandmapThread()
{

}

/////////////////////////////////////////////////////////////////////////////////////
//
//		Public Methods
//
/////////////////////////////////////////////////////////////////////////////////////

void LandmapThread::operator()(std::vector<double>* dataIn, int startPos, int count, std::vector<double>* dataOut)
{
	double		*pdElev = 0;
//	CDEMProcess	*pDEMProcess;
	double		*pdTmpDEMGrid;
	double		*pdDEMGrid;
	double		*pdBottomUp;
	short		*pshFlowGrid;
	int			*piShedGrid;
	int			*piShedWGrid;
	int			*piUpSlpGrid;
	int			*piDrecGrid;
	int			*piDEMSort;

	//*********************************
	//IGK
	//3 Arrays used in calculation of volume to flood, mm to flood, and pArea
	double		*pdVol2FlGrid;
	double		*pdMm2FlGrid;
	int			*piPArea;
	//**********************************

	bool bValid = true;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//		Property Methods
//
/////////////////////////////////////////////////////////////////////////////////////

void LandmapThread::SetMissDataValue(double missDataValue)
{
	m_dMissDataValue = missDataValue;
}

void LandmapThread::SetPitDepth(double pitDepth)
{
	m_dPitDepth = pitDepth;
}

void LandmapThread::SetPitArea(long pitArea)
{
	m_lPitArea = pitArea;
}

void LandmapThread::SetPitVolume(double pitVolume)
{
	m_dPitVolume = pitVolume;
}

void LandmapThread::SetInvertedElev(bool invertedElev)
{
	m_bInvertedElev = invertedElev;
}

double LandmapThread::GetMissDataValue()
{
	return m_dMissDataValue;
}

double LandmapThread::GetPitDepth()
{
	return m_dPitDepth;
}

long LandmapThread::GetPitArea()
{
	return m_lPitArea;
}

double LandmapThread::GetPitVolume()
{
	return m_dPitVolume;
}

bool LandmapThread::GetInvertedElev()
{
	return m_bInvertedElev;
}
