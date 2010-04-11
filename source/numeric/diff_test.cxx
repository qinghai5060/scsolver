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

#include "numeric/diff.hxx"
#include "numeric/funcobj.hxx"
#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#include <memory>

using ::std::vector;
using ::std::cout;
using ::std::endl;
using ::std::string;
using ::std::auto_ptr;

using namespace scsolver::numeric;

class TestFailed {};

class BaseTestFunc : public SimpleFuncObj
{
public:
    BaseTestFunc(size_t varIndex) : SimpleFuncObj(varIndex)
    {
    }

    virtual ~BaseTestFunc() throw() {}
    virtual double evalDef1() const = 0;
    virtual double evalDef2() const = 0;

private:
    BaseTestFunc();
};

class TestFunc : public BaseTestFunc
{
public:
    TestFunc() : BaseTestFunc(1)
    {
    }

    virtual ~TestFunc() throw()
    {
    }

	virtual const string getFuncString() const
	{
		return string("f(x) = 2x^2 + 5x");
	}

//  virtual const vector<double>& getVars() const
//  {
//      return m_Vars;
//  }
//
//  virtual void setVars(const vector<double>& vars)
//  {
//      vector<double> tmp(vars);
//      m_Vars.swap(tmp);
//  }
//
//  virtual void setVar(size_t index, double var)
//  {
//      m_Vars.at(index) = var;
//  }

    virtual double eval() const
    {
//      double x = m_Vars.at(0);
        double x = getVar(0);
        return (2*x + 5)*x;
    }

    virtual double evalDef1() const
    {
//      double x = m_Vars.at(0);
        double x = getVar(0);
        return 4*x + 5;
    }

    virtual double evalDef2() const
    {
        return 4;
    }

//private:
//    vector<double> m_Vars;
};

class TestFunc2 : public BaseTestFunc
{
public:
    TestFunc2() : BaseTestFunc(1)
    {
    }

    virtual ~TestFunc2() throw()
    {
    }

	virtual const string getFuncString() const
	{
		return string("f(x) = 2x^3 + 5x^2 - 2x");
	}

	virtual double eval() const
	{
        // ( (2*x + 5) * x - 2)*x
//      double x = m_Vars.at( 0 );
        double x = getVar(0);
        double f = ((2*x + 5)*x - 2)*x;
        return f;
	}

    virtual double evalDef1() const
    {
        // 6x^2 + 10x - 2 = (6x + 10) * x - 2
//      double x = m_Vars.at(0);
        double x = getVar(0);
        double f = (6*x + 10) * x - 2;
        return f;
    }

    virtual double evalDef2() const
    {
//      double x = m_Vars.at(0);
        double x = getVar(0);
        return 12*x + 10;
    }

//    virtual const vector<double>& getVars() const
//    {
//        return m_Vars;
//    }
//
//    virtual void setVars(const vector<double>& vars)
//    {
//        vector<double> tmp(vars);
//        m_Vars.swap(tmp);
//    }
//
//    virtual void setVar(size_t index, double var)
//    {
//        m_Vars.at(index) = var;
//    }
//
//private:
//    vector<double> m_Vars;
};

void checkDelta(double delta)
{
    cout << "delta = " << delta << endl;
    if (delta > 0.05)
        throw TestFailed();
}

void test(BaseTestFunc* pF)
{
    static const double vars[] = {0, 1.5, 10.0, 20.5, 50.0};
    for (int i = 0; i < 5; ++i)
    {
        double var = vars[i];
        cout << "----------------------------------------" << endl;
        cout << " x = " << var << endl;
//      vector<double> cnX;
//      cnX.push_back(var);
        auto_ptr<NumericalDiffer> pDiff( new NumericalDiffer );
//      pDiff->setVariables( cnX );
        pDiff->setVariable(var);
        pDiff->setPrecision(5);
        SingleVarFuncObj& rSingleFuncObj = pF->getSingleVarFuncObj(0);
        pDiff->setFuncObject(&rSingleFuncObj);
        double answer = pDiff->run();
        double delta = fabs(answer - pF->evalDef1())/fabs(answer);
        checkDelta(delta);
        
        pDiff->setSecondOrder(true);
        answer = pDiff->run();
        delta = (answer - pF->evalDef2())/answer;
        checkDelta(delta);
    }
}

int main()
{
    try
    {
        auto_ptr<TestFunc> pF( new TestFunc );
        test(pF.get());
        auto_ptr<TestFunc2> pF2( new TestFunc2 );
        test(pF2.get());
        cout << "Test passed!" << endl;
    }
    catch (const TestFailed& )
    {
        cout << "Test failed" << endl;
    }
}
















