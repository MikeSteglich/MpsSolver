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

#ifndef SOLVERGUROBI_HH
#define SOLVERGUROBI_HH


#include "Solver.hh"
#include "../Solution/Solution.hh"

/**
 * @brief The SolverGurobi class
 */
class SolverGurobi : public Solver
{


public:

    /**
     * @brief Constructor
     * @param opts          pointer to a MpsSolverOptions object
     * @param mpsData       pointer to a MpsData object
     * @param sol           pointer to a Solution object
     */
    SolverGurobi(MpsSolverOptions *opts, MpsData *mpsData, Solution *sol) : Solver(opts,mpsData,sol) {}

    /**
     * @brief Destructor
     */
    ~SolverGurobi() {}

    /**
     * @brief solves the problem
     */
    void solve();

private:

    /**
     * @brief generates the the solver specific command line for the solver
     */
    void generateCmdLine();

    /**
     * @brief reads the solver specific solution file
     */
    void readSolFile();

};



#endif // SOLVERGUROBI_HH
