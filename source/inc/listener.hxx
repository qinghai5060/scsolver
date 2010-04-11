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

#ifndef _LISTENER_HXX_
#define _LISTENER_HXX_

#include "unoglobal.hxx"
#include "type.hxx"
#include "baselistener.hxx"

#include <cppuhelper/implbase1.hxx>
#include <com/sun/star/sheet/XRangeSelectionListener.hpp>
#include <com/sun/star/awt/XWindow.hpp>

#include <memory>

namespace com { namespace sun { namespace star {

    namespace sheet
    {
        class RangeSelectionEvent;
        class XRangeSelection;
    }

}}}



namespace scsolver {

class RngBtnListener;
class BaseDialog;
class Listener;
class FocusListener;
class ItemListener;
class ActionListener;

class MessageDialog;

class SolverDialog;
class SolveBtnListener;
class CloseBtnListener;
class RadioBtnListener;
class ConstEditBtnListener;
class WindowFocusListener;
class WindowMouseListener;
class ConstListBoxListener;
class OKCancelBtnListener;

//--------------------------------------------------------------------------
// Class RngSelListener

class RngSelListener : public ::cppu::WeakImplHelper1< ::com::sun::star::sheet::XRangeSelectionListener >
{
public:
    RngSelListener( BaseDialog*, RngBtnListener*, const rtl::OUString& );
    ~RngSelListener() throw();
    
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& ) 
        throw ( ::com::sun::star::uno::RuntimeException );
    virtual void SAL_CALL done( const ::com::sun::star::sheet::RangeSelectionEvent& ) 
        throw ( ::com::sun::star::uno::RuntimeException );
    virtual void SAL_CALL aborted( const ::com::sun::star::sheet::RangeSelectionEvent& ) 
        throw ( ::com::sun::star::uno::RuntimeException );
    
private:
    BaseDialog* m_pDlg;
    RngBtnListener* m_pBtn;
    rtl::OUString m_sEditName;
};


//---------------------------------------------------------------------------
// Class RngBtnListener

class RngBtnListener : public ActionListener
{
public:
    RngBtnListener( BaseDialog* pDlg, ::com::sun::star::uno::Reference< ::com::sun::star::sheet::XRangeSelection >, const rtl::OUString& );
    virtual ~RngBtnListener() throw();

    // XEventListener
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& oEvt ) 
        throw ( ::com::sun::star::uno::RuntimeException );

    // XActionListener
    virtual void SAL_CALL actionPerformed( const ::com::sun::star::awt::ActionEvent& ) 
        throw ( ::com::sun::star::uno::RuntimeException );

    void registerRngSelListener();  
    void setEventOwner( bool b ) { m_bEventOwner = b; }
    void setSingleCell( bool b ) { m_bSingleCell = b; }
    bool isEventOwner() { return m_bEventOwner; }
    
private:

    ::com::sun::star::uno::Reference< ::com::sun::star::sheet::XRangeSelection > m_xRngSel;
    RngSelListener* m_pRngSelListener;
    rtl::OUString m_sEditName;
    bool m_bEventOwner;
    bool m_bSingleCell;
};


class SolveBtnListener : public ActionListener
{

public:
    
    SolveBtnListener( SolverDialog* );
    virtual ~SolveBtnListener() throw();

    // XEventListener
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& ) 
        throw ( ::com::sun::star::uno::RuntimeException );

    // XActionListener
    virtual void SAL_CALL actionPerformed( const ::com::sun::star::awt::ActionEvent& ) 
        throw ( ::com::sun::star::uno::RuntimeException );
};

/**
 * Generic close button listener.  You can use this class for
 * any dialog that needs a simple "close" action.  Just pass a
 * pointer to the target dialog to the constructor when
 * instantiating an object, and register it with the "close"
 * button.
 */
class CloseBtnListener : public ActionListener
{
public:
    CloseBtnListener( BaseDialog* pDlg );
    virtual ~CloseBtnListener() throw();

    // XEventListener
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& oEvt )
        throw ( ::com::sun::star::uno::RuntimeException );

    // XActionListener
    virtual void SAL_CALL actionPerformed( const ::com::sun::star::awt::ActionEvent& )
        throw ( ::com::sun::star::uno::RuntimeException );
};


class SaveBtnListener : public ActionListener
{

public:
    
    SaveBtnListener( SolverDialog* pDlg );
    virtual ~SaveBtnListener() throw();

    // XEventListener
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& oEvt ) 
        throw ( ::com::sun::star::uno::RuntimeException );

    // XActionListener
    virtual void SAL_CALL actionPerformed( const ::com::sun::star::awt::ActionEvent& ) 
        throw ( ::com::sun::star::uno::RuntimeException );
};


class LoadBtnListener : public ActionListener
{

public:
    
    LoadBtnListener( SolverDialog* pDlg );
    virtual ~LoadBtnListener() throw();

    // XEventListener
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& oEvt ) 
        throw ( ::com::sun::star::uno::RuntimeException );

    // XActionListener
    virtual void SAL_CALL actionPerformed( const ::com::sun::star::awt::ActionEvent& ) 
        throw ( ::com::sun::star::uno::RuntimeException );
};


class ResetBtnListener : public ::scsolver::ActionListener
{

public:
    
    ResetBtnListener( SolverDialog* pDlg );
    virtual ~ResetBtnListener() throw();

    // XEventListener
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& oEvt ) 
        throw ( ::com::sun::star::uno::RuntimeException );

    // XActionListener
    virtual void SAL_CALL actionPerformed( const ::com::sun::star::awt::ActionEvent& ) 
        throw ( ::com::sun::star::uno::RuntimeException );

private:
    std::auto_ptr<MessageDialog> m_pDlg;    
};


class OptionBtnListener : public ::scsolver::ActionListener
{
public:
    OptionBtnListener( SolverDialog* pDlg );
    virtual ~OptionBtnListener() throw();

    // XEventListener
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& oEvt ) 
        throw ( ::com::sun::star::uno::RuntimeException );

    // XActionListener
    virtual void SAL_CALL actionPerformed( const ::com::sun::star::awt::ActionEvent& oEvt ) 
        throw ( ::com::sun::star::uno::RuntimeException );

private:
    std::auto_ptr<MessageDialog> m_pDlg;
};


class ConstEditBtnListener : public ::scsolver::ActionListener
{
    
public:

    ConstEditBtnListener( SolverDialog*, ConstButtonType );
    virtual ~ConstEditBtnListener() throw();

    // XEventListener
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& oEvt ) 
        throw ( ::com::sun::star::uno::RuntimeException );

    // XActionListener
    virtual void SAL_CALL actionPerformed( const ::com::sun::star::awt::ActionEvent& ) 
        throw ( ::com::sun::star::uno::RuntimeException );

private:

    ConstButtonType m_eBtnType;
    ConstButtonType getButtonType() const { return m_eBtnType; }

};

#if 0
class WindowFocusListener : public FocusListener
{
public:

    WindowFocusListener( BaseDialog* );
    virtual ~WindowFocusListener() throw() {}
    
    virtual rtl::OUString getListenerType() const { return ascii( "FocusListener" ); }
    
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& ) 
        throw ( ::com::sun::star::uno::RuntimeException ) {}
    virtual void SAL_CALL focusGained( const ::com::sun::star::awt::FocusEvent& ) 
        throw( ::com::sun::star::uno::RuntimeException );
    virtual void SAL_CALL focusLost( const ::com::sun::star::awt::FocusEvent& ) 
        throw( ::com::sun::star::uno::RuntimeException );
    
};
#endif

class WindowMouseListener : public MouseListener
{
public:

    WindowMouseListener( BaseDialog* );
    virtual ~WindowMouseListener() throw();
    
    virtual void SAL_CALL mousePressed( const ::com::sun::star::awt::MouseEvent& ) 
        throw( ::com::sun::star::uno::RuntimeException );
    virtual void SAL_CALL mouseReleased( const ::com::sun::star::awt::MouseEvent& ) 
        throw( ::com::sun::star::uno::RuntimeException ) {}
    virtual void SAL_CALL mouseEntered( const ::com::sun::star::awt::MouseEvent& ) 
        throw( ::com::sun::star::uno::RuntimeException ) {}
    virtual void SAL_CALL mouseExited( const ::com::sun::star::awt::MouseEvent& ) 
        throw( ::com::sun::star::uno::RuntimeException ) {}
    
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& ) 
        throw ( ::com::sun::star::uno::RuntimeException ) {}
};

//--------------------------------------------------------------------------

class OKCancelBtnListener : public ::scsolver::ActionListener
{
public:
    
    OKCancelBtnListener( BaseDialog*, const rtl::OUString& );
    virtual ~OKCancelBtnListener() throw();

    // XEventListener
    virtual void SAL_CALL disposing( const ::com::sun::star::lang::EventObject& ) 
        throw ( ::com::sun::star::uno::RuntimeException );

    // XActionListener
    virtual void SAL_CALL actionPerformed( const ::com::sun::star::awt::ActionEvent& ) 
        throw ( ::com::sun::star::uno::RuntimeException );
        
private:
    rtl::OUString m_sMode;
};

}


#endif // _LISTENER_HXX
