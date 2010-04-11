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

#ifndef _LPUNO_HXX_
#define _LPUNO_HXX_

#include "lpbase.hxx"
#include "unoglobal.hxx"
#include <memory>

namespace scsolver {

class CalcInterface;

namespace numeric { namespace lp {

class UnoAlgorithmImpl;

class UnoAlgorithm : public BaseAlgorithm
{
public:
	UnoAlgorithm( const rtl::OUString&, CalcInterface* );
	virtual ~UnoAlgorithm() throw();

	virtual void solve();

private:
	std::auto_ptr<UnoAlgorithmImpl> m_pImpl;
};



}}}


#endif

