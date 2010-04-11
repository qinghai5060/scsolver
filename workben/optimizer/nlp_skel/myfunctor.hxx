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

#ifndef _SCSOLVER_NUMERIC_MYFUNCTOR_
#define _SCSOLVER_NUMERIC_MYFUNCTOR_

#include "numeric/funcobj.hxx"
#include <vector>

namespace scsolver {

namespace numeric { 

namespace nlp {

class MyFunctor : public BaseFuncObj
{
public:
    MyFunctor();
    ~MyFunctor() throw();

    virtual double eval() const;

    /**
     * Return a display-friendly function string.
     *
     * @return std::string
     */
    virtual const std::string getFuncString() const;

    /// Returns all variables setting them in the vector \a rVars.
    virtual void getVars(::std::vector<double>& rVars) const;

    /// Returns a variable indicated by \a index.
    virtual double getVar(size_t index) const;

    /// Returns the current number of variables.
    virtual size_t getVarCount() const;

    virtual void setVar(size_t index, double var);

    virtual void setVars(const std::vector<double> &vars);

private:
    ::std::vector<double> mVars;
};

} // namespace nlp 
} // namespace numeric
} // namespace scsolver

#endif
