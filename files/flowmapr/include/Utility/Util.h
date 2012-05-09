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
//		Util.h
//
//		Various utility functions
//
/////////////////////////////////////////////////////////////////////////////////////

#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <ctime>
#include "boost\tokenizer.hpp"
#include "boost\lexical_cast.hpp"

class Util
{
public:
	static	Util*						getSingleton();
			std::vector<std::string>	explode(std::string line, std::string delim);
			std::string					implode(std::vector<std::string> data, std::string delim);
			std::string					getTimestamp();
protected:
	Util();
	~Util();
private:
	static Util* m_instance;
};
#endif // UTIL_H