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

#include "Solver.hh"
#include "../ToolsAndData/MpsSolverException.hh"
#include "../ToolsAndData/FileTools.hh"



#include <iostream>
#include <fstream>



/**
 * @brief Constructor
 * @param opts          pointer to a MpsSolverOptions object
 * @param mpsData       pointer to a MpsData object
 * @param sol           pointer to a Solution object
 */
Solver::Solver(MpsSolverOptions *opts, MpsData *mpsData, Solution *sol)
{
    _opts=opts;
    _mpsData=mpsData;
    _sol=sol;

    char tmpFilename[L_tmpnam];
    std::tmpnam(tmpFilename);

    _instanceBaseName = tmpFilename;

    _instanceFileName = _instanceBaseName+".mps";
    _instanceSolName = _instanceBaseName+".sol";
    _instanceCmdName = _instanceBaseName+".cmd";

    _solverCmdLine="";
}



/**
 * @brief writes the generated Mps into the tmp path
 */
void Solver::writeInstanceFile() {
    try {
        ofstream mpsFile;
        mpsFile.open(_instanceFileName);
        mpsFile << _mpsData->mpsOut()->str();
        mpsFile.close();
    } catch (exception &e)
    {
        throw MpsSolverException("Problems while writing instance file <" + _instanceFileName + "> " + string(e.what()));
    }
}


/**
 * @brief deletes all generated files in the tmp folder
 */
void Solver::deleteTmpFiles() {
    if (FileTools::exists(_instanceFileName))
        remove(_instanceFileName.c_str());

    if (FileTools::exists(_instanceSolName))
        remove(_instanceSolName.c_str());

    if (FileTools::exists(_instanceCmdName))
        remove(_instanceCmdName.c_str());
}

/**
 * @brief starts the solver as external process
 * @return          status of the solver
 */
int Solver::execute() {

    writeInstanceFile();

    int buffSize = 128;
    char buffer[buffSize];
    int ret=-1;

    if (!FileTools::exists(_opts->solverBinary())) {
        throw MpsSolverException("Binary for solver  <" + _opts->solver()   + "> does not exist: " +  _opts->solverBinary() );
    }

    if (_solverCmdLine.empty()) {
        throw MpsSolverException("Internal error, no cmd line for solver <"+ _opts->solver() +"> specified" );
    }

    if (!_opts->isSilent()) {
        cout << endl << "Solving instance using " + _opts->solver()  << endl << endl;
    }

    try {
        FILE* pipe = popen(_solverCmdLine.c_str(), "r");

        if (!pipe)
        {
            throw MpsSolverException("Cannot execute solver <"+ _opts->solver() +">" );
        }

        while (fgets(buffer, 128, pipe) != NULL) {
            if (!_opts->isSilent())
                cout << buffer;
        }

        if (!_opts->isSilent())
            cout << endl <<endl;

        ret=pclose(pipe);

        if (ret!=0) {
            throw MpsSolverException("Solver <"+ _opts->solver() +"> finished with failures" );
        }

    } catch (exception const& e) {
        throw MpsSolverException("Solver <"+ _opts->solver() +"> finished with failures: " + e.what() );
    }

    return ret;
}


string Solver::readXmlVal(string valString) {
    string ret;
    vector <string> valList;
    StringTools::split(StringTools::lrTrim(valString),valList,"=");
    if (valList.size()==0)
        throw MpsSolverException("internal error while reading Cplex solution file <"+valString+">");
    ret = valList[1];
    StringTools::cleanFromDoubleQuotes(ret);
    return ret;

}

