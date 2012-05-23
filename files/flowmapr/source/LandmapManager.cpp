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

}

LandmapManager::~LandmapManager()
{

}

void LandmapManager::start()
{
	CSVReader *reader = new CSVReader(Settings::getSingleton()->getValue("input_file_name"));
	std::vector<Record> recs = reader->Read();
	std::vector<double> dataIn;
	std::vector<double> dataOut;

	// grab the records from the CSV and dump the values from the rows into a vector
	for(std::vector<Record>::iterator iter = recs.begin(); iter != recs.begin(); iter++)
	{
		for(int i = 0; i < iter->fields; i++)
		{
			dataIn.push_back(iter->data[i]);
		}
	}

	LandmapThread thread;
	m_tgThreadPool.add_thread(boost::thread(&thread, &dataIn, &dataOut)*);

}

void LandmapManager::stop()
{

}
