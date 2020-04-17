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


#ifndef SOLUTION_HH
#define SOLUTION_HH

#include <string>
#include <vector>
#include <map>

#include "../ToolsAndData/MpsData.hh"
#include "../ToolsAndData/StringTools.hh"

using namespace std;


/**
 * @brief class for storing a solution element (variable or constraint)
 */
class SolutionElement {

private:
    double _activity;                ///< activity of the variable or constraint
    double _marginal;                ///< marginal of the variable or constraint

    ModelElement *_modElement;      ///< model element of the variable or constraint


public:

    /**
     * @brief  constructor
     */
    SolutionElement();

    /**
     * @brief  sets the corresponding model element for the variable or constraint
     * @param modElement        model element
     */
    inline void setModelElement(ModelElement *modElement) { _modElement=modElement;}

    /**
     * @brief returns the name of the variable or constraint
     * @return      name of the variable or constraint
     */
    inline string name() { return _modElement->name();}

    /**
     * @brief  returns the type of the variable or constraint
     * @return      type of the variable or constraint
     */
    inline string type() { return _modElement->type();}

    /**
     * @brief returns the activity of the variable or constraint
     * @return      activity of the variable or constraint
     */
    inline double activity() { return _activity;}

    /**
     * @brief sets the activity bound of the variable or constraint
     * @param activity        activity  of the variable or constraint
     */
    inline void setActivity(double activity) { _activity=activity;}

    /**
     * @brief returns the lower bound of the variable or constraint
     * @return      lower bound of the variable or constraint
     */
    inline double lowerBound() {return _modElement->lowerBound(); }

    /**
     * @brief returns the upper bound of the variable or constraint
     * @return      upper bound of the variable or constraint
     */
    inline double upperBound() {return _modElement->upperBound(); }

    /**
     * @brief  returns the marginal of the variable or constraint
     * @return      marginal of the variable or constraint
     */
    inline double marginal() { return _marginal;}

    /**
     * @brief sets the marginal bound of the variable or constraint
     * @param marginal        marginal  of the variable or constraint
     */
    inline void setMarginal(double marginal) { _marginal=marginal;}



};


/**
 *  place holder for class solution
 */
class Solution;


/**
 * class for storing a single solution
 */
class SingleSolution {

private:

    MpsSolverOptions *_opts;                // pointer to a MpsSolverOptions object
    MpsData *_mpsData;                      // pointer to a MpsData object


    string _status;                         // solution status
    double _value;                          // objective function value
    vector<SolutionElement> _variables;     // vector of all variable objects
    vector<SolutionElement> _constraints;   // vector of all constraint objects


public:

    /**
     * @brief Constructor
     * @param opts          pointer to a MpsSolverOptions object
     * @param mpsData       pointer to a MpsData object
     */
    SingleSolution(MpsSolverOptions *opts, MpsData *mpsData);

    /**
     * @brief Destructor
     */
    inline ~SingleSolution() {}

    /**
     * @brief returns the status of the solution
     * @return      status
     */
    inline string status() { return _status;}

    /**
     * @brief sets the status of the solution
     * @param status
     */
    inline void setStatus(string status) { _status=status;}

    /**
     * @brief returns the objective function value of the solution
     * @return
     */
    inline double value() { return _value;}

    /**
     * @brief sets the objective function value of the solution
     * @param value     objective function value of the solution
     */
    inline void setValue(double value) { _value=value;}

    /**
     * @brief returns all variables
     * @return  pointer to a vector of the SolutionElement objects of all variables
     */
    inline vector<SolutionElement>* variables() { return &_variables;}

    /**
     * @brief adds a variable object to the solution
     * @param variable  SolutionElement object of a variable
     */
    inline void setVariable(SolutionElement& variable) {_variables.push_back(variable);}

    /**
     * @brief returns a pointer to an object of a variable
     * @param idx       Index of the variable
     * @return          pointer to an object of a variable
     */
    inline SolutionElement* variable(unsigned long idx) {return &_variables[idx]; }

    /**
     * @brief returns all constraints
     * @return  pointer to a vector of the SolutionElement objects of all constraints
     */
    inline vector<SolutionElement>* constraints() { return &_constraints;}

    /**
     * @brief adds a constraint object to the solution
     * @param variable  SolutionElement object of a constraint
     */
    inline void setConstraint(SolutionElement& constraint) {_constraints.push_back(constraint);}

    /**
     * @brief returns a pointer to an object of a constraint
     * @param idx       Index of the constraint
     * @return          pointer to an object of a constraint
     */
    inline SolutionElement* constraint(unsigned long idx) {return &_constraints[idx]; }

    /**
     * @brief calculates the activity of a constraint (used for Gurobi and Scip)
     * @param con       pointer to an object of a constraint
     * @param sols      pointer to an object of a single solution
     * @return          activity of the constraint
     */
    double calculateConActivity(SolutionElement* con, Solution* sols);

};



/**
 * @brief The Solution class
 */
class Solution
{

private:
    MpsSolverOptions *_opts;                            // pointer to a MpsSolverOptions object
    MpsData *_mpsData;                                  // pointer to a MpsData object

    map<string,unsigned long>   _colNameMap;            // mapping between the names and indices of the variables
    map<string,unsigned long>   _rowNameMap;            // mapping between the names and indices of the constraints

    int _nrOfSolutions;                                 // count of solutions

    string _status;                                     // generell status

    vector<SingleSolution> _solutions;                  // all solutions

    vector<ModelElement*> _modVariables;                // all variables
    vector<ModelElement*> _modConstraints;              // all constraint

public:
    /**
     * @brief constructor
     * @param opts          pointer to a MpsSolverOptions object
     * @param mpsData       pointer to a MpsData object
     */
    Solution (MpsSolverOptions *opts, MpsData *mpsData);

    /**
     * @brief Destructor
     */
    inline ~Solution()	{  }

    /**
     * @brief returns the name of the problem
     * @return      name of the problem
     */
    inline string problemName() {return StringTools::modelName(_opts->problem());}

    /**
     * @brief returns the objective function sense
     * @return      objective function sense <"MIN","MAX">
     */
    inline string objSense() { return _opts->objSense();}

    /**
     * @brief returns the name of the objective function
     * @return      name of the objective function
     */
    inline string objName() { return _opts->objName();}

    /**
     * @brief returns the count of the variables
     * @return      count of the variables
     */
    inline unsigned long nrOfVariables() {return _mpsData->nrOfVars();}

    /**
     * @brief returns the count of the constraints
     * @return      count of the constraints
     */
    inline unsigned long nrOfConstraints() {return _mpsData->nrOfCons();}

    /**
     * @brief returns the name of the solver
     * @return      solver name
     */
    inline string solver() { return _opts->solver();}

    /**
     * @brief returns the count of the solutions
     * @return      count of the solutions
     */
    inline int nrOfSolutions() {return _nrOfSolutions;}

    /**
     * @brief sets the count of the solutions
     * @param nr         count of the solutions
     */
    inline void setNrOfSolutions(int nr) {_nrOfSolutions=nr;}


    /**
     * @brief returns the generell status of all solutions
     * @return      generell status
     */
    inline string status() {return _status;}

    /**
     * @brief sets the generell status of all solutions
     * @param status        generell status of all solutions
     */
    inline void setStatus(string status) {_status=status;}

    /**
     * @brief returns all solutions
     * @return      vector of all solutions
     */
    inline vector<SingleSolution>& solutions() {return _solutions;}

    /**
     * @brief adds a new single solution
     * @param sol   SingleSolution object
     */
    inline void setSolution(SingleSolution& sol) { _solutions.push_back(sol);}


    /**
     * @brief returns a  single solution found
     * @param idx       Index of the single solution
     * @return          SingleSolution object
     */
    inline SingleSolution* solution(int idx) {return &_solutions[idx];}

    /**
     * @brief Returns the index of a variable based on its name
     * @param name      name of the variable
     * @return          index of the variable
     */
    inline unsigned long varIdxByName(string& name) { return _colNameMap[name];}

    /**
     * @brief Returns the index of a constraint based on its name
     * @param name      name of the constraint
     * @return          index of the constraint
     */
    inline unsigned long conIdxByName(string& name) { return _rowNameMap[name];}

    /**
     * @brief returns whether the solution contains marginals
     * @return          true if the solution contains marginals
     */
    inline bool hasMarginal() { return !_mpsData->hasInteger();}

    /**
     * @brief Returns a pointer to a variable object
     * @param idx       index of the variable
     * @return          pointer to the ModelElement object of the variable
     */
    inline ModelElement *modelVariable(unsigned long idx) {return _modVariables[idx]; }

    /**
     * @brief Returns a pointer to a constraint object
     * @param idx       index of the constraint
     * @return          pointer to the ModelElement object of the constraint
     */
    inline ModelElement *modelConstraint(unsigned long idx) {return _modConstraints[idx]; }

};


#endif // SOLUTION_HH
