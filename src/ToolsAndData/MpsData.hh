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


#ifndef MPSSOLVERDATA_HH
#define MPSSOLVERDATA_HH

#include <vector>
#include <string>
#include <map>
#include <sstream>

#include "MpsSolverOptions.hh"

using namespace std;



/**
 * class for storing a model element (variable or constraint) to the solution
 */
class ModelElement {
private:

    string _name;                       ///< name of the variable or constraint
    string _type;                       ///< type of the variable or constraint

    double _lowerBound;                 ///< lower bound of the variable or constraint
    double _upperBound;                 ///< upper bound of the variable or constraint

public:

    /**
     * @brief constructor
     */
    ModelElement();

    /**
     * @brief destructor
     */
    ~ModelElement() {}

    /**
     * @brief gets the name of the variable or constraint
     */
    inline string& name() { return _name;}

    /**
     * @brief sets the name of the variable or constraint
     * @param name        name of the variable or constraint
     */
    inline void setName(string name) { _name=name;}

    /**
     * @brief gets the type of the variable or constraint
     */
    inline string& type() { return _type;}

    /**
     * @brief sets the type of the variable or constraint
     * @param type        type of the variable or constraint
     */
    inline void setType(string type) { _type=type;}

    /**
     * @brief gets the lower bound of the variable or constraint
     */
    inline double lowerBound() {return _lowerBound; }

    /**
     * sets the lower bound of the variable or constraint
     * @param lowerBound        lower bound of the variable or constraint
     */
    inline void setLowerBound(double lowerBound) { _lowerBound=lowerBound;}

    /**
     * @brief gets the upper bound of the variable or constraint
     */
    inline double upperBound() {return _upperBound; }

    /**
     * @brief sets the upper bound of the variable or constraint
     * @param upperBound        upper bound of the variable or constraint
     */
    inline void setUpperBound(double upperBound) { _upperBound=upperBound;}

};


/**
 * class for storing data of a mps file
 */
class MpsData {
private:

    unsigned long _nrOfVars;                        // number of variables
    unsigned long _nrOfCons;                        // number of constraints

    vector<ModelElement*> _variables;               // vector of the variables
    vector<ModelElement*> _constraints;             // vector of the constraints

    map<string,ModelElement*>   _colNameMap;        // nameString based map of the variables
    map<string,ModelElement*>   _rowNameMap;        // nameString based map of the constraints

    map<string,map<string,double>> _coeffs;         // LHS coefficients of the problem

    ostringstream _mpsOut;                          // outstream of the Mps file to be generated

    MpsSolverOptions* _opts;                        // pointer of MpsSolver optons object

    bool _hasInteger;                               // true if the problem contains integer vars

public:

    /**
     * @brief constructor
     */
    MpsData(MpsSolverOptions* opts);

    /**
     * @brief destructor
     */
    ~MpsData();

    /**
     * @brief reads the data of a mps Mps file into the MpsData object
     */
    void readMpsData();

    /**
     * @brief Returns the count of the variables
     * @return  count of the variables
     */
    unsigned long nrOfVars() { return _nrOfVars;}

    /**
     * @brief Returns the count of the constraints
     * @return  count of the constraints
     */
    unsigned long nrOfCons() { return _nrOfCons;}

    /**
     * @brief Returns a coefficient of the LHS of the problem
     * @param row   row name
     * @param col   col name
     * @return      coefficient
     */
    double* coeff(const string& row, const string& col) {  return &_coeffs[row][col];   }

    /**
     * @brief Returns the LHS coefficients of the problem
     * @return      two-dimensional map of the LHS coefficients of the problem
     */
    map<string,map<string,double>>* coeffs() { return &_coeffs;}

    /**
     * @brief Returns the vector of all variables
     * @return      vector of pointers of ModelElement objects of the variables
     */
    vector<ModelElement*>& constraints() {return _constraints;}

    /**
     * @brief Returns the vector of all constraints
     * @return      vector of pointers of ModelElement objects of the constraints
     */
    vector<ModelElement*>& variables() {return _variables;}

    /**
     * @brief Returns whether the problem contains integers
     * @return      True if e problem contains integer variables
     */
    bool hasInteger() {return _hasInteger;}

    /**
     * @brief Returns the generated Mps file
     * @return      pointer of ostringstream object of the generated Mps file
     */
    ostringstream* mpsOut() { return &_mpsOut; }

};




#endif // MPSSOLVERDATA_HH
