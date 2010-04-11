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

#include "numeric/rosenbrock.hxx"
#include "numeric/exception.hxx"
#include "numeric/nlpmodel.hxx"
#include "numeric/funcobj.hxx"
#include "numeric/quadfitlinesearch.hxx"
#include "numeric/matrix.hxx"
#include "tool/global.hxx"

#include <sstream>
#include <cmath>
#include <cassert>
#include <vector>
#include <stdio.h>

using ::scsolver::vectorToMatrix;
using namespace ::scsolver::numeric;
using ::std::vector;

namespace scsolver { namespace numeric { namespace nlp {

typedef vector<double> CoordinateVector;

Rosenbrock::Rosenbrock() :
    BaseAlgorithm(),
    m_maxIteration(10000)
{
}

Rosenbrock::~Rosenbrock()
{
}

static void debugPrint(const vector<double>& vars, const char* msg, bool debug, FILE* fs = stdout)
{
    if (!debug)
        return;

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

static void printCoordinateVectors(const vector<CoordinateVector>& cvectors, bool debug)
{
    if (!debug)
        return;

    FILE* fs = stdout;
    fprintf(fs, "Coordinate vectors\n");

    vector<CoordinateVector>::const_iterator itrBeg = cvectors.begin(), itrEnd = cvectors.end();
    for (vector<CoordinateVector>::const_iterator itr = itrBeg; itr != itrEnd; ++itr)
    {
        size_t index = distance(itrBeg, itr);
        ::std::ostringstream os;
        os << "  d" << index;
        debugPrint(*itr, os.str().c_str(), debug, fs);
    }
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

/** 
 * Calculate the norm of a vector.
 * 
 * @param mx matrix instance containing only a single row
 * 
 * @return double norm
 */
static double calcNorm(const Matrix& mx)
{
    if (mx.rows() > 1 && mx.cols() > 1)
        Exception("cannot calculate the norm of a non-vector matrix.");

    if (mx.empty())
        Exception("cannot calculate the norm of an empty matrix.");

    size_t n = mx.cols();
    double norm = 0.0;
    for (size_t i = 0; i < n; ++i)
        norm += mx(0, i)*mx(0, i);

    norm = sqrt(norm);
    return norm;
}

/** 
 * Update the coordinate vectors by the Gram-Schmidt procedure.
 * 
 * @param cvectors (reference) coordinate vectors
 * @param lambdas a set of lambda values.
 */
static void updateCoordinateVectors(vector<CoordinateVector>& cvectors, const vector<double>& lambdas, bool debug=false)
{
    if (debug)
        fprintf(stdout, "nlp::updateCoordinateVectors: --begin\n");

    // Assume that every matrix is a single row matrix unless stated otherwise.

    size_t n = cvectors.size();
    vector<Matrix> newVectors;
    newVectors.reserve(n);
    for (size_t j = 0; j < n; ++j)
    {
        if (debug)
            fprintf(stdout, "nlp::updateCoordinateVectors: Iteration %d (lambda = %g)\n", j, lambdas[j]);

        // First, calculate matrix a.
        Matrix a;
        if (lambdas[j] == 0)
            vectorToMatrix(cvectors[j], a);
        else
        {
            Matrix tmpa(1, n);
            for (size_t i = j; i < n; ++i)
            {
                if (debug)
                    fprintf(stdout, "nlp::updateCoordinateVectors:     i = %d\n", i);
                Matrix tmpd;
                vectorToMatrix(cvectors[i], tmpd);
                if (debug)
                {
                    fprintf(stdout, "nlp::updateCoordinateVectors:       tmpd = ");
                    tmpd.print();
                }
                tmpa += lambdas[i] * tmpd;
            }
            a.swap(tmpa);
        }

        if (debug)
        {
            fprintf(stdout, "nlp::updateCoordinateVectors:   calculated a = ");
            a.print();
        }

        // Next, calculate matrix b.
        Matrix b = a;
        for (size_t i = 0; i < j; ++i)
        {
            if (debug)
                fprintf(stdout, "nlp::updateCoordinateVectors:     i = %d\n", i);
            Matrix newd = newVectors[i];
            Matrix tmp = a * newd.trans() * newd; // row mx * col mx * row mx
            if (debug)
            {
                fprintf(stdout, "nlp::updateCoordinateVectors:       tmp = ");
                tmp.print();
            }
            b -= tmp;
        }
        if (debug)
        {
            fprintf(stdout, "nlp::updateCoordinateVectors:   calculated b = ");
            b.print();
        }
        double norm = calcNorm(b);
        Matrix newd = b/norm;
        newVectors.push_back(newd);
    }

    if (debug)
    {
        size_t n2 = newVectors.size();
        for (size_t i = 0; i < n2; ++i)
        {
            fprintf(stdout, "nlp::updateCoordinateVectors: new d%d = ", i);
            newVectors[i].print();
        }
    }

    // Now, update the coordinate vectors.
    assert(n == newVectors.size());
    for (size_t i = 0; i < n; ++i)
    {
        vector<double>& dest = cvectors[i];
        const Matrix& src  = newVectors[i];
        size_t colSize = src.cols();
        assert(colSize == dest.size());
        for (size_t j = 0; j < colSize; ++j)
            dest[j] = src(0, j);
    }

    if (debug)
        fprintf(stdout, "nlp::updateCoordinateVectors: --end\n");
}

void Rosenbrock::solve()
{
    const double eps = 1.0e-6;

    bool debug = isDebug();
    Model& model = *getModel();
    if (debug)
        model.print();
    BaseFuncObj& F = *model.getFuncObject();

    vector<double> vars;
    model.getVars(vars);
    size_t varCount = vars.size();
    double fval = F(vars);
    if (debug)
    {
        debugPrint(vars, "initial vars", debug);
        printf("F(x) = %g\n", fval);
    }

    // Generate the initial coordinate vectors which are identical to the
    // coordinate axes.
    vector<CoordinateVector> directions;
    directions.reserve(varCount);
    for (size_t i = 0; i < varCount; ++i)
    {
        CoordinateVector vec(varCount, 0.0);
        vec[i] = 1.0;
        directions.push_back(vec);
    }

    vector<double> prevVars(vars);
    vector<double> lambdas(varCount);
    for (size_t i = 0; i < m_maxIteration; ++i)
    {
        if (debug)
        {
            fprintf(stdout, "ITERATION %d\n", i);
            printCoordinateVectors(directions, debug);
        }

        for (size_t j = 0; j < varCount; ++j)
        {
            if (debug)
                fprintf(stdout, "  line search along d%d\n", j);

            try
            {
                SingleVarFuncObj& singleFunc = F.getSingleVarFuncObjByRatio(directions[j]);
                QuadFitLineSearch qfit;
                qfit.setFuncObj(&singleFunc);
                qfit.setDebug(false);
                double res = qfit.solve();
                lambdas[j] = res - prevVars[j];
            }
            catch (const ::std::exception& e)
            {
                // Keep going even if the line search fails.  In this case, just
                // use the last good position to calculate the lambda.
                if (debug)
                    fprintf(stdout, "  line search failed: %s\n", e.what());

                double res = F.getVar(j);
                lambdas[j] = res - prevVars[j];
            }
        }

        vector<double> tmpVars;
        F.getVars(tmpVars);
        debugPrint(tmpVars, "  variables", debug);
        debugPrint(lambdas, "  lambdas", debug);
        double dist = calcDeltaDistance(prevVars, tmpVars);
        if (debug)
            fprintf(stdout, "  distance = %g\n", dist);
        
        if (dist < eps)
            // solution found
            return;

        // Construct a new set of coordinate vectors.
        updateCoordinateVectors(directions, lambdas, false);
        
        prevVars = tmpVars;
    }

    throw ModelInfeasible();
}

}}}
