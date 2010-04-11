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
#include "numeric/funcobj.hxx"
#include "numeric/testtool.hxx"
#include <cmath>

using namespace ::scsolver::numeric;
using namespace ::std;

class TestFunc1 : public SingleVarTestFuncBase
{
public:
    virtual double eval() const
    {
        double var = getVar();
        return (var - 2.0)*(var*2.0 + 5.0) + 10.0;
    }

    virtual const string getFuncString() const
    {
        // 2*x^2 + x
        return string("(x - 2) * (2x + 5) + 10");
    }
};

class TestFunc2 : public SingleVarTestFuncBase
{
public:
    virtual double eval() const
    {
        double var = getVar();
        return ::std::sin(var) * var;
    }

    /**
     * Return a display-friendly function string (e.g. x^3 + 2*x^2 + 4).
     */
    virtual const string getFuncString() const
    {
        return string("sin(x) * x");
    }
};

class TestFunc3 : public SingleVarTestFuncBase
{
public:
    virtual double eval() const
    {
        double x = getVar();
        return x*(2*x-20)*(x*x+5*x+10);
    }

    /**
     * Return a display-friendly function string (e.g. x^3 + 2*x^2 + 4).
     */
    virtual const string getFuncString() const
    {
        return string("x*(2*x-20)*(x*x+5*x+10)");
    }
};

class TestFunc4 : public SingleVarTestFuncBase
{
public:
    virtual double eval() const
    {
        using namespace std;
        double x = getVar();
        return ((x-2)*(x*2+8) + 45)*(2*x+3)*cos(x/2);
    }

    /**
     * Return a display-friendly function string (e.g. x^3 + 2*x^2 + 4).
     */
    virtual const string getFuncString() const
    {
        return string("((x-2)*(x*2+8) + 45)*(2*x+3)*cos(x/2)");
    }
};

class TestFunc5 : public SingleVarTestFuncBase
{
public:
    virtual double eval() const
    {
        using namespace std;
        double x = getVar();
        return (x-2)*(x-2)*(x-2)*(x-2) + (x-6)*(x-6);
    }

    /**
     * Return a display-friendly function string (e.g. x^3 + 2*x^2 + 4).
     */
    virtual const::std::string getFuncString() const
    {
        return string("(x - 2)^4 + (x - 6)^2");
    }
};

class TestFunc6 : public SingleVarTestFuncBase
{
public:
    virtual double eval() const
    {
        double x = getVar();
        return x*x;
    }

    /**
     * Return a display-friendly function string (e.g. x^3 + 2*x^2 + 4).
     */
    virtual const::std::string getFuncString() const
    {
        return string("x^2");
    }
};

class TestFunc7 : public SingleVarTestFuncBase
{
public:
    virtual double eval() const
    {
        double x = getVar();
        return (x-1)*(x-1);
    }

    /**
     * Return a display-friendly function string (e.g. x^3 + 2*x^2 + 4).
     */
    virtual const::std::string getFuncString() const
    {
        return string("(x-1)^2");
    }
};

class TestFunc8 : public SingleVarTestFuncBase
{
public:
    virtual double eval() const
    {
        double x = getVar();
        return (x-2)*(x-2)*(x-2)*(x-2) + 0.006715617*x*x;
    }

    /**
     * Return a display-friendly function string (e.g. x^3 + 2*x^2 + 4).
     */
    virtual const::std::string getFuncString() const
    {
        return string("(x-2)^4 + (0.081948871*x)^2");
    }
};

int main()
{
    runSingleVarTestFunc(new BisectionSearch(-100, 100), new TestFunc1);
    runSingleVarTestFunc(new BisectionSearch(-10, -1),   new TestFunc2);
    runSingleVarTestFunc(new BisectionSearch(-100, 100), new TestFunc3);
    runSingleVarTestFunc(new BisectionSearch(-100, 100), new TestFunc4);
    runSingleVarTestFunc(new BisectionSearch(-100, 100), new TestFunc5);
    runSingleVarTestFunc(new BisectionSearch(-100, 50),  new TestFunc6);
    runSingleVarTestFunc(new BisectionSearch(-100, 50),  new TestFunc7);
    runSingleVarTestFunc(new BisectionSearch(-100, 50),  new TestFunc8);
}

