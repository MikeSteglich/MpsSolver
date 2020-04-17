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
#include <vector>
#include <chrono>
#include "MpsSolver.hh"
#include "ToolsAndData/MpsSolverException.hh"


using namespace std;

/**
 * @brief main
 * @param argc
 * @param argv
 * @return ret  Status of MpsSolver
 */
int main(int argc, const char *argv[])
{
    int ret = 0;
    vector<string> args(argv, argv + argc);

    try
    {
        MpsSolver prob = MpsSolver();
        prob.parseOptions(args);

        if (prob.opts()->actionVersion()) {
            prob.version();
        }

        if (prob.opts()->actionHelp()) {
            prob.help();
        }

        if (prob.opts()->actionRun()) {
            if (!prob.opts()->actionVersion())
                prob.version();
            prob.solve();
        }  
     
        ret = 0;
    }
    catch (MpsSolverException &e)
    {
        cerr << endl << "MpsSolver error: " << e.what() << endl;
        ret = -1;
    }
    catch (exception &ex)
    {
        cerr << endl << "Error: " << ex.what() << endl;
        ret = -1;
    }
    return ret;
}
