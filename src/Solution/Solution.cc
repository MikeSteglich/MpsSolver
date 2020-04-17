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


#include "Solution.hh"
#include "../ToolsAndData/StringTools.hh"
#include "../ToolsAndData/MpsSolverException.hh"


using namespace std;





/**
 * @brief  constructor
 */
SolutionElement::SolutionElement() {
    _activity=0;
    _marginal=0;
    _modElement=NULL;
}


/**
 * @brief Constructor
 * @param opts          pointer to a MpsSolverOptions object
 * @param mpsData       pointer to a MpsData object
 */
SingleSolution::SingleSolution(MpsSolverOptions *opts, MpsData *mpsData) {
    _status="";
    _value=0;
    _opts=opts;
    _mpsData=mpsData;
}


/**
 * @brief calculates the activity of a constraint (used for Gurobi and Scip)
 * @param con       pointer to an object of a constraint
 * @param sols      pointer to an object of a single solution
 * @return          activity of the constraint
 */
double SingleSolution::calculateConActivity(SolutionElement* con, Solution* sols)  {

    unsigned long idx;

    string row=con->name();
    string var="";

    if (row.empty())
        throw MpsSolverException("Internal error while calculating constraint activity: Name of the constraint unknown.");

    auto itc=_mpsData->coeffs()->find(row)->second.begin();
    double activity=0;
    double coeff=0;

    while(itc!=_mpsData->coeffs()->find(row)->second.end()) {
        var=itc->first;
        coeff=itc->second;
        idx=sols->varIdxByName(var);
        activity+= coeff*_variables[idx].activity();
        itc++;
    }

    return activity;
}

/**
 * @brief constructor
 * @param opts          pointer to a MpsSolverOptions object
 * @param mpsData       pointer to a MpsData object
 */
Solution::Solution(MpsSolverOptions *opts, MpsData *mpsData) {

    _nrOfSolutions=0;
    _opts=opts;
    _mpsData=mpsData;


    _modConstraints=_mpsData->constraints();

    unsigned long conIdx=0;
    for (ModelElement* me: _modConstraints) {
        _rowNameMap[me->name() ] = conIdx;
        conIdx++;
    }

    _modVariables=_mpsData->variables();
    unsigned long varIdx=0;
    for (ModelElement* me: _modVariables) {
        _colNameMap[me->name() ] = varIdx;
        varIdx++;
    }

}








