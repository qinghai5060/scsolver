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

#ifndef _NUMERIC_ALGORITHM_HXX_
#define _NUMERIC_ALGORITHM_HXX_

#include <memory>
#include <boost/shared_ptr.hpp>
#include <vector>

namespace scsolver {
    class Timer;
}

namespace scsolver { namespace numeric {

class BaseFuncObj;
class SingleVarFuncObj;

class FuncObjectNotSet : public ::std::exception {};

/** 
 * Algorithm derived from Chapter 4.2 (p.93) of "A First Course In Numerical 
 * Analysis 2nd ed. by Anthony Ralston and Philip Rabinowitz".
 */
class NumericalDiffer
{
    static const double OMEGA;

public:
    NumericalDiffer();
    ~NumericalDiffer() throw();

    void setPrecision(unsigned long n);
    void setSecondOrder(bool b);
    void setVariable(double var);
    void setFuncObject(SingleVarFuncObj* pFuncObj);
    void setTimer(const ::scsolver::Timer* pTimer);

    double run();

private:
    void initialize();
    void setDirty();
    void appendNewH();
    void setT(unsigned long m, unsigned long i, double fVal);
    double getT(unsigned long m, unsigned long i);
    double T0(unsigned long i);
    double Tm();
    double Tm(unsigned long m, unsigned long i = 0);

    bool isTimedOut() const;

private:
    unsigned long m_nPrec;
    bool m_bSecondOrder;
    SingleVarFuncObj* m_pFuncObj;
    const ::scsolver::Timer* m_pTimer;
    double m_var;

    ::std::vector<double> m_cnX;
    ::std::vector<double> m_cnH;
    ::std::vector< ::std::vector<double> > m_cnT;
};

}}

#endif
