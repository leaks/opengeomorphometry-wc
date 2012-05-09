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
//		CSVWriter.h
//
//
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include "DataWriter.h"
#include "DataStructs.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "boost\lexical_cast.hpp"
#include "Util.h"

class CSVWriter : public DataWriter
{
public: // Constructors
					CSVWriter(std::string fileName) : DataWriter(fileName) {};

public: // Public Methods
	int				Write(std::vector<Record> data);

private: // Private Methods
	std::string		getLine(Record data);
};

#endif // CSV_WRITER_H