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
//		LandmapManager.cpp
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#include "LandmapManager.h"

LandmapManager::LandmapManager() : ThreadManager()
{
	for(int i = 0; i < m_iMaxThreads; i++)
		m_vThreadPool.push_back(LandmapThread());
}

LandmapManager::~LandmapManager()
{

}

void LandmapManager::start()
{
	for(std::vector<LandmapThread>::iterator iter = m_vThreadPool.begin(); iter != m_vThreadPool.end(); iter++)
	{

	}
}

void LandmapManager::stop()
{

}

void LandmapManager::assignWork(int threadID, int start, int end)
{

}