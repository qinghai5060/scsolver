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

#ifndef _MSGDLG_HXX_
#define _MSGDLG_HXX_

#include <basedlg.hxx>
#include <baselistener.hxx>

namespace scsolver { 


class MessageDialog : public BaseDialog
{
public:
	MessageDialog( SolverImpl*, const rtl::OUString& );
	virtual ~MessageDialog() throw();

	virtual void setVisible( bool );
	virtual bool doneRangeSelection() const { return true; }
	virtual void close();

	void setMessage( const rtl::OUString& );
	void initialize();
	void registerListeners();

private:
	rtl::OUString m_sMessage;
};



}


#endif
