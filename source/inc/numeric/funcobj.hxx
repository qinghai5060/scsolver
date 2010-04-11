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

#ifndef _SCSOLVER_NUMERIC_FUNCOBJ_HXX_
#define _SCSOLVER_NUMERIC_FUNCOBJ_HXX_

#include <exception>
#include <memory>
#include <string>
#include <vector>

namespace scsolver { namespace numeric {

class SingleVarFuncObj;

/// Base class for function object to be used for NLP algorithms.
class BaseFuncObj
{
public:
    /// The constructor.
    BaseFuncObj();
    /// The copy constructor.
    BaseFuncObj(const BaseFuncObj& r);
    /// The class destructor, declared virtual.
    virtual ~BaseFuncObj() {}

    /// Returns all variables setting them in the vector \a rVars.
    virtual void getVars(::std::vector<double>& rVars) const = 0;
    /// Returns a variable indicated by \a index.
    virtual double getVar(size_t index) const = 0;
    /// Sets the internal variables to the ones contained in vector \a vars.
    virtual void setVars(const ::std::vector<double>& vars) = 0;
    /// Sets one specific variable, indicated by \a index, to \a var.
    virtual void setVar(size_t index, double var) = 0;
    /// Returns the current number of variables.
    virtual size_t getVarCount() const = 0;
    /// Evaluates the function.
    virtual double eval() const = 0;
    /// Return a display-friendly function string.
    virtual const ::std::string getFuncString() const = 0;
    /// Evaluates the function for variables \a vars.
    double operator()(const ::std::vector<double>& vars);
    /// Returns a functor to change only one variable.
    SingleVarFuncObj& getSingleVarFuncObj(size_t varIndex);
    /// Returns a functor to change in one specific direction.
    SingleVarFuncObj& getSingleVarFuncObjByRatio(const ::std::vector<double>& ratios);

private:
    /// Pointer to the private implementation.
    /**
     * FIXME it might just be unnecessary to keep a pointer to it around.
     */
    ::std::auto_ptr<SingleVarFuncObj> m_pSVFuncObj;
};

/// An abstract subclass of BaseFuncObj with some implementation.
/**
 * This subclass of BaseFuncObj is still abstract
 */
class SimpleFuncObj : public BaseFuncObj
{
public:
    /// The default constructor, does nothing particular.
    SimpleFuncObj() {}
    /// A constructor from a variable count.
    explicit SimpleFuncObj(size_t varCount);
    /// The class destructor, declared virtual.
    virtual ~SimpleFuncObj() {}

    /// Allow access to the vector of variables.
    virtual void getVars(::std::vector<double>& rVars) const;
    /// Get one particular variable from the vector.
    virtual double getVar(size_t index) const { return m_vars[index]; }
    /// Set one specific variable to a value.
    virtual void setVar(size_t index, double var);
    /// Set the entire variable vector.
    virtual void setVars(const ::std::vector<double>& vars);
    /// Returns the number of variables internally stored.
    virtual size_t getVarCount() const { return m_vars.size(); }

private:
    /// The stored variables.
    ::std::vector<double> m_vars;
};


/// Non-linear function object that only has one variable.
/**
 * These objects are used for line-search algorithms.
 */
class SingleVarFuncObj
{
public:
    /// Sets the internal variable to \a var.
    virtual void setVar(double var) = 0;
    /// Returns the internal variable.
    virtual double getVar() const = 0;
    /// Returns the function evaluation.
    virtual double eval() const = 0;

    /// Return a display-friendly function string (e.g. x^3 + 2*x^2 + 4).
    virtual const ::std::string getFuncString() const = 0;

    /// Evaluates the function.
    double operator()(double var);
};


}}

#endif

