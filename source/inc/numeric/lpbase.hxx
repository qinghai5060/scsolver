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

#ifndef _LPBASE_HXX_
#define _LPBASE_HXX_

#include <memory>

namespace scsolver { namespace numeric {

class Matrix;

namespace lp {

class Model;
class BaseAlgorithmImpl;

class BaseAlgorithm
{
public:
	BaseAlgorithm();
	virtual ~BaseAlgorithm() = 0;

	virtual void solve() = 0;

	Model* getModel() const;
	void setModel( Model* );

	::scsolver::numeric::Matrix getSolution() const;
	void setSolution( const ::scsolver::numeric::Matrix& );

private:
	::std::auto_ptr<BaseAlgorithmImpl> m_pImpl;
};

}}}

#endif
