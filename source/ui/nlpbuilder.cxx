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

#include "nlpbuilder.hxx"
#include "solver.hxx"
#include "xcalc.hxx"
#include "numeric/nlpmodel.hxx"
#include "numeric/cellfuncobj.hxx"

#include "com/sun/star/table/CellAddress.hpp"

#include <list>
#include <stdio.h>

using std::list;
using scsolver::numeric::nlp::Model;
using scsolver::numeric::CellFuncObj;
using com::sun::star::table::CellAddress;

namespace scsolver {

class NlpModelBuilderImpl
{
public:
	NlpModelBuilderImpl( SolverImpl* p ) :
		m_pSolverImpl(p),
        m_pFuncObj(NULL)
	{
	}

	~NlpModelBuilderImpl() throw()
	{
	}

	Model getModel()
	{
		Model model;

        if (m_pFuncObj)
        {
            m_pFuncObj->setTargetCell(m_ObjFormAddr);
            model.setFuncObject(m_pFuncObj);
            CalcInterface* pCalc = m_pSolverImpl->getCalcInterface();
            list<CellAddress>::iterator it,
                itBeg = m_cnDecVarAddr.begin(),
                itEnd = m_cnDecVarAddr.end();
            for (it = itBeg; it != itEnd; ++it)
            {
                model.pushVar(pCalc->getCellValue(*it));
                m_pFuncObj->appendDecVarCell(*it);
            }
            model.print();
        }

		return model;
	}

	void setObjectiveFormulaAddress( CellAddress addr )
	{
		m_ObjFormAddr = addr;
	}

	void clearDecVarAddresses()
	{
		m_cnDecVarAddr.clear();
	}

	void appendDecVarAddress( CellAddress addr )
	{
		m_cnDecVarAddr.push_back(addr);
	}

    void setFuncObj(CellFuncObj* p)
    {
        m_pFuncObj = p;
    }

private:
	SolverImpl* m_pSolverImpl;
	CellAddress m_ObjFormAddr;
	list<CellAddress> m_cnDecVarAddr;
    CellFuncObj* m_pFuncObj;
};

//-----------------------------------------------------------------

NlpModelBuilder::NlpModelBuilder( SolverImpl* p ) :
	m_pImpl( new NlpModelBuilderImpl(p) )
{
	fprintf( stderr, "NlpModelBuilder ctor\n" );
}

NlpModelBuilder::~NlpModelBuilder() throw()
{
	fprintf( stderr, "NlpModelBuilder dtor\n" );
}

void NlpModelBuilder::setFuncObj(CellFuncObj* p)
{
    m_pImpl->setFuncObj(p);
}

void NlpModelBuilder::setObjectiveFormulaAddress( CellAddress addr )
{
	m_pImpl->setObjectiveFormulaAddress(addr);
}

void NlpModelBuilder::clearDecVarAddresses()
{
	m_pImpl->clearDecVarAddresses();
}

void NlpModelBuilder::appendDecVarAddress( CellAddress addr )
{
	m_pImpl->appendDecVarAddress(addr);
}

Model NlpModelBuilder::getModel() const
{
	return m_pImpl->getModel();
}

}
