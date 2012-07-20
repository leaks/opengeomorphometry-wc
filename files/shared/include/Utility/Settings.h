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
#include <utility>
#include "Util.h"
#include "boost/lexical_cast.hpp"

// Ugly, but complaint with C++ 2003 spec 14.7.3/2 and 14.5.2/2
namespace detail
{
	template <class T>
	T getValue( const std::map<std::string, std::pair<std::string,bool>> &m, const std::string &key )
	{
		return boost::lexical_cast<T>( m.count( key ) > 0 ? m.find( key )->second.first : "" );
	}
	
	template <> inline
	std::string getValue( const std::map<std::string, std::pair<std::string,bool>> &m, const std::string &key )
	{
		return m.count( key ) > 0 ? m.find( key )->second.first : "";
	}
}

class Settings
{
public: // Public Methods
	static	Settings*	getSingleton();
	template <class T>
	static	T		getValue( const std::string &s ) { return detail::getValue<T>( m_mSettings, s ); }
	static	void		setValue(std::string key, std::string value, bool cmdLine = false);
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
	static	std::map<std::string, std::pair<std::string,bool>>	m_mSettings;
};

#endif // SETTINGS_H
