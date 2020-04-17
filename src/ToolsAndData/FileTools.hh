/***********************************************************************
 *  This code is part of MpsSolver
 *
 *  Copyright (C)  Mike Steglich - Technical University of Applied Sciences
 *  Wildau, Germany 
 *
 *  MpsSolver is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MpsSolver is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
 *  License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/


#ifndef FILEBASE_HH
#define FILEBASE_HH

#include <string>

using namespace std;



    class FileTools
    {

    protected:
     
        /**
         * constructor
         */
        FileTools();									

        /**
		 * destructor
		 */
     ~FileTools();

	
    public:
	

        /**
         * @brief get path separator char
         * @param path          path
         * @param psDef         default path separator char
         * @return              path separator char
         */
        static char getDirSepChar(const char *path, const char psDef = '/');

        /**
         * @brief replace last component of path by another file name
         * @param path          path
         * @param file          other file name
         * @param ps            path separator char
         * @return              replaced file name
         */
        static string replFileInPath(string *path, string *file, char ps);


        /**
         * @brief Checks whether a file exists
         * @param fileName      Name of the file
         * @return              True or false
         */
        static bool exists(const string& fileName);
    };

    


#endif // FILEBASE_HH
