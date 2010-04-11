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

#ifndef _SCSOLVER_NUMERIC_BASELINESEARCH_HXX_
#define _SCSOLVER_NUMERIC_BASELINESEARCH_HXX_

#include "numeric/type.hxx"

namespace scsolver {
    class Timer;
}

namespace scsolver { namespace numeric { 

class SingleVarFuncObj;

class BaseLineSearch
{
public:
    BaseLineSearch();
    explicit BaseLineSearch(SingleVarFuncObj* pFuncObj);
    virtual ~BaseLineSearch() = 0;

    virtual double solve() = 0;

    void setGoal(GoalType goal);
    GoalType getGoal() const;

    void setDebug(bool b);
    bool isDebug() const;

    void setFuncObj(SingleVarFuncObj* p);
    SingleVarFuncObj* getFuncObj() const;

    void setTimer(const ::scsolver::Timer* pTimer);
    const ::scsolver::Timer* getTimer() const;

private:

private:
    SingleVarFuncObj* m_pFuncObj;
    const ::scsolver::Timer* m_pTimer;
    GoalType m_goal;
    bool m_debug;
};

}}

#endif
