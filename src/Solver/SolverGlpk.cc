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


#include "SolverGlpk.hh"
#include "../Solution/Solution.hh"
#include "../ToolsAndData/MpsSolverException.hh"
#include "../ToolsAndData/StringTools.hh"

#include <iostream>
#include <fstream>
#include <cmath>

/**
 * @brief solves the problem
 */
void SolverGlpk::solve() {
    deleteTmpFiles();
    generateCmdLine();
    if (execute()==0)
       readSolFile();
    deleteTmpFiles();
}

/**
 * @brief generates the the solver specific command line for the solver
 */
void SolverGlpk::generateCmdLine() {

    _solverCmdLine=_opts->solverBinary()+" --freemps " + " --"+StringTools::lowerCase(_opts->objSense())+ " --write "+_instanceSolName;

    for (size_t i=0; i<_opts->solverOptions().size(); i++)
            _solverCmdLine+= " --" + _opts->solverOptions()[i].key + " " + _opts->solverOptions()[i].value;

    _solverCmdLine+=  " " + _instanceFileName + " 2>&1";
}

/**
 * @brief reads the solver specific solution file
 */
void SolverGlpk::readSolFile() {

    string line;

    unsigned long varIdx=0;
    unsigned long conIdx=0;

    if (!_opts->isSilent()) {
        cout  <<  "Reading solution" << endl;
    }

    try {

        ifstream  solFile( _instanceSolName, ifstream::binary) ;

        if (!solFile.is_open())
            throw MpsSolverException("Cannot read solution file <"+_instanceSolName+">");

        SingleSolution solution = SingleSolution(_opts,_mpsData);

        while ( getline( solFile, line) ) {

            line=StringTools::lrTrim(line);

            //comment or eof
            if ( StringTools::startsWith(line,"c") || StringTools::startsWith(line,"e") )
                continue;

            vector<string> solList;
            StringTools::split(line,solList);

            // obj function
            if ( StringTools::startsWith(line,"s") ) {

                if ( StringTools::lrTrim(solList[4])=="u") {
                    _sol->setNrOfSolutions(0);
                    _sol->setStatus("undefined");
                } else if ( StringTools::lrTrim(solList[4])=="f" ||  StringTools::lrTrim(solList[4])=="o"  ) {

                    _sol->setNrOfSolutions(1);
                    _sol->setStatus("normal");

                    if ( StringTools::lrTrim(solList[4])=="f" ) {
                        if ( StringTools::lrTrim(solList[1])=="bas")
                            solution.setStatus("optimal");
                        else if ( StringTools::lrTrim(solList[1])=="mip")
                            solution.setStatus("feasible");
                    }

                    if ( StringTools::lrTrim(solList[4])=="o" )
                        solution.setStatus("optimal");

                    int vPos;
                    vPos = solList.size() -1;

                    double objVal;
                    if (!StringTools::toDouble(solList[vPos],objVal))
                         throw MpsSolverException("Internal error while reading objValue");
                    solution.setValue(objVal);

                } else if ( StringTools::lrTrim(solList[4])=="i") {
                    _sol->setNrOfSolutions(0);
                    _sol->setStatus("infeasible");
                } else if ( StringTools::lrTrim(solList[4])=="n") {
                    _sol->setNrOfSolutions(0);
                    _sol->setStatus("no feasible solution");
                }

                continue;
            }

            if ( StringTools::startsWith(line,"i") || StringTools::startsWith(line,"j") ) {

                // i rows - j columns

                SolutionElement solElem;
                solElem.setMarginal(0);

                double activity=0;
                double marginal=0;

                int aPos=0;
                int mPos=0;

                if (solList.size()==3) {
                    //mip
                    aPos=2;
                    mPos=-1;
                } else if (solList.size()==4) {
                    //interior-point
                    aPos=2;
                    mPos=3;
                } else if (solList.size()==5) {
                    //LP
                    aPos=3;
                    mPos=4;
                }

                if (!StringTools::toDouble(solList[aPos],activity))
                     throw MpsSolverException("Internal error while reading activity");

                if (mPos>0) {
                    if (!StringTools::toDouble(solList[mPos],marginal))
                         throw MpsSolverException("Internal error while reading marginal");
                }

                solElem.setActivity(activity);
                solElem.setMarginal(marginal);

                if ( StringTools::startsWith(line,"j") ){
                    solElem.setModelElement(_sol->modelVariable(varIdx));
                    if (_sol->modelVariable(varIdx)->type()!="C") {
                        activity=round(activity);
                        solElem.setActivity(activity);
                    }
                    solution.setVariable(solElem);
                    varIdx++;
                }

                if ( StringTools::startsWith(line,"i") ) {
                    solElem.setModelElement(_sol->modelConstraint(conIdx));
                    solution.setConstraint(solElem);
                    conIdx++;

                }
            }
        }
        _sol->setSolution(solution);
    }
    catch (exception& e) {
        throw MpsSolverException("Problems while reading <" + _instanceSolName + "> " + string(e.what()));
    }
}


