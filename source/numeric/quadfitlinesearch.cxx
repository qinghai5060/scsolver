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

#include "numeric/quadfitlinesearch.hxx"
#include "numeric/exception.hxx"
#include "numeric/funcobj.hxx"
#include "numeric/polyeqnsolver.hxx"
#include "numeric/matrix.hxx"
#include "numeric/diff.hxx"
#include "tool/global.hxx"
#include "tool/timer.hxx"

#include <stdio.h>
#include <memory>
#include <vector>
#include <cmath>
#include <exception>
#include <string>
#include <sstream>

using ::std::auto_ptr;
using ::std::vector;
using ::std::exception;
using ::std::swap;
using ::std::fabs;
using ::std::string;

namespace scsolver { namespace numeric {

class StepCalculationFailed : public exception
{
public:
    virtual const char* what() const throw()
    {
        return "step calculation failed during quadratic fit search";
    }
};

class InitialPointsNotFound : public exception
{
public:
    virtual const char* what() const throw()
    {
        return "initial 3 points not found";
    }
};

struct QuadFitSearchData
{
    double P1;
    double P2;
    double P3;

    double StepLength;
    double XOffset;

    GoalType _GoalType;
    SingleVarFuncObj* pFunc;

    explicit QuadFitSearchData(SingleVarFuncObj* p) :
        StepLength(0.0),
        XOffset(0.0),
        pFunc(p)
    {
    }
};

enum ArmijoCheckStatus
{
    /**
     * unknown status
     */
    Armijo_Unknown,

    /**
     * initial check of f(step) <= f(0) + step*e*ff(0) failed, which means 
     * f(step) > f(0) + step*e*ff(0).
     */
    Armijo_InitialCheckFailed,

    /**
     * alpha check of f(step*alpha) <= f(0) + step*alphae*ff(0) failed.
     */
    Armijo_AlphaCheckFailed,

    /**
     * all conditions are met.
     */
    Armijo_Success
};

static ArmijoCheckStatus satisfiesArmijosRule(QuadFitSearchData& rData, double step, 
                                              const double e, const double alpha, 
                                              const double f0, const double ff0, 
                                              const double xoffset = 0.0,
                                              bool debug = false);

/** 
 * Find the largest step length that satisfies 
 *  
 *   f(step) <= f(0) + step * e * ff(0)
 *
 * @author kyoshida
 * 
 * @param pFunc 
 * @param step 
 * @param e 
 * @param alpha 
 * @param f0 
 * @param ff0 
 * 
 * @return largest step length that satisfies the above condition.
 */
static double findLargestStep(QuadFitSearchData& rData, double step, 
                              const double e, const double alpha, const double f0, const double ff0, const double xoffset)
{
//  fprintf(stdout, "numeric::findLargestStep: --begin (step = %g)\n", step);

    ArmijoCheckStatus status = satisfiesArmijosRule(rData, step, e, alpha, f0, ff0, false);
    if (status != Armijo_Success)
        // The initial step length already fails.  Just return the original 
        // step length.
        return step;

    double lastGoodStep = step;
    do
    {
        lastGoodStep = step;
        step *= alpha;

        rData.pFunc->setVar(step + xoffset);
        double testLeft = rData.pFunc->eval();
        double testRight = f0 + step*e*ff0;
//      fprintf(stdout, "numeric::findLargestStep:   step = %g; f left = %g; f right = %g\n",
//              step, testLeft, testRight);

        if (testLeft > testRight)
        {
//          fprintf(stdout, "numeric::findLargestStep:   test failed \n");
            // the condition "f(step) <= f(0) + step * e * ff(0)" is not met.
            break;
        }
    }
    while (true);

//  fprintf(stdout, "numeric::findLargestStep: --end (final step = %g)\n", lastGoodStep);
    return lastGoodStep;
}

/** 
 * Check if the step value satisfies Armijo's rule.
 *
 * @author kyoshida
 * 
 * @param pFunc 
 * @param step 
 * @param e 
 * @param alpha 
 * @param f0 
 * @param ff0 
 * 
 * @return true if the step value satisfies Armijo's rule, false otherwise.
 */
static ArmijoCheckStatus satisfiesArmijosRule(QuadFitSearchData& rData, double step, 
                                              const double e, const double alpha, 
                                              const double f0, const double ff0,
                                              const double xoffset,
                                              bool debug)
{
    if (debug)
        fprintf(stdout, "numeric::satisfiesArmijosRule: ---------- step = %g\n", step);

    // Check for f(step) <= f(0) + step * e * ff(0)

    rData.pFunc->setVar(step + xoffset);
    double testLeft = rData.pFunc->eval();
    double testRight = f0 + step*e*ff0;
    if (debug)
        fprintf(stdout, "numeric::satisfiesArmijosRule:   (initial check) step = %g; f left = %g; f right = %g\n",
                step, testLeft, testRight);

    if (testLeft > testRight)
        // the condition "f(step) <= f(0) + step * e * ff(0)" is not met.
        return Armijo_InitialCheckFailed;

    // Next, check for f(alpha*step) > f(0) + alpha*step * e * ff(0)

    rData.pFunc->setVar(step*alpha + xoffset);
    testLeft = rData.pFunc->eval();

    testRight = f0 + ff0*step*alpha*e;

    if (debug)
        fprintf(stdout, "numeric::satisfiesArmijosRule:   (alpha check)   step*alpha = %g; f left = %g; f right = %g\n",
                step*alpha, testLeft, testRight);

    if (testLeft <= testRight)
        return Armijo_AlphaCheckFailed;

    if (debug)
        fprintf(stdout, "numeric::satisfiesArmijosRule:   this satisfies Armijo's rule.\n");

    return Armijo_Success;
}

static void printArmijoStatus(ArmijoCheckStatus status)
{
    switch (status)
    {
        case Armijo_AlphaCheckFailed:
            printf("  alpha check failed\n");
        break;
        case Armijo_InitialCheckFailed:
            printf("  initial check failed\n");
        break;
        case Armijo_Success:
            printf("  success\n");
        break;
        case Armijo_Unknown:
            printf("  unknown\n");
        break;
        default:
            printf("  other\n");
    }
}

static void calcStepLength(QuadFitSearchData& rData, const ::scsolver::Timer* pTimer, bool debug)
{
    double step = 10;

    double e = 0.2, alpha = 2.0;

    NumericalDiffer diff;
    diff.setTimer(pTimer);
    diff.setFuncObject(rData.pFunc);
    diff.setPrecision(5);

    double xoffset = 0.0;

    // f'(0)
    diff.setVariable(xoffset);
    double ff0 = diff.run();

    if ((ff0 > 0 ? ff0 : -ff0) < 1.0e-16)
    {
        xoffset += 1.0;
        diff.setVariable(xoffset);
        ff0 = diff.run();
    }

    // f(0)
    rData.pFunc->setVar(xoffset);
    double f0 = rData.pFunc->eval();

    bool negativeDirection = false;
    if (ff0 > 0)
    {
        negativeDirection = true;
        step = -step;
    }

    if (debug)
        fprintf(stdout, "numeric::calcStepLength:   f(%g) = %g; ff(%g) = %g\n", xoffset, f0, xoffset, ff0);

    // loop until a first step value that satisfies Armijo's rule is found.
    ArmijoCheckStatus armijoStatus = Armijo_Unknown;
    bool checkNeverFailed = true;
    for (int i = 0; i < 4000000; ++i)
    {
        armijoStatus = satisfiesArmijosRule(rData, step, e, alpha, f0, ff0, xoffset, debug);
        if (debug)
            printArmijoStatus(armijoStatus);
        if (armijoStatus == Armijo_Success)
            break;

        checkNeverFailed = false;

        if (armijoStatus == Armijo_InitialCheckFailed)
        {
            step /= alpha;
            continue;
        }

        step *= alpha;
    }

    if (armijoStatus != Armijo_Success)
        throw StepCalculationFailed();

    if (checkNeverFailed)
        step = findLargestStep(rData, step, e, alpha, f0, ff0, xoffset);

    if (debug)
        fprintf(stdout, "numeric::calcStepLength: final step length = %g\n", step);

    rData.StepLength = step;
    rData.XOffset = xoffset;
}

/**
 * Find an initial set of points (P1, P2 and P3 in this order from left to 
 * right) such that P1 > P2 and P2 < P3.
 *  
 * @param data
 * 
 * @return bool true if successful, or false otherwise.
 */
static void findInitialPoints(QuadFitSearchData& data, const ::scsolver::Timer* pTimer, bool debug)
{
    const int maxIteration = 2000;

    // First, find an acceptable step length.
    calcStepLength(data, pTimer, debug);
    double step = data.StepLength;
    if (debug)
        fprintf(stdout, "numeric::findInitialPoints:   calculated step length = %g\n", data.StepLength);

    SingleVarFuncObj& F = *data.pFunc;

    // Now, find three points such that the middle point has the lowest f(x) 
    // value.  Note that the step length can be negative if the slope at the
    // x-offset position is descending in the negative-x direction.

    // initial point is x1 = 0, then the first test point is xt = x1 + step.
    double x1 = data.XOffset, x2 = data.XOffset, x3 = data.XOffset;
    double f1 = F(x1);
    bool pointsFound = false;
    if (debug)
        fprintf(stdout, "numeric::findInitialPoints:   initial x1 = %g\n", x1);
    if (f1 <= F(x1 + step))
    {
        if (debug)
            fprintf(stdout, "numeric::findInitialPoints:   F(x1) <= F(x1 + step) - keep halving the x1 - x3 interval\n");

        x3 = x1 + step;

        // Now, keep halving the x1 - x3 interval.
        for (int i = 0; i < maxIteration; ++i)
        {
            x2 = (x1 + x3)/2.0;
            double f2 = F(x2);
            double f3 = F(x3);
            if (debug)
                fprintf(stdout, "numeric::findInitialPoints:   f1 = %g; f2 = %g; f3 = %g\n", f1, f2, f3);
            if (!isfinite(f2) || !isfinite(f3))
            {
                if (debug)
                    fprintf(stdout, "numeric::findInitialPoints:   either f2 or f3 is not a number.\n");
                break;
            }
            if (f2 < f1 && f2 < f3)
            {
                pointsFound = true;
                break;
            }

            x3 = x2;
        }
    }
    else
    {
        if (debug)
            fprintf(stdout, "numeric::findInitialPoints:   F(x1) > F(x1 + step) : keep doubling the x1 - x2 interval\n");

        x2 = x1 + step;

        // Keep doubling the x1 - x2 interval.
        for (int i = 0; i < maxIteration; ++i)
        {
            x3 = x1 + (x2 - x1)*2;
            if (debug)
                fprintf(stdout, "numeric::findInitialPoints:   iter %d: x1 = %g; x2 = %g; x3 = %g\n", i, x1, x2, x3);
            double f2 = F(x2);
            double f3 = F(x3);
            if (debug)
                fprintf(stdout, "numeric::findInitialPoints:   f1 = %g; f2 = %g; f3 = %g\n", f1, f2, f3);
            if (!isfinite(f2) || !isfinite(f3))
            {
                if (debug)
                    fprintf(stdout, "numeric::findInitialPoints:   either f2 or f3 is not a number.\n");
                break;
            }
            if (f2 < f1 && f2 < f3)
            {
                pointsFound = true;
                break;
            }

            x2 = x3;
        }
    }

    if (!pointsFound)
        throw InitialPointsNotFound();

    // Optionally re-order the points so that x1 < x2 < x3.
    if (x1 > x3)
        ::std::swap(x1, x3);

    data.P1 = x1;
    data.P2 = x2;
    data.P3 = x3;
}

class PrefixedSingleVarFuncObj : public SingleVarFuncObj
{
public:
    explicit PrefixedSingleVarFuncObj(SingleVarFuncObj& rParent, double prefix) :
        m_rParent(rParent), m_prefix(prefix)
    {
    }

    virtual double eval() const
    {
        return m_prefix * m_rParent.eval();
    }

    virtual const::std::string getFuncString() const
    {
        ::std::ostringstream os;
        os << m_prefix << " * f(x) where f(x) = ";
        os << m_rParent.getFuncString();
        return os.str();
    }

    virtual double getVar() const
    {
        return m_rParent.getVar();
    }

    virtual void setVar(double var)
    {
        m_rParent.setVar(var);
    }

    void setPrefix(double prefix)
    {
        m_prefix = prefix;
    }

private:
    SingleVarFuncObj& m_rParent;
    double m_prefix;
};

// --------------------------------------------------------------------------

QuadFitLineSearch::QuadFitLineSearch() :
    BaseLineSearch(NULL),
    m_maxIteration(500)
{
}

QuadFitLineSearch::QuadFitLineSearch(SingleVarFuncObj* pFuncObj) :
    BaseLineSearch(pFuncObj),
    m_maxIteration(500)
{
}

QuadFitLineSearch::~QuadFitLineSearch()
{
}

double QuadFitLineSearch::solve()
{
    SingleVarFuncObj* pOrigFuncObj = getFuncObj();
    bool debug = isDebug();
    const ::scsolver::Timer* pTimer = getTimer();

    if (!pOrigFuncObj)
        throw Exception("QuadFitLineSearch::solve: no function object set");

    PrefixedSingleVarFuncObj prefixedFuncObj(*pOrigFuncObj, 1.0);
    if (getGoal() == GOAL_MAXIMIZE)
        prefixedFuncObj.setPrefix(-1.0);

    SingleVarFuncObj* pFuncObj = &prefixedFuncObj;

    if (debug)
        fprintf(stdout, "QuadFitLineSearch::solve:   function = %s\n", pFuncObj->getFuncString().c_str());
    QuadFitSearchData data(pFuncObj);
    data._GoalType = getGoal();

    // 1. Find three points such that the 2nd point be the lowest.
    // 2. Find the quadratic function that passes through all three points.
    // 3. Find the minimizer point of that quadratic function.
    // 4. Replace one of the three points with the minimizer point based on
    //    some conditions.
    // 5. Terminate when P1 - P3 < e.

    findInitialPoints(data, pTimer, debug);

    if (debug)
        fprintf(stdout, "QuadFitLineSearch::solve:   initial points: p1 = %g; p2 = %g; p3 = %g\n",
                data.P1, data.P2, data.P3);

    SingleVarFuncObj& F = *pFuncObj;

    const double eps = 1.0e-6; // tolerance limit (epsilon)

    bool solutionFound = false;
    double oldRange = data.P3 - data.P1;;
    for (size_t i = 0; i < m_maxIteration; ++i)
    {
        if (pTimer && pTimer->isTimedOut())
            throw IterationTimedOut();

        if (debug)
            fprintf(stdout, "QuadFitLineSearch::solve: ITERATION %d\n", i);

        // Solve the quadratic function.
        PolyEqnSolver eqnSolver;
        eqnSolver.addDataPoint(data.P1, F(data.P1));
        eqnSolver.addDataPoint(data.P2, F(data.P2));
        eqnSolver.addDataPoint(data.P3, F(data.P3));
        Matrix sol = eqnSolver.solve();
        if (debug)
        {
            fprintf(stdout, "QuadFitLineSearch::solve:   3-pt quad equation: ");
            sol.trans().print();
        }
    
        // Get the peak of that quad function.
        double x, y;
        getQuadraticPeak(x, y, sol);
        if (debug)
            fprintf(stdout, "QuadFitLineSearch::solve:   peak at (%g, %g)\n", x, y);
    
        if (data.P1 > x || x > data.P3)
        {
#if 1
            // For now we call it a 'solution' and move on when the quad peak
            // occurs outside the current range.  Since a line search is
            // allowed to be inexact, this should not cause too much trouble.

            solutionFound = true;
            break;
#else
            ::std::ostringstream os;
            os << "calculated quadratic peak is not between P1 and P3";
            os << "  P1 = " << data.P1 << "  P3 = " << data.P3 << "  quadratic peak = " << x;
            throw Exception(os.str());
#endif
        }

        double delta = x - data.P2;

        if (debug)
            fprintf(stdout, "QuadFitLineSearch::solve:   delta = %g\n", delta);

        if ((delta > 0 ? delta : -delta) < 3.89e-15)
        {
            // case 3
            if (debug)
                fprintf(stdout, "QuadFitLineSearch::solve:   case 3 (equality)\n");

            if (data.P3 - data.P1 < eps)
            {
                solutionFound = true;
                break;
            }
                
            double l1 = data.P2 - data.P1, l2 = data.P3 - data.P2;
            if (l1 > l2)
            {
                if (l1 > eps/2.0)
                    data.P2 -= eps*1.0e-1;
                else
                    throw exception();
            }
            else
            {
                if (l2 > eps/2.0)
                    data.P2 += eps*1.0e-1;
                else
                    throw exception();
            }
        }
        else if (delta > 0)
        {
            // case 1 - test point is on the right side of P2.
            if (debug)
                fprintf(stdout, "QuadFitLineSearch::solve:   case 1 (test > P2)\n");

            if (F(x) >= F(data.P2))
            {
                data.P3 = x;
                if (data.P2 - data.P3 < 3.89e-15)
                    data.P3 += eps*1.0e-1;
            }
            else
            {
                swap(data.P1, data.P2);
                data.P2 = x;
            }
        }
        else if (delta < 0)
        {
            // case 2 - test point is on the left side of P2.
            if (debug)
                fprintf(stdout, "QuadFitLineSearch::solve:   case 2 (test < P2)\n");

            double valTest = F(x), valP2 = F(data.P2);
            if (debug)
            {
                fprintf(stdout, "QuadFitLineSearch::solve:     F(test) = %g; F(P2) = %g (delta = %g)\n", 
                        valTest, valP2, valTest - valP2);
            }

            if (valTest >= valP2)
            {
                if (debug)
                    fprintf(stdout, "QuadFitLineSearch::solve:     F(test) >= F(P2)\n");

                data.P1 = x;
                if (data.P1 - data.P2 < 3.89e-15)
                    data.P1 -= eps*1.0e-1;
            }
            else
            {
                if (debug)
                    fprintf(stdout, "QuadFitLineSearch::solve:     F(test) < F(P2)\n");

                swap(data.P2, data.P3);
                data.P2 = x;
            }
        }
        else
        {
            throw Exception("unknown condition encountered");
        }

        double newRange = data.P3 - data.P1;
        if (debug)
        {
            fprintf(stdout, "QuadFitLineSearch::solve:   new points: p1 = %g; p2 = %g; p3 = %g (range = %g; delta = %g)\n",
                    data.P1, data.P2, data.P3, newRange, oldRange - newRange);
        }

        if (newRange < eps)
        {
            solutionFound = true;
            break;
        }

        double l1 = data.P2 - data.P1;
        double l2 = data.P3 - data.P2;
        if (debug)
            fprintf(stdout, "QuadFitLineSearch::solve:   l1 = %g; l2 = %g\n", data.P2 - data.P1, data.P3 - data.P2);

        if (l1 < eps/2.0 && l2 > eps/2.0)
        {
            // Length between P1 and P2 is less than half the epsion.  Push P2 away from P1 to make more room.
            data.P2 = data.P1 + eps/2.0;
            if (debug)
                fprintf(stdout, "QuadFitLineSearch::solve:   l1 is less than half the epsilon\n");
        }
        else if (l1 > eps/2.0 && l2 < eps/2.0)
        {
            // Length between P2 and P3 is less than half the epsion.  Push P2 away from P3 to make more room.
            data.P2 = data.P3 - eps/2.0;
            if (debug)
                fprintf(stdout, "QuadFitLineSearch::solve:   l2 is less than half the epsilon\n");
        }

        oldRange = newRange;
    }

    if (!solutionFound)
        throw MaxIterationReached();

    if (debug)
        fprintf(stdout, "QuadFitLineSearch::solve: final points: p1 = %g; p2 = %g; p3 = %g\n",
                data.P1, data.P2, data.P3);

    return data.P2;
}

}}
