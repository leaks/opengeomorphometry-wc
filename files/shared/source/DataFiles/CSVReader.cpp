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
//		CSVReader.cpp
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#include "DataFiles/CSVReader.h"

/////////////////////////////////////////////////////////////////////////////////////
//
//		Public Methods
//
/////////////////////////////////////////////////////////////////////////////////////
std::vector<Record> CSVReader::Read()
{
	std::vector<Record> data;
	Record rec;
	std::fstream fin(m_sFileName, std::fstream::in);
	std::string line;

	if(fin.fail())
	{
		if(fin.is_open())
			fin.close();
		return data;
	}
	std::getline(fin, line);
	
	while(!fin.eof())
	{
		std::getline(fin, line);
		data.push_back(getData(line));
	}

	fin.close();
	
	return data;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//		Private Methods
//
/////////////////////////////////////////////////////////////////////////////////////
Record CSVReader::getData(std::string line)
{
	Record rec;
	std::vector<std::string> fields = Util::getSingleton()->explode(line, m_sDelim);
	double * dataArr = new double[fields.size()];
	unsigned int count = 0;
	for( std::vector<std::string>::iterator iter = fields.begin(); iter != fields.end(); iter++)
	{
		dataArr[count++] = boost::lexical_cast<double>(*iter);
	}
	rec.fields = fields.size();
	rec.data = dataArr;
	return rec;
}