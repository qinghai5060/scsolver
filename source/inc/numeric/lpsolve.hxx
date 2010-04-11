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

#ifndef _SCSOLVER_LPSOLVE_HXX_
#define _SCSOLVER_LPSOLVE_HXX_

#include "lpbase.hxx"
#include <memory>

namespace scsolver {

namespace numeric { namespace lp {

class LpSolveImpl;

/**
 * This class is a front-end for the lp_solve MILP library.
 */
class LpSolve : public BaseAlgorithm
{
public:
	LpSolve();
	virtual ~LpSolve() throw();

	virtual void solve();

private:
	std::auto_ptr<LpSolveImpl> m_pImpl;
};



}}}


#endif

