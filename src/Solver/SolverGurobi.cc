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


#include "SolverGurobi.hh"


#include "../Solution/Solution.hh"
#include "../ToolsAndData/MpsSolverException.hh"
#include "../ToolsAndData/StringTools.hh"

#include <iostream>
#include <fstream>
#include <cmath>


/**
 * @brief solves the problem
 */
void SolverGurobi::solve() {
    deleteTmpFiles();
    generateCmdLine();
    if (execute()==0)
       readSolFile();
    deleteTmpFiles();
}


/**
 * @brief generates the the solver specific command line for the solver
 */
void SolverGurobi::generateCmdLine() {

    string solPool = ( _opts->solutionPool() ? "1" :"0");

    _solverCmdLine=_opts->solverBinary()+" "+solPool+" " + StringTools::replaceString(_instanceFileName," ","%") + " " + StringTools::replaceString(_instanceSolName," ","%");

    for (size_t i=0; i<_opts->solverOptions().size(); i++)
        _solverCmdLine+= " " + _opts->solverOptions()[i].key + "=" + _opts->solverOptions()[i].value;

    _solverCmdLine+= " 2>&1";
}

/**
 * @brief reads the solver specific solution file
 */
void SolverGurobi::readSolFile() {

    string line;

    unsigned long varIdx;
    unsigned long conIdx;

    unsigned long lineNr=0;
    bool headerSection=false;
    bool conSection=false;
    bool varSection=false;

    bool continueReading=true;

    int nrOfSolutions=0;

    if (!_opts->isSilent()) {
        cout  <<  "Reading solution" << endl;
    }


    SingleSolution* solution;

    try {

        ifstream  solFile( _instanceSolName, ifstream::binary) ;

        if (!solFile.is_open())
            throw MpsSolverException("Cannot read solution file <"+_instanceSolName+">");

        while ( getline( solFile, line) && continueReading ) {

            line=StringTools::lrTrim(line);

            lineNr++;
            if (lineNr==1) {
                if (!StringTools::startsWith(line,"<?xml"))
                    throw MpsSolverException("Wrong file type for file  <"+_instanceSolName+">");
                continue;
            }

            if (lineNr==2) {
                if ( !StringTools::startsWith(line,"<CmplGurobiSolutions"))
                    throw MpsSolverException("Wrong file type for file  <"+_instanceSolName+">");
                continue;
            }

            if (StringTools::startsWith(line,"<solution")) {
                solution = new SingleSolution(_opts,_mpsData);

                varIdx=0;
                conIdx=0;

                headerSection=false;
                conSection=false;
                varSection=false;

                continue;
            }

            if (StringTools::startsWith(line,"</solution") ) {

                if(solution->constraints()->size()==0) {
                    vector<double> vars;
                    for (auto& var: *solution->variables())
                        vars.push_back(var.activity());

                    unsigned long idx=0;

                    for(unsigned long i=0; i<_mpsData->nrOfCons(); i++) {
                        SolutionElement solElem;
                        solElem.setModelElement(_sol->modelConstraint(idx));
                        double conAct = solution->calculateConActivity(&solElem,_sol );
                        solElem.setActivity(conAct);
                        solution->setConstraint(solElem);
                        idx++;
                    }
                }

                _sol->setSolution(*solution);
                delete solution;

                headerSection=false;
                conSection=false;
                varSection=false;
            }

            if (StringTools::startsWith(line,"<header")) {
                headerSection=true;
                conSection=false;
                varSection=false;
            }

            if (StringTools::startsWith(line,"<constraints")) {
                headerSection=false;
                conSection=true;
                varSection=false;
                continue;
            }

            if (StringTools::startsWith(line,"</constraints")) {
                headerSection=false;
                conSection=false;
                varSection=false;
                continue;
            }

            if (StringTools::startsWith(line,"<variables")) {
                headerSection=false;
                conSection=false;
                varSection=true;
                continue;
            }

            if (StringTools::startsWith(line,"</variables")) {
                headerSection=false;
                conSection=false;
                varSection=false;
                continue;
            }

            if (headerSection) {
                vector<string> varXmlVals;
                StringTools::split(line,varXmlVals);

                string valStr=readXmlVal(varXmlVals[1]);
                double objVal;
                if (!StringTools::toDouble(valStr,objVal))
                    throw MpsSolverException("Internal error while reading objective function value from Gurobi solution file");

                nrOfSolutions++;
                solution->setValue(objVal);

                string objStatus=StringTools::replaceString(readXmlVal(varXmlVals[2]),"_"," ");
                solution->setStatus(objStatus);

                continue;
            }

            if (varSection || conSection) {

                SolutionElement solElem;
                solElem.setMarginal(0);

                double activity=0;
                double marginal=0;

                vector<string> varXmlVals;
                StringTools::split(line,varXmlVals);

                string valStr=readXmlVal(varXmlVals[2]);
                if (!StringTools::toDouble(valStr,activity))
                    throw MpsSolverException("Internal error while reading activity from Gurobi solution file");
                solElem.setActivity(activity);

                if (!_mpsData->hasInteger() ) {
                    string valStr=readXmlVal(varXmlVals[3]);
                    if (!StringTools::toDouble(valStr,marginal))
                        throw MpsSolverException("Internal error while reading marginal from Gurobi solution file");
                    solElem.setMarginal(marginal);
                }

                if (varSection) {
                    solElem.setModelElement(_sol->modelVariable(varIdx));
                    if (_sol->modelVariable(varIdx)->type()!="C") {
                        activity=round(activity);
                        solElem.setActivity(activity);
                    }
                    solution->setVariable(solElem);
                    varIdx++;
                } else {
                    solElem.setModelElement(_sol->modelConstraint(conIdx));
                    solution->setConstraint(solElem);
                    conIdx++;

                }
                continue;
            }
        }

    } catch (exception& e) {
        throw MpsSolverException("Problems while reading <" + _instanceSolName + "> " + string(e.what()));
    }

    _sol->setStatus("normal");
    _sol->setNrOfSolutions(nrOfSolutions);
}


