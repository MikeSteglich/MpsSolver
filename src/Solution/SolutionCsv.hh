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


#ifndef SOLUTIONCSV_HH
#define SOLUTIONCSV_HH

#include <string>
#include <iostream>

#include "../ToolsAndData/MpsSolverOptions.hh"
#include "../ToolsAndData/MpsSolverException.hh"

#include "Solution.hh"


/**
 * @brief The SolutionCsv class
 */
class SolutionCsv
{


public:

    /**
     * @brief Constructor
     * @param opts      pointer to MpsSolverOptions object
     * @param sol       pointer to Solution object
     */
    SolutionCsv(MpsSolverOptions *_opts, Solution *sol);


    /**
      * Destructor
      */
    ~SolutionCsv() {}

    /**
     * @brief excutes the solutionReport
     */
    void solutionReport();

protected:


private:

    MpsSolverOptions  *_opts;       // pointer to MpsSolverOptions object
    Solution *_sol;                 // pointer to Solution object

    ostream *_ostr;                 // pointer to ostream object to write the solution(s)

    /**
     * @brief writes the solution(s) into a file or stdOut in Ascii format
     * @param ostr       pointer to ostream object to write the solution(s)
     */
    void writeSolReport( ostream& ostr);

    /**
     * @brief writes the solution of a variable into a file or stdOut in Ascii format
     * @param i          number of the solution
     * @param j          index of the variable
     * @param ostr       pointer to ostream object to write the solution(s)
     */
    void writeVarValues(unsigned long i, unsigned long j , ostream& ostr);

    /**
     * @brief writes the solution of a constraint into a file or stdOut in Ascii format
     * @param i          number of the solution
     * @param j          index of the constraint
     * @param ostr       pointer to ostream object to write the solution(s)
     */
    void writeConValues(unsigned long i, unsigned long j , ostream& ostr);
};


#endif // SOLUTIONCSV_HH
