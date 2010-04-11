/****************************************************************************
 * Copyright (c) 2005-2009 Kohei Yoshida
 *               2009 Federico Zenith.
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

#include "numeric/funcobj.hxx"
#include <cmath>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

using namespace ::scsolver::numeric;
using namespace ::std;

/// Exception class for use in this test suite.
class TestFailed : public ::std::exception
{
public:
    /// Initialise the internal string.
    explicit TestFailed(const char* reason) : m_reason(reason) {}
    /// States that the destructor cannot throw anything.
    virtual ~TestFailed() throw() {}
    /// Reimplementation of the function in std::exception.
    const char* what() const throw() { return m_reason.c_str(); }
private:
    /// The error message.
    string m_reason;
};

/// A test functor representing a sum.
class TestBaseFunc1 : public SimpleFuncObj
{
public:
    /// Test-case constructor.
    /**
     * Initialises a functor with three internal variables.
     */
    TestBaseFunc1() : SimpleFuncObj(3) {}

    /// Functor evaluation.
    /**
     * The function's evaluation simply sums up the three variables.
     */
    virtual double eval() const
    {
        const double x1 = getVar(0);
        const double x2 = getVar(1);
        const double x3 = getVar(2);
        return x1 + x2 + x3;
    }

    /// Return a display-friendly function string.
    virtual const::std::string getFuncString() const
    {
        return string("x1 + x2 + x3");
    }
};

/// A test functor representing a sum.
class TestBaseFunc2 : public SimpleFuncObj
{
public:
    /// Test-case constructor.
    /**
     * Initialises a functor with six internal variables.
     */
    TestBaseFunc2() : SimpleFuncObj(6) {}

    /// Functor evaluation.
    /**
     * The function's evaluation simply sums up the six variables.
     */
    virtual double eval() const
    {
        const double x1 = getVar(0);
        const double x2 = getVar(1);
        const double x3 = getVar(2);
        const double x4 = getVar(3);
        const double x5 = getVar(4);
        const double x6 = getVar(5);
        return x1 + x2 + x3 + x4 + x5 + x6;
    }

    /// Return a display-friendly function string.
    virtual const::std::string getFuncString() const
    {
        return string("x1 + x2 + x3 + x4 + x5 + x6");
    }
};

/// Checks the behaviour of a functor whose variables are locked except one.
/**
 * This function receives a functor \a rBaseFunc whose variable \a varIndex is
 * the only one that can be modified, and is currently at value
 * \a varIndexValue. The function checks that this value is actually at the
 * given value, and that all others are exactly zero.
 */
void checkVarValues(const BaseFuncObj& rBaseFunc, const size_t varIndex,
                    const double varIndexValue)
{
    cout << "\tchecking variable values..." << endl;
    cout << "\t\t(";

    const size_t varCount = rBaseFunc.getVarCount();

    for (size_t i = 0; i < varCount; ++i)
    {
        const double varValue = rBaseFunc.getVar(i);
        if (i > 0)
            cout << ", ";
        cout << varValue;
        if (i == varIndex && varValue != varIndexValue)
                throw TestFailed("Variable value is incorrect.");
        if( i != varIndex && varValue != 0.0)
                throw TestFailed("Locked variable value is not zero.");
    }
    cout << ")" << endl;
}

/// Resets all variables in a functor to zero and check assignment.
/**
 * For a given functor, this function assigns a vector containing only zeroes
 * to its variables, and then checks that each of them is actually zero.
 */
void resetVarValues(BaseFuncObj& rFuncObj)
{
    const size_t varCount = rFuncObj.getVarCount();
    rFuncObj.setVars( vector<double>(varCount, 0.0) );

    for (size_t i = 0; i < varCount; ++i) {
        if (rFuncObj.getVar(i) != 0.0)
            throw TestFailed("After resetting, a variable is not zero.");
    }
}

/// Extracts the index of the first non-zero item of a vector.
/**
 * Takes in the \a ratios argument and finds the first non-zero item in the
 * vector. This function assumes that there actually is a non-zero ratio, if not
 * it will throw an exception.
 */
size_t getFirstNonZeroItem( const vector<double> ratios )
{
    for( size_t index = 0; index < ratios.size(); ++index ) {
        if( ratios[index] > numeric_limits<double>::epsilon() )
            return index;
    }
    throw TestFailed("I got a ratio vector with all components < eps.");
}

/// Checks the behaviour of a ratio-locked functor.
/**
 * This function checks that, for every ratio other than the master one, a
 * functor returns the same value for variables as the one that would be
 * expected by manually multiplying by the \a ratios argument.
 */
void checkVarRatio(BaseFuncObj& rFuncObj, const vector<double>& ratios)
{
    cout << "\tchecking ratio..." << endl;
    const size_t varCount = rFuncObj.getVarCount();
    if (varCount < 2)
        return;

    const size_t nonZeroIndex = getFirstNonZeroItem(ratios);
    const double functorMaster = rFuncObj.getVar(nonZeroIndex);
    const double vectorMaster = ratios.at(nonZeroIndex);
    for (size_t i = 0; i < varCount; ++i) {
        if( i == nonZeroIndex )
            continue;

        const double functorValue = rFuncObj.getVar(i);
        const double vectorValue  = functorMaster*ratios.at(i)/vectorMaster;
        const double delta = functorValue/vectorValue - 1.0;
        cout << "\t\tvalues: " << functorValue << " vs. ";
        cout << vectorValue << "\t(delta = " << delta << ")" << endl;

        /* Use _twice_ the epsilon given by numeric_limits, since both variables
         * functorValue and vectorValue can have been rounded off. */
        if ( fabs(delta) > 2.0*numeric_limits<double>::epsilon() )
            throw TestFailed("Ratio is incorrect.");
    }
}

/// Generate a random vector.
/**
 * Returns a vector of \c double floating-point pseudo-random numbers of size
 * given by argument \a size.
 *
 * @return generated vector of \c double random numbers.
 */
vector<double> getRandomVector( size_t size )
{
    vector<double> randomVector(size);
    for( size_t index = 0; index < size; ++index ) {
        randomVector[index] = double(rand())/double(RAND_MAX);
    }
    return randomVector;
}

/// Run tests for the locked-by-index and locked-by-ratio function views.
/**
 * This function takes a \a functor and checks that both its locked-by-index and
 * locked-by-ratio function views are able to set values and return them
 * correctly.
 */
void runTest(BaseFuncObj& functor)
{
    cout << endl << endl;
    cout << "running test on function: " << functor.getFuncString();
    cout << " (variable count: " << functor.getVarCount() << ")" << endl;
    cout << endl;

    const size_t varCount = functor.getVarCount();

    // Test the locked-by-index version.
    for (size_t i = 0; i < varCount; ++i) {
        resetVarValues(functor);
        SingleVarFuncObj& rSingleVarFunc = functor.getSingleVarFuncObj(i);
        for (size_t incStep = 0; incStep < 20; ++incStep) {
            const double newVar = rSingleVarFunc.getVar() + 3.0;
            rSingleVarFunc.setVar(newVar);
            checkVarValues(functor, i, newVar);
        }
    }

    resetVarValues(functor);

    // Test the locked-by-ratio version.
    const vector<double> ratios = getRandomVector( functor.getVarCount() );
    SingleVarFuncObj& rSingleVarFunc = functor.getSingleVarFuncObjByRatio(ratios);
    for (size_t incStep = 0; incStep < 20; ++incStep) {
        const double newVar = rSingleVarFunc.getVar() + 3.0;
        rSingleVarFunc.setVar(newVar);
        checkVarRatio(functor, ratios);
    }
}

int main()
{
    try {
        TestBaseFunc1 firstFunctor;
        TestBaseFunc2 secondFunctor;
        runTest( firstFunctor );
        runTest( secondFunctor );
        cout << "Test successful." << endl;
    } catch (const ::std::exception& e) {
        cout << e.what() << endl;
        return 1;
    }
    return 0;
}
