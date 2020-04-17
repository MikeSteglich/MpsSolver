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


#ifndef SOLVER_HH
#define SOLVER_HH


#include "../ToolsAndData/MpsSolverOptions.hh"
#include "../ToolsAndData/MpsData.hh"
#include "../Solution/Solution.hh"




#define SOLVER_UNDEFINED 99
#define SOLVER_OK 102;
#define SOLVER_FAIL 103;


#if defined(_WIN32) || defined(WIN32)
    #define POPEN(f,a) = _popen(f,a)
    #define PCLOSE(f,a) = _popen(p)
#else
    #define POPEN(f,a) = popen(f,a)
    #define PCLOSE(f,a) = popen(p)
#endif


/**
 * @brief The Solver class
 */
class Solver
{


public:
    /**
     * @brief Constructor
     * @param opts          pointer to a MpsSolverOptions object
     * @param mpsData       pointer to a MpsData object
     * @param sol           pointer to a Solution object
     */
    Solver(MpsSolverOptions *opts, MpsData *mpsData, Solution *sol);

    /**
     * @brief Destructor
     */
    ~Solver() {}


protected:

    MpsSolverOptions *_opts;            // pointer to a MpsSolverOptions object
    MpsData *_mpsData;                  // pointer to a MpsData object
    Solution *_sol;                     // pointer to a Solution object

    string _instanceBaseName;           // full file name of the problem to be solved w/o extension
    string _instanceFileName;           // full file name of the mps file to be generated and solved
    string _instanceSolName;            // full file name of the solution file to be generated
    string _instanceCmdName;            // full file name of the command file to be generated

    string _solverCmdLine;              // command line for the solver

    /**
     * @brief writes the generated Mps into the tmp folder
     */
    void writeInstanceFile();

    /**
     * @brief starts the solver as external process
     * @return          status of the solver
     */
    int execute();

    /**
     * @brief deletes all generated files in the tmp folder
     */
    void deleteTmpFiles();


    /**
     * @brief returns a value of a XML string
     * @param valString     XML string
     * @return              value of a XML string
     */
    string readXmlVal(string valString);


};


#endif // SOLVER_HH
