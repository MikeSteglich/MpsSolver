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


#include "SolutionXml.hh"
#include "../ToolsAndData/StringTools.hh"


#include <iomanip>
#include <fstream>
#include <cmath>


/**
 * @brief Constructor
 * @param opts      pointer to MpsSolverOptions object
 * @param sol       pointer to Solution object
 */
SolutionXml::SolutionXml(MpsSolverOptions *opts,  Solution *sol)
{
    _opts=opts;
    _sol = sol;
}

/**
 * @brief excutes the solutionReport
 */
void SolutionXml::solutionReport() {

    if (_sol) {
        try {

            if (_opts->solutionXml()) {
                if (!_opts->isSilent()) {
                    cout << "Writing solution to Xml file <"<< _opts->solutionXmlFile() << "> " << endl;
                }
                _ostr = new ofstream(_opts->solutionXmlFile());
            }
            writeSolReport(*_ostr);
        }

        catch (exception& e) {
            throw MpsSolverException("Problems while writing sol file <" + _opts->solutionXmlFile() + "> " + string(e.what()));
        }
    }
}

/**
 * @brief writes the solution(s) into a file or stdOut in Ascii format
 * @param ostr       pointer to ostream object to write the solution(s)
 */
void SolutionXml::writeSolReport( ostream& ostr) {

    ostr << "<?xml version = \"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl;
    ostr << "<CmplSolutions version=\"1.1\">" << endl;
    ostr << "   <general>" << endl;
    ostr << "       <instanceName>" <<  _sol->problemName()  << "</instanceName>" << endl;
    ostr << "       <nrOfVariables>" << _sol->nrOfVariables() << "</nrOfVariables>" << endl;
    ostr << "       <nrOfConstraints>" << _sol->nrOfConstraints() << "</nrOfConstraints>" << endl;
    ostr << "       <objectiveName>" << _sol->objName() << "</objectiveName>" << endl;
    ostr << "       <objectiveSense>" << _sol->objSense() << "</objectiveSense>" << endl;
    ostr << "       <nrOfSolutions>" << _sol->nrOfSolutions()<< "</nrOfSolutions>" <<  endl;

    ostr << "       <solverName>" << _sol->solver() << "</solverName>" << endl;

    ostr << "       <solverMsg>" <<  _sol->status() << "</solverMsg>" << endl;

    ostr << "       <variablesDisplayOptions>" ;

    if (_opts->displayIgnoreVars())
        ostr << "ignoreAllVariables ";
    else {
        if (_opts->displayIgnoreZeros())
            ostr << "nonZeroVariables ";

        ostr << "(";
        if(_opts->displayVarPatterns().size()==0)
            ostr << "all";
        else {
            for (int j=0; j<_opts->displayVarPatterns().size(); j++){
                if (j>0) ostr << ",";
                ostr << _opts->displayVarPatterns()[j];
            }
        }

        ostr << ")";
    }
    ostr  << "</variablesDisplayOptions>" << endl;

    ostr << "       <constraintsDisplayOptions>" ;
    if (_opts->displayIgnoreCons())
        ostr << "ignoreAllConstraints ";
    else {
        if (_opts->displayIgnoreZeros())
            ostr << "nonZeroConstraints ";
        ostr << "(";
        if(_opts->displayConPatterns().size()==0)
            ostr << "all";
        else {
            for (int j=0; j<_opts->displayConPatterns().size(); j++) {
                if (j>0) ostr << ",";
                ostr << _opts->displayConPatterns()[j];
            }
        }

        ostr << ")";
    }
    ostr << "</constraintsDisplayOptions>" <<endl;

    ostr << "   </general>" << endl;

    for (int i=0; i<_sol->nrOfSolutions(); i++) {

        ostr << "   <solution idx=\""<< i << "\" status=\""<<  _sol->solution(i)->status() << "\" value=\"" <<  _sol->solution(i)->value() << "\">" << endl;


        if (!_opts->displayIgnoreVars()) {

            ostr << "       <variables>"<< endl;

            if (_opts->displayVarPatterns().size()==0) {
                for (unsigned long j=0; j<_sol->nrOfVariables(); j++ ) {
                    if (!(_sol->solution(i)->variable(j)->activity()==0.0 && _opts->displayIgnoreZeros()) ) {
                        writeVarValues( i,j, ostr) ;
                    }
                }
            } else {
                for (int k=0; k<_opts->displayVarPatterns().size(); k++) {
                    int starPos=_opts->displayVarPatterns()[k].find_first_of("*");
                    string tmpStr=_opts->displayVarPatterns()[k].substr(0,starPos);

                    for (unsigned long j=0; j<_sol->nrOfVariables(); j++ ) {
                        if ( (starPos>-1 && StringTools::startsWith(  _sol->solution(i)->variable(j)->name() ,tmpStr) ) ||
                             (starPos==-1 &&  _sol->solution(i)->variable(j)->name()==tmpStr) ) {
                            if (!(_sol->solution(i)->variable(j)->activity()==0.0 && _opts->displayIgnoreZeros()) ) {
                                writeVarValues( i,j, ostr) ;
                            }
                        }
                    }
                }
            }
            ostr << "       </variables>" << endl;
        }

        if (!_opts->displayIgnoreCons()) {
            ostr << "       <linearConstraints> " << endl;

            if (_opts->displayConPatterns().size()==0) {
                for (unsigned long j=0; j<_sol->nrOfConstraints(); j++ ) {
                    if ( !(_sol->solution(i)->constraint(j)->activity()==0.0 && _opts->displayIgnoreZeros()))  {
                        writeConValues(i,j, ostr) ;
                    }
                }
            } else {
                for (int k=0; k<_opts->displayConPatterns().size(); k++) {
                    int starPos=_opts->displayConPatterns()[k].find_first_of("*");
                    string tmpStr=_opts->displayConPatterns()[k].substr(0,starPos);

                    for (unsigned long j=0; j<_sol->nrOfConstraints(); j++ ) {
                        if ( (starPos>-1 && StringTools::startsWith(  _sol->solution(i)->constraint(j)->name() ,tmpStr) ) ||
                             (starPos==-1 &&  _sol->solution(i)->constraint(j)->name()==tmpStr) ) {
                            if (!(_sol->solution(i)->constraint(j)->activity()==0.0 && _opts->displayIgnoreZeros()) ) {
                                writeConValues( i,j, ostr) ;
                            }
                        }
                    }
                }
            }
            ostr << "       </linearConstraints>" << endl;
        }
        ostr << "   </solution>"<< endl;
    }
    ostr << "</CmplSolutions>"<< endl;
}

/**
 * @brief writes the solution of a variable into a file or stdOut in Ascii format
 * @param i          number of the solution
 * @param j          index of the variable
 * @param ostr       pointer to ostream object to write the solution(s)
 */
void SolutionXml::writeVarValues(unsigned long i, unsigned long j , ostream& ostr) {

    ostr << "           <variable idx=\""<< j << "\" name=\""<< _sol->solution(i)->variable(j)->name() <<
            "\" type=\"" << _sol->solution(i)->variable(j)->type() <<
            "\" activity=\"" << ( _sol->solution(i)->variable(j)->type()=="C" ? _sol->solution(i)->variable(j)->activity() : round(_sol->solution(i)->variable(j)->activity() )) << "\"";

    ostr <<  " lowerBound=\"" << _sol->solution(i)->variable(j)->lowerBound() ;
    ostr << "\" upperBound=\"" << _sol->solution(i)->variable(j)->upperBound() ;

    ostr << "\" marginal=\"" ;
    if (_sol->hasMarginal())
        ostr <<  right << _sol->solution(i)->variable(j)->marginal()  ;
    else
        ostr  << "NaN" ;
    ostr  << "\"/>" << endl;

}

/**
 * @brief writes the solution of a constraint into a file or stdOut in Ascii format
 * @param i          number of the solution
 * @param j          index of the constraint
 * @param ostr       pointer to ostream object to write the solution(s)
 */
void SolutionXml::writeConValues( unsigned long i, unsigned long j , ostream& ostr) {

    ostr << "           <constraint idx=\""<< j << "\" name=\""<< _sol->solution(i)->constraint(j)->name() <<
            "\" type=\"" << _sol->solution(i)->constraint(j)->type()  <<
            "\" activity=\"" << _sol->solution(i)->constraint(j)->activity() << "\"";

    ostr <<  " lowerBound=\"" << _sol->solution(i)->constraint(j)->lowerBound();
    ostr << "\" upperBound=\"" << _sol->solution(i)->constraint(j)->upperBound();

    ostr << "\" marginal=\"";

    if (_sol->hasMarginal())
        ostr << _sol->solution(i)->constraint(j)->marginal() ;
    else
        ostr  << "NaN" ;
    ostr  << "\"/>" << endl;

}






