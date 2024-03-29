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

#include "optiondlg.hxx"
#include "unoglobal.hxx"
#include "tool/global.hxx"
#include "listener.hxx"
#include "solver.hxx"

#include <memory>

#include "com/sun/star/awt/XCheckBox.hpp"

#include "scsolver.hrc"

using namespace ::com::sun::star;
using namespace ::com::sun::star::uno;
using ::std::auto_ptr;

namespace scsolver {

/**
 * This class defines the action to be performed when the OK
 * button in the Option dialog is pressed.
 */
class OptionDlgOKAction : public ActionObject
{
public:
	OptionDlgOKAction()
	{
	}

	virtual ~OptionDlgOKAction() throw()
	{
	}

	virtual void execute( BaseDialog *dlg, const awt::ActionEvent& /*e*/ )
	{
		OptionDialog* p = static_cast<OptionDialog*>(dlg);
		p->setVisible(false);
        OptionData* pOption = p->getSolverImpl()->getOptionData();
        pOption->setModelType(p->getModelType());
        pOption->setVarPositive(p->isVarPositive());
        pOption->setVarInteger(p->isVarInteger());
	}
};

/**
 * Specifies the action to perform when the window closing event
 * is received (typically when the user clicks the close button
 * on the title bar).
 */
class OptionDlgWinCloseAction : public SimpleActionObject
{
public:
	virtual void execute( BaseDialog *dlg )
	{
		dlg->close();
	}
};

//-----------------------------------------------------------------

struct OptionDialogImpl
{
	auto_ptr<ActionObject>       pActionOK;
	auto_ptr<SimpleActionObject> pActionWinClose;

	TopWindowListener* pTopWinListener;
	ActionListener*    pOKListener;
	CloseBtnListener*  pCloseListener;
};

//-----------------------------------------------------------------

OptionDialog::OptionDialog( SolverImpl* p ) :
	BaseDialog( p ),
	m_pImpl( new OptionDialogImpl )
{
	initialize();
    initOptionData();
}

OptionDialog::~OptionDialog() throw()
{
}

void OptionDialog::initialize()
{
	int nWidth = 200, nHeight = 150;
	initializeDefault( 
		static_cast<sal_Int16>(nWidth), static_cast<sal_Int16>(nHeight), 
		getResStr(SCSOLVER_STR_OPTIONDLG_TITLE) );

	sal_Int32 nX = 5, nY = 5, nMargin = 5;

	addFixedLine( nX, nY, nWidth-nX-nMargin, 12, ascii("flOptions"), 
				  getResStr(SCSOLVER_STR_OPTIONDLG_TITLE) );
	nY += 13;
    addCheckBox(nX, nY+2, nWidth-nX-nMargin, 12, ascii("cbLinear"), 
                getResStr(SCSOLVER_STR_OPTION_ASSUME_LINEAR) );

    nY += 13;
	addCheckBox( nX, nY+2, nWidth-nX-nMargin, 12, ascii("cbPositiveValue"),
                 getResStr(SCSOLVER_STR_OPTION_VAR_POSITIVE) );

    nY += 13;
	addCheckBox( nX, nY+2, nWidth-nX-nMargin, 12, ascii("cbIntegerValue"),
                 getResStr(SCSOLVER_STR_OPTION_VAR_INTEGER) );

	addButton( nWidth-110, nHeight-20, 50, 15, ascii("btnOK"), 
			   getResStr(SCSOLVER_STR_BTN_OK) );

	addButton( nWidth-55, nHeight-20, 50, 15, ascii("btnCancel"), 
			   getResStr(SCSOLVER_STR_BTN_CANCEL) );

	registerListeners();
}

void OptionDialog::initOptionData()
{
    OptionData* pData = getSolverImpl()->getOptionData();
    if (!pData)
        return;

    setVarPositive(pData->getVarPositive());
    setVarInteger(pData->getVarInteger());
    setModelType(pData->getModelType());
}

void OptionDialog::registerListeners()
{
	m_pImpl->pActionOK.reset( new OptionDlgOKAction );
	m_pImpl->pOKListener = new ActionListener( this, m_pImpl->pActionOK.get() );
	registerListener( ascii("btnOK"), m_pImpl->pOKListener );

	m_pImpl->pCloseListener = new CloseBtnListener(this);
	registerListener( ascii("btnCancel"), m_pImpl->pCloseListener );

	m_pImpl->pActionWinClose.reset( new OptionDlgWinCloseAction );
	m_pImpl->pTopWinListener = new TopWindowListener(this);
	m_pImpl->pTopWinListener->setActionClosing(m_pImpl->pActionWinClose.get());
	registerListener( m_pImpl->pTopWinListener );
}

void OptionDialog::unregisterListeners() throw()
{
	unregisterListener( ascii("btnOK"), m_pImpl->pOKListener );
	unregisterListener( ascii("btnCancel"), m_pImpl->pCloseListener );
	unregisterListener( m_pImpl->pTopWinListener );
}

bool OptionDialog::doneRangeSelection() const
{
	return false;
}

void OptionDialog::close()
{
	setVisible(false);
}

void OptionDialog::setVisible( bool b )
{
	setVisibleDefault( b );
}

OptModelType OptionDialog::getModelType() const
{
	Reference<uno::XInterface> oWgt = getWidgetByName( ascii("cbLinear") );
	Reference<awt::XCheckBox> xCB( oWgt, UNO_QUERY );

	return xCB->getState() ? OPTMODELTYPE_LP : OPTMODELTYPE_NLP;
}

void OptionDialog::setModelType( OptModelType type )
{
	Reference<uno::XInterface> oWgt = getWidgetByName( ascii("cbLinear") );
	Reference<awt::XCheckBox> xCB( oWgt, UNO_QUERY );
	if ( type == OPTMODELTYPE_LP )
		xCB->setState(1);
	else
		xCB->setState(0);
}

bool OptionDialog::isVarPositive() const
{
    Reference<uno::XInterface> oWgt = getWidgetByName( ascii("cbPositiveValue") );
    Reference<awt::XCheckBox> xCB( oWgt, UNO_QUERY );
    return xCB->getState();
}

void OptionDialog::setVarPositive(bool b)
{
    Reference<uno::XInterface> oWgt = getWidgetByName( ascii("cbPositiveValue") );
    Reference<awt::XCheckBox> xCB( oWgt, UNO_QUERY );
    xCB->setState(b);
}

bool OptionDialog::isVarInteger() const
{
    Reference<uno::XInterface> oWgt = getWidgetByName( ascii("cbIntegerValue") );
    Reference<awt::XCheckBox> xCB( oWgt, UNO_QUERY );
    return xCB->getState();
}

void OptionDialog::setVarInteger(bool b)
{
    Reference<uno::XInterface> oWgt = getWidgetByName( ascii("cbIntegerValue") );
    Reference<awt::XCheckBox> xCB( oWgt, UNO_QUERY );
    xCB->setState(b);
}

}
