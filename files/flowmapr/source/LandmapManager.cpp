/////////////////////////////////////////////////////////////////////////////////////
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
	std::vector<double> dataIn = getData();
	std::vector<double> dataOut = dataIn; // ensure they're the same size 
	
	// find out how large the chunks are
	int count = dataIn.size() / m_iMaxThreads;
	int pos = 0;

	// parcel out the data to the threads.
	LandmapThread thread;
	for(int i = 0; i < m_iMaxThreads; i++)
	{
		m_tgThreadPool.create_thread(boost::bind(&thread, &dataIn, pos, count, &dataOut));
		pos += count;
	}

	//wait for execution to finish
	m_tgThreadPool.join_all();


}

void LandmapManager::stop()
{

}

std::vector<double> LandmapManager::getData()
{
	std::vector<double> data;
	CSVReader *reader = new CSVReader(Settings::getSingleton()->getValue("input_file_name"));
	std::vector<Record> recs = reader->Read();
	std::string inputType = Settings::getSingleton()->getValue("input_type");
	if(inputType.compare("grid")==0)
	{
		// grab the records from the CSV and dump the values from the rows into a vector
		for(std::vector<Record>::iterator iter = recs.begin(); iter != recs.begin(); iter++)
		{
			for(int i = 0; i < iter->fields; i++)
			{
				data.push_back(iter->data[i]);
			}
		}
	}
	else if(inputType.compare("column")==0)
	{
		for(std::vector<Record>::iterator iter = recs.begin(); iter != rec.end(); iter++)
		{
			data.push_back(iter->data[0];
		}
	}
	else if(inputType.compare("latlong")==0)
	{
		for(std::vector<Record>::iterator iter = recs.begin(); iter != rec.end(); iter++)
		{
			data.push_back(iter->data[2];
		}
	}
	return data;
}
