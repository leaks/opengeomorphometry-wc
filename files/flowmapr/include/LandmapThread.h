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
//		LandmapThread.h
//
//		Thread instance for processing the file
//
//		Author: M Harrison mharrison@niagararesearch.org
//
/////////////////////////////////////////////////////////////////////////////////////

#ifndef LANDMAP_THREAD_H
#define LANDMAP_THREAD_H

#include <fstream>
#include <string>
#include <string.h> // memcpy
#include <math.h>
#include <stdio.h>
#include <vector>
#include "VOLDFile.h"
#include "landmapStructs.h"
#include "DEMProcess.h"
#include "Sort.h"
#include "ThreadObject.h"
#include "Settings.h"
#include "Logger.h"
#include "DataFiles/DataStructs.h"
#include "boost/thread.hpp"
#include "boost/bind.hpp"

class LandmapThread : public ThreadObject
{
public:
	typedef struct param_t
	{
		std::vector<double> input;
		std::vector<Record> pit;
		std::vector<Record> fill;
		std::vector<Record> pond;
		std::vector<Record> vold;
		std::vector<Record> mold;
		std::vector<Record> dem;
		std::vector<Record> idem;
		std::vector<Record> ipit;
	}PARAM_T;

public: // Constructors
					LandmapThread();
					~LandmapThread();

public: // Public Methods
	void			run();
	void			run(param_t* data, int startPos, int count);
	//void			operator()();
	void			operator()(param_t* data, int startPos, int count);

//public: // Property Methods
//	void			SetMissDataValue(double missDataValue);
//	void			SetPitDepth(double pitDepth);
//	void			SetPitArea(long pitArea);
//	void			SetPitVolume(double pitVolume);
//	void			SetInvertedElev(bool invertedElev);
//
//	double			GetMissDataValue();
//	double			GetPitDepth();
//	long			GetPitArea();
//	double			GetPitVolume();
//	bool			GetInvertedElev();

private: // Property Fields
	double			m_dMissDataValue;
	double			m_dPitDepth;
	long			m_lPitArea;
	double			m_dPitVolume;
	bool			m_bInvertedElev;
	int				m_iLength;
	int				m_iWidth;

}; //LandmapProcess

#endif //LANDMAP_THREAD_H
