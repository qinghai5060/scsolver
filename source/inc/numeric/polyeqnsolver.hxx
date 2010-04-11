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

#ifndef _SCSOLVER_NUMERIC_POLYEQNSOLVER_HXX_
#define _SCSOLVER_NUMERIC_POLYEQNSOLVER_HXX

#include <list>
#include <exception>

namespace scsolver { namespace numeric {

class Matrix;

class NotEnoughDataPoints : public ::std::exception 
{
public:
    virtual const char* what() const throw();
};

struct DataPoint
{
    double X;
    double Y;

    DataPoint(double x, double y);
};

/**
 * This class is used to solve a polynomial equation of an arbitrary degree
 * based on a set of data points.  It returns an array of coefficients from 
 * the lowest order to highest, and the order of the equation equals the 
 * number of data points minus 1 e.g. if two data points are given, the 
 * equation to be solved will be linear, if three data points are given, the 
 * equation will be quadratic, and so on.
 */
class PolyEqnSolver
{
public:
    PolyEqnSolver();
    ~PolyEqnSolver() throw();

    void addDataPoint(double x, double y);
    const Matrix solve();
    void clear();

    /** 
     * Return the number of data points. 
     * 
     * @return size_t 
     */
    size_t size() const;

private:
    ::std::list<DataPoint> m_DataPoints;
};

// ----------------------------------------------------------------------------

/**
 * Calculate the peak position of a quadratic equation.  The quadratic 
 * equation must be specified in terms of a set of three coefficients: A, B 
 * and C as in f(x) = Ax^2 + Bx + C, and they must be given as a column matrix 
 * of (C, B, A) (note the reverse order). 
 *  
 * @param x the x position of the quadratic peak
 * @param y the y value at the quadratic peak
 * @param coef = (C, B, A) as in f(x) = Ax^2 + Bx + C.  Note that the order of
 *             the coefficients are from the x^0 term toward x^2.
 */
void getQuadraticPeak(double& x, double& y, const Matrix& coef);

}}

#endif
