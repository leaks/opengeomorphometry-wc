//////////////////////////////////////////////////////////////////////////////////////
//This file is part of open-geomorphometry.
//
//    open-geomorphometry is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at oyur option) any later version.
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
//		Main.cpp
//
//		main application entry point for headless version.
//
/////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "LandmapManager.h"
#include "Utility\Settings.h"
#include "Utility\Logger.h"

// Main entry point
int main(int argc, char *argv[])
{
	
	std::string param;
	if( argc > 1)
	{
		for(int i = 1; i < argc; i++)
		{
			param = std::string(argv[i]);
			if(param.find("-f=") >= 0)
			{
				Settings::getSingleton()->setValue("input_file_name", param.substr(4));
			}
			else if(param.find("-c=") >= 0)
			{
				Settings::getSingleton()->setFileName(param.substr(4));
			}
			else if(param.find("-t=") >= 0)
			{
				Settings::getSingleton()->setValue("input_type", param.substr(4));
			}
			else if(param.find("-h") >= 0 || param.find("--help") >= 0)
			{
				std::cout << "FlowMapR - NEED DESCRIPTION" << std::endl;
				std::cout << "USAGE STRING" << std::endl;
				std::cout << "\t-h\t--help\t Prints this message." << std::endl;
				std::cout << "\t-f\t--file\t Input file path." << std::endl;
				std::cout << "\t-c\t--config\t Config file path (Optional)." << std::endl;
				std::cout << "\t-t\t--type\t Input file type (Optional)." << std::endl;
				return 0;
			}
		}

		//start the work manager
	}
	else
	{
		std::cout << "Usage: flowmapr -f=filename. See -h or --help for more options." << std::endl;
	}
	return 0;
}