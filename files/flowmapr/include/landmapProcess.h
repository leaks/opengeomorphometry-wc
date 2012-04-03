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
//		landmapProcess.h
//
//		main application entry point for headless version.
//
/////////////////////////////////////////////////////////////////////////////////////

#ifndef LANDMAP_PROCESS_H
#define LANDMAP_PROCESS_H

#include <fstream>
#include <string>
#include <math.h>
#include <stdio.h>
#include "VOLDFile.h"
#include "landmapStructs.h"
#include "DBFFile.h"
#include "DEMProcess.h"
#include "Sort.h"

class LandmapProcess
{
public: // Constructors
					LandmapProcess();
					~LandmapProcess();

public: // Public Methods
	double*			ReadDbf(std::string temp);
	unsigned int	RunProcess(void * param);

public: // Property Methods
	void			SetFullCurrentPath(std::string fullPath);
	void			SetFileName(std::string fileName);
	void			SetX(double x);
	void			SetY(double y);
	void			SetMissDataValue(double missDataValue);
	void			SetPitDepth(double pitDepth);
	void			SetPitArea(long pitArea);
	void			SetPitVolume(double pitVolume);
	void			SetRow(int row);
	void			SetGridSize(double gridSize);
	void			SetColumn(int column);
	void			SetWorkingDir(std::string workingDir);
	void			SetInvertedElev(bool invertedElev);

	std::string		GetFullCurrentPath();
	std::string		GetFileName();
	double			GetX();
	double			GetY();
	double			GetMissDataValue();
	double			GetPitDepth();
	long			GetPitArea();
	double			GetPitVolume();
	int				GetRow();
	double			GetGridSize();
	int				GetColumn();
	std::string		GetWorkingDir();
	bool			GetInvertedElev();

private: // Property Fields
	std::string		m_sFullCurrentPath;
	std::string		m_sFileName;
	double			m_dX;
	double			m_dY;
	double			m_dMissDataValue;
	double			m_dPitDepth;
	long			m_lPitArea;
	double			m_dPitVolume;
	int				m_iRow;
	double			m_dGridSize;
	int				m_iColumn;
	std::string		m_sWorkingDirectory;
	bool			m_bInvertedElev;
}; //LandmapProcess

#endif //LANDMAP_PROCESS_H