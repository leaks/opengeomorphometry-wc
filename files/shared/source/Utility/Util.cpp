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
//		Util.cpp
//
//		Various utility functions
//
//		Author: M Harrison mharrison@niagararesearch.org
//
/////////////////////////////////////////////////////////////////////////////////////

#include "Utility/Util.h"

Util* Util::m_instance = 0;

/////////////////////////////////////////////////////////////////////////////////////
//
//		Public Methods
//
/////////////////////////////////////////////////////////////////////////////////////
Util * Util::getSingleton()
{
	if(!m_instance)
		m_instance = new Util();
	return m_instance;
}

std::vector<std::string> Util::explode(std::string line, std::string delim)
{
	std::vector<std::string> data;

	if(line.find(delim) == -1)
	{
		data.push_back(line);
	}
	else
	{
		boost::tokenizer<boost::escaped_list_separator<char> > tok(line);
		for(boost::tokenizer<boost::escaped_list_separator<char> >::iterator iter = tok.begin(); iter != tok.end(); ++iter)
		{
			data.push_back(*iter);
		}
	}
	return data;
}

std::string Util::implode(std::vector<std::string> data, std::string delim)
{
	std::string line = "";

	for( std::vector<std::string>::iterator iter = data.begin(); iter != data.end(); iter++)
	{
		line = line + *iter + delim; 
	}
	line = line.substr(0, line.size() - 1);

	return line;
}

std::string Util::getTimestamp()
{
	std::time_t rawtime;
	struct tm * timeinfo;

	rawtime = std::time(NULL);
	timeinfo = std::localtime(&rawtime);

	return std::string(std::asctime(timeinfo));
}

/////////////////////////////////////////////////////////////////////////////////////
//
//		Protected Methods
//
/////////////////////////////////////////////////////////////////////////////////////
Util::~Util()
{
	if(m_instance)
		delete(m_instance);
}