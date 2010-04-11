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

#include <com/sun/star/awt/XFixedText.hpp>
#include <msgdlg.hxx>
#include <memory>

using namespace ::com::sun::star;
using ::com::sun::star::uno::UNO_QUERY;
using ::com::sun::star::uno::Reference;

namespace scsolver {

MessageDialog::MessageDialog( SolverImpl* p, const rtl::OUString& sMsg ) 
		: BaseDialog( p ), m_sMessage( sMsg )
{
	initialize();
}

MessageDialog::~MessageDialog() throw()
{
}

void MessageDialog::setVisible( bool bVisible )
{
	if ( bVisible )
		toFront();

	setVisibleDefault( bVisible );
}

void MessageDialog::close()
{
	setVisible(false);
}

void MessageDialog::setMessage( const rtl::OUString& sMsg )
{
	m_sMessage = sMsg;
	Reference< uno::XInterface > oWgt = getWidgetByName( ascii( "ftMessage" ) );
	Reference< awt::XFixedText > xTxt( oWgt, UNO_QUERY );
	xTxt->setText( sMsg );
}

void MessageDialog::initialize()
{
	const sal_Int16 nBtnWidth = 30, nBtnHeight = 14, nMargin = 3;
	sal_Int16 nWidth = 120, nHeight = 40;

	initializeDefault( nWidth, nHeight, ascii_i18n( "Message Dialog" ) );

	addFixedText( 5, 5, nWidth - 10, 10, ascii( "ftMessage" ), m_sMessage );
	addButton( (nWidth - nBtnWidth)/2, nHeight - nBtnHeight - nMargin, nBtnWidth, nBtnHeight, 
			ascii( "btnOk" ), ascii( "OK" ), awt::PushButtonType_OK );

	registerListeners();
}

void MessageDialog::registerListeners()
{
}


}






