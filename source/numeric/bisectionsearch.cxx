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

#include "numeric/bisectionsearch.hxx"
#include "numeric/exception.hxx"
#include "numeric/funcobj.hxx"
#include "numeric/diff.hxx"

namespace scsolver { namespace numeric {

BisectionSearch::BisectionSearch(double lower, double upper) :
    BaseLineSearch(),
    m_lowerBound(lower),
    m_upperBound(upper),
    m_maxIteration(100)
{
}

BisectionSearch::~BisectionSearch()
{
}

double BisectionSearch::solve()
{
    SingleVarFuncObj* pFuncObj = getFuncObj();
    bool debug = isDebug();
    if (!pFuncObj)
        throw Exception("BisectionSearch::solve: no function object set");

    if (debug)
        fprintf(stdout, "BisectionSearch::solve:   function = %s\n", pFuncObj->getFuncString().c_str());

    SingleVarFuncObj& F = *pFuncObj;
    NumericalDiffer diff;
    diff.setFuncObject(pFuncObj);
    double oldInterval = 0.0;

    for (size_t i = 0; i < m_maxIteration; ++i)
    {
        if (debug)
            fprintf(stdout, "BisectionSearch::solve: ITERATION %d\n", i);

        double interval = m_upperBound - m_lowerBound;
        if (interval < 3.89e-15 || oldInterval == interval)
        {
            if (i == 0)
                throw Exception("BisectionSearch::solve: interval is already too small in the first iteration");

            // range has become significantly small, or the interval has not 
            // shrunk since the previous interation.
            return m_lowerBound;
        }

        double x = m_lowerBound + interval/2.0;
        diff.setVariable(x);
        double ff = diff.run();
        if (debug)
        {
            fprintf(stdout, "BisectionSearch::solve:   f(%g) = %g; f'(%g) = %g\n", 
                    x, F(x), x, ff);
            fprintf(stdout, "BisectionSearch::solve:   range: %g - %g (interval = %g)\n",
                    m_lowerBound, m_upperBound, interval);
        }

        if ((ff > 0 ? ff : -ff) < 3.89e-15)
            // f'(x) is close enough to zero to be zero-equivalent.
            return x;

        if (ff > 0)
            // minimum occurs to the left of current position.
            m_upperBound = x;
        else
            // minimum occurs to the right of current position.
            m_lowerBound = x;

        oldInterval = interval;
    }

    throw MaxIterationReached();
    return 0.0;
}

}}
