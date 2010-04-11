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

#include "numeric/lpbase.hxx"
#include "numeric/lpmodel.hxx"
#include "numeric/matrix.hxx"
#include <list>
#include <vector>
#include <cstddef>
#include "tool/global.hxx"

using ::std::vector;
using ::std::list;
using ::std::cout;
using ::std::endl;

namespace scsolver { namespace numeric { namespace lp {


class BaseAlgorithmImpl
{
public:
	BaseAlgorithmImpl();
	~BaseAlgorithmImpl() throw();

	Model* getModel() const { return m_pModel; }
	void setModel( Model* p ) { m_pModel = p; }
	
	Matrix getSolution() const { return m_mxSolution; }
	void setSolution( const Matrix& );

private:
	Model* m_pModel;						// original model
	Matrix m_mxSolution;

	struct ConstDecVar
	{
		size_t Id;
		double Value;
	};

    /* This list is used to store IDs and values of constant equivalent
       values. */
	std::list<ConstDecVar> m_cnConstDecVarList;
};

BaseAlgorithmImpl::BaseAlgorithmImpl() :
	m_pModel( NULL ),
	m_mxSolution( 0, 0 )
{
}

BaseAlgorithmImpl::~BaseAlgorithmImpl() throw()
{
}

void BaseAlgorithmImpl::setSolution( const Matrix& other )
{
	Matrix m( other );
	m_mxSolution.swap( m );
}

BaseAlgorithm::BaseAlgorithm() :
	m_pImpl( new BaseAlgorithmImpl() )
{
}

BaseAlgorithm::~BaseAlgorithm()
{
}

Model* BaseAlgorithm::getModel() const
{
	return m_pImpl->getModel();
}

void BaseAlgorithm::setModel( Model* p )
{
	m_pImpl->setModel( p );
}

Matrix BaseAlgorithm::getSolution() const
{
	return m_pImpl->getSolution();
}

void BaseAlgorithm::setSolution( const Matrix& mx )
{
	m_pImpl->setSolution( mx );
}

}}}
