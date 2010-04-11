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

#ifndef _SCSOLVER_NUMERIC_BISECTIONSEARCH_HXX_
#define _SCSOLVER_NUMERIC_BISECTIONSEARCH_HXX_

#include "numeric/baselinesearch.hxx"
#include <cstddef>

namespace scsolver { namespace numeric {

class SingleVarFuncObj;

class BisectionSearch : public BaseLineSearch
{
public:
    BisectionSearch(double lower, double upper);

    virtual ~BisectionSearch();

    virtual double solve();

    void setSearchInterval(double lower, double upper);

private:
    BisectionSearch(); // disabled
    
private:
    double m_lowerBound;
    double m_upperBound;
    size_t m_maxIteration;
};



}}


#endif
