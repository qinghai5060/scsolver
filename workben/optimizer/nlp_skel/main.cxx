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

#include "numeric/nlpmodel.hxx"
#include "myoptimizer.hxx"
#include "myfunctor.hxx"

#include <vector>

using namespace scsolver::numeric;
using namespace scsolver::numeric::nlp;
using std::vector;

int main (int argc, char *argv[])
{
    // Instantiate model and optimizer classes.
    Model* model         = new Model();
    BaseAlgorithm* opt   = new MyOptimizer();
    BaseFuncObj* functor = new MyFunctor();

    // The model has two variables.
    model->pushVar(0.0);
    model->pushVar(0.0);
    model->setFuncObject(functor);
    model->setVarBound(0, BOUND_LOWER,  1.0);
    model->setVarBound(0, BOUND_UPPER, 23.5);
    model->setVarBound(1, BOUND_LOWER,  0.0);
    model->setGoal(GOAL_MINIMIZE);
    model->print();

    // The optimization class receives the model, then optimizes it.  In 
    // production code, the solve() call needs to be surrounded with a 
    // try block because when the model is infeasible or something bad 
    // happens, the optimizer class throws an exception.  So, we need to 
    // catch it.
    opt->setModel(model);
    opt->solve();

    // Once the optimization is complete, get the solution.
    vector<double> sol;
    opt->getSolution(sol);

    printf("solution is: \n");
    size_t n = sol.size();
    for (size_t i = 0; i < n; ++i)
        printf("  x%d = %.2f\n", i+1, sol[i]);

    // Destroy class instances.
    delete opt;
    delete functor;
    delete model;
}
