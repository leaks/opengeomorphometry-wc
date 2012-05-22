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
//		Logger.h
//
//		internal logging utility
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <fstream>
#include "Util.h"
#include "Settings.h"
#include "boost/lexical_cast.hpp"

enum LOG_OPTIONS{
	FILE_OUT = 1,
	CONSOLE_OUT = 2,
	DEBUG = 16,
	INFO = 32,
	WARNING = 64
};

class Logger
{
public:
	static Logger * getSingleton();
	static void		log(std::string msg);
	static void		log(std::string msg, LOG_OPTIONS opts);
protected:
	Logger();
	~Logger();

private:
	static void		writeLine(std::string line);

private:
	static Logger*		m_pInstance;
	static std::string	m_sFileName;
	static LOG_OPTIONS	m_eDefaultLogOptions;
	static LOG_OPTIONS	m_eLogLevel;
};

#endif // LOGGER_H