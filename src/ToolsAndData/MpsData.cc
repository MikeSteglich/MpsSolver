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
#include <sstream>


#include "MpsData.hh"
#include "MpsSolverException.hh"
#include "StringTools.hh"
#include "FileTools.hh"




using namespace std;


#define INF  std::numeric_limits<double>::infinity()

/**
 * @brief constructor
 */
ModelElement::ModelElement() {
    _type="";
    _name="";
    _lowerBound=-INF;
    _upperBound=INF;
}


/**
 * @brief constructor
 */
MpsData::MpsData(MpsSolverOptions* opts)
{
    _nrOfVars=0;
    _nrOfCons=0;

    _variables = vector<ModelElement*> (0);
    _constraints = vector<ModelElement*> (0);

    _opts = opts;

    _hasInteger=false;
}

/**
 * @brief destructor
 */
MpsData::~MpsData() {}

#define UDFSEC -1
#define OBJSEC 1
#define ROWSEC 2
#define COLSEC 3
#define RHSSEC 4
#define BNDSEC 5
#define SOSSEC 6
#define MISSEC 7


/**
 * @brief reads the data of a mps Mps file into the MpsData object
 */
void MpsData::readMpsData() {

    int section=UDFSEC;
    bool secStarted=false;
    bool objFound=false;
    bool intMarker=false;

    vector <string> otherSecs {"RANGES","INDICATORS","PWLNAM","PWLCON","QMATRIX","QUADOBJ","QSECTION"};

    try
    {
        ifstream mpsFile(_opts->problem());

        if (!mpsFile.is_open())
            throw MpsSolverException("Mps file cannot be openend: " + _opts->problem());

        string line, oLine;

        while (getline(mpsFile, line))
        {
            oLine=line;
            line = StringTools::lTrim(line);

            if (!StringTools::startsWith(StringTools::lTrim(line), "*") && (!StringTools::lrTrim(line).empty()))
            {

                if (StringTools::startsWith(StringTools::upperCase(line),"NAME")) {
                    section=OBJSEC;
                    secStarted=true;
                } else if (StringTools::upperCase(line)=="ROWS") {

                    if ( _opts->solver()=="CPLEX" || _opts->solver()=="SCIP" || _opts->solver() == "GUROBI") {
                        _mpsOut << "OBJSENSE" << endl << "\t" << _opts->objSense() << endl;
                    }

                    section=ROWSEC;
                    secStarted=true;

                } else if (StringTools::upperCase(line)=="COLUMNS") {
                    section=COLSEC;
                    secStarted=true;

                } else if (StringTools::upperCase(line)=="RHS") {
                    section=RHSSEC;
                    secStarted=true;

                }  else if (StringTools::upperCase(line)=="BOUNDS") {
                    section=BNDSEC;
                    secStarted=true;

                } else if (StringTools::upperCase(line)=="ENDATA") {
                    section=UDFSEC;
                    secStarted=true;

                }  else if (StringTools::upperCase(line)=="SOS") {
                    if (_opts->solver()=="CPLEX" || _opts->solver()=="GUROBI" || _opts->solver()=="SCIP") {
                        secStarted=true;
                        section=SOSSEC;
                    } else{
                        throw MpsSolverException("SOS definitions have not yet been implemented");
                    }
                    //toDo

                }  else if (StringTools::findInStringVector(otherSecs,StringTools::upperCase(line)) ) {
                    if (_opts->solver()=="CPLEX" || _opts->solver()=="GUROBI" ) {
                        secStarted=true;
                    } else {
                        throw MpsSolverException(line+" definition have not yet been implemented");
                    }
                }

                if (secStarted) {
                    secStarted=false;
                    _mpsOut << oLine << endl;
                    continue;
                }


                if (section==OBJSEC) {
                    if (StringTools::upperCase(line)=="OBJSENSE" || StringTools::upperCase(line)=="OBJNAME") {
                        throw MpsSolverException("OBJNAME and OBJSENSE definitions have not yet been implemented");
                    }

                } else if (section==ROWSEC) {

                    string name, type;
                    StringTools::getOptionValue(line,type, name);

                    if  ( StringTools::upperCase(type)=="N" && !objFound) {
                        _opts->setObjName(name);
                        objFound=true;

                    } else {

                        ModelElement* con = new ModelElement();
                        con->setName(name);
                        con->setType(type);

                        if (type=="L")
                            con->setUpperBound(0);

                        if (type=="G")
                            con->setLowerBound(0);

                        if (type=="E") {
                            con->setLowerBound(0);
                            con->setUpperBound(0);
                        }

                        _constraints.push_back(con);

                        _rowNameMap[name] = _constraints[_nrOfCons];
                        _nrOfCons++;
                    }

                    _mpsOut << oLine << endl;
                    continue;

                } else if (section==COLSEC) {

                    if (StringTools::contains(StringTools::upperCase(line),"SOSORG") ) {
                        if (_opts->solver()!="CPLEX" && _opts->solver()!="GUROBI" && _opts->solver()!="SCIP") {
                            throw MpsSolverException("SOS definitions have not yet been implemented");
                        } else {
                            _mpsOut << oLine << endl;
                            continue;
                        }
                    }

                    if ( StringTools::contains(StringTools::upperCase(line),"INTORG") || (StringTools::contains(StringTools::upperCase(line),"INTEND"))  ) {

                        if ( StringTools::contains(StringTools::upperCase(line),"INTORG") ) {
                            intMarker=true;
                            _hasInteger=true;
                        } else {
                            intMarker=false;
                        }


                        if (_opts->intRelax()) {
                            intMarker=false;
                        } else {
                            _mpsOut << oLine << endl;
                        }
                        continue;
                    }

                    vector <string> colEntries (0);
                    StringTools::split(line,colEntries);

                    bool isRow=false;

                    string vName=colEntries[0];
                    colEntries.erase(colEntries.begin());

                    if (_colNameMap.find(vName) == _colNameMap.end()) {

                        ModelElement* var =  new ModelElement();
                        var->setName(vName);

                        if (intMarker) {
                            var->setType("I");
                        } else{
                            var->setType("C");
                        }

                        _variables.push_back(var);

                        _colNameMap[vName]=_variables[_nrOfVars];
                        _nrOfVars++;
                    }

                    string rName="";
                    double coeff=0;
                    for (string const& entry: colEntries) {

                        if(!isRow) {
                            rName=entry;
                            isRow=true;
                        } else {
                            if (StringTools::toDouble(entry,coeff)) {
                                _coeffs[rName][vName]=coeff;
                                isRow=false;
                                rName="";
                                coeff=0;
                            } else {
                                throw MpsSolverException("Error while reading MPS file <" +_opts->problem()+"> -wrong coefficient <"+entry+">");
                            }
                        }
                    }

                    _mpsOut << oLine << endl;
                    continue;


                } else if (section==RHSSEC) {

                    vector <string> rowEntries (0);
                    StringTools::split(line,rowEntries);

                    rowEntries.erase(rowEntries.begin());

                    string rName="";
                    double val=0;

                    ModelElement* con = nullptr;
                    bool isRow=false;

                    for (string const& entry: rowEntries) {

                        if(!isRow) {
                            rName=entry;
                            isRow=true;
                            con =_rowNameMap[rName];
                        } else {
                            if (StringTools::toDouble(entry,val) ) {

                                if(con->type()=="L") {
                                    con->setLowerBound(-INF);
                                    con->setUpperBound(val);
                                } else if(con->type()=="G") {
                                    con->setLowerBound(val);
                                    con->setUpperBound(INF);
                                } else if(con->type()=="E") {
                                    con->setLowerBound(val);
                                    con->setUpperBound(val);
                                } else if(con->type()=="N") {
                                    con->setLowerBound(-INF);
                                    con->setUpperBound(INF);
                                }

                                isRow=false;
                                rName="";
                            }  else {
                                throw MpsSolverException("Error while reading MPS file <" +_opts->problem()+"> -wrong RHS <"+entry+">");
                            }
                        }
                    }
                    _mpsOut << oLine << endl;
                    continue;


                } else if (section==BNDSEC) {


                    vector <string> colEntries (0);
                    StringTools::split(line,colEntries);

                    string vName=colEntries[2];
                    double bnd = -INF;
                    ModelElement* var = _colNameMap[vName];

                    if (colEntries.size()==4) {
                        if ( StringTools::toDouble(colEntries[3],bnd) ) {

                        }   else {
                            throw MpsSolverException("Error while reading MPS file <" +_opts->problem()+"> -wrong bound <"+colEntries[3]+">");
                        }
                    }

                    string bType = StringTools::upperCase(colEntries[0]);

                    if (bType=="LO") {
                        var->setLowerBound(bnd);

                    } else if (bType=="UP") {
                        if (var->lowerBound()<0)
                            var->setLowerBound(0);
                        var->setUpperBound(bnd);
                        if (bnd==1.0 && var->type()=="I")
                            var->setType("B");

                    }  else if (bType=="FX") {
                        var->setLowerBound(bnd);
                        var->setUpperBound(bnd);

                    } else if (bType=="FR") {
                        var->setLowerBound(-INF);
                        var->setUpperBound(INF);

                    } else if (bType=="MI") {
                        var->setLowerBound(-INF);
                        if (var->upperBound()<0)
                            var->setUpperBound(0);

                    } else if (bType=="PL") {
                        if (var->lowerBound()<0)
                            var->setLowerBound(0);
                        var->setUpperBound(INF);

                    }  else if (bType=="BV") {
                        var->setLowerBound(0);
                        var->setUpperBound(1);
                        var->setType("B");

                    }  else if (bType=="LI") {
                        var->setLowerBound(bnd);

                    } else if (bType=="UI") {
                        if (var->lowerBound()<0)
                            var->setLowerBound(0);
                        var->setUpperBound(INF);

                    }

                    _mpsOut << oLine << endl;
                    continue;

                } else if (section==SOSSEC) {
                    _mpsOut << oLine << endl;
                }

            } else {
                _mpsOut << oLine << endl;
            }
        }

        mpsFile.close();
    }
    catch (exception &e)
    {
        throw MpsSolverException("Problems while reading <" + _opts->problem() + "> " + string(e.what()));
    }

}





