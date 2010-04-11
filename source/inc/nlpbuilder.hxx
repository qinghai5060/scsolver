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

#ifndef _SCSOLVER_NLPBUILDER_HXX_
#define _SCSOLVER_NLPBUILDER_HXX_

#include <memory>

namespace com { namespace sun { namespace star { namespace table {
	class CellAddress;
}}}}

namespace scsolver {

class SolverImpl;

namespace numeric { 

    class CellFuncObj;

namespace nlp {

	class Model;

}}

class NlpModelBuilderImpl;

class NlpModelBuilder
{
public:
	NlpModelBuilder( SolverImpl* p );
	~NlpModelBuilder() throw();

    void setFuncObj(scsolver::numeric::CellFuncObj* p);
	void setObjectiveFormulaAddress( ::com::sun::star::table::CellAddress addr );
	void clearDecVarAddresses();
	void appendDecVarAddress( ::com::sun::star::table::CellAddress addr );
	numeric::nlp::Model getModel() const;

private:
	NlpModelBuilder();
	std::auto_ptr<NlpModelBuilderImpl> m_pImpl;
};

}

#endif
