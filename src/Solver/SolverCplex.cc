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


#include "SolverCplex.hh"

#include "../Solution/Solution.hh"
#include "../ToolsAndData/MpsSolverException.hh"
#include "../ToolsAndData/StringTools.hh"

#include <iostream>
#include <fstream>
#include <cmath>



/**
 * @brief solves the problem
 */
void SolverCplex::solve() {
    deleteTmpFiles();
    generateCmdLine();
    if (execute()==0)
       readSolFile();
    deleteTmpFiles();
}


/**
 * @brief writes the Cplex command file
 */
void SolverCplex::writeCmdFile( ){

    try {
        ofstream  cmdFile( _instanceCmdName.c_str() ) ;

        cmdFile <<  " read " << _instanceFileName << endl;

        for (size_t i=0; i<_opts->solverOptions().size(); i++) {
            cmdFile << "set ";
            cmdFile << StringTools::replaceString(_opts->solverOptions()[i].key,"/"," ");
            cmdFile <<  " " + _opts->solverOptions()[i].value << endl;
        }

        cmdFile << ( _mpsData->hasInteger() && !_opts->intRelax() ? "mipopt" : "opt") << endl;

        cmdFile << "write " << _instanceSolName   ;
        cmdFile << ( _opts->solutionPool() ? " all" :"");
        cmdFile << endl;

        cmdFile.close();
    }
    catch (exception& ex) {
        throw MpsSolverException("Internal error while writing Cplex cmd file <"+_instanceCmdName+"> :"+ex.what());
    }
}

/**
 * @brief generates the the solver specific command line for the solver
 */
void SolverCplex::generateCmdLine() {

    writeCmdFile();
    _solverCmdLine=_opts->solverBinary()+" -f " + _instanceCmdName + " 2>&1";
}


/**
 * @brief reads the solver specific solution file
 */
void SolverCplex::readSolFile() {

    string line;

    double inf = std::numeric_limits<double>::infinity();

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

            if (lineNr==2 && !StringTools::startsWith(line,"<CPLEXSolution"))
                throw MpsSolverException("Wrong file type for file  <"+_instanceSolName+">");

            if (StringTools::startsWith(line,"<CPLEXSolutions"))
                continue;

            if (StringTools::startsWith(line,"<CPLEXSolution")) {

                solution = new SingleSolution(_opts,_mpsData);

                varIdx=0;
                conIdx=0;

                headerSection=false;
                conSection=false;
                varSection=false;

                continue;
            }

            if (StringTools::startsWith(line,"</CPLEXSolution") && !StringTools::startsWith(line,"</CPLEXSolutions")) {
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
                continue;
            }

            if (StringTools::startsWith(line,"<linearConstraints")) {

                headerSection=false;
                conSection=true;
                varSection=false;
                continue;
            }

            if (StringTools::startsWith(line,"</linearConstraints")) {

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

                if (StringTools::startsWith(line, "objectiveValue")) {
                    string objValStr=readXmlVal(line);
                    double objVal;
                    if (!StringTools::toDouble(objValStr,objVal))
                        throw MpsSolverException("Internal error while reading objective function value from Cplex solution file");

                    nrOfSolutions++;
                    solution->setValue(objVal);
                    continue;
                }

                if (StringTools::startsWith(line, "solutionStatusString")) {
                    string objStatus=readXmlVal(line);
                    solution->setStatus(objStatus);

                    if (StringTools::contains(objStatus,"infeasible") ||  StringTools::contains(objStatus,"undefined")) {
                        _sol->setNrOfSolutions(0);
                        continueReading=false;
                        continue;
                    }
                    continue;
                }
            }

            if (varSection) {

                SolutionElement solElem;
                solElem.setMarginal(0);

                double activity=0;
                double marginal=0;

                int actPos=-1;
                int margPos=-1;

                vector<string> varXmlVals;
                StringTools::split(line,varXmlVals);

                for (unsigned i=0; i<varXmlVals.size();i++) {
                    if (StringTools::startsWith(varXmlVals[i],"value"))
                        actPos=i;
                    if (StringTools::startsWith(varXmlVals[i],"reducedCost"))
                        margPos=i;
                }

                string valStr=readXmlVal(varXmlVals[actPos]);
                if (!StringTools::toDouble(valStr,activity))
                    throw MpsSolverException("Internal error while reading activity from Cplex solution file");
                solElem.setActivity(activity);

                if (!_mpsData->hasInteger() || margPos>-1) {
                    string valStr=readXmlVal(varXmlVals[margPos]);
                    if (!StringTools::toDouble(valStr,marginal))
                        throw MpsSolverException("Internal error while reading marginal from Cplex solution file");
                    solElem.setMarginal(marginal);
                }

                solElem.setModelElement(_sol->modelVariable(varIdx));
                if (_sol->modelVariable(varIdx)->type()!="C") {
                    activity=round(activity);
                    solElem.setActivity(activity);
                }

                solution->setVariable(solElem);
                varIdx++;

                continue;
            }

            if (conSection) {

                SolutionElement solElem;
                solElem.setMarginal(0);
                solElem.setModelElement(_sol->modelConstraint(conIdx));

                double activity=0;
                double slack=0;
                double marginal=0;

                int actPos=-1;
                int margPos=-1;

                vector<string> conXmlVals;
                StringTools::split(line,conXmlVals);

                for (unsigned i=0; i<conXmlVals.size();i++) {
                    if (StringTools::startsWith(conXmlVals[i],"slack"))
                        actPos=i;
                    if (StringTools::startsWith(conXmlVals[i],"dual"))
                        margPos=i;
                }

                string valStr=readXmlVal(conXmlVals[actPos]);
                if (!StringTools::toDouble(valStr,slack))
                    throw MpsSolverException("Internal error while reading activity from Cplex solution file");

                if (slack>0) {
                    activity=solElem.upperBound()-slack;
                } else if (slack<0) {
                    if (solElem.type()=="N")
                        activity=slack * -1;
                    else
                        activity=solElem.lowerBound()+slack * -1;
                } else {
                    if (solElem.upperBound()!=inf) {
                        activity=solElem.upperBound();
                    } else  if (solElem.lowerBound()!=-inf) {
                        activity=solElem.lowerBound();
                    }

                }

                solElem.setActivity(activity);

                if (!_mpsData->hasInteger()|| margPos>-1) {
                    string valStr=readXmlVal(conXmlVals[margPos]);
                    if (!StringTools::toDouble(valStr,marginal))
                        throw MpsSolverException("Internal error while reading marginal from Cplex solution file");
                    solElem.setMarginal(marginal);
                }

                solution->setConstraint(solElem);
                conIdx++;
                continue;
            }
        }
    } catch (exception& e) {
        throw MpsSolverException("Problems while reading <" + _instanceSolName + "> " + string(e.what()));
    }
    _sol->setStatus("normal");
    _sol->setNrOfSolutions(nrOfSolutions);
}


