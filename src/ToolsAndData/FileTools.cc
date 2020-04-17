/***********************************************************************
 *  This code is part of MpsSolver
 *
 *  Copyright (C) Mike Steglich - Technical University of Applied Sciences
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

#include "FileTools.hh"
#include "StringTools.hh"

#include <cstdlib>
#include <ctime>
#include <fstream>


/**
* @brief constructor
*/
FileTools::FileTools()
{
}

/**
* @brief destructor
*/
FileTools::~FileTools()
{
}


/**
     * @brief get path separator char
     * @param path          path
     * @param psDef         default path separator char
     * @return              path separator char or '\0' if no path
     */
char FileTools::getDirSepChar(const char *path, const char psDef)
{
    if (path && *path)
    {
        // path separator can be '/' or '\'
        for (int i = 0; path[i] != '\0'; i++)
        {
            if (path[i] == '/' || path[i] == '\\')
                return path[i];
        }
    }

    return psDef;
}


/**
 * @brief replace last component of path by another file name
 * @param path          path
 * @param file          other file name
 * @param ps            path separator char
 * @return              replaced file name
 */
string FileTools::replFileInPath(string *path, string *file, char ps)
{
    if (!path || path->empty()) {
        if (file)
            return string(*file);
        else
            return string();
    }

    size_t p = path->find_last_of(ps);
    if (p == string::npos) {
        string res(*path);
        res += ps;
        if (file)
            res += *file;
        return res;
    }
    else {
        string res(*path, 0, p+1);
        if (file)
            res += *file;
        return res;
    }
}


/**
 * @brief Checks whether a file exists
 * @param fileName      Name of the file
 * @return              True or false
 */
bool FileTools::exists(const string &fileName)
{
    ifstream f(fileName.c_str());
    return f.good();
}


