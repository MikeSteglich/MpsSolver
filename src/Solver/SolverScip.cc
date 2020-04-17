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

#include "SolverScip.hh"
#include "../Solution/Solution.hh"
#include "../ToolsAndData/MpsSolverException.hh"
#include "../ToolsAndData/StringTools.hh"

#include <iostream>
#include <fstream>
#include <cmath>


/**
 * @brief solves the problem
 */
void SolverScip::solve() {
    deleteTmpFiles();
    generateCmdLine();

    if (!_mpsData->hasInteger()){
        cout << endl << "Warning: Scip runs only (M)ILP problems, marginals are not available in solution" << endl;
    }
    if (execute()==0)
       readSolFile();
    deleteTmpFiles();
}


/**
 * @brief writes the Scip option file
 */
void SolverScip::writeOptFile(){

    try {
        ofstream  optFile( _instanceCmdName.c_str() ) ;

        for (size_t i=0; i<_opts->solverOptions().size(); i++) {
            optFile << _opts->solverOptions()[i].key << " = " << _opts->solverOptions()[i].value << endl;
        }
        optFile.close();
    }
    catch (exception& ex) {
        throw MpsSolverException("Internal error while writing scip option file <"+_instanceCmdName+"> :"+ex.what());
    }
}

/**
 * @brief generates the the solver specific command line for the solver
 */
void SolverScip::generateCmdLine() {
    writeOptFile();
    _solverCmdLine=_opts->solverBinary()+" -c \"set load " + _instanceCmdName + "\" ";
    _solverCmdLine+=" -c \"read "+_instanceFileName+"\"";
    _solverCmdLine+=" -c optimize";
    _solverCmdLine+=" -c \"write solu "+_instanceSolName+"\"";
    _solverCmdLine+=" -c quit  2>&1";
}

/**
 * @brief reads the solver specific solution file
 */
void SolverScip::readSolFile() {

    int nrOfSolutions=0;
    unsigned long lineNr=0;

    string line;

    if (!_opts->isSilent()) {
        cout  <<  "Reading solution" << endl;
    }

    _sol->setStatus("normal");
    _sol->setNrOfSolutions(nrOfSolutions);

    SingleSolution solution = SingleSolution(_opts,_mpsData);

    for (unsigned long j=0; j<_sol->nrOfVariables(); j++) {
        SolutionElement solElem;
        solElem.setModelElement(_sol->modelVariable(j));
        solution.setVariable(solElem);
    }

    try {
        ifstream  solFile( _instanceSolName, ifstream::binary) ;

        if (!solFile.is_open())
            throw MpsSolverException("Cannot read solution file <"+_instanceSolName+">");

        while ( getline( solFile, line)  ) {

            line=StringTools::lrTrim(line);
            vector<string> solList;

            lineNr++;
            if (lineNr==1) {
                StringTools::split(line,solList,":");
                solution.setStatus(solList[1]);
                if (  StringTools::contains(StringTools::lrTrim(solList[1]),"optimal"  )) {
                    nrOfSolutions=1;
                } else {
                    nrOfSolutions=0;
                    break;
                }
                continue;
            }

            if (lineNr==2) {
                StringTools::split(line,solList, ":");
                double objVal;
                if (!StringTools::toDouble(solList[1],objVal))
                    throw MpsSolverException("Internal error while reading obValue");
                solution.setValue(objVal);
                continue;
            }

            if (lineNr>2) {
                StringTools::split(line,solList);
                string varName=StringTools::lrTrim(solList[0]);
                double activity;
                if (!StringTools::toDouble(solList[1],activity))
                    throw MpsSolverException("Internal error while reading activity");

                unsigned long idx=_sol->varIdxByName(varName);
                if (_sol->modelVariable(idx)->type()!="C") {
                    activity=round(activity);
                 }
                solution.variable(idx)->setActivity(activity);
            }
        }

        unsigned long idx=0;

        for(unsigned long i=0; i<_mpsData->nrOfCons(); i++) {
            SolutionElement solElem;
            solElem.setModelElement(_sol->modelConstraint(idx));
            double conAct = solution.calculateConActivity(&solElem,_sol );
            solElem.setActivity(conAct);
            solution.setConstraint(solElem);
            idx++;
        }

        _sol->setSolution(solution);

    } catch (exception& e) {
        throw MpsSolverException("Problems while reading <" + _instanceSolName + "> " + string(e.what()));
    }

    _sol->setStatus("normal");
    _sol->setNrOfSolutions(nrOfSolutions);
}



