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
//		Logger.cpp
//
//		internal logging utility
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#include "Utility/Logger.h"

/////////////////////////////////////////////////////////////////////////////////////
//
//		Default values for static members
//
/////////////////////////////////////////////////////////////////////////////////////
Logger* Logger::m_pInstance = 0;
LOG_OPTIONS Logger::m_eDefaultLogOptions = (LOG_OPTIONS) (LOG_OPTIONS::CONSOLE_OUT | LOG_OPTIONS::FILE_OUT);
LOG_OPTIONS Logger::m_eLogLevel = LOG_OPTIONS::INFO;
std::string Logger::m_sFileName = "log.txt";

/////////////////////////////////////////////////////////////////////////////////////
//
//		Public Methods
//
/////////////////////////////////////////////////////////////////////////////////////
Logger* Logger::getSingleton()
{
	if(!m_pInstance)
		m_pInstance = new Logger();
	return m_pInstance;
}

void Logger::log(std::string msg)
{
	log(msg, (LOG_OPTIONS)(m_eLogLevel | m_eDefaultLogOptions));
}

void Logger::log(std::string msg, LOG_OPTIONS opts)
{
	opts = (LOG_OPTIONS)(opts | m_eDefaultLogOptions);
	if((opts & WARNING) == WARNING)
	{
		if((opts & m_eLogLevel) == WARNING)
		{
			//return;

			if(((opts & m_eDefaultLogOptions) & FILE_OUT) == FILE_OUT)
			{
				writeLine(Util::getSingleton()->getTimestamp() + " WARNING: " + msg);
			}

			if(((opts & m_eDefaultLogOptions) & CONSOLE_OUT) == CONSOLE_OUT)
			{
				std::cout << Util::getSingleton()->getTimestamp() << " WARNING: " << msg << std::endl;
			}
		}
	}

	if((opts & INFO) == INFO)
	{
		if((opts & m_eLogLevel) == INFO)
		{
			//return;

			if(((opts & m_eDefaultLogOptions) & FILE_OUT) == FILE_OUT)
			{
				writeLine(Util::getSingleton()->getTimestamp() + " INFO: " + msg);
			}

			if(((opts & m_eDefaultLogOptions) & CONSOLE_OUT) == CONSOLE_OUT)
			{
				std::cout << Util::getSingleton()->getTimestamp() << " INFO: " << msg << std::endl;
			}
		}
	}
	
	if((opts & DEBUG) == DEBUG)
	{
		if((opts & m_eLogLevel) == DEBUG)
		{
			//return;

			if(((opts & m_eDefaultLogOptions) & FILE_OUT) == FILE_OUT)
			{
				writeLine(Util::getSingleton()->getTimestamp() + " DEBUG: " + msg);
			}

			if(((opts & m_eDefaultLogOptions) & CONSOLE_OUT) == CONSOLE_OUT)
			{
				std::cout << Util::getSingleton()->getTimestamp() << " DEBUG: " << msg << std::endl;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////
//
//		Constructor
//
/////////////////////////////////////////////////////////////////////////////////////
Logger::Logger()
{
	m_sFileName = Settings::getSingleton()->getValue<std::string>("log_file");
	if(m_sFileName.length() < 1)
	{
		m_sFileName = "log.txt";
		Settings::getSingleton()->setValue("log_file", m_sFileName);
	}

	m_eDefaultLogOptions = (LOG_OPTIONS)Settings::getSingleton()->getValue<int>("log_default_options");
	m_eLogLevel = (LOG_OPTIONS)Settings::getSingleton()->getValue<int>("log_level");
}

Logger::~Logger()
{
	if(m_pInstance)
		delete m_pInstance;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//		Private Methods
//
/////////////////////////////////////////////////////////////////////////////////////
void Logger::writeLine(std::string line)
{
	std::fstream fout(m_sFileName, std::fstream::out | std::fstream::app);
	if(fout.fail())
	{
		if(fout.is_open())
			fout.close();
		return;
	}
	fout << line << std::endl;
	fout.close();
}