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
//		ThreadObject.h
//
//		Base object class for threading objects
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef THREAD_OBJECT_H
#define THREAD_OBJECT_H

#include "boost/thread.hpp"

class ThreadObject
{
public: // Constructor
					ThreadObject() {};
	virtual			~ThreadObject() {};

public: // Public Methods
	virtual void	run() {};
	virtual void	join() { m_tThread.join(); };

protected: // Property fields
	boost::thread	m_tThread;
};

#endif // THREAD_OBJECT_H