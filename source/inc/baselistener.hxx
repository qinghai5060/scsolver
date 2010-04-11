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

#ifndef _BASELISTENER_HXX_
#define _BASELISTENER_HXX_

#ifndef _CPPU_MACROS_HXX_
#include "cppu/macros.hxx"
#endif

#include "unoglobal.hxx"

#include <cppuhelper/implbase1.hxx>
#include <com/sun/star/awt/XActionListener.hpp>
#include <com/sun/star/awt/XItemListener.hpp>
#include <com/sun/star/awt/XFocusListener.hpp>
#include <com/sun/star/awt/XMouseListener.hpp>
#include <com/sun/star/awt/XTopWindowListener.hpp>

#include <memory>

namespace scsolver {

class BaseDialog;

//--------------------------------------------------------------------------
// Listener Class for Dialog Widgets

class SimpleActionObject
{
public:
    SimpleActionObject();
    virtual ~SimpleActionObject() throw() = 0;
    virtual void execute( BaseDialog* dlg ) = 0;
};

/**
 * This class defines action for ActionListener object.
 */
class ActionObject
{
public:
    ActionObject();
    virtual ~ActionObject() throw() = 0;
    virtual void execute( BaseDialog* dlg, const ::com::sun::star::awt::ActionEvent& e ) = 0;
};

/**
 * Generic action listener class.  You can either derive a child
 * class from it, or directly instantiate it with an instance of
 * ActionObject which defines the action to be performed.  If an
 * instance of ActionObject is passed as a pointer, the memory
 * of that instance must be managed by the calling function.
 */
class ActionListener : public ::cppu::WeakImplHelper1< ::com::sun::star::awt::XActionListener >
{
public:
    ActionListener( BaseDialog* pDlg );
    ActionListener( BaseDialog* pDlg, ActionObject* pAction );
    virtual ~ActionListener() throw();
    
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& e ) throw ( ::com::sun::star::uno::RuntimeException );
    virtual void SAL_CALL actionPerformed( const ::com::sun::star::awt::ActionEvent& e ) throw ( ::com::sun::star::uno::RuntimeException );

protected:
    BaseDialog* getDialog() const;

private:
    ActionObject* m_pAction;
    BaseDialog*   m_pDlg;
};

// ============================================================================

class ItemListener : public ::cppu::WeakImplHelper1< ::com::sun::star::awt::XItemListener >
{
public:
    class Action
    {
    public:
        virtual ~Action() = 0;
        virtual void itemStateChanged(const ::com::sun::star::awt::ItemEvent& rEvt) = 0;
    };

    ItemListener(Action* pAction);
    virtual ~ItemListener();

    virtual void SAL_CALL disposing(const ::com::sun::star::lang::EventObject& rEvt) 
            throw (::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL itemStateChanged(const ::com::sun::star::awt::ItemEvent& rEvt) 
            throw (::com::sun::star::uno::RuntimeException);

private:
    ::std::auto_ptr<Action> mpAction;
};

// ============================================================================

/** 
 * TODO: Define an action class for this listener.
 *
 * @author kyoshida
 * @return 
 */
class FocusListener : public ::cppu::WeakImplHelper1< ::com::sun::star::awt::XFocusListener >
{
public:
    FocusListener() {}
    virtual ~FocusListener() throw() = 0;
    
    virtual void SAL_CALL focusGained( const ::com::sun::star::awt::FocusEvent& ) throw( ::com::sun::star::uno::RuntimeException ) {}
    virtual void SAL_CALL focusLost( const ::com::sun::star::awt::FocusEvent& ) throw( ::com::sun::star::uno::RuntimeException ) {}
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& ) throw ( ::com::sun::star::uno::RuntimeException ) {}
};  

/** 
 * TODO: Define an action class for this listener. 
 */
class MouseListener : public ::cppu::WeakImplHelper1< ::com::sun::star::awt::XMouseListener >
{
public:
    MouseListener() {}
    virtual ~MouseListener() throw() = 0;
    
    virtual void SAL_CALL mousePressed( const ::com::sun::star::awt::MouseEvent& ) throw( ::com::sun::star::uno::RuntimeException ) {}
    virtual void SAL_CALL mouseReleased( const ::com::sun::star::awt::MouseEvent& ) throw( ::com::sun::star::uno::RuntimeException ) {}
    virtual void SAL_CALL mouseEntered( const ::com::sun::star::awt::MouseEvent& ) throw( ::com::sun::star::uno::RuntimeException ) {}
    virtual void SAL_CALL mouseExited( const ::com::sun::star::awt::MouseEvent& ) throw( ::com::sun::star::uno::RuntimeException ) {}
    
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& ) throw ( ::com::sun::star::uno::RuntimeException ) {}
};

class TopWindowListenerImpl;

class TopWindowListener : public ::cppu::WeakImplHelper1< ::com::sun::star::awt::XTopWindowListener >
{
public:
    TopWindowListener(BaseDialog* pDlg);
    virtual ~TopWindowListener() throw();

    virtual void SAL_CALL windowOpened( const ::com::sun::star::lang::EventObject& e )
            throw (::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL windowClosing( const ::com::sun::star::lang::EventObject& e )
            throw (::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL windowClosed( const ::com::sun::star::lang::EventObject& e )
            throw (::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL windowMinimized( const ::com::sun::star::lang::EventObject& e )
            throw (::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL windowNormalized( const ::com::sun::star::lang::EventObject& e )
            throw (::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL windowActivated( const ::com::sun::star::lang::EventObject& e )
            throw (::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL windowDeactivated( const ::com::sun::star::lang::EventObject& e )
            throw (::com::sun::star::uno::RuntimeException);

    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& e )
            throw (::com::sun::star::uno::RuntimeException);

    void setActionClosing( SimpleActionObject* p );

private:
    ::std::auto_ptr<TopWindowListenerImpl> m_pImpl;
};

}

#endif
