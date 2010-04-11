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

#include "baselistener.hxx"
#include "basedlg.hxx"

using namespace ::com::sun::star;
using ::com::sun::star::uno::RuntimeException;

namespace scsolver {

SimpleActionObject::SimpleActionObject()
{
}

SimpleActionObject::~SimpleActionObject() throw()
{
}

ActionObject::ActionObject()
{
}

ActionObject::~ActionObject() throw()
{
}

ActionListener::ActionListener( BaseDialog* pDlg ) :
    m_pAction(NULL), m_pDlg(pDlg)
{
}


ActionListener::ActionListener( BaseDialog* pDlg, ActionObject* pAction ) :
    m_pAction(pAction), m_pDlg(pDlg)
{
}

ActionListener::~ActionListener() throw()
{
}

void SAL_CALL ActionListener::disposing( const lang::EventObject& )
	throw ( RuntimeException )
{
}

void SAL_CALL ActionListener::actionPerformed( const awt::ActionEvent& e )
	throw ( RuntimeException )
{
	if (m_pAction)
		m_pAction->execute(m_pDlg, e);
}

BaseDialog* ActionListener::getDialog() const
{
    return m_pDlg;
}

// ============================================================================

ItemListener::Action::~Action()
{
}

ItemListener::ItemListener(Action* pAction) :
    mpAction(pAction)
{
}

ItemListener::~ItemListener()
{
}

void SAL_CALL ItemListener::disposing(const lang::EventObject& /*rEvt*/)
    throw (RuntimeException)
{
}

void SAL_CALL ItemListener::itemStateChanged(const awt::ItemEvent& rEvt)
    throw (RuntimeException)
{
    if (mpAction.get())
        mpAction->itemStateChanged(rEvt);
}

// ============================================================================

FocusListener::~FocusListener() throw()
{
}

MouseListener::~MouseListener() throw()
{
}

//-----------------------------------------------------------------

class TopWindowListenerImpl
{
public:
	TopWindowListenerImpl(BaseDialog* pDlg) :
        mpDlg(pDlg),
		pCloseAction(NULL)
	{
	}

	~TopWindowListenerImpl() throw()
	{
	}

    BaseDialog* mpDlg;
	SimpleActionObject* pCloseAction;
};

TopWindowListener::TopWindowListener( BaseDialog* pDlg ) :
	m_pImpl(new TopWindowListenerImpl(pDlg))
{
}

TopWindowListener::~TopWindowListener() throw()
{
}

void SAL_CALL TopWindowListener::windowOpened( const lang::EventObject& )
		throw(RuntimeException)
{
}

void SAL_CALL TopWindowListener::windowClosing( const lang::EventObject& )
		throw(RuntimeException)
{
	if (m_pImpl->pCloseAction == NULL)
	{
		m_pImpl->mpDlg->setVisible(false);
		return;
	}
	m_pImpl->pCloseAction->execute(m_pImpl->mpDlg);
}

void SAL_CALL TopWindowListener::windowClosed( const lang::EventObject& )
		throw (RuntimeException)
{
}

void SAL_CALL TopWindowListener::windowMinimized( const lang::EventObject& )
		throw (RuntimeException)
{
}

void SAL_CALL TopWindowListener::windowNormalized( const lang::EventObject& )
		throw (RuntimeException)
{
}

void SAL_CALL TopWindowListener::windowActivated( const lang::EventObject& )
		throw (RuntimeException)
{
}

void SAL_CALL TopWindowListener::windowDeactivated( const lang::EventObject& )
		throw (RuntimeException)
{
}

void SAL_CALL TopWindowListener::disposing( const lang::EventObject& )
		throw (RuntimeException)
{
}

void TopWindowListener::setActionClosing( SimpleActionObject* p )
{
	m_pImpl->pCloseAction = p;
}

}

