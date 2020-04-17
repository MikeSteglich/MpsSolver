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


#include "SolutionAscii.hh"
#include "../ToolsAndData/StringTools.hh"

#include <iomanip>
#include <fstream>
#include <cmath>


/**
 * @brief Constructor
 * @param opts     pointer to MpsSolverOptions object
 * @param sol       pointer to Solution object
 */
SolutionAscii::SolutionAscii(MpsSolverOptions *opts,  Solution *sol)
{
    _opts = opts;
    _sol = sol;
}

/**
 * @brief excutes the solutionReport
 */
void SolutionAscii::solutionReport() {

    if (_sol) {
        try {

            if (!(_opts->solutionAscii() || _opts->solutionCsv() || _opts->solutionXml())) {
                if (!_opts->isSilent()) {
                    cout << "Reporting solution" << endl;
                }
                _ostr = &cout;
            }

            if (_opts->solutionAscii()) {
                if (!_opts->isSilent()) {
                    cout << "Writing solution to Ascii file <"<< _opts->solutionAsciiFile() << ">" << endl;
                }
                _ostr = new ofstream(_opts->solutionAsciiFile());
            }
            writeSolReport(*_ostr);
        }

        catch (exception& e) {
            throw MpsSolverException("Problems while writing sol file <" + _opts->solutionAsciiFile() + "> " + string(e.what()));
        }
    }
}


/**
 * @brief writes the solution(s) into a file or stdOut in Ascii format
 * @param ostr       pointer to ostream object to write the solution(s)
 */
void SolutionAscii::writeSolReport(ostream& ostr) {

    ostr << setw(105) << setfill('-') << "" <<endl;
    ostr <<  setfill(' ');

    ostr << setw(21) << left << "Problem  " <<  _sol->problemName() << endl;

    ostr << setw(21) << left <<  "Nr. of variables" << _sol->nrOfVariables()<< endl;
    ostr << setw(21) << left <<  "Nr. of constraints" << _sol->nrOfConstraints() << endl;
    ostr << setw(21) << left <<  "Objective name   " << _sol->objName() << endl;
    if (_sol->nrOfSolutions()>1) {
        ostr << setw(21) << left <<  "Nr. of solutions " << _sol->nrOfSolutions() << endl;
    }
    ostr << setw(21) << left << "Solver name" << _sol->solver() << endl;

    ostr << setw(21) << left << "Display variables" ;

    if (_opts->displayIgnoreVars())
        ostr << "ignore all variables ";
    else {

        if (_opts->displayIgnoreZeros())
            ostr << "nonzero variables ";

        ostr << "(";
        if (_opts->displayVarPatterns().size()==0)
            ostr << "all";
        else {
            for (int j=0; j<_opts->displayVarPatterns().size(); j++) {
                if (j>0) ostr << ",";
                ostr << _opts->displayVarPatterns()[j];
            }
        }
        ostr << ")";
    }
    ostr << endl;

    ostr << setw(21) << left << "Display constraints" ;

    if (_opts->displayIgnoreCons())
        ostr << "ignore all constraints ";
    else {

        if (_opts->displayIgnoreZeros())
            ostr << "nonzero constraints ";

        ostr << "(";
        if (_opts->displayConPatterns().size()==0)
            ostr << "all";
        else {
            for (int j=0; j<_opts->displayConPatterns().size(); j++) {
                if (j>0) ostr << ",";
                ostr << _opts->displayConPatterns()[j];
            }
        }
        ostr << ")";
    }

    ostr << endl;

    ostr << setw(105) << setfill('-') << "" <<endl;
    ostr <<  setfill(' ');

    if (_sol->nrOfSolutions()==0) {
        ostr << endl;
        ostr << setw(21) << left <<  "No solution has been found " << endl << endl;
        ostr << setw(105) << setfill('-') << "" <<endl;
    }

    for (int i=0; i<_sol->nrOfSolutions(); i++) {

        ostr << endl;

        ostr <<  setfill(' ');

        if (_sol->nrOfSolutions()>1) {
            ostr << setw(21) << left <<  "Solution nr. " << i+1 << endl;
        }
        ostr << setw(21) << left <<  "Objective status " << _sol->solution(i)->status() << endl;
        ostr << setw(21) << left <<  "Objective value  " << _sol->solution(i)->value()  << " (" << _sol->objSense() << "!)" << endl;
        ostr <<endl;


        if (!_opts->displayIgnoreVars()) {
            ostr << "Variables " <<  endl;

            ostr << setw(20) << left <<  "Name";
            ostr << setw(5) << right <<  "Type";
            ostr << setw(20) << right <<   "Activity";
            ostr << setw(20) << right <<  "Lower bound";
            ostr << setw(20) << right <<  "Upper bound";
            ostr << setw(20) << right <<  "Marginal" << endl;
            ostr << setw(105) << setfill('-') << "" <<endl;
            ostr <<  setfill(' ');

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

            ostr << setw(105) << setfill('-') << "" <<endl;
            ostr <<  setfill(' ');
            ostr << endl;
        }

        if (!_opts->displayIgnoreCons()) {
            ostr << "Constraints " <<  endl;

            ostr << setw(20) << left <<  "Name";
            ostr << setw(5) << right <<  "Type";
            ostr << setw(20) << right <<   "Activity";
            ostr << setw(20) << right <<  "Lower bound";
            ostr << setw(20) << right <<  "Upper bound";
            ostr << setw(20) << right <<  "Marginal" << endl;
            ostr << setw(105) << setfill('-') << "" <<endl;
            ostr <<  setfill(' ');

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

            ostr << setw(105) << setfill('-') << "" <<endl;
            //ostr << setw(105) << setfill('-') << "";

        }

    }
}

/**
 * @brief writes the solution of a variable into a file or stdOut in Ascii format
 * @param i          number of the solution
 * @param j          index of the variable
 * @param ostr       pointer to ostream object to write the solution(s)
 */
void SolutionAscii::writeVarValues(unsigned long i, unsigned long j , ostream& ostr) {
    ostr << setw(20) << left <<  _sol->solution(i)->variable(j)->name();
    ostr << setw(5) <<  right << _sol->solution(i)->variable(j)->type();
    ostr << setw(20) << right << ( _sol->solution(i)->variable(j)->type()=="C" ? _sol->solution(i)->variable(j)->activity() : round(_sol->solution(i)->variable(j)->activity() ));

    ostr << setw(20) << right << _sol->solution(i)->variable(j)->lowerBound();
    ostr << setw(20) << right << _sol->solution(i)->variable(j)->upperBound();
    if (_sol->hasMarginal())
        ostr << setw(20)  << right<< _sol->solution(i)->variable(j)->marginal();
    else
        ostr << setw(20) << right << "-" ;
    ostr << endl;
}

/**
 * @brief writes the solution of a constraint into a file or stdOut in Ascii format
 * @param i          number of the solution
 * @param j          index of the constraint
 * @param ostr       pointer to ostream object to write the solution(s)
 */
void SolutionAscii::writeConValues( unsigned long i, unsigned long j , ostream& ostr) {

    ostr << setw(20) <<   left <<  _sol->solution(i)->constraint(j)->name();
    ostr << setw(5) <<  right << _sol->solution(i)->constraint(j)->type();
    ostr << setw(20) << right << _sol->solution(i)->constraint(j)->activity() ;


    ostr << setw(20) << right << _sol->solution(i)->constraint(j)->lowerBound();

    ostr << setw(20) << right << _sol->solution(i)->constraint(j)->upperBound();
    if (_sol->hasMarginal())
        ostr << setw(20)  << right<< _sol->solution(i)->constraint(j)->marginal();
    else
        ostr << setw(20) << right << "-" ;
    ostr << endl;
}







