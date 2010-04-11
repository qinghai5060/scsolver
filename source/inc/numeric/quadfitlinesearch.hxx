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

#ifndef _SCSOLVER_NUMERIC_QUADFIT_LINESEARCH_HXX_
#define _SCSOLVER_NUMERIC_QUADFIT_LINESEARCH_HXX_

#include "numeric/baselinesearch.hxx"
#include <cstddef>

namespace scsolver {
    class Timer;
}

namespace scsolver { namespace numeric {

class SingleVarFuncObj;

/** 
 * This class implements quadratic fit line search as described in p.280 of
 * Nonlinear Programming Theory and Algorithms, 2nd edition by Banaraa,
 * M.S., Sherali, H.D., and Shetty, C.M. 
 *  
 * The algorithm implemented here has been modified from the original one 
 * described in the above reference. 
 */
class QuadFitLineSearch : public BaseLineSearch
{
public:
    QuadFitLineSearch();
    explicit QuadFitLineSearch(SingleVarFuncObj* pFuncObj);
    virtual ~QuadFitLineSearch();

    virtual double solve();

private:
    size_t m_maxIteration;
};

}}

#endif
