/****************************************************************************
 * Copyright (c) 2005-2009 Kohei Yoshida
 * 
 * This code is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3 only,
 * as published by the Free Software Foundation.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License version 3 for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * version 3 along with this work.  If not, see
 * <http://www.gnu.org/licenses/>.
 ****************************************************************************/
 
#ifndef _SOLVEMODEL_HXX_
#define _SOLVEMODEL_HXX_

#include <memory>

namespace scsolver {

namespace numeric {
	class Matrix;
}

class SolverImpl;
class SolveModelImpl;

/**
 * This class oversees the actual process of solving a model,
 * i.e. constructing a model from a sheet, passing it to the
 * backend model container, executing the algorithm to get a
 * solution, and (if successful) put the solution back into the
 * sheet.  This class should be the only class exposed to class
 * SolverImpl wrt solving action.
 */
class SolveModel
{
public:
	SolveModel( SolverImpl* );
	~SolveModel() throw();
	
	void solve();
	bool isSolved() const;
	
private:
	std::auto_ptr<SolveModelImpl> m_pImpl;	
};


}


#endif
