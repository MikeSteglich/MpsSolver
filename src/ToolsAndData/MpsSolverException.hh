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

#ifndef MpsSolverException_HH
#define MpsSolverException_HH

#include <string>
#include <exception>

using namespace std;

/**
 * @brief The MpsSolverException class
 */
class MpsSolverException : public exception
{
private:
    string _what;
    
public:
    /**
     * constructor
     * @param msg				exception message
     */
    MpsSolverException(const string msg) { _what = msg; }
    
    /**
     * destructor
     */
    virtual ~MpsSolverException() noexcept			{ }
    
    /**
     * get exception info
     */
    virtual const char *what() const noexcept	{ return _what.c_str(); }

};


#endif /* MpsSolverException_HH */
