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
//		Settings.cpp
//
//		Setting file manager
//
//////////////////////////////////////////////////////////////////////////////////////

#include "Settings.h"

Settings* Settings::m_pInstance = 0;

/////////////////////////////////////////////////////////////////////////////////////
//
//		Public Methods
//
/////////////////////////////////////////////////////////////////////////////////////
Settings* Settings::getSingleton()
{
	if(!m_pInstance)
		m_pInstance = new Settings();
	return m_pInstance;
}

std::string Settings::getValue(std::string key)
{
	return m_mSettings.find(key)->second;
}

void Settings::setValue(std::string key, std::string value)
{
	if(m_mSettings.count(key) > 0)
		m_mSettings.find(key)->second == value;
	else
		m_mSettings.insert(std::pair<std::string, std::string>(key,value));
	update(key);
}

void Settings::setFileName(std::string fileName)
{
	m_sFileName = fileName;
}

/////////////////////////////////////////////////////////////////////////////////////
//
//		Constructor
//
/////////////////////////////////////////////////////////////////////////////////////
Settings::Settings()
{
	read();
}

Settings::~Settings()
{
	if(m_pInstance)
		delete m_pInstance;
	write();
}
/////////////////////////////////////////////////////////////////////////////////////
//
//		Private Methods
//
/////////////////////////////////////////////////////////////////////////////////////
void Settings::read()
{
	std::fstream fin(m_sFileName, std::fstream::in);
	std::string line;

	if(fin.fail())
	{
		if(fin.is_open())
			fin.close();
		return;
	}

	while(!fin.eof())
	{
		std::getline(fin, line);
		parseLine(line);
	}
	fin.close();
}

void Settings::write()
{
	for(std::map<std::string, std::string>::iterator iter = m_mSettings.begin(); iter != m_mSettings.end(); iter++)
	{
		writeLine(iter->first + ":" + iter->second);
	}
}

void Settings::update(std::string key)
{
	writeLine(key + ":" + m_mSettings.find(key)->second);
}

void Settings::writeLine(std::string line)
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

void Settings::parseLine(std::string line)
{
	int pos = line.find_first_of(':');
	m_mSettings.insert(std::pair<std::string, std::string>(line.substr(0, pos), line.substr(pos+1)));
}