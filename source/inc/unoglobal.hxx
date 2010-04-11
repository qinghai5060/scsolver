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

#ifndef _UNOGLOBAL_HXX_
#define _UNOGLOBAL_HXX_

#include <sal/config.h>
#include <cppuhelper/implementationentry.hxx>
#include <vector>
#include <string>

namespace scsolver {

void printOUStr( const rtl::OUString & );
#define ascii_i18n ascii
const ::rtl::OUString ascii(const sal_Char* text);
const ::com::sun::star::uno::Any asciiAny(const sal_Char* text);

void split( const rtl::OUString&, const rtl::OUString&, std::vector<rtl::OUString>& );


struct Global
{
	static const rtl::OUString STRING_SEPARATOR;
	static const rtl::OUString MODEL_METADATA_NAME;
};


/**
 * This class is designed to be a generic exception class that
 * can have arbitrary error message.  It consists of one ASCII
 * message for stdout, and one OUString message which can be
 * localized.
 */
class RuntimeError : public ::std::exception
{
public:
	RuntimeError( const rtl::OUString& umsg );
	~RuntimeError() throw();

	virtual const char* what() const throw();
	const rtl::OUString getMessage() const throw();

private:
	rtl::OUString m_sUniMsg;
};


}

#endif //_UNOGLOBAL_HXX_
