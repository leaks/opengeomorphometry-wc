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
//		TXTReader.cpp
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#include "DataFiles/TXTReader.h"

/////////////////////////////////////////////////////////////////////////////////////
//
//		Public Methods
//
/////////////////////////////////////////////////////////////////////////////////////
std::vector<Record> TXTReader::Read()
{
	std::vector<Record> data;
	Record rec;
	std::fstream fin(m_sFileName, std::fstream::in);
	std::string line;

	if(fin.fail())
	{
		if(fin.fail())
		{
			if(fin.is_open())
				fin.close();
			return data;
		}
	}

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
Record TXTReader::getData(std::string line)
{
	Record rec;
	double *dataArr = new double[1];
	dataArr[0] = boost::lexical_cast<double>(line);
	rec.fields = 1;
	rec.data = dataArr;
	return rec;
}