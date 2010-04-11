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

#ifndef _LPMODEL_HXX_
#define _LPMODEL_HXX_

#include <numeric/type.hxx>
#include <vector>
#include <memory>

namespace scsolver { namespace numeric { 

class Matrix;

namespace lp {


class BaseAlgorithm;
class ModelImpl;


/** 
 * This class is responsible for constructing an LP model which is to be 
 * solved by using an LP algorithm class defined separately.
 */
class Model
{
public:

    Model();
    Model( const Model& );
    ~Model();
    
    void print() const;

    size_t getDecisionVarSize() const;

    /**
     * This returns the number of constraints defined in the model,
     * which is equivalent of the row size of the constraint matrix.
     * 
     * @return size_t number of constraints
     */
    size_t getConstraintCount() const;

    double getCost( size_t rowid ) const;
    ::scsolver::numeric::Matrix getCostVector() const;
    void setCostVectorElement( size_t, double );
    void setCostVector( const ::std::vector<double>& );

    double   getVarBound( size_t, BoundType ) const;
    void     setVarBound( size_t, BoundType, double );
    bool isVarBounded( size_t, BoundType ) const;

    void deleteVariables( const ::std::vector<size_t>& );

    GoalType getGoal() const;
    void setGoal( GoalType );

    double getSolveToValue() const;
    void setSolveToValue(double val);
    
    unsigned long getPrecision() const;
    void       setPrecision( unsigned long );

    bool getVarPositive() const;
    void setVarPositive( bool );

    bool getVarInteger() const;
    void setVarInteger( bool b );

    bool getVerbose() const;
    void setVerbose( bool );

    double getConstraint( size_t, size_t ) const;
    ::scsolver::numeric::Matrix getConstraintMatrix() const;
    ::scsolver::numeric::Matrix getRhsVector() const;
    double getRhsValue( size_t ) const;
    void setRhsValue( size_t, double );
    ::std::vector< ::scsolver::numeric::EqualityType > getEqualityVector() const;
    ::scsolver::numeric::EqualityType getEquality( size_t ) const;
    void addConstraint( const ::std::vector<double>&, EqualityType, double );
    void setStandardConstraintMatrix( const ::scsolver::numeric::Matrix&, const ::scsolver::numeric::Matrix& );

private:
    std::auto_ptr<ModelImpl> m_pImpl;
};


}}}


#endif
