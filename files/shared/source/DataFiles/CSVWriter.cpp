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
//		CSVWriter.cpp
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#include "DataFiles/CSVWriter.h"

/////////////////////////////////////////////////////////////////////////////////////
//
//		Public Methods
//
/////////////////////////////////////////////////////////////////////////////////////
int CSVWriter::Write(std::vector<Record> data)
{
	std::fstream fout (m_sFileName, std::fstream::out);

	if(fout.fail())
	{
		if(fout.is_open())
			fout.close();
		return -1;
	}

	for(std::vector<Record>::iterator iter = data.begin(); iter != data.end(); iter++)
	{
		fout << getLine(*iter) << std::endl;
	}

	fout.close();
	
	return (int)data.size();
}

/////////////////////////////////////////////////////////////////////////////////////
//
//		Private Methods
//
/////////////////////////////////////////////////////////////////////////////////////
std::string CSVWriter::getLine(Record data)
{
	std::vector<std::string> line;
	
	for(int i = 0; i < data.fields; i++)
	{
		line.push_back(boost::lexical_cast<std::string>(data.data[i]));
	}
	return Util::getSingleton()->implode(line, m_sDelim);
}