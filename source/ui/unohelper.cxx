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
 
#include "unohelper.hxx"
#include <com/sun/star/beans/XPropertySet.hpp>

using ::com::sun::star::uno::UNO_QUERY;

namespace scsolver { namespace unohelper {

 
uno::Any getPropertyValue( const Reference< uno::XInterface >& obj, const rtl::OUString& sPropName )
{
	Reference< beans::XPropertySet > xPS( obj, UNO_QUERY );
	OSL_ASSERT( xPS != NULL );
	return xPS.is() ? xPS->getPropertyValue( sPropName ) : uno::Any();
}

void setPropertyValue( const Reference< uno::XInterface >& obj, const rtl::OUString& sPropName,
		const sal_Int32 n )
{
	uno::Any aVal;
	aVal <<= n;
	setPropertyValue( obj, sPropName, aVal );
}

void setPropertyValue( const Reference< uno::XInterface >& obj, const rtl::OUString& sPropName,
		const uno::Any& aVal )
{
	Reference< beans::XPropertySet > xPS( obj, UNO_QUERY );
	OSL_ASSERT( xPS != NULL );
	xPS->setPropertyValue( sPropName, aVal );
}


}}

















