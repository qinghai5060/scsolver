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

#include "numeric/cycliccoordinate.hxx"
#include "numeric/exception.hxx"
#include "numeric/nlpmodel.hxx"
#include "numeric/funcobj.hxx"
#include "numeric/quadfitlinesearch.hxx"
#include "numeric/type.hxx"

#include <cmath>
#include <stdio.h>

using namespace ::scsolver::numeric;
using ::std::vector;

namespace scsolver { namespace numeric { namespace nlp {

CyclicCoordinate::CyclicCoordinate() :
    BaseAlgorithm(),
    m_maxIteration(2000),
    m_debug(false)
{
}

CyclicCoordinate::~CyclicCoordinate()
{
}

void CyclicCoordinate::debugPrint(const vector<double>& vars, const char* msg) const
{
    if (!m_debug)
        return;

    FILE* fs = stdout;
    fprintf(fs, "%s: (", msg);
    size_t n = vars.size();
    for (size_t i = 0; i < n; ++i)
    {
        if (i > 0)
            fprintf(fs, ", ");
        fprintf(fs, "%g", vars[i]);
    }
    fprintf(fs, ")\n");
}

static double calcDeltaDistance(const vector<double>& dists)
{
    double dist = 0.0;
    vector<double>::const_iterator itr = dists.begin(), itrEnd = dists.end();
    for (; itr != itrEnd; ++itr)
        dist += (*itr) * (*itr);

    dist = ::std::pow(dist, 1.0/static_cast<double>(dists.size()));
    return dist;
}

void CyclicCoordinate::solve()
{
    const double eps = 1.0e-5;

    Model& model = *getModel();
    model.print();
    BaseFuncObj& F = *model.getFuncObject();

    vector<double> vars;
    model.getVars(vars);
    double fval = F(vars);
    if (m_debug)
    {
        debugPrint(vars, "initial vars");
        printf("F(x) = %g\n", fval);
    }

    // Iterate cyclically.
    size_t varCount = vars.size();
    double dist = 0.0;
    vector<double> deltas(varCount);
    for (size_t i = 0; i < m_maxIteration; ++i)
    {
        if (m_debug)
            printf("ITERATION %d\n", i);

        for (size_t varIndex = 0; varIndex < varCount; ++varIndex)
        {
            SingleVarFuncObj& rSingleFuncObj = F.getSingleVarFuncObj(varIndex);
            QuadFitLineSearch qfit(&rSingleFuncObj);
            qfit.setGoal(GOAL_MINIMIZE);
            qfit.setDebug(false);
            double res = qfit.solve();
            F.setVar(varIndex, res);
            deltas[varIndex] = res - deltas[varIndex];
        }
        vector<double> tmpVars;
        F.getVars(tmpVars);
        dist = calcDeltaDistance(deltas);
        debugPrint(tmpVars, "  after the iteration"); 
        debugPrint(deltas, "  deltas");
        if (m_debug)
        {
            printf("  dist = %g\n", dist);
            printf("  F(x) = %g\n", F.eval());
        }
        if (dist < eps)
            return;

        deltas = tmpVars;
    }

    throw ModelInfeasible();
}

}}}
