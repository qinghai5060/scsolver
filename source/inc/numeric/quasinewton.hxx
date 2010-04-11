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

#ifndef OPRES_NLP_NLPNEWTON_HXX_
#define OPRES_NLP_NLPNEWTON_HXX_

#include <numeric/nlpbase.hxx>
#include <memory>

namespace scsolver { namespace numeric { namespace nlp {

class QuasiNewtonImpl;

class QuasiNewton : public BaseAlgorithm
{
    friend class QuasiNewtonImpl;

public:
	QuasiNewton();
	virtual ~QuasiNewton() throw();

	virtual void solve();

private:
	std::auto_ptr<QuasiNewtonImpl> m_pImpl;
};


}}}


#endif
