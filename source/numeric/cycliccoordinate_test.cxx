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
#include "numeric/funcobj.hxx"
#include "numeric/type.hxx"
#include "numeric/nlpmodel.hxx"
#include "numeric/exception.hxx"

#include <cstdlib>
#include <memory>

using namespace ::scsolver::numeric;
using ::scsolver::numeric::nlp::CyclicCoordinate;
using ::std::string;
using ::std::vector;
using ::std::auto_ptr;

void debugPrint(const vector<double>& vars, const char* msg)
{
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

class TestFunc1 : public SimpleFuncObj
{
public:
    TestFunc1() :
        SimpleFuncObj(2)
    {
    }

    virtual ~TestFunc1()
    {
    }

    virtual double eval() const
    {
        vector<double> vars;
        getVars(vars);
        double term1 = vars[0] - 2;
        term1 *= term1*term1*term1;

        double term2 = vars[0] - 2.0*vars[1];
        term2 *= term2;

        return term1 + term2;
    }

    /**
     * Return a display-friendly function string.
     */
    virtual const string getFuncString() const
    {
        return string("(x1 - 2)^4 + (x1 - 2*x2)^2");
    }
};

class TestFunc2 : public SimpleFuncObj
{
public:
    TestFunc2() :
        SimpleFuncObj(3)
    {
        setVar(0, 0.0);
        setVar(1, 0.0);
        setVar(2, 0.0);
    }

    virtual ~TestFunc2()
    {
    }

    virtual double eval() const
    {
        vector<double> vars;
        getVars(vars);
        double x1 = vars[0], x2 = vars[1], x3 = vars[2];
        return (x1*x1 - 2)*(x2 + 5) + (1 - x2)*(4 + x3);
    }

    virtual const string getFuncString() const
    {
        return string("(x1*x1 - 2)*(x2 + 5) + (1 - x2)*(4 + x3)");
    }
};

void runTest1()
{
    fprintf(stdout, "runTest1: --begin\n");
    nlp::Model model;
    model.setGoal(GOAL_MINIMIZE);

    auto_ptr<BaseFuncObj> func1(new TestFunc1);
    model.setFuncObject(func1.get());
    model.pushVar(0.0);
    model.pushVar(3.0);

    auto_ptr<CyclicCoordinate> nlpSolver(new CyclicCoordinate);
    nlpSolver->setModel(&model);
    try
    {
        nlpSolver->solve();
    }
    catch (const ::std::exception& e)
    {
        fprintf(stdout, "runTest1:   standard exception: %s\n", e.what());
    }
    vector<double> sol;
    nlpSolver->getSolution(sol);
    debugPrint(sol, "solution");
    fprintf(stdout, "runTest1: --end\n");
}

void runTest(BaseFuncObj* pFuncObj)
{
    auto_ptr<BaseFuncObj> func(pFuncObj);
    nlp::Model model;
    model.setGoal(GOAL_MINIMIZE);
    model.setFuncObject(func.get());

    // Set initial variables to the mode instance.
    vector<double> vars;
    func->getVars(vars);
    vector<double>::const_iterator itr = vars.begin(), itrEnd = vars.end();
    for (; itr != itrEnd; ++itr)
        model.pushVar(*itr);

    auto_ptr<CyclicCoordinate> nlpSolver(new CyclicCoordinate);
    nlpSolver->setModel(&model);
    try
    {
        nlpSolver->solve();
        vector<double> sol;
        nlpSolver->getSolution(sol);
        debugPrint(sol, "solution");
    }
    catch (const ::std::exception& e)
    {
        fprintf(stdout, "  standard exception: %s\n", e.what());
    }
}

int main()
{
    runTest1();
//  runTest(new TestFunc2);
    return EXIT_SUCCESS;
}
