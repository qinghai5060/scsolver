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

#include "numeric/nlpmodel.hxx"
#include "numeric/nlpbase.hxx"
#include "numeric/funcobj.hxx"
#include "numeric/exception.hxx"
#include "numeric/type.hxx"
#include "tool/global.hxx"
#include <vector>
#include <iostream>
#include <algorithm>

#include <stdio.h>

using namespace ::scsolver::numeric;
using ::std::vector;
using ::std::for_each;

namespace scsolver { namespace numeric { namespace nlp {

namespace {

/** Decision variable data structure */
struct DecVar
{
    double Value;

    bool   UpperBoundEnabled;
    double UpperBoundValue;

    bool   LowerBoundEnabled;
    double LowerBoundValue;

    DecVar() :
        Value(0.0),
        UpperBoundEnabled(false), 
        UpperBoundValue(0.0),
        LowerBoundEnabled(false), 
        LowerBoundValue(0.0)
    {}
};

#ifndef _MSC_VER
class ConstraintPrinter : public ::std::unary_function<Constraint, void>
{
public:
    explicit ConstraintPrinter(FILE* fs) : 
        m_fs(fs) 
    {
    }

    void operator()(const Constraint& c) const
    {
        fprintf(m_fs, "%s ", c.FuncObj->getFuncString().c_str());
        switch (c.Equality)
        {
            case GREATER_EQUAL: fprintf(m_fs, ">= "); break;
            case EQUAL:         fprintf(m_fs, "= "); break;
            case LESS_EQUAL:    fprintf(m_fs, "<= "); break;
        }
        fprintf(m_fs, "%g", c.RightValue);
        fprintf(m_fs, "\n");
    }

private:
    FILE* m_fs;
};
#endif

}

Constraint::Constraint(BaseFuncObj* pFunc, EqualityType eq, double rightValue) :
    FuncObj(pFunc), Equality(eq), RightValue(rightValue) 
{
}

// ============================================================================

class ModelImpl
{
public:
    ModelImpl();
    ModelImpl(const ModelImpl& other);
    ~ModelImpl() throw();

    void setPrecision( unsigned long n ) { m_nPrec = n; }
    unsigned long getPrecision() const { return m_nPrec; }

    void setGoal( GoalType e ) { m_eGoal = e; }
    GoalType getGoal() const { return m_eGoal; }

    void setVerbose( bool b ) { m_bVerbose = b; }
    bool getVerbose() const { return m_bVerbose; }

    void setFuncObject(BaseFuncObj* pFuncObj);
    BaseFuncObj* getFuncObject() const { return m_pFuncObj; }

    void pushVar( double value )
    {
        DecVar var;
        var.Value = value;
        m_Vars.push_back(var);
    }

    void getVars( vector<double>& vars ) const
    {
        size_t n = m_Vars.size();
        vector<double> tmp;
        tmp.reserve(n);
        vector<DecVar>::const_iterator itr = m_Vars.begin(), itrEnd = m_Vars.end();
        for (; itr != itrEnd; ++itr)
            tmp.push_back(itr->Value);
        vars.swap(tmp);
    }

    double getVarBound( size_t index, BoundType bound ) const
    {
        switch (bound)
        {
        case BOUND_LOWER:
            if ( !m_Vars.at(index).LowerBoundEnabled )
                throw UnboundedException();
            return m_Vars.at(index).LowerBoundValue;
        case BOUND_UPPER:
            if ( !m_Vars.at(index).UpperBoundEnabled )
                throw UnboundedException();
            return m_Vars.at(index).UpperBoundValue;
        }
        return 0.0;
    }

    void setVarBound( size_t index, BoundType bound, double value )
    {
        switch (bound)
        {
        case BOUND_LOWER:
            m_Vars.at(index).LowerBoundValue = value;
            m_Vars.at(index).LowerBoundEnabled = true;
            break;
        case BOUND_UPPER:
            m_Vars.at(index).UpperBoundValue = value;
            m_Vars.at(index).UpperBoundEnabled = true;
            break;
        }
    }

    bool isVarBounded( size_t index, BoundType bound ) const
    {
        switch (bound)
        {
        case BOUND_LOWER:
            return m_Vars.at(index).LowerBoundEnabled;
        case BOUND_UPPER:
            return m_Vars.at(index).UpperBoundEnabled;
        }
        throw AssertionWrong();
        return false;
    }

    void pushConstraint(BaseFuncObj* pFunc, EqualityType eq, double rightValue);
    const vector<Constraint>& getAllConstraints() const;

    void print() const;

private:
    BaseFuncObj*    m_pFuncObj;
    vector<DecVar>      m_Vars;
    vector<Constraint>  m_Constraints;

    unsigned long   m_nPrec;
    GoalType        m_eGoal;
    bool            m_bVerbose;
};

ModelImpl::ModelImpl() : 
    m_pFuncObj(NULL),
    m_nPrec( 9 ), 
    m_eGoal( GOAL_UNKNOWN ), 
    m_bVerbose(false)
{
}

ModelImpl::ModelImpl(const ModelImpl& other) : 
    m_pFuncObj(other.m_pFuncObj),
    m_nPrec(other.m_nPrec)
{
}

ModelImpl::~ModelImpl() throw()
{
}

void ModelImpl::setFuncObject(BaseFuncObj* pFuncObj)
{
    m_pFuncObj = pFuncObj;
}

void ModelImpl::pushConstraint(BaseFuncObj* pFunc, EqualityType eq, double rightValue)
{
    m_Constraints.push_back(Constraint(pFunc, eq, rightValue));
}

const vector<Constraint>& ModelImpl::getAllConstraints() const
{
    return m_Constraints;
}

void ModelImpl::print() const
{
    FILE* fs = stdout;

    // Objective function and goal.
    fprintf(fs, "--------------------------------------------------------------------\n");
    fprintf(fs, "objective: ");
    if (m_pFuncObj)
        fprintf(fs, "%s\n", m_pFuncObj->getFuncString().c_str());
    else
        fprintf(fs, "object function not set!\n");

    fprintf(fs, "goal: ");
    switch ( m_eGoal )
    {
    case GOAL_MAXIMIZE:
        fprintf(fs, "maximize");
        break;
    case GOAL_MINIMIZE:
        fprintf(fs, "minimize");
        break;
    default:
        fprintf(fs, "unknown");
    }
    fprintf(fs, "\n");
    fprintf(fs, "--------------------------------------------------------------------\n");

    size_t n = m_Vars.size();
    for (size_t i = 0; i < n; ++i)
    {
        const DecVar& var = m_Vars.at(i);
        fprintf(fs, "var %d: ", i);
        if (var.LowerBoundEnabled)
            fprintf(fs, "%.2f", var.LowerBoundValue);
        else 
            fprintf(fs, "[unbounded]");
        fprintf(fs, " - ");

        if (var.UpperBoundEnabled)
            fprintf(fs, "%.2f", var.UpperBoundValue);
        else
            fprintf(fs, "[unbounded]");
        fprintf(fs, "\n");
    }
    fprintf(fs, "--------------------------------------------------------------------\n");
    if (m_Constraints.empty())
        fprintf(fs, "model is unconstrained\n");
    else
    {
        fprintf(fs, "Constraints\n");
#ifndef _MSC_VER
        for_each(m_Constraints.begin(), m_Constraints.end(), ConstraintPrinter(fs));
#endif
    }
    fprintf(fs, "--------------------------------------------------------------------\n");
    fflush(fs);
}

//---------------------------------------------------------------------------
// ENVELOPE

Model::Model() : m_pImpl(new ModelImpl)
{
}

Model::Model( const Model& other ) : m_pImpl(new ModelImpl(*other.m_pImpl))
{
}

Model::~Model() throw()
{
}

void Model::print() const
{
    m_pImpl->print();
}

void Model::setPrecision( unsigned long n )
{
    m_pImpl->setPrecision( n );
}

unsigned long Model::getPrecision() const
{
    return m_pImpl->getPrecision();
}

void Model::setGoal( GoalType e )
{
    m_pImpl->setGoal( e );
}

GoalType Model::getGoal() const
{
    return m_pImpl->getGoal();
}

void Model::setVerbose( bool b )
{
    m_pImpl->setVerbose( b );
}

bool Model::getVerbose() const
{
    return m_pImpl->getVerbose();
}

void Model::setFuncObject(BaseFuncObj* pFuncObj)
{
    m_pImpl->setFuncObject(pFuncObj);
}

BaseFuncObj* Model::getFuncObject() const
{
    return m_pImpl->getFuncObject();
}

void Model::pushVar( double var )
{
    m_pImpl->pushVar(var);
}

void Model::getVars( vector<double>& vars ) const
{
    m_pImpl->getVars(vars);
}

double Model::getVarBound( size_t index, BoundType bound ) const
{
    return m_pImpl->getVarBound(index, bound);
}

void Model::setVarBound( size_t index, BoundType bound, double value )
{
    m_pImpl->setVarBound(index, bound, value);
}

bool Model::isVarBounded( size_t index, BoundType bound ) const
{
    return m_pImpl->isVarBounded(index, bound);
}

void Model::pushConstraint(BaseFuncObj* pFunc, EqualityType eq, double rightValue)
{
    m_pImpl->pushConstraint(pFunc, eq, rightValue);
}

const vector<Constraint>& Model::getAllConstraints() const
{
    return m_pImpl->getAllConstraints();
}

}}}

