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

#ifndef _SCSOLVER_LPBUILDER_HXX_
#define _SCSOLVER_LPBUILDER_HXX_

#include "type.hxx"
#include <memory>
#include <vector>
#include <com/sun/star/table/CellAddress.hpp>


namespace scsolver {

namespace numeric { namespace lp {
    class Model;
}}

class LpModelBuilderImpl;


/** This is a container class that holds the 3D addresses of cells that 
    respectively contain the left and right hand side of the constraint 
    formula and its equality. */
class ConstraintAddress
{
public:
    ConstraintAddress();
    ConstraintAddress( const ConstraintAddress& );
    ~ConstraintAddress() throw();

    bool equals( const ConstraintAddress& ) const;
    bool operator==( const ConstraintAddress & ) const;

    ::com::sun::star::table::CellAddress getLeftCellAddr() const;
    void setLeftCellAddr( const ::com::sun::star::table::CellAddress& addr );

    ::com::sun::star::table::CellAddress getRightCellAddr() const;
    void setRightCellAddr( const ::com::sun::star::table::CellAddress& addr );
    double getLeftCellValue() const;
    double getRightCellValue() const;
    void setLeftCellValue( double value );
    void setRightCellValue( double value );
    bool isLeftCellNumeric() const;
    bool isRightCellNumeric() const;

    numeric::EqualityType getEquality() const;
    void setEquality( numeric::EqualityType eq );

private:

    ::com::sun::star::table::CellAddress Left;
    ::com::sun::star::table::CellAddress Right;
    numeric::EqualityType Equal;

    bool m_bIsLHSNumber:1;
    bool m_bIsRHSNumber:1;
    double m_fLHSValue;
    double m_fRHSValue;
};

/** 
 * This class transforms a user-defined model on a spreadsheet into a
 * standard linear model.  The model thereby created is to be passed onto 
 * one of the actual algorithm classes for solving it. 
 */
class LpModelBuilder
{
public:
    LpModelBuilder();
    ~LpModelBuilder();

    numeric::lp::Model getModel();

    // Optimization objective
    numeric::GoalType getGoal() const;
    void setGoal( numeric::GoalType );
    double getSolveToValue() const;
    void setSolveToValue(double val);

    // Objective Formula
    const ::com::sun::star::table::CellAddress getObjectiveFormulaAddress() const;
    void setObjectiveFormulaAddress( const ::com::sun::star::table::CellAddress& );
    
    // Decision Variables
    void setDecisionVarAddress( const ::com::sun::star::table::CellAddress& );
    std::vector< ::com::sun::star::table::CellAddress > getAllDecisionVarAddresses() const;
    void clearDecisionVarAddresses();
    
    // Cost Vector
    double getCostVector( const ::com::sun::star::table::CellAddress& );
    void setCostVector( const ::com::sun::star::table::CellAddress&, double );

    // Constraints
    void clearConstraintAddresses();
    void setConstraintAddress( const ConstraintAddress& );
    std::vector< ConstraintAddress > getAllConstraintAddresses() const;
    void setConstraintMatrixSize( size_t, size_t );
    void setConstraintCoefficient( const ::com::sun::star::table::CellAddress&, 
                                   const ConstraintAddress&, double, double );
    
    // Temporary formula strings
    const rtl::OUString getTempCellFormula( const ::com::sun::star::table::CellAddress& ) const;
    void setTempCellFormula( const ::com::sun::star::table::CellAddress&, 
                             const rtl::OUString& );
    
private:

    std::auto_ptr<LpModelBuilderImpl> m_pImpl;
};



}

#endif
