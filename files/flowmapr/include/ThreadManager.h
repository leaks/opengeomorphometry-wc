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
//		ThreadManager.h
//
//		Base class for thread management
//
//		Author: M Harrison mharrison@niagararesearch.org
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef THREAD_MANAGER_H
#define THREAD_MANAGER_H

#include <vector>
#include "ThreadObject.h"
#include "Settings.h"
#include "boost/lexical_cast.hpp"

class ThreadManager
{
public: // Constructor
					ThreadManager();
	virtual			~ThreadManager() {};

public: // Public Methods
	virtual void	start() {};
	virtual void	stop() {};

protected:
	virtual void	assignWork(int threadID) = 0;

protected: // Property Methods
	//std::vector<ThreadObject>	m_vtThreads;
	int							m_iMaxThreads;
};

#endif // THREAD_MANAGER_H