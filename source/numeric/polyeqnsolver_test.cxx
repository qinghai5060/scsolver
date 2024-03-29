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

#include "numeric/polyeqnsolver.hxx"
#include "numeric/matrix.hxx"
#include <list>
#include <cmath>
#include <stdio.h>

using namespace ::scsolver::numeric;
using namespace ::std;

class TestFailed {};

class PolyEqnSolverTest : public PolyEqnSolver
{
public:
    void addDataPoint(double x, double y)
    {
        fprintf(stdout, "PolyEqnSolverTest::addDataPoint: adding (%g, %g)\n", x, y);
        DataPoint pt(x, y);
        m_DataPoints.push_back(pt);
        PolyEqnSolver::addDataPoint(x, y);
    }

    const Matrix solve()
    {
        fprintf(stdout, "PolyEqnSolverTest::solve: ------------------------------\n");
        Matrix sol = PolyEqnSolver::solve();

        if (sol.cols() != 1)
        {
            printf("solution must be a single-column matrix.\n");
            throw TestFailed();
        }

        printf("solution = ");
        sol.trans().print(5);
        verifySolution(sol);
        return sol;
    }

    void clear()
    {
        PolyEqnSolver::clear();
        size_t n = PolyEqnSolver::size();
        if (n != 0)
        {
            fprintf(stdout, "PolyEqnSolverTest::clear: data point size is not zero.\n");
            throw TestFailed();
        }

        // Don't forget to clear this too.
        m_DataPoints.clear();
    }

private:

    void verifySolution(const Matrix& solution)
    {
        size_t deltaCount = 0;
        size_t n = solution.rows();
        list<DataPoint>::const_iterator itr = m_DataPoints.begin(), itrEnd = m_DataPoints.end();
        for (; itr != itrEnd; ++itr)
        {
            double verTerm = 1.0, leftSum = 0.0;
            for (size_t i = 0; i < n; ++i)
            {
                leftSum += solution(i, 0) * verTerm;
                verTerm *= itr->X;
            }

            if (itr->Y != leftSum)
            {
                printf("  delta = %.20f\n", itr->Y - leftSum);
                ++deltaCount;
            }
        }

        if (!deltaCount)
            printf("solution verified\n");
        else
        {
            printf("delta count = %d\n", deltaCount);
            throw TestFailed();
        }
    }

    list<DataPoint> m_DataPoints;
};

//--------------------------------------------------------------------

class QuadPeakTest
{
public:
    QuadPeakTest()
    {
    }

    void set(double a, double b, double c)
    {
        printf("--------------------------------------------------------------------\n");
        m_A = a;
        m_B = b;
        m_C = c;
        Matrix coef(3, 1);
        coef(0, 0) = m_C;
        coef(1, 0) = m_B;
        coef(2, 0) = m_A;
        getQuadraticPeak(m_X, m_Y, coef);
    }

    void print() const
    {
        printf("f(x) = %g x^2 ", m_A);
        if (m_B >= 0.0)
            printf("+ ");
        else
            printf("- ");
        printf("%g x ", fabs(m_B));
        if (m_C >= 0.0)
            printf("+ ");
        else
            printf("- ");
        printf("%g : f(x) peaks at (%g, %g)\n", m_C, m_X, m_Y);
    }

    void verify() const
    {
        static const double step = 1.0;
        static const int count = 10;

        double lambda = step;
        for (int i = 0; i < count; ++i)
        {
            double left  = eval(m_X - lambda);
            double right = eval(m_X + lambda);
            double delta = fabs(left - right);
            //printf("  f(%.2f) = %.3f  f(%.2f) = %.3f  (delta = %.4f)\n", m_X - lambda, left, m_X + lambda, right, delta);
            if (delta/left > 0.00000000000005)
            {
                printf("  delta is not zero (%.20f)\n", delta/left);
                throw TestFailed();
            }

            lambda += step;
        }
        printf("quadratic peak verified\n");
    }

private:

    double eval(double x) const
    {
        return x*x*m_A + x*m_B + m_C;
    }

    double m_A; // coefficient of the x^2 term
    double m_B; // coefficient of the x^1 term
    double m_C; // coefficient of the x^0 term (constant)

    double m_X;
    double m_Y;
};

void runTest()
{
    PolyEqnSolverTest polySolver;
    polySolver.addDataPoint(1.0, 32.0);
    polySolver.addDataPoint(5.0, 2.0);
    polySolver.addDataPoint(9.0, 10.0);
    polySolver.solve();

    polySolver.clear();
    polySolver.addDataPoint(0.0, 2.0);
    polySolver.addDataPoint(2.0, 6.0);
    polySolver.solve();

    polySolver.clear();
    polySolver.addDataPoint(1, 12);
    polySolver.addDataPoint(2, 8);
    polySolver.addDataPoint(3, 7);
    polySolver.solve();

    try
    {
        polySolver.clear();
        polySolver.solve();
        throw TestFailed();
    }
    catch( const NotEnoughDataPoints& )
    {
        printf("NotEnoughDataPoints exception caught on zero data point (expected).\n");
    }

    try
    {
        polySolver.clear();
        polySolver.addDataPoint(1.0, 1.0);
        polySolver.solve();
        throw TestFailed();
    }
    catch( const NotEnoughDataPoints& )
    {
        printf("NotEnoughDataPoints exception caught on 1 data point (expected).\n");
    }
}

void runQuadPeakTest()
{
    QuadPeakTest qpt;
    for (int a = -8; a < 9; ++a)
    {
        for (int b = -4; b < 5; ++b)
        {
            for (int c = -10; c < 11; ++c)
            {
                qpt.set(a, b, c);
                qpt.print();
                qpt.verify();
            }
        }
    }
}

int main()
{
    try
    {
        runTest();
        runQuadPeakTest();
    }
    catch ( const TestFailed& )
    {
        printf("***************************\n");
        printf("*    UNIT TEST FAILED...  *\n");
        printf("***************************\n");
        return 1;
    }

    printf("***************************\n");
    printf("*    UNIT TEST PASSED!!!  *\n");
    printf("***************************\n");
}
