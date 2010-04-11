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

#include "numeric/hookejeeves.hxx"
#include "numeric/exception.hxx"
#include "numeric/nlpmodel.hxx"
#include "numeric/funcobj.hxx"
#include "numeric/quadfitlinesearch.hxx"

#include <cmath>
#include <stdio.h>

using namespace ::scsolver::numeric;
using ::std::vector;

namespace scsolver { namespace numeric { namespace nlp {

HookeJeeves::HookeJeeves() :
    BaseAlgorithm(),
    m_maxIteration(10000)
{
}

HookeJeeves::~HookeJeeves()
{
}

static void debugPrint(const vector<double>& vars, const char* msg, bool debug)
{
    if (!debug)
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

static double calcDeltaDistance(const vector<double>& pt1, const vector<double>& pt2)
{
    size_t n = pt1.size();
    if (n != pt2.size())
        throw Exception("size of the current point differs from the size of the previous point.");

    double dist = 0.0;
    for (size_t i = 0; i < n; ++i)
    {
        double one = pt2[i] - pt1[i];
        one *= one;
        dist += one;
    }

    dist = ::std::pow(dist, 1.0/static_cast<double>(n));
    return dist;
}

static void getPatternSearchVector(const vector<double>& pt1, const vector<double>& pt2, 
                                   vector<double>& vec)
{
    using ::scsolver::numeric::Exception;

    size_t n = pt1.size();
    if (n != pt2.size())
        throw Exception("size of the current point differs from the size of the previous point.");

    vector<double> tmp(n);
    for (size_t i = 0; i < n; ++i)
        tmp[i] = pt2[i] - pt1[i];
    vec.swap(tmp);
}

void HookeJeeves::solve()
{
    const double eps = 1.0e-6;
    bool debug = isDebug();
    Model& model = *getModel();
    model.print();
    BaseFuncObj& F = *model.getFuncObject();

    vector<double> vars;
    model.getVars(vars);
    double fval = F(vars);
    if (debug)
    {
        debugPrint(vars, "initial vars", debug);
        printf("F(x) = %g\n", fval);
    }

    // Iterate cyclically along the axes.
    size_t varCount = vars.size();
    vector<double> prevVars(vars);
    for (size_t i = 0; i < m_maxIteration; ++i)
    {
        if (debug)
            printf("ITERATION %d\n", i);

        for (size_t varIndex = 0; varIndex < varCount; ++varIndex)
        {
            SingleVarFuncObj& rSingleFuncObj = F.getSingleVarFuncObj(varIndex);
            QuadFitLineSearch qfit(&rSingleFuncObj);
            qfit.setGoal(GOAL_MINIMIZE);
            qfit.setDebug(false);
            double res = qfit.solve();
            F.setVar(varIndex, res);
        }
        vector<double> tmpVars;
        F.getVars(tmpVars);
        debugPrint(tmpVars, "  after the iteration", debug); 
        if (debug)
        {
            printf("  F(x) = %g\n", F.eval());
        }

        double dist = calcDeltaDistance(prevVars, tmpVars);
        if (debug)
            printf("  distance = %g\n", dist);

        if (dist < eps)
            return;
        
        vector<double> vec;
        getPatternSearchVector(prevVars, tmpVars, vec);
        debugPrint(vec, "pattern search vector", debug);
        SingleVarFuncObj& rSingleFuncObj = F.getSingleVarFuncObjByRatio(vec);
        QuadFitLineSearch qfit(&rSingleFuncObj);
        qfit.setGoal(GOAL_MINIMIZE);
        qfit.setDebug(false);
        qfit.solve();
        vector<double> tmpVars2;
        F.getVars(tmpVars2);
        debugPrint(tmpVars2, "end of pattern search", debug);
        
        prevVars = tmpVars2;
    }

    throw ModelInfeasible();
}

}}}
