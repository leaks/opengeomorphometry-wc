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
//////////////////////////////////////////////////////////////////////////////////////

#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "Util.h"

class Settings
{
public: // Public Methods
	static	Settings*	getSingleton();
	static	std::string	getValue(std::string key);
	static	void		setValue(std::string key, std::string value);
	static	void		setFileName(std::string fileName);

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