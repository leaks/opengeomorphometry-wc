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
//		Settings.h
//
//		Setting file manager
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "Util.h"
#include "boost/lexical_cast.hpp"

class Settings
{
public: // Public Methods
	static	Settings*	getSingleton();
	template <class T>
	static	T			getValue(std::string key){return boost::lexical_cast<T>((m_mSettings.count(key) > 0) ? m_mSettings.find(key)->second : "");};
	template <>
	static std::string	getValue<std::string>(std::string key){return (m_mSettings.count(key) > 0) ? m_mSettings.find(key)->second : "";};
	static	void		setValue(std::string key, std::string value);
	static	void		setFileName(std::string fileName);
	static	void		forceRead();

protected: // Constructors
						Settings();
						~Settings();

private: // Private Methods
	static void			read();
	static void			write();
	static void			update(std::string key);
	static void			writeLine(std::string line);
	static void			parseLine(std::string line);

private: // Properties
	static	Settings*							m_pInstance;
	static	std::string							m_sFileName;
	static	std::map<std::string, std::string>	m_mSettings;
};

#endif // SETTINGS_H