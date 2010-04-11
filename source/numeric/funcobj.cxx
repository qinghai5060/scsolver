/****************************************************************************
 * Copyright (c) 2005-2009 Kohei Yoshida
 *               2009 Federico Zenith
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

#include "numeric/exception.hxx"
#include "numeric/funcobj.hxx"

#include <cassert>
#include <cmath>
#include <limits>
#include <string>
#include <vector>

using ::std::vector;
using ::std::string;

namespace scsolver { namespace numeric {

/// Locks all variables of a functor except one.
/**
 * Wrapper class to use a multivariable function object as a single variable
 * one by locking all variables but one.
 */
class BaseFuncSingleObjImpl : public SingleVarFuncObj
{
public:
    /// The class constructor.
    BaseFuncSingleObjImpl(BaseFuncObj& rParent, size_t varIndex);
    /// Sets the free variable to \a var.
    virtual void setVar(double var) { m_rParent.setVar(freeIndex, var); }
    /// Returns the result of the free variable.
    virtual double getVar() const { return m_rParent.getVar(freeIndex); }
    /// Evaluates the function at the current value.
    virtual double eval() const { return m_rParent.eval(); }
    /// Gets the function string from the parent function.
    virtual const string getFuncString() const {
        return m_rParent.getFuncString();
    }

private:
    /// A reference to the parent function.
    BaseFuncObj& m_rParent;
    /// The index of the free variable.
    const size_t freeIndex;
};

/**
 * This constructor sets the object's parent function and the index of the
 * variable to let free, while all others are locked. The constructor also
 * asserts that the variable index is sane.
 */
BaseFuncSingleObjImpl::BaseFuncSingleObjImpl(BaseFuncObj& rParent,
                                             size_t varIndex) :
m_rParent(rParent),freeIndex(varIndex)
{
    assert( varIndex < m_rParent.getVarCount() );
}

/// A class to constrain a multivariable function on a line.
/**
 * This class wraps a multi-variable function object into a single-variable
 * one by locking the change ratio of the variables.  The variable in the
 * wrapping class will always be the 1st variable of the original function.
 * For instance, a 3-variable function can be turned into a single-variable
 * function by specifying change ratios of (3.0, 2.0, 1.5).  In this case,
 * the relationships x2 = 2.0/3.0 x x1 and x3 = 1.5/3.0 x x1 are 
 * established. 
 */
class BaseFuncRatioObjImpl : public SingleVarFuncObj
{
public:
    /// Constructor, setting the \a parent functor and the \a ratios.
    BaseFuncRatioObjImpl(BaseFuncObj& rParent, const vector<double>& ratios);

    /// Set a new value for the base-index variable.
    virtual void setVar(double var);
    /// Return the base-index variable's value.
    virtual double getVar() const { return m_rParent.getVar(baseRatioIndex); }
    /// Evaluates the function.
    virtual double eval() const { return m_rParent.eval(); }
    /// Return a function description.
    virtual const string getFuncString() const;

private:
    /// A support function used by the constructor.
    size_t findNonZeroIndex() const;

    /// The parent functor.
    BaseFuncObj& m_rParent;
    /// The vector of ratios identifying the direction of variation.
    const vector<double> m_ratios;
    /// The index of the reference variable.
    const size_t baseRatioIndex;
};

/**
 * The constructor stores the given \a parent and \a ratios into the private
 * variables. It then proceeds to check whether the variable count is larger
 * than zero and equal to the size of the \a ratios argument, otherwise it
 * throws an exception. It then proceeds to look for a non-zero reference
 * ratio.
 */
BaseFuncRatioObjImpl::BaseFuncRatioObjImpl(BaseFuncObj& rParent,
                                           const vector<double>& ratios) :
m_rParent(rParent),m_ratios(ratios),baseRatioIndex(findNonZeroIndex())
{
}

/**
 * This function seeks the first variable in the vector of ratios that is larger
 * than a minimum epsilon. It asserts that the variable count is larger than
 * zero and equal to the size of the \a ratios argument. If no variables are
 * larger found to be larger than epsilon, an exception is thrown.
 */
size_t BaseFuncRatioObjImpl::findNonZeroIndex() const
{
    const size_t totalVariables = m_rParent.getVarCount();
    assert(m_ratios.size() == totalVariables && totalVariables != 0);

    for (size_t i = 0; i < totalVariables; ++i) {
        if ( fabs( m_ratios[i] ) > std::numeric_limits<double>::epsilon() )
            return i;
    }
    throw Exception("BaseFuncRatioObjImpl::findNonZeroIndex(): variable index "
                    "not found");
}

/**
 * When given a new value \a var for the variable chosen as base, it updates
 * the variable in the parent functor and adjusts all others according to the
 * internal ratios.
 */
void BaseFuncRatioObjImpl::setVar(double var)
{
    const double delta = var - m_rParent.getVar(baseRatioIndex);
    const size_t n = m_ratios.size();
    const double baseRatio = m_ratios[baseRatioIndex];

    for(size_t i = 0; i < n; ++i) {
        if(i == baseRatioIndex)
            m_rParent.setVar( i, var );
        else {
            const double ratio = m_ratios[i]/baseRatio;
            const double newValue = m_rParent.getVar(i) + delta*ratio;
            m_rParent.setVar( i, newValue );
        }
    }
}

/**
 * Returns the parent's descriptive string, appended (if there are any
 * variables) with the current locked ratio.
 */
const string BaseFuncRatioObjImpl::getFuncString() const
{
    string funcStr = m_rParent.getFuncString();
    if (m_ratios.empty())
        return funcStr;

    funcStr += ": locked ratio (";
    const size_t n = m_ratios.size();
    for (size_t i = 0; i < n; ++i)
    {
        if (i > 0)
            funcStr += ", ";
        funcStr += m_ratios.at(i);
    }
    funcStr += ")";
    return funcStr;
}


/**
 * This constructor simply initialises the internal pointer to the
 * implementation to NULL.
 */
BaseFuncObj::BaseFuncObj() :
    m_pSVFuncObj(NULL)
{
}

/**
 * The copy constructor makes sure that the new object does not have the same
 * pointer to the implementation, and initialises m_pSVFuncObj to NULL.
 * It is not necessary to copy it because it is always created on the spot when
 * needed.
 */
BaseFuncObj::BaseFuncObj(const BaseFuncObj& r) :
    m_pSVFuncObj(NULL)
{
}

/**
 * Given a list of arguments provided as a vector to the parentheses operator,
 * this function returns the function evaluation after setting the internal
 * variables to \a vars.
 */
double BaseFuncObj::operator()(const vector<double>& vars)
{
    setVars(vars);
    return eval();
}

/**
 * This function generates a new single-variable functor for the present
 * multivariable functor, allowing only the variable identified by \a varIndex
 * to be changed. If one such functor had been previously allocated, it is
 * destroyed.
 * FIXME isn't it simpler to generate the object only if the pointer is NULL
 * FIXME (lazy evaluation), and simply call m_pSVFuncObj->setVar(varIndex)?
 * FIXME Or better yet, return by value, since BaseFuncObj does nothing with
 * FIXME m_pSVFuncObj?
 */
SingleVarFuncObj& BaseFuncObj::getSingleVarFuncObj(size_t varIndex)
{
    m_pSVFuncObj.reset(new BaseFuncSingleObjImpl(*this, varIndex));
    return *m_pSVFuncObj;
}

/**
 * This function generates a new single-variable functor for the present
 * multivariable functor, allowing change only in one particular direction.
 * If one such functor was previously allocated, it is destroyed.
 * FIXME see getSingleVarFuncObj(), same arguments.
 */
SingleVarFuncObj&
BaseFuncObj::getSingleVarFuncObjByRatio(const vector<double>& ratios)
{
    m_pSVFuncObj.reset(new BaseFuncRatioObjImpl(*this, ratios));
    return *m_pSVFuncObj;
}

/**
 * This constructor initialises the internal variable vector to the size
 * indicated by variable \a varCount.
 */
SimpleFuncObj::SimpleFuncObj(size_t varCount) :
m_vars(varCount)
{
}

/**
 * This assigns the values of m_vars to reference \a rVars, so that the caller
 * can access it.
 * TODO change code to return a vector. Check the code downstream!
 * FIXME this could cause problems if rVars.size() > m_vars.size().
 */
void SimpleFuncObj::getVars(vector<double>& rVars) const
{
    rVars.assign(m_vars.begin(), m_vars.end());
}

/**
 * If the given \a index is valid, it sets the value of the \a index-th
 * component of m_vars to \a var.
 */
void SimpleFuncObj::setVar(size_t index, double var)
{
    assert(index < m_vars.size());
    m_vars[index] = var;
}

/**
 * Sets the internal variables. The method is implemented in an exception-safe
 * way: vector::swap() is used after a copy of \a vars is initialised to avoid
 * that an exception leaves m_vars in a half-broken state.
 */
void SimpleFuncObj::setVars(const vector<double>& vars)
{
    vector<double> tmp(vars);
    m_vars.swap(tmp);
}

/**
 * This operator sets the variable of the functor to \a var and evaluates it,
 * returning the result.
 */
double SingleVarFuncObj::operator()(double var)
{
    setVar(var);
    return eval();
}

}}
