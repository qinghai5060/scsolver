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

#ifndef _LPSIMPLEX_HXX_
#define _LPSIMPLEX_HXX_

#include "lpbase.hxx"
#include <memory>

namespace scsolver { namespace numeric { namespace lp {

class Model;
class RevisedSimplexImpl;
class BoundedRevisedSimplexImpl;

class RevisedSimplex : public BaseAlgorithm
{
public:
	RevisedSimplex();
	virtual ~RevisedSimplex() throw();

	virtual void solve();

	void setEnableTwoPhaseSearch( bool );

private:
	std::auto_ptr<RevisedSimplexImpl> m_pImpl;
};


class BoundedRevisedSimplex : public BaseAlgorithm
{
public:
	BoundedRevisedSimplex();
	virtual ~BoundedRevisedSimplex() throw();

	virtual void solve();

private:
	std::auto_ptr<BoundedRevisedSimplexImpl> m_pImpl;
};


}}}


#endif
