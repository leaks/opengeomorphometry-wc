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
	if(m_bStarted)
		m_tgThreadPool.interrupt_all();
}

void LandmapManager::start()
{
	m_bStarted = true;
	//std::vector<double> dataIn = getData();
	//std::vector<double> dataOut = dataIn; // ensure they're the same size 
	std::string outFileName = Settings::getSingleton()->getValue<std::string>("output_file_name");
	std::string outFileType = Settings::getSingleton()->getValue<std::string>("output_file_type");
	LandmapThread::param_t data;
	data.input = getData();
	std::string inputType = Settings::getSingleton()->getValue<std::string>("input_type");

	// find out how large the chunks are
	// default values

	int count = (int)data.input.size() / m_iMaxThreads;
	int pos = 0;

	if(inputType.compare("grid")==0)
	{
		count = (Settings::getSingleton()->getValue<int>("input_file_length")/ m_iMaxThreads) * Settings::getSingleton()->getValue<int>("input_file_width");
	}
	//else if(inputType.compare("column")==0)
	//{

	//}
	//else if(inputType.compare("latlong")==0)
	//{

	//}
	
	// parcel out the data to the threads.
	LandmapThread thread;
	for(int i = 0; i < m_iMaxThreads; i++)
	{
		m_tgThreadPool.create_thread(boost::bind(&LandmapThread::run, &thread, &data, pos, count));
		pos += count;
		if(pos > (int)data.input.size())
			break;
	}

	//wait for execution to finish
	m_tgThreadPool.join_all();

	//TXTWriter *txtWriter = new TXTWriter(Settings::getSingleton()->getValue<std::string>("output_file_name"));

	//txtWriter->Write(getRecords(dataOut));

	//delete(txtWriter);

	CSVWriter *csvWriter = new CSVWriter(outFileName + "-dem." + outFileType);
	csvWriter->Write(data.dem);
	delete(csvWriter);

	csvWriter = new CSVWriter(outFileName + "-fill." + outFileType);
	csvWriter->Write(data.fill);
	delete(csvWriter);

	csvWriter = new CSVWriter(outFileName + "-pit." + outFileType);
	csvWriter->Write(data.pit);
	delete(csvWriter);

	csvWriter = new CSVWriter(outFileName + "-pond." + outFileType);
	csvWriter->Write(data.pond);
	delete(csvWriter);

	csvWriter = new CSVWriter(outFileName + "-mold." + outFileType);
	csvWriter->Write(data.mold);
	delete(csvWriter);

	csvWriter = new CSVWriter(outFileName + "-vold." + outFileType);
	csvWriter->Write(data.vold);
	delete(csvWriter);

	csvWriter = new CSVWriter(outFileName + "-idem." + outFileType);
	csvWriter->Write(data.idem);
	delete(csvWriter);

	csvWriter = new CSVWriter(outFileName + "-ipit." + outFileType);
	csvWriter->Write(data.ipit);
	delete(csvWriter);
}

void LandmapManager::stop()
{
	m_bStarted = false;
}

std::vector<double> LandmapManager::getData()
{
	std::vector<double> data;
	
	std::vector<Record> recs;// = reader->Read();
	std::string inputType = Settings::getSingleton()->getValue<std::string>("input_type");
	if(inputType.compare("grid")==0)
	{
		CSVReader *reader = new CSVReader(Settings::getSingleton()->getValue<std::string>("input_file_name"));
		recs = reader->Read();
		// grab the records from the CSV and dump the values from the rows into a vector
		for(std::vector<Record>::iterator iter = recs.begin(); iter != recs.end(); iter++)
		{
			for(int i = 0; i < iter->fields; i++)
			{
				data.push_back(iter->data[i]);
			}
		}
		delete(reader);
	}
	else if(inputType.compare("column")==0)
	{
		for(std::vector<Record>::iterator iter = recs.begin(); iter != recs.end(); iter++)
		{
			data.push_back(iter->data[0]);
		}
	}
	else if(inputType.compare("latlong")==0)
	{
		for(std::vector<Record>::iterator iter = recs.begin(); iter != recs.end(); iter++)
		{
			data.push_back(iter->data[2]);
		}
	}
	return data;
}

std::vector<Record> LandmapManager::getRecords(std::vector<double> data, int fields)
{
	std::vector<Record> recs;
	Record rec;
	double* dataArr = new double[fields];

	for(std::vector<double>::iterator iter = data.begin(); iter != data.end(); iter++)
	{
		for(int i = 0; i < fields; i++)
		{
			dataArr[i] = *iter;
			if(++iter == data.end())
				break;
		}
		rec.data = dataArr;
		rec.fields = 1;
		recs.push_back(rec);
	}
	return recs;
}