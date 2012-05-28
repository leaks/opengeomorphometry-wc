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
//		TXTReader.h
//
//
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef TXT_READER_H
#define TXT_READER_H

#include "DataReader.h"
#include "DataStructs.h"
#include "Util.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "boost/lexical_cast.hpp"

class TXTReader : public DataReader
{
public: // Constructors
						TXTReader(std::string fileName) : DataReader(fileName) {};

public: // Public Methods
	std::vector<Record>	Read();

private: // Private Methods
	Record				getData(std::string line);
};

#endif // TXT_READER_H