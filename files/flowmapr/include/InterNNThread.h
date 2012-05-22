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
//		InterNNThread.h
//
//		Thread instance for interpolating the file using Natural Neighbour
//
//		Author: M Harrison mharrison@niagararesearch.org
//
/////////////////////////////////////////////////////////////////////////////////////

#ifndef INTER_NN_THREAD_H
#define INTER_NN_THREAD_H

#include "ThreadObject.h"

class InterNNThread : public ThreadObject
{
public:
	InterNNThread();
	~InterNNThread();

public:
	void run();
	
};

#endif // INTER_NN_THREAD_H