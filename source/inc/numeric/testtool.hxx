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

#ifndef _SCSOLVER_NUMERIC_TESTTOOL_HXX_
#define _SCSOLVER_NUMERIC_TESTTOOL_HXX_

#ifndef SCSOLVER_UNITTEST
#warning "this header should only be used in unit-testing code."
#endif

#include "numeric/funcobj.hxx"
#include "numeric/baselinesearch.hxx"
#include "numeric/nlpbase.hxx"
#include "numeric/nlpmodel.hxx"
#include "numeric/type.hxx"
#include <stdio.h>
#include <string>
#include <memory>
#include <vector>

class SingleVarTestFuncBase : public ::scsolver::numeric::SingleVarFuncObj
{
public:
    virtual double getVar() const
    {
        return mVar;
    }

    virtual void setVar(double var)
    {
        mVar = var;
    }

private:
    double mVar;
};

template<typename DoubleContainerType>
void printContainer(typename DoubleContainerType::const_iterator begin, 
                    typename DoubleContainerType::const_iterator end, 
                    const char* msg)
{
    FILE* fs = stdout;
    fprintf(fs, "%s: (", msg);

    for (typename DoubleContainerType::const_iterator itr = begin; itr != end; ++itr)
    {
        if (itr != begin)
            fprintf(fs, ", ");
        fprintf(fs, "%g", *itr);
    }
    fprintf(fs, ")\n");
}

void runSingleVarTestFunc(::scsolver::numeric::BaseLineSearch* _lineSearch, 
                          ::scsolver::numeric::SingleVarFuncObj* _funcObj,
                          ::scsolver::numeric::GoalType _goal = ::scsolver::numeric::GOAL_MINIMIZE)
{
    using namespace ::scsolver::numeric;
    ::std::auto_ptr<BaseLineSearch> pLineSearch(_lineSearch);
    ::std::auto_ptr<SingleVarFuncObj> pFunc(_funcObj);
    pLineSearch->setFuncObj(pFunc.get());

    fprintf(stdout, "--------------------------------------------------------------------\n");
    fprintf(stdout, "now testing the function [ %s ]\n", pFunc->getFuncString().c_str());
    pLineSearch->setGoal(_goal);
    pLineSearch->setDebug(true);
    double x = pLineSearch->solve();

    switch (_goal)
    {
        case ::scsolver::numeric::GOAL_MAXIMIZE:
            fprintf(stdout, "maximization point (%g)\n", x);
        break;
        case ::scsolver::numeric::GOAL_MINIMIZE:
            fprintf(stdout, "minimization point (%g)\n", x);
        break;
        case ::scsolver::numeric::GOAL_UNKNOWN:
        default:
            // not handled
        break;
    }
}

/** 
 * Run a single test for non-linear programming algorithm.  The initial 
 * variables must be passed with the function object.  This function manages
 * the life cycles of the algorithm and the function object instances, so 
 * the calling function does not need to delete these instances once they 
 * are passed to this function. 
 * 
 * @param _nlpSolver new'ed NLP algorithm implementation instance.
 * @param _funcObj new'ed NLP function object instance.
 */
void runNonLinearTest(::scsolver::numeric::nlp::BaseAlgorithm* _nlpSolver, 
                      ::scsolver::numeric::BaseFuncObj* _funcObj)
{
    using namespace ::scsolver::numeric;

    ::std::auto_ptr<nlp::BaseAlgorithm> nlpSolver(_nlpSolver);
    ::std::auto_ptr<BaseFuncObj> func(_funcObj);
    nlp::Model model;
    model.setGoal(GOAL_MINIMIZE);
    model.setFuncObject(func.get());

    // Transfer initial variables from the function object to the mode instance.
    ::std::vector<double> vars;
    func->getVars(vars);
    ::std::vector<double>::const_iterator itr = vars.begin(), itrEnd = vars.end();
    for (; itr != itrEnd; ++itr)
        model.pushVar(*itr);

    nlpSolver->setModel(&model);
    nlpSolver->setDebug(true);
    try
    {
        nlpSolver->solve();
        ::std::vector<double> sol;
        nlpSolver->getSolution(sol);
        printContainer< ::std::vector<double> >(sol.begin(), sol.end(), "solution");
    }
    catch (const ::std::exception& e)
    {
        fprintf(stdout, "  standard exception: %s\n", e.what());
    }
}

#endif
