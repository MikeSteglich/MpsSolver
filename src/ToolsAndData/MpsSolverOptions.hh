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

#ifndef MPSSOLVEROPTIONS_HH
#define MPSSOLVEROPTIONS_HH

#include <string>
#include <vector>
#include <map>

using namespace std;

/**
 * @brief The MpsSolverOptions class
 */
class MpsSolverOptions
{

public:
    /**
     * @brief The solverOption struct
     */
	struct solverOption
	{
		string key;
		string value;
	};

public:
    /**
     * @brief Constructor
     */
	MpsSolverOptions();

    /**
     * @brief Destructor
     */
    ~MpsSolverOptions();

    /**
     * @brief       Parses the command line arguments
     * @param args  String vector with the command line args
     */
	void parseOptions(vector<string> args);

    /**
     * @brief       Prints usages on stdOut
     */
	void usage();

    /**
     * @brief sets problem name
     * @param problem   problem name
     */
	void setProblem(string &problem);

    /**
     * @brief Returns the problem name
     * @return  problem name
     */
    string& problem();

    /**
     * @brief sets the solver name
     * @param fName     solver name
     */
	void setSolver(string &fName);

    /**
     * @brief returns the solver
     * @return      solver name
     */
    string& solver();

    /**
     * @brief Returns the file name of the solver binary including the full path
     * @return      file name of the solver binary
     */
    string& solverBinary();

    /**
     * @brief sets the name of the objective function
     * @param obj       name of the objective function
     */
	void setObjName(string &obj);

    /**
     * @brief Returns the name of the objective function
     * @return  name of the objective function
     */
    string& objName();

    /**
     * @brief sets the objective function sense
     * @param sense     <"max"|"min">
     */
	void setObjSense(string &sense);

    /**
     * @brief returns the objective function sense
     * @return <"max"|"min">
     */
    string& objSense();

    /**
     * @brief adds a solver option
     * @param opt   struct of the solver option
     */
	void addSolverOption(solverOption &opt);

    /**
     * @brief returns all solver options
     * @return  vector of solverOption objects
     */
    vector<solverOption>& solverOptions();

    /**
     * @brief sets IntRelax option
     * @param opt   true if integers have to be relaxed
     */
	void setIntRelax(bool opt);

    /**
     * @brief returns the intRelax option
     * @return  true if integers have to be relaxed
     */
	bool intRelax();

    /**
     * @brief sets whether all messages to stdOut should be suppressed option
     * @param opt   true if all messages to stdOut should be suppressed
     */
	void setIsSilent(bool opt);

    /**
     * @brief returns the isSilent option
     * @return  true if all messages to stdOut should be suppressed
     */
	bool isSilent();

    /**
     * @brief sets whether only non zero activities are shown in the solution(s)
     * @param opt   true if only non zero activities are shown in the solution(s)
     */
	void setDisplayIgnoreZeros(bool opt);

    /**
     * @brief returns whether only non zero activities are shown in the solution(s)
     * @return   true if only non zero activities are shown in the solution(s)
     */
	bool displayIgnoreZeros();

    /**
     * @brief sets whether all constraints are not shown in the solution(s)
     * @param opt   true if all constraints are not shown in the solution(s)
     */
	void setDisplayIgnoreCons(bool opt);

    /**
     * @brief returns whether all constraints are not shown in the solution(s)
     * @param opt   true if all constraints are not shown in the solution(s)
     */
	bool displayIgnoreCons();

    /**
     * @brief sets whether all variables are not shown in the solution(s)
     * @param opt   true if all variables are not shown in the solution(s)
     */
	void setDisplayIgnoreVars(bool opt);

    /**
     * @brief returns whether all variables are not shown in the solution(s)
     * @param opt   true if all variables are not shown in the solution(s)
     */
	bool displayIgnoreVars();

    /**
     * @brief adds a pattern to show only variables with names matching the varPatterns in the solution(s)
     * @param opt   variable name pattern
     */
	void addDisplayVarPatterns(string &opt);

    /**
     * @brief returns a vector with all variable name patterns
     * @return      vector with all variable name patterns
     */
    vector<string>& displayVarPatterns();

    /**
     * @brief adds a pattern to show only constraints with names matching the varPatterns in the solution(s)
     * @param opt   constraint name pattern
     */
	void addDisplayConPatterns(string &opt);

    /**
     * @brief returns a vector with all constraint name patterns
     * @return      vector with all constraint name patterns
     */
    vector<string>& displayConPatterns();

    /**
     * @brief sets whether multiple solutions shall be shown (only Cplex and Gurobi)
     * @param opt   true if  multiple solutions shall be shown
     */
	void setSolutionPool(bool opt);

    /**
     * @brief returns whether multiple solutions shall be shown (only Cplex and Gurobi)
     * @return   true if  multiple solutions shall be shown
     */
	bool solutionPool();

    /**
     * @brief Sets whether the solution(s) should be written into an Ascii file
     * @param opt   true if the solution(s) should be written into an Ascii file
     */
	void setSolutionAscii(bool opt);

    /**
     * @brief Returns whether the solution(s) should be written into an Ascii file
     * @return      true if the solution(s) should be written into an Ascii file
     */
	bool solutionAscii();

    /**
     * @brief Sets the fileName in that the solution(s) should be written in  Ascii format
     * @param   fileName
     */
	void setSolutionAsciiFile(string &fileName);

    /**
     * @brief Returns the fileName in that the solution(s) should be written in  Ascii format
     * @return  File name
     */
    string& solutionAsciiFile();

    /**
     * @brief Sets whether the solution(s) should be written into a Csv file
     * @param opt   true if the solution(s) should be written into a Csv file
     */
	void setSolutionCsv(bool opt);

    /**
     * @brief Returns whether the solution(s) should be written into a Csv file
     * @return      true if the solution(s) should be written into a Csv file
     */
	bool solutionCsv();

    /**
     * @brief Sets the fileName in that the solution(s) should be written in Csv format
     * @param   fileName
     */
	void setSolutionCsvFile(string &fileName);

    /**
     * @brief Returns the fileName in that the solution(s) should be written in Csv format
     * @return  File name
     */
    string& solutionCsvFile();

    /**
     * @brief Sets whether the solution(s) should be written into an Xml file
     * @param opt   true if the solution(s) should be written into an Xml file
     */
	void setSolutionXml(bool opt);

    /**
     * @brief Returns whether the solution(s) should be written into an Xml file
     * @return      true if the solution(s) should be written into an Xml file
     */
	bool solutionXml();

    /**
     * @brief Sets the fileName in that the solution(s) should be written in Xml format
     * @param   fileName
     */
	void setSolutionXmlFile(string &fileName);

    /**
     * @brief Returns the fileName in that the solution(s) should be written in Xml format
     * @return  File name
     */
    string& solutionXmlFile();

    /**
     * @brief Returns the action that the usages have to be written to stdOut
     * @param opt   true if action has to be started to write the usages to stdOut
     */
    bool actionHelp();

    /**
     * @brief Returns the action that the version has to be written to stdOut
     * @param opt   true if action has to be started to write the version to stdOut
     */
	bool actionVersion();

    /**
     * @brief Returns the action that the problem has to be solved
     * @param opt   true if action has to be started to solve the problem
     */
	bool actionRun();

    /**
     * @brief Returns the precision for a zero activity
     * @return precision for a zero activity
     */
    double resultZeroPrecision();

private:

    /**
     * @brief reads the option file
     */
    void readOptFile();


    string _problem;                        // full file name of the problem

    string _solver;                         // name of the solver

    vector<string> _implementedSolvers;     // list of the implemented solvers (out of the MpsSolver.opt file)
    map<string, string> _solverBinNames;    // list of the full file names of the solver binaries  (out of the MpsSolver.opt file)

    string _objName;                        // name of the objective function
    string _objSense;                       // objective function sense

    vector<solverOption> _solverOpts;       // vector of solverOption objects

    bool _intRelax;                         // true if integers have to be relaxed
    bool _isSilent;                         //  true if all messages to stdOut should be suppressed

    bool _displayIgnoreZeros;                  // true if only non zero activities are shown in the solution(s)
    bool _displayIgnoreCons;                // true if all constraints are not shown in the solution(s)
    bool _displayIgnoreVars;                //  true if all constraints are not shown in the solution(s)

    vector<string> _displayVarPatterns;     // vector of patterns to show only variables with names matching the patterns in the solution(s)
    vector<string> _displayConPatterns;     // vector of patterns to show only constraints with names matching the patterns in the solution(s)

    bool _solutionPool;                     // true if  multiple solutions shall be shown

    bool _solutionAscii;                    // true if the solution(s) should be written into an Ascii file
    string _solutionAsciiFileName;          // fileName in that the solution(s) should be written in  Ascii format

    bool _solutionCsv;                      // true if the solution(s) should be written into a Csv file
    string _solutionCsvFileName;            // fileName in that the solution(s) should be written in Csv format

    bool _solutionXml;                      // true if the solution(s) should be written into an Xml file
    string _solutionXmlFileName;            // fileName in that the solution(s) should be written in Xml format


    string _optFileName;                    // Name of the option file (default MpsSolver.opts)
    string _binaryFileName;                 // full file name of the MpsSolver binary
    string _mpsSolverHome;                  // full path of the MpsSolver binary

    bool _actionHelp;                       // true if action has to be started to write the usages to stdOut
    bool _actionVersion;                    // true if action has to be started to write the version to stdOut
    bool _actionRun;                        // true if action has to be started to solve the problem

    double _resultZeroPrecision;            // Result zero precision
};



#endif /* MPSSOLVEROPTIONS_HH_ */
