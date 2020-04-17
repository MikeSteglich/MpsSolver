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

#include <iostream>
#include <fstream>
#include <algorithm>

#include "MpsSolverOptions.hh"
#include "FileTools.hh"
#include "StringTools.hh"
#include "MpsSolverException.hh"

using namespace std;

/**
 * @brief Constructor
 */
MpsSolverOptions::MpsSolverOptions()
{
    _problem = "";

    _solver = "CBC";

    _objName = "";
    _objSense = "MAX";

    _solverOpts = vector<solverOption>();

    _intRelax = false;
    _isSilent = false;

    _displayIgnoreZeros = false;
    _displayIgnoreCons = false;
    _displayIgnoreVars = false;

    _displayVarPatterns = vector<string>();
    _displayConPatterns = vector<string>();

    _solutionPool = false;

    _solutionAscii = false;
    _solutionAsciiFileName = "";

    _solutionCsv = false;
    _solutionCsvFileName = "";

    _solutionXml = false;
    _solutionXmlFileName = "";

    _optFileName = "MpsSolver.opt";
    _binaryFileName = "";
    _mpsSolverHome ="";

    _actionHelp = false;
    _actionVersion = false;

    _actionRun = false;

    _resultZeroPrecision=1e-9;
}

/**
 * @brief Destructor
 */
MpsSolverOptions::~MpsSolverOptions() {}


/**
 * @brief       Parses the command line arguments
 * @param args  String vector with the command line args
 */
void MpsSolverOptions::parseOptions(vector<string> args)
{
    string key, val, oKey;

    _binaryFileName = StringTools::lrTrim(args[0]);
    args.erase(args.begin());
    readOptFile();

    key="";
    val="";
    for (const string& argument : args)
    {
        string arg = StringTools::lrTrim(argument);

        if (!StringTools::startsWith(arg, "-")) {
            throw MpsSolverException("Unknown argument <" + arg + ">");
        }
        else {
            val="";
            key="";
            StringTools::getOptionValue(arg, key, val, "=");
            oKey=key;
            key = StringTools::upperCase(key);
        }

        if (StringTools::upperCase(key)=="-PROBLEM")
        {
            if (!_problem.empty())
                throw MpsSolverException("Multiple specification of Mps file <" + val + ">");
            if (val.empty())
                throw MpsSolverException("Mps file is not specified ");

            _problem = val;
            _actionRun=true;
            continue;

        } else if (StringTools::upperCase(key)=="-SOLVER")
        {
            val=StringTools::upperCase(val);
            if (StringTools::findInStringVector(_implementedSolvers,val))
                _solver = val;
            else
                throw MpsSolverException("Solver <" + val + "> is not implemended");
            continue;
        }
        /*else if (StringTools::upperCase(key)=="-OBJ")
        {
            if (val.empty())
                throw MpsSolverException("Objective function definition without value");
            else
                _objName = val;
            continue;
        }*/
        else if (StringTools::upperCase(key)=="-OBJSENSE")
        {
            if (val.empty())
            {
                throw MpsSolverException("Objective sense definition without value");
            }
            else
            {
                _objSense = StringTools::upperCase(val);
                if (!(_objSense == "MAX" || _objSense == "MIN"))
                    throw MpsSolverException("Wrong objective sense <" + val + ">");
                continue;
            }
        }
        else if (StringTools::upperCase(key)=="-INTRELAX")
        {
            _intRelax = true;
            continue;
        }
        else if (StringTools::upperCase(key)=="-SILENT")
        {
            _isSilent = true;
            continue;
        }
        else if (StringTools::upperCase(key)=="-IGNOREZEROS")
        {
            _displayIgnoreZeros = true;
            continue;
        }
        else if (StringTools::upperCase(key)=="-IGNOREVARS")
        {
            _displayIgnoreVars = true;
            continue;
        }
        else if (StringTools::upperCase(key)=="-IGNORECONS")
        {
            _displayIgnoreCons = true;
            continue;
        }
        else if (StringTools::upperCase(key)=="-SOLUTIONPOOL")
        {
            _solutionPool = true;
            continue;
        }
        else if (StringTools::upperCase(key)=="-DISPLAYVARS")
        {
            if (val.empty())
            {
                throw MpsSolverException("Value for display option missing");
            }
            else
            {
                StringTools::split(val, _displayVarPatterns, "+");
                continue;
            }
        }
        else if (StringTools::upperCase(key)=="-DISPLAYCONS")
        {
            if (val.empty())
            {
                throw MpsSolverException("Value for display option missing");
            }
            else
            {
                StringTools::split(val, _displayConPatterns, "+");
                continue;
            }
        }
        else if (StringTools::upperCase(key)=="-SOLUTIONASCII")
        {
            _solutionAscii = true;
            if (val.empty())
            {
                _solutionAsciiFileName = StringTools::problemName(_problem) + ".sol";
            }
            else
            {
                _solutionAsciiFileName = val;
                continue;
            }
        }
        else if (StringTools::upperCase(key)=="-SOLUTIONCSV")
        {
            _solutionCsv = true;
            if (val.empty())
            {
                _solutionCsvFileName = StringTools::problemName(_problem) + ".csv";
            }
            else
            {
                _solutionCsvFileName = val;
                continue;
            }
        }
        else if (StringTools::upperCase(key)=="-SOLUTIONXML")
        {
            _solutionXml = true;
            if (val.empty())
            {
                _solutionXmlFileName = StringTools::problemName(_problem) + ".xsol";
                continue;
            }
            else
            {
                _solutionXmlFileName = val;
                continue;
            }
        }
        else if (StringTools::upperCase(key)=="-V")
        {
            _actionVersion = true;
            continue;
        }
        else if (StringTools::upperCase(key)=="-H")
        {
            _actionHelp = true;
            continue;
        }
        else //SolverOptions
        {
            oKey=oKey.substr(1);
            solverOption sOpt;
            sOpt.key=oKey;
            sOpt.value=val;
            _solverOpts.push_back(sOpt);
        }
    }
    if (_problem.empty() && !(_actionVersion || _actionHelp) )
        throw MpsSolverException("No MPS file specified");

}

/**
 * @brief       Prints usages on stdOut
 */
void MpsSolverOptions::usage() {
    cout << "MpsSolver problem=<mpsFileName.mps> [options]*" << endl ;
    cout << "Options" << endl;
    cout << "-solver=<cbc|gkpk|scip|cplex|gurobi>           -	Specification of the solver" << endl;
    cout << "-<solverOptionKey>=<solverOptionValue>         -	Specification of solver specific options" << endl;
    cout << "-obj=<objectiveFunctionName>                   -	Specification of name of the objective function" << endl;
    cout << "-objSense=<min|max>                            -	Specification of the objective sense" << endl;
    cout << "-intRelax                                      -	Enables integer relaxation" << endl;
    cout << "-silent                                        -	Suppresses output on stdOut" << endl;
    cout << "-ignoreZeros                                   -	Shows only activities unequal to zero in the solution(s)" << endl;
    cout << "-ignoreVars                                    -	Suppresses variables in the solution(s)" << endl;
    cout << "-ignoreCons                                    -	Suppresses constraints in the solution(s)" << endl;
    cout << "-solutionPool                                  -	Shows multiple solutions (only Cplex and Gurobi)" << endl;
    cout << "-displayVars=<varPattern1[+varPattern2]*       -	Shows only variables matching one of the varPatterns in the solution(s)" << endl;
    cout << "-displayCons=<conPattern1[+conPattern2]*       -	Shows only constraints matching one of the conPatterns in the solution(s)" << endl;
    cout << "-solutionAscii[=<solutionfile>]                -	Writes the solution(s) as Ascii file, the filename can be specified (default mpsFileName.sol) " << endl;
    cout << "-solutionCsv[=<solutionfile>]                  -	Writes the solution(s) as Csv file, the filename can be specified (default mpsFileName.csv) " << endl;
    cout << "-solutionXml[=<solutionfile>]                  -	Writes the solution(s) as Xml file, the filename can be specified (default mpsFileName.xsol) " << endl;
}

/**
 * @brief sets problem name
 * @param problem   problem name
 */
void MpsSolverOptions::setProblem(string &problem) { _problem = problem; }

/**
 * @brief Returns the problem name
 * @return  problem name
 */
string& MpsSolverOptions::problem() { return _problem; }

/**
 * @brief sets the solver name
 * @param fName     solver name
 */
void MpsSolverOptions::setSolver(string &fName) { _solver = fName; }

/**
 * @brief returns the solver
 * @return      solver name
 */
string& MpsSolverOptions::solver() { return _solver; }

/**
 * @brief Returns the file name of the solver binary including the full path
 * @return      file name of the solver binary
 */
string& MpsSolverOptions::solverBinary() {return _solverBinNames[_solver]; }

/**
 * @brief sets the name of the objective function
 * @param obj       name of the objective function
 */
void MpsSolverOptions::setObjName(string &obj) { _objName = obj; }

/**
 * @brief Returns the name of the objective function
 * @return  name of the objective function
 */
string& MpsSolverOptions::objName() { return _objName; }

/**
 * @brief sets the objective function sense
 * @param sense     <"max"|"min">
 */
void MpsSolverOptions::setObjSense(string &sense)
{
    if (StringTools::upperCase(sense)!="MAX" && StringTools::upperCase(sense)!="MIN" ) {
       throw MpsSolverException("Wrong objective sense <" + sense + ">");
    }
    _objSense = sense;
}

/**
 * @brief returns the objective function sense
 * @return <"max"|"min">
 */
string& MpsSolverOptions::objSense() { return _objSense; }

/**
 * @brief adds a solver option
 * @param opt   struct of the solver option
 */
void MpsSolverOptions::addSolverOption(solverOption &opt) { _solverOpts.push_back(opt); }

/**
 * @brief returns all solver options
 * @return  vector of solverOption objects
 */
vector<MpsSolverOptions::solverOption>& MpsSolverOptions::solverOptions() { return _solverOpts; }

/**
 * @brief sets IntRelax option
 * @param opt   true if integers have to be relaxed
 */
void MpsSolverOptions::setIntRelax(bool opt) { _intRelax = opt; }

/**
 * @brief returns the intRelax option
 * @return  true if integers have to be relaxed
 */
bool MpsSolverOptions::intRelax() { return _intRelax; }

/**
 * @brief sets whether all messages to stdOut should be suppressed option
 * @param opt   true if all messages to stdOut should be suppressed
 */
void MpsSolverOptions::setIsSilent(bool opt) { _isSilent = opt; }

/**
 * @brief returns the isSilent option
 * @return  true if all messages to stdOut should be suppressed
 */
bool MpsSolverOptions::isSilent() { return _isSilent; }

/**
 * @brief sets whether only non zero activities are shown in the solution(s)
 * @param opt   true if only non zero activities are shown in the solution(s)
 */
void MpsSolverOptions::setDisplayIgnoreZeros(bool opt) { _displayIgnoreZeros = opt; }

/**
 * @brief returns whether only non zero activities are shown in the solution(s)
 * @return   true if only non zero activities are shown in the solution(s)
 */
bool MpsSolverOptions::displayIgnoreZeros() { return _displayIgnoreZeros; }

/**
 * @brief sets whether all constraints are not shown in the solution(s)
 * @param opt   true if all constraints are not shown in the solution(s)
 */
void MpsSolverOptions::setDisplayIgnoreCons(bool opt) { _displayIgnoreCons = opt; }

/**
 * @brief returns whether all constraints are not shown in the solution(s)
 * @param opt   true if all constraints are not shown in the solution(s)
 */
bool MpsSolverOptions::displayIgnoreCons() { return _displayIgnoreCons; }

/**
 * @brief sets whether all variables are not shown in the solution(s)
 * @param opt   true if all variables are not shown in the solution(s)
 */
void MpsSolverOptions::setDisplayIgnoreVars(bool opt) { _displayIgnoreVars = opt; }

/**
 * @brief returns whether all variables are not shown in the solution(s)
 * @param opt   true if all variables are not shown in the solution(s)
 */
bool MpsSolverOptions::displayIgnoreVars() { return _displayIgnoreVars; }

/**
 * @brief adds a pattern to show only variables with names matching the varPatterns in the solution(s)
 * @param opt   variable name pattern
 */
void MpsSolverOptions::addDisplayVarPatterns(string &opt) { _displayVarPatterns.push_back(opt); }

/**
 * @brief returns a vector with all variable name patterns
 * @return      vector with all variable name patterns
 */
vector<string>& MpsSolverOptions::displayVarPatterns() { return _displayVarPatterns; }

void MpsSolverOptions::addDisplayConPatterns(string &opt) { _displayConPatterns.push_back(opt); }

/**
 * @brief returns a vector with all constraint name patterns
 * @return      vector with all constraint name patterns
 */
vector<string>& MpsSolverOptions::displayConPatterns() { return _displayConPatterns; }

/**
 * @brief sets whether multiple solutions shall be shown (only Cplex and Gurobi)
 * @param opt   true if  multiple solutions shall be shown
 */
void MpsSolverOptions::setSolutionPool(bool opt) { _solutionPool = opt; }

/**
 * @brief returns whether multiple solutions shall be shown (only Cplex and Gurobi)
 * @return   true if  multiple solutions shall be shown
 */
bool MpsSolverOptions::solutionPool() { return _solutionPool; }

/**
 * @brief Sets whether the solution(s) should be written into an Ascii file
 * @param opt   true if the solution(s) should be written into an Ascii file
 */
void MpsSolverOptions::setSolutionAscii(bool opt) { _solutionAscii = opt; }

/**
 * @brief Returns whether the solution(s) should be written into an Ascii file
 * @return      true if the solution(s) should be written into an Ascii file
 */
bool MpsSolverOptions::solutionAscii() { return _solutionAscii; }

/**
 * @brief Sets the fileName in that the solution(s) should be written in  Ascii format
 * @param   fileName
 */
void MpsSolverOptions::setSolutionAsciiFile(string &fileName) { _solutionAsciiFileName = fileName; }

/**
 * @brief Returns the fileName in that the solution(s) should be written in  Ascii format
 * @return  File name
 */
string& MpsSolverOptions::solutionAsciiFile() { return _solutionAsciiFileName; }


/**
 * @brief Sets whether the solution(s) should be written into a Csv file
 * @param opt   true if the solution(s) should be written into a Csv file
 */
void MpsSolverOptions::setSolutionCsv(bool opt) { _solutionCsv = opt; }

/**
 * @brief Returns whether the solution(s) should be written into a Csv file
 * @return      true if the solution(s) should be written into a Csv file
 */
bool MpsSolverOptions::solutionCsv() { return _solutionCsv; }

/**
 * @brief Sets the fileName in that the solution(s) should be written in Csv format
 * @param   fileName
 */
void MpsSolverOptions::setSolutionCsvFile(string &fileName) { _solutionCsvFileName = fileName; }

/**
 * @brief Returns the fileName in that the solution(s) should be written in Csv format
 * @return  File name
 */
string& MpsSolverOptions::solutionCsvFile() { return _solutionCsvFileName; }

/**
 * @brief Sets whether the solution(s) should be written into an Xml file
 * @param opt   true if the solution(s) should be written into an Xml file
 */
void MpsSolverOptions::setSolutionXml(bool opt) { _solutionXml = opt; }

/**
 * @brief Returns whether the solution(s) should be written into an Xml file
 * @return      true if the solution(s) should be written into an Xml file
 */
bool MpsSolverOptions::solutionXml() { return _solutionXml; }

/**
 * @brief Sets the fileName in that the solution(s) should be written in Xml format
 * @param   fileName
 */
void MpsSolverOptions::setSolutionXmlFile(string &fileName) { _solutionXmlFileName = fileName; }

/**
 * @brief Returns the fileName in that the solution(s) should be written in Xml format
 * @return  File name
 */
string& MpsSolverOptions::solutionXmlFile() { return _solutionXmlFileName; }

/**
 * @brief Returns the action that the usages have to be written to stdOut
 * @param opt   true if action has to be started to write the usages to stdOut
 */
bool MpsSolverOptions::actionHelp() { return _actionHelp; }

/**
 * @brief Returns the action that the version has to be written to stdOut
 * @param opt   true if action has to be started to write the version to stdOut
 */
bool MpsSolverOptions::actionVersion() { return _actionVersion; }

/**
 * @brief Returns the action that the problem has to be solved
 * @param opt   true if action has to be started to solve the problem
 */
bool MpsSolverOptions::actionRun() { return _actionRun; }

/**
 * @brief Returns the precision for a zero activity
 * @return precision for a zero activity
 */
double MpsSolverOptions::resultZeroPrecision() {
    return _resultZeroPrecision;
}


/**
 * @brief reads the option file
 */
void MpsSolverOptions::readOptFile()
{
    char dirSepChar = FileTools::getDirSepChar(_binaryFileName.c_str());
    string optFullfileName(FileTools::replFileInPath(&_binaryFileName, &_optFileName, dirSepChar));

    try
    {

        ifstream optFile(optFullfileName.c_str(), ifstream::binary);

        if (!optFile.is_open())
            throw MpsSolverException("File cannot be openend: " + _optFileName);

        string line;

        while (getline(optFile, line))
        {
            line = StringTools::lTrim(line);

            if (!StringTools::startsWith(StringTools::lTrim(line), "#") && (!StringTools::lrTrim(line).empty()))
            {

                string solver, solverPath;
                StringTools::getOptionValue(line, solver, solverPath);

                solver = StringTools::upperCase((StringTools::lrTrim(solver)));
                solverPath = StringTools::lrTrim(solverPath);
                string dirSep(1,dirSepChar);

                if ( !StringTools::startsWith(solverPath, dirSep ) ) {
                    solverPath = FileTools::replFileInPath(&_binaryFileName, &solverPath, dirSepChar);
                }

                _implementedSolvers.push_back(solver);
                _solverBinNames[solver] = solverPath;
            }
        }

        optFile.close();
    }
    catch (exception &e)
    {
        throw MpsSolverException("Problems while reading <" + _optFileName + "> " + string(e.what()));
    }
}


