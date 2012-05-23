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
//		LandmapManager.h
//
//		Setting file manager
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef LANDMAP_MANAGER_H
#define LANDMAP_MANAGER_H

#include "DataFiles\DataStructs.h"
#include "DataFiles\CSVWriter.h"
#include "DataFiles\CSVReader.h"
#include "ThreadManager.h"
#include "LandmapThread.h"

class LandmapManager : public ThreadManager
{
public: // Constructors
	LandmapManager();
	~LandmapManager();

public: // Public Methods
	void	start();
	void	stop();
};

#endif // LANDMAP_MANAGER_H