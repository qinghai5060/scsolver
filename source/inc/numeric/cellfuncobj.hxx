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

#ifndef _SCSOLVER_NUMERIC_CELLFUNCOBJ_HXX_
#define _SCSOLVER_NUMERIC_CELLFUNCOBJ_HXX_

#include "numeric/funcobj.hxx"
#include "unoglobal.hxx"
#include <memory>
#include <vector>
#include <string>

namespace com { namespace sun { namespace star { namespace table {
	class CellAddress;
}}}}

namespace scsolver {

class CalcInterface;

namespace numeric { 

struct CellFuncObjImpl;

class CellFuncObj : public BaseFuncObj
{
public:
	CellFuncObj( ::scsolver::CalcInterface* pCalc );
	virtual ~CellFuncObj() throw();

    virtual void getVars(::std::vector<double>& rVars) const;
    virtual double getVar(size_t index) const;
    virtual void setVars(const std::vector<double>& vars);
    virtual void setVar(size_t index,  double var);
    virtual size_t getVarCount() const;

    virtual double eval() const;

	/**
	 * Return a display-friendly function string.
	 *
	 * @return std::string
	 */
	virtual const std::string getFuncString() const;

	void setTargetCell( const ::com::sun::star::table::CellAddress& addr );
	void appendDecVarCell( const ::com::sun::star::table::CellAddress& addr );

private:
	CellFuncObj();

    ::std::vector<double> mFakeVars;
    ::std::auto_ptr<CellFuncObjImpl> m_pImpl;
};


}}

#endif
