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

#include "numeric/baselinesearch.hxx"
#include "numeric/funcobj.hxx"

namespace scsolver { namespace numeric { 

BaseLineSearch::BaseLineSearch() :
    m_pFuncObj(NULL),
    m_pTimer(NULL),
    m_goal(GOAL_UNKNOWN),
    m_debug(false)
{
}

BaseLineSearch::BaseLineSearch(SingleVarFuncObj* pFuncObj) :
    m_pFuncObj(pFuncObj),
    m_pTimer(NULL),
    m_goal(GOAL_UNKNOWN),
    m_debug(false)
{
}

BaseLineSearch::~BaseLineSearch()
{
}

void BaseLineSearch::setGoal(GoalType goal)
{
    m_goal = goal;
}

GoalType BaseLineSearch::getGoal() const
{
    return m_goal;
}

void BaseLineSearch::setDebug(bool b)
{
    m_debug = b;
}

bool BaseLineSearch::isDebug() const
{
    return m_debug;
}

void BaseLineSearch::setFuncObj(SingleVarFuncObj* p)
{
    m_pFuncObj = p;
}

SingleVarFuncObj* BaseLineSearch::getFuncObj() const
{
    return m_pFuncObj;
}

void BaseLineSearch::setTimer(const ::scsolver::Timer* pTimer)
{
    m_pTimer = pTimer;
}

const ::scsolver::Timer* BaseLineSearch::getTimer() const
{
    return m_pTimer;
}

}}
