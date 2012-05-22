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
//		DataWriter.h
//
//
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef DATA_WRITER_H
#define DATA_WRITER_H

#include "DataStructs.h"
#include <string>
#include <vector>

class DataWriter
{
public: // Constructors
				DataWriter(std::string fileName) : m_sFileName(fileName) {};
	virtual		~DataWriter();

public: // Public Methods
	virtual int Write(std::vector<Record> data) = 0;

protected: // Property Fields
	std::string m_sFileName;
};

#endif // DATA_WRITER_H