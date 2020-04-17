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



#include "SolverCbc.hh"
#include "../Solution/Solution.hh"
#include "../ToolsAndData/MpsSolverException.hh"
#include "../ToolsAndData/StringTools.hh"

#include <iostream>
#include <fstream>
#include <cmath>


/**
 * @brief solves the problem
 */
void SolverCbc::solve() {
    deleteTmpFiles();
    generateCmdLine();
    if (execute()==0)
       readSolFile();
    deleteTmpFiles();
}


/**
 * @brief generates the the solver specific command line for the solver
 */
void SolverCbc::generateCmdLine() {
    _solverCmdLine=_opts->solverBinary()+" "+ _instanceFileName;

    for (size_t i=0; i<_opts->solverOptions().size(); i++)
        _solverCmdLine+= " " + _opts->solverOptions()[i].key + " " + _opts->solverOptions()[i].value;

    _solverCmdLine+= " " + _opts->objSense() + " " +" solve gsolu "+ _instanceSolName + " 2>&1";
}

/**
 * @brief reads the solver specific solution file
 */
void SolverCbc::readSolFile() {

    unsigned long lineNr;
    string line;

    bool varSection;
    bool conSection;
    bool headerSection;

    unsigned headerLines;

    lineNr=0;
    headerSection=true;
    conSection=false;
    varSection=false;

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

            lineNr++;

            line=StringTools::lrTrim(line);
            vector<string> solList;

            StringTools::split(line,solList);

            if (headerSection) {
                if (lineNr==1) {
                    if (_opts->solver()=="GLPK") {
                        headerLines=2;
                    } else {
                        headerLines=3;
                    }
                    continue;
                }

                if (lineNr==2) {

                    if (solList[0]=="2" || solList[0]=="5"  ) {
                        _sol->setNrOfSolutions(1);
                        _sol->setStatus("normal");
                        solution.setStatus("optimal");

                    } else if (solList[0]=="1") {
                        _sol->setNrOfSolutions(0);
                        _sol->setStatus("undefined");
                    } else {
                        _sol->setNrOfSolutions(0);
                        _sol->setStatus("infeasible");
                    }

                    if (_sol->nrOfSolutions()>0) {

                        int valPos=0;
                        if (solList.size()==3) {
                            valPos=2;
                        } else {
                            valPos=1;
                        }

                        double objVal;
                        if (!StringTools::toDouble(solList[valPos],objVal))
                            throw MpsSolverException("Internal error while reading objValue");

                        solution.setValue(objVal);

                    } else {
                        break;
                    }
                    continue;
                }

                if (lineNr>headerLines) {
                    headerSection=false;
                    varSection=false;
                    conSection=true;
                }
            }

            if ( conSection || varSection ) {

                SolutionElement solElem;
                solElem.setMarginal(0);

                double activity=0;
                double marginal=0;

                int aPos=0;
                int mPos=0;

                if (solList.size()==1) {
                    aPos=0;
                    mPos=-1;

                } else if (solList.size()==2) {
                    aPos=0;
                    mPos=1;

                } else if (solList.size()==3) {
                    aPos=1;
                    mPos=2;
                }

                if (!StringTools::toDouble(solList[aPos],activity))
                    throw MpsSolverException("Internal error while reading activity");

                if (mPos>0) {
                    if (!StringTools::toDouble(solList[mPos],marginal))
                        throw MpsSolverException("Internal error while reading mariginal");
                }


                solElem.setActivity(activity);
                solElem.setMarginal(marginal);

                if (conSection) {
                    solElem.setModelElement(_sol->modelConstraint(conIdx));
                    solution.setConstraint(solElem);
                    conIdx++;
                } else {
                    solElem.setModelElement(_sol->modelVariable(varIdx));

                    if (_sol->modelVariable(varIdx)->type()!="C") {
                        activity=round(activity);
                        solElem.setActivity(activity);
                    }

                    solution.setVariable(solElem);
                    varIdx++;
                }

                if (lineNr>headerLines+_sol->nrOfConstraints()-1) {
                    headerSection=false;
                    varSection=true;
                    conSection=false;
                }
                continue;
            }

        }
        _sol->setSolution(solution);
    }
    catch (exception& e) {
        throw MpsSolverException("Problems while reading <" + _instanceSolName + "> " + string(e.what()));
    }
}

