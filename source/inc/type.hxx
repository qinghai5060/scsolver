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

#ifndef _TYPE_HXX_
#define _TYPE_HXX_

#include <cppuhelper/implementationentry.hxx>
#include <numeric/lpmodel.hxx>

namespace scsolver {


enum ConstButtonType 
{
	CONST_ADD,
	CONST_CHANGE,
	CONST_DELETE 
};

/** This class is used to hold a constraint item for dialog component. */
class ConstraintString
{
public:
	rtl::OUString Left;
	rtl::OUString Right;
	scsolver::numeric::EqualityType Equal;
};

}

#endif
