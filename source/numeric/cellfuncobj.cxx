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

#include "numeric/cellfuncobj.hxx"
#include "xcalc.hxx"
#include "unoglobal.hxx"
#include "com/sun/star/table/CellAddress.hpp"
#include <vector>
#include <sstream>
#include <iostream>
#include <stdio.h>

using com::sun::star::table::CellAddress;
using ::std::vector;
using ::std::string;
using ::std::ostringstream;

namespace scsolver { namespace numeric {

struct CellFuncObjImpl
{
	CalcInterface* pCalc;
	CellAddress TargetCell;
	vector<CellAddress> DecVarCells;
};

//-----------------------------------------------------------------

/*
CellFuncObj::CellFuncObj()
{
	// disabled
}
*/

CellFuncObj::CellFuncObj( CalcInterface* pCalc ) :
	m_pImpl( new CellFuncObjImpl )
{
	m_pImpl->pCalc = pCalc;
}

CellFuncObj::~CellFuncObj() throw()
{
}

void CellFuncObj::getVars(vector<double>& rVars) const
{
    vector<double> vars;
    const vector<CellAddress>& cells = m_pImpl->DecVarCells;
    vars.reserve(cells.size());
    for (vector<CellAddress>::const_iterator itr = cells.begin(), itrEnd = cells.end();
          itr != itrEnd; ++itr)
        vars.push_back(m_pImpl->pCalc->getCellValue(*itr));

    rVars.swap(vars);
}

double CellFuncObj::getVar(size_t index) const
{
    const CellAddress& rAddr = m_pImpl->DecVarCells.at(index);
    return m_pImpl->pCalc->getCellValue(rAddr);
}

void CellFuncObj::setVars(const vector<double>& vars)
{
	vector<CellAddress>::const_iterator itr, 
		itrBeg = m_pImpl->DecVarCells.begin(), 
		itrEnd = m_pImpl->DecVarCells.end();

    vector<double>::const_iterator itrVar,
        itrVarBeg = vars.begin(), itrVarEnd = vars.end();

    for (itr = itrBeg, itrVar = itrVarBeg; itr != itrEnd && itrVar != itrVarEnd; ++itr, ++itrVar)
        m_pImpl->pCalc->setCellValue(*itr, *itrVar);
}

void CellFuncObj::setVar(size_t index, double var)
{
    if ( index >= m_pImpl->DecVarCells.size() )
        return;

    m_pImpl->pCalc->setCellValue( m_pImpl->DecVarCells.at(index), var);
}

size_t CellFuncObj::getVarCount() const
{
    return m_pImpl->DecVarCells.size();
}

double CellFuncObj::eval() const
{
	return m_pImpl->pCalc->getCellValue( m_pImpl->TargetCell );
}

const std::string CellFuncObj::getFuncString() const
{
	ostringstream os;
	int sheet = m_pImpl->TargetCell.Sheet;
	int column = m_pImpl->TargetCell.Column;
	int row = m_pImpl->TargetCell.Row;
	os << "(" << sheet << ", " << column << ", " << row << ")";

	return os.str();
}

void CellFuncObj::setTargetCell( const CellAddress& addr )
{
	m_pImpl->TargetCell = addr;
}

void CellFuncObj::appendDecVarCell( const CellAddress& addr )
{
	m_pImpl->DecVarCells.push_back(addr);
}

}}

