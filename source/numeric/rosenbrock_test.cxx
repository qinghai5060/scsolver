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
#include "numeric/funcobj.hxx"
#include "numeric/type.hxx"
#include "numeric/nlpmodel.hxx"
#include "numeric/exception.hxx"
#include "numeric/testtool.hxx"

using namespace ::scsolver::numeric;
using namespace ::std;
using ::scsolver::numeric::nlp::Rosenbrock;

class TestFunc1 : public SimpleFuncObj
{
public:
    TestFunc1() :
        SimpleFuncObj(2)
    {
        // initial values
        setVar(0, 0);
        setVar(1, 3);
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

int main()
{
    runNonLinearTest(new Rosenbrock, new TestFunc1);
}
