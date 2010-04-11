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

#ifndef _UNOHELPER_HXX_
#define _UNOHELPER_HXX_
 
#include <unoglobal.hxx>

using namespace ::com::sun::star;
using ::com::sun::star::uno::Reference;
using ::com::sun::star::uno::XInterface;

namespace scsolver { namespace unohelper {

uno::Any getPropertyValue( const Reference< XInterface >&, const rtl::OUString& );

template<typename T>
void getPropertyValue( const Reference< XInterface >& obj, const rtl::OUString& sPropName, T& t )
{
	uno::Any aValue = getPropertyValue( obj, sPropName );
	aValue >>= t;
} 

void setPropertyValue( const Reference< uno::XInterface >&, const rtl::OUString&, const sal_Int32 );
void setPropertyValue( const Reference< XInterface >&, const rtl::OUString&, const uno::Any& );

}}
 
 
#endif
