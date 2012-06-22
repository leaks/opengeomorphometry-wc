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
//		DataReader.h
//
//
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef DATA_READER_H
#define DATA_READER_H

#include "DataStructs.h"
#include <string>
#include <vector>

class DataReader
{
public: // Constructors
								DataReader(std::string filename):m_sFileName(filename) {};
	//virtual						~DataReader();

public: // Public Methods
	virtual std::vector<Record>	Read() = 0;

protected: // Property Fields
	std::string		m_sFileName;
};

#endif // DATA_READER_H