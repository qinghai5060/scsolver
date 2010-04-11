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

#include "myfunctor.hxx"

using std::string;
using std::vector;

namespace scsolver {

namespace numeric { 

namespace nlp {

MyFunctor::MyFunctor() :
    mVars(2)
{
}

MyFunctor::~MyFunctor() throw()
{
}

double MyFunctor::eval() const
{
    double x1 = mVars[0], x2 = mVars[1];
    return x1*x1 + x2*x1 + 10.0;
}

const string MyFunctor::getFuncString() const
{
    string foo = "f(x1, x2) = x1*x1 + x2*x1 + 10";
    return foo;
}

void MyFunctor::getVars(vector<double>& rVars) const
{
    vector<double> vars = mVars;
    rVars.swap(vars);
}

double MyFunctor::getVar(size_t index) const
{
    return mVars.at(index);
}

size_t MyFunctor::getVarCount() const
{
    return mVars.size();
}

void MyFunctor::setVar(size_t index, double var)
{
    if (index >= 2)
        return;
    mVars.at(index) = var;
}

void MyFunctor::setVars(const vector<double>& vars)
{
    vector<double> tmp(vars);
    mVars.swap(tmp);
}

} // namespace nlp 
} // namespace numeric
} // namespace scsolver
