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

#include "dialog.hxx"
#include "tool/global.hxx"
#include "unoglobal.hxx"
#include "solver.hxx"
#include "listener.hxx"
#include "xcalc.hxx"
#include "unohelper.hxx"
#include "msgdlg.hxx"
#include "optiondlg.hxx"

#include <rtl/ustrbuf.hxx>

#include "scsolver.hrc"

#include <com/sun/star/awt/PosSize.hpp>
#include <com/sun/star/awt/PushButtonType.hpp>
#include <com/sun/star/awt/XButton.hpp>
#include <com/sun/star/awt/XControl.hpp>
#include <com/sun/star/awt/XControlModel.hpp>
#include <com/sun/star/awt/XControlContainer.hpp>
#include <com/sun/star/awt/XListBox.hpp>
#include <com/sun/star/awt/XRadioButton.hpp>
#include <com/sun/star/awt/XTextComponent.hpp>
#include <com/sun/star/awt/XWindow.hpp>
#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/container/XNameContainer.hpp>
#include <com/sun/star/container/XNamed.hpp>
#include <com/sun/star/frame/XDispatchProvider.hpp>
#include <com/sun/star/frame/XModel.hpp>
#include <com/sun/star/frame/XNotifyingDispatch.hpp>
#include <com/sun/star/lang/XInitialization.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/lang/XServiceInfo.hpp>
#include <com/sun/star/lang/XServiceName.hpp>
#include <com/sun/star/sheet/XRangeSelection.hpp>
#include <com/sun/star/uno/XComponentContext.hpp>

#include <iostream>

using namespace ::scsolver::numeric;
using ::com::sun::star::container::XNameContainer;
using ::com::sun::star::uno::Reference;
using ::com::sun::star::uno::XInterface;
using ::com::sun::star::uno::UNO_QUERY;
using ::com::sun::star::uno::Sequence;
using ::scsolver::RadioBtnListener;
using ::rtl::OUString;
using ::std::vector;
using ::std::cout;
using ::std::endl;
using ::std::distance;

namespace {

using namespace ::scsolver;

// ----------------------------------------------------------------------------

class ConstraintListBoxAction : public ItemListener::Action
{
public:
    explicit ConstraintListBoxAction(SolverDialog* pDlg) :
        mpDlg(pDlg)
    {
    }

    virtual ~ConstraintListBoxAction()
    {
    }

    virtual void itemStateChanged(const awt::ItemEvent& /*rEvt*/)
    {
        mpDlg->enableWidget(ascii("btnConstChange"));
        mpDlg->enableWidget(ascii("btnConstDelete"));
    }

private:
    SolverDialog* mpDlg;
};

// ----------------------------------------------------------------------------

class GoalRadioBtnAction : public ItemListener::Action
{
public:
    enum BtnType
    {
        Max,
        Min,
        ToValue
    };
    explicit GoalRadioBtnAction(SolverDialog* pDlg, BtnType eType) :
        mpDlg(pDlg), meBtnType(eType)
    {
    }

    virtual ~GoalRadioBtnAction()
    {
    }

    virtual void itemStateChanged(const awt::ItemEvent& rEvt)
    {
        mpDlg->enableWidget(ascii("editSolveToValue"), meBtnType == ToValue);
    }

private:
    SolverDialog* mpDlg;
    BtnType meBtnType;
};

}

namespace scsolver {

/**
 * Function object that defines action when a window close event
 * is received (typically when the user clicks the "x" button on
 * the title bar).
 */
class ConstDlgCloseAction : public SimpleActionObject
{
public:
    virtual void execute(BaseDialog *dlg)
    {
        dlg->close();
    }
};

/**
 * Defines action when a window close event is received.
 */
class SolverDlgCloseAction : public SimpleActionObject
{
public:
    virtual void execute(BaseDialog *dlg)
    {
        dlg->close();
    }
};

ConstEditDialog::ConstEditDialog( SolverImpl* p ) :
    BaseDialog(p),
    m_bIsChangeMode(false),
    m_nConstraintId(0),
    m_pOKListener(NULL),
    m_pCancelListener(NULL),
    m_pLeftRngListener(NULL),
    m_pRightRngListener(NULL),
    m_pTopWindowListener(NULL),
    m_pCloseAction( new ::ConstDlgCloseAction )
{
    initialize();
}

ConstEditDialog::~ConstEditDialog() throw()
{   
    unregisterListeners();
}

void ConstEditDialog::registerListeners()
{
    CalcInterface* pCalc = getSolverImpl()->getCalcInterface();

    m_pOKListener = new OKCancelBtnListener( this, ascii( "OK" ) );
    registerListener( ascii( "btnOK" ), m_pOKListener );
    
    m_pCancelListener = new OKCancelBtnListener( this, ascii( "Cancel" ) );
    registerListener( ascii( "btnCancel" ), m_pCancelListener );
    
    Reference< sheet::XRangeSelection > xRngSel = pCalc->getXRangeSelection();
    OSL_ASSERT( xRngSel != NULL );
    
    m_pLeftRngListener = new RngBtnListener( this, xRngSel, ascii( "editLeft" ) );
    m_pLeftRngListener->setSingleCell( true );
    registerListener( ascii( "btnLeft" ), m_pLeftRngListener );
        
    m_pRightRngListener = new RngBtnListener( this, xRngSel, ascii( "editRight" ) );
    m_pRightRngListener->setSingleCell( true );
    registerListener( ascii( "btnRight" ), m_pRightRngListener );

    m_pTopWindowListener = new TopWindowListener(this);
    m_pTopWindowListener->setActionClosing(m_pCloseAction);
    registerListener(m_pTopWindowListener);
}

void ConstEditDialog::unregisterListeners()
{
    unregisterListener( ascii( "btnOK" ), m_pOKListener );
    unregisterListener( ascii( "btnCancel" ), m_pCancelListener );
    unregisterListener( ascii( "btnLeft" ), m_pLeftRngListener );
    unregisterListener( ascii( "btnRight" ), m_pRightRngListener );
    unregisterListener(m_pTopWindowListener);
}

void ConstEditDialog::initialize()
{
    initializeDefault( static_cast<sal_Int16>(180), static_cast<sal_Int16>(55), 
        getResStr(SCSOLVER_STR_CONSTRAINTDLG_TITLE) );
    
    sal_Int32 nX, nY;
    
    nX = 5;
    nY = 5;
    addFixedText( nX, nY, 100, 10, ascii( "ftLeft" ), getResStr(SCSOLVER_STR_CELL_REFERENCE) );
    nY += 10;
    addRangeEdit( nX, nY, 70, 12, ascii( "editLeft" ), ascii( "btnLeft" ) );
    nX += 72;
    apWidgetProp p = addListBox( nX, nY, 25, 12, ascii( "lbEqual" ) );

    uno::Any aDropdown, aItems, aMultiSel, aSelItems;
    uno::Sequence< rtl::OUString > sItems( 3 );
    sItems[0] = ascii( "<=" );
    sItems[1] = ascii( "=" );
    sItems[2] = ascii( ">=" );
    aItems <<= sItems;
    uno::Sequence< sal_Int16 > sSelItems( 1 );
    sSelItems[0] = 0;
    aSelItems <<= sSelItems;
    aDropdown <<= static_cast<sal_Bool>(true);
    aMultiSel <<= static_cast<sal_Bool>(false);
    p->setPropertyValue( "Dropdown", aDropdown );
    p->setPropertyValue( "StringItemList", aItems );
    p->setPropertyValue( "MultiSelection", aMultiSel );
    p->setPropertyValue( "SelectedItems", aSelItems );
    
    nX += 27;
    addFixedText( nX, 5, 100, 10, ascii( "ftRight" ), getResStr(SCSOLVER_STR_CONSTRAINT) );
    addRangeEdit( nX, nY, 70, 12, ascii( "editRight" ), ascii( "btnRight" ) );
    
    nX = 5;
    nY = 35;
    addButton( nX, nY, 50, 15, ascii( "btnOK" ), getResStr(SCSOLVER_STR_BTN_OK) );
    addButton( nX+55, nY, 50, 15, ascii( "btnCancel" ), getResStr(SCSOLVER_STR_BTN_CANCEL) );   

    registerListeners();
}

void ConstEditDialog::reset()
{
    Reference< uno::XInterface > oEditLeft = getWidgetByName( ascii( "editLeft" ) );
    OSL_ASSERT( oEditLeft != NULL );
    Reference< awt::XTextComponent > xCompL( oEditLeft, UNO_QUERY );
    xCompL->setText( ascii( "" ) );
    
    Reference< uno::XInterface > oEditRight = getWidgetByName( ascii( "editRight" ) );
    OSL_ASSERT( oEditRight != NULL );
    Reference< awt::XTextComponent > xCompR( oEditRight, UNO_QUERY );
    xCompR->setText( ascii( "" ) );
    
    Reference< uno::XInterface > oEq = getWidgetByName( ascii( "lbEqual" ) );
    OSL_ASSERT( oEq != NULL );
    Reference< awt::XListBox > xEq( oEq, UNO_QUERY );
    xEq->selectItemPos( 0, true );
}

void ConstEditDialog::setVisible( bool bVisible )
{
    if ( bVisible )
        toFront();

    setVisibleDefault( bVisible );
}

bool ConstEditDialog::doneRangeSelection() const
{
    SolverDialog* pDlg = getSolverImpl()->getMainDialog();
    pDlg->setVisible( true );
    setFocus();

    if ( !isCellRangeGeometryEqual() )
    {
        pDlg->showMessage( getResStr(SCSOLVER_STR_MSG_REF_CON_RANGE_MISMATCH) );
        return false;
    }

    return true;
}

void ConstEditDialog::close()
{
    reset();
    setVisible(false);
}

rtl::OUString ConstEditDialog::getLeftCellReference()
{
    Reference< uno::XInterface > oEdit = getWidgetByName( ascii( "editLeft" ) );
    Reference< awt::XTextComponent > xComp( oEdit, UNO_QUERY );
    return xComp->getText();
}

rtl::OUString ConstEditDialog::getRightCellReference()
{
    Reference< uno::XInterface > oEdit = getWidgetByName( ascii( "editRight" ) );
    Reference< awt::XTextComponent > xComp( oEdit, UNO_QUERY );
    return xComp->getText();
}

EqualityType ConstEditDialog::getEquality()
{
    Reference< uno::XInterface > oEq = getWidgetByName( ascii( "lbEqual" ) );
    Reference< awt::XListBox > xEq( oEq, UNO_QUERY );
    if ( xEq != NULL )
    {
        sal_Int16 nPos = xEq->getSelectedItemPos();
        switch( nPos )
        {
            case 0:
                return LESS_EQUAL;
            case 1:
                return EQUAL;
            case 2:
                return GREATER_EQUAL;
            default:
                OSL_ASSERT( !"Unexpected (in)equal sign" );
        }
    }
    else
        OSL_ASSERT( !"xEq == NULL" );
    
    return EQUAL;
}

void ConstEditDialog::setConstraintId( sal_uInt32 nId )
{
    m_nConstraintId = nId;
}

const rtl::OUString ConstEditDialog::getLeftCellReference() const
{
    Reference< uno::XInterface > oEdit = getWidgetByName( ascii( "editLeft" ) );
    Reference< awt::XTextComponent > xComp( oEdit, UNO_QUERY );
    return xComp->getText();
}

const rtl::OUString ConstEditDialog::getRightCellReference() const
{
    Reference< uno::XInterface > oEdit = getWidgetByName( ascii( "editRight" ) );
    Reference< awt::XTextComponent > xComp( oEdit, UNO_QUERY );
    return xComp->getText();
}

void ConstEditDialog::setLeftCellReference( const rtl::OUString& s )
{
    Reference< uno::XInterface > oEdit = getWidgetByName( ascii( "editLeft" ) );
    Reference< awt::XTextComponent > xComp( oEdit, UNO_QUERY ); 
    xComp->setText( s );
}

void ConstEditDialog::setRightCellReference( const rtl::OUString& s )
{
    Reference< uno::XInterface > oEdit = getWidgetByName( ascii( "editRight" ) );
    Reference< awt::XTextComponent > xComp( oEdit, UNO_QUERY ); 
    xComp->setText( s );
}

void ConstEditDialog::setEquality( const EqualityType e )
{
    Reference< uno::XInterface > oEq = getWidgetByName( ascii( "lbEqual" ) );
    Reference< awt::XListBox > xEq( oEq, UNO_QUERY );
    if ( xEq != NULL )
    {
        switch( e )
        {
            case LESS_EQUAL:
                xEq->selectItemPos( 0, true );
                break;
            case EQUAL:
                xEq->selectItemPos( 1, true );
                break;
            case GREATER_EQUAL:
                xEq->selectItemPos( 2, true );
                break;
            default:
                OSL_ASSERT( !"Unexpected Equality" );
        }
    }
    else
        OSL_ASSERT( !"xEq == NULL" );
}

/** Checks if given two cell range references in the constraint dialog have 
    identical geometry. 
    
    @return true if the two cell ranges have identical geometry or at least
    one cell range is empty, or false otherwise.
 */
bool ConstEditDialog::isCellRangeGeometryEqual() const
{
    using table::CellRangeAddress;
    
    rtl::OUString sLeftRef = getLeftCellReference();
    rtl::OUString sRightRef = getRightCellReference();
    if ( sLeftRef.getLength() > 0 && sRightRef.getLength() > 0 )
    {
        CalcInterface* pCalc = getSolverImpl()->getCalcInterface();

        CellRangeAddress aLAddr = pCalc->getCellRangeAddress( sLeftRef );
        CellRangeAddress aRAddr = pCalc->getCellRangeAddress( sRightRef );
        sal_Int32 nLCol = aLAddr.EndColumn - aLAddr.StartColumn;
        sal_Int32 nRCol = aRAddr.EndColumn - aRAddr.StartColumn;
        sal_Int32 nLRow = aLAddr.EndRow - aLAddr.StartRow;
        sal_Int32 nRRow = aRAddr.EndRow - aRAddr.StartRow;

        return nLCol == nRCol && nLRow == nRRow;
    }

    return true;
}

//-----------------------------------------------------------------

SolverDialog::SolverDialog( SolverImpl* p )
    : BaseDialog( p ),

    m_pConstEditDialog( NULL ),
    m_pOptionDialog( NULL ),
    m_pTopWindowListener(NULL),
    m_pTargetRngListener( NULL ),
    m_pVarCellsRngListener( NULL ),
    m_pSolveListener( NULL ),
    m_pOKListener( NULL ),
    m_pSaveListener( NULL ),
    m_pLoadListener( NULL ),
    m_pResetListener( NULL ),
    m_pOptionListener( NULL ),
    m_pBtnMaxListener( NULL ),
    m_pConstAddListener( NULL ),
    m_pConstChangeListener( NULL ),
    m_pConstDeleteListener( NULL ),
    m_pConstListBoxListener( NULL ),
    m_pCloseAction( new SolverDlgCloseAction )
{
    initialize();
}

SolverDialog::~SolverDialog() throw()
{   
    unregisterListeners();
}

void SolverDialog::initialize()
{
    initializeDefault( static_cast<sal_Int16>(260), static_cast<sal_Int16>(200), 
        getResStr(SCSOLVER_STR_MAINDLG_TITLE) );

    sal_Int32 nX = 0, nY = 0;
    
    nY = 5;
    addFixedLine( 5, nY, 195, 12, ascii( "flModel" ), getResStr(SCSOLVER_STR_DEFINE_MODEL) );
    
    nY += 13;
    addFixedText( 10, nY+2, 60, 12, ascii( "ftTargetCell" ), getResStr(SCSOLVER_STR_SET_TARGET_CELL) );
    addRangeEdit( 70, nY, 80, 12, ascii( "editTargetCell" ), ascii( "btnTargetCellRange" ) );

    // Radio buttons for goal selection.
    nY += 18;
    addFixedText( 10, nY, 35, 12, ascii( "ftObj" ), getResStr(SCSOLVER_STR_GOAL) );
    addRadioButton( 50, nY, 40, 12, ascii( "rbMax" ), getResStr(SCSOLVER_STR_MAXIMIZE) );
    addRadioButton( 115, nY, 40, 12, ascii( "rbMin" ), getResStr(SCSOLVER_STR_MINIMIZE) );
    
    nY += 14;
    addRadioButton(50, nY, 55, 12, ascii("rbSolveToValue"), getResStr(SCSOLVER_STR_SOLVE_TO_VALUE));
    apWidgetProp p = addEdit(110, nY-1, 30, 12, ascii("editSolveToValue"));
    p->setEnabled(false);
        
    // Decision variable cells
    nY += 18;
    addFixedText( 10, nY+2, 65, 12, ascii( "ftDecVars" ), getResStr(SCSOLVER_STR_DECISIONVAR_CELLS) );
    addRangeEdit( 85, nY, 115, 12, ascii( "editVarCells" ), ascii( "btnRangeSelect" ) );
    
    nY += 20;
    addFixedLine( 5, nY, 195, 12, ascii( "flConstraints" ), getResStr(SCSOLVER_STR_CONSTRAINT_SEP) );
    
    nY += 13;
    nX = 10;
    addListBox( nX, nY, 130, 80, ascii( "lbConstraint" ) );
    nX += 135;

    addButton( nX, nY,    50, 15, ascii( "btnConstAdd" ), getResStr(SCSOLVER_STR_BTN_ADD) );

    p = addButton( 
        nX, nY+20, 50, 15, ascii( "btnConstChange" ), getResStr(SCSOLVER_STR_BTN_CHANGE) );
    p->setEnabled(false);
    
    p = addButton( 
        nX, nY+40, 50, 15, ascii( "btnConstDelete" ), getResStr(SCSOLVER_STR_BTN_DELETE) );
    p->setEnabled(false);
    
    addButton( 205, 10, 50, 15, ascii( "btnSolve" ), getResStr(SCSOLVER_STR_BTN_SOLVE) );
    addButton( 205, 30, 50, 15, ascii( "btnReset" ), getResStr(SCSOLVER_STR_BTN_RESET) );
    
    p = addButton( 205, 50, 50, 15, ascii( "btnOptions" ), getResStr(SCSOLVER_STR_BTN_OPTIONS) );

    addButton( 205, 90, 50, 15, ascii( "btnSave" ), getResStr(SCSOLVER_STR_BTN_SAVE_MODEL) );
    addButton( 205, 110, 50, 15, ascii( "btnLoad" ), getResStr(SCSOLVER_STR_BTN_LOAD_MODEL) );

    // Buttons at the bottom of the dialog.
    addButton( 150, 180, 50, 15, ascii("btnClose"), getResStr(SCSOLVER_STR_BTN_CLOSE) );
    addButton( 205, 180, 50, 15, ascii("btnHelp"),  ascii(""), awt::PushButtonType_HELP );

    registerListeners();
}

void SolverDialog::registerListeners()
{
    CalcInterface* pCalc = getSolverImpl()->getCalcInterface();

    Reference< sheet::XRangeSelection > xRngSel = pCalc->getXRangeSelection();
    OSL_ASSERT( xRngSel != NULL );

    m_pVarCellsRngListener = new RngBtnListener( this, xRngSel, ascii( "editVarCells" ) );
    m_pVarCellsRngListener->setSingleCell( false );
    registerListener( ascii( "btnRangeSelect" ), m_pVarCellsRngListener );

    m_pTargetRngListener = new RngBtnListener( this, xRngSel, ascii( "editTargetCell" ) );
    m_pTargetRngListener->setSingleCell( true );
    registerListener( ascii( "btnTargetCellRange" ), m_pTargetRngListener );
    
    m_pSolveListener = new SolveBtnListener( this );
    registerListener( ascii( "btnSolve" ), m_pSolveListener );
    
    m_pOKListener = new CloseBtnListener( this );
    registerListener( ascii( "btnClose" ), m_pOKListener );
    
    m_pSaveListener = new SaveBtnListener( this );
    registerListener( ascii( "btnSave" ), m_pSaveListener );
    m_pLoadListener = new LoadBtnListener( this );
    registerListener( ascii( "btnLoad" ), m_pLoadListener );
    
    m_pResetListener = new ResetBtnListener( this );
    registerListener( ascii( "btnReset" ), m_pResetListener );

    m_pOptionListener = new OptionBtnListener( this );
    registerListener( ascii( "btnOptions" ), m_pOptionListener );

    // Listeners for the goal radio buttons.
    m_pBtnMaxListener     = new ItemListener(new GoalRadioBtnAction(this, GoalRadioBtnAction::Max));
    m_pBtnMinListener     = new ItemListener(new GoalRadioBtnAction(this, GoalRadioBtnAction::Min));
    m_pBtnToValueListener = new ItemListener(new GoalRadioBtnAction(this, GoalRadioBtnAction::ToValue));
    registerListener(ascii("rbMax"),          m_pBtnMaxListener);
    registerListener(ascii("rbMin"),          m_pBtnMinListener);
    registerListener(ascii("rbSolveToValue"), m_pBtnToValueListener);
    
    m_pConstAddListener = new ConstEditBtnListener( this, CONST_ADD );
    registerListener( ascii( "btnConstAdd" ), m_pConstAddListener );

    m_pConstChangeListener = new ConstEditBtnListener( this, CONST_CHANGE );
    registerListener( ascii( "btnConstChange" ), m_pConstChangeListener );

    m_pConstDeleteListener = new ConstEditBtnListener( this, CONST_DELETE );
    registerListener( ascii( "btnConstDelete" ), m_pConstDeleteListener );

    m_pConstListBoxListener = new ItemListener(new ConstraintListBoxAction(this));
    registerListener( ascii( "lbConstraint" ), m_pConstListBoxListener );
    
    m_pTopWindowListener = new TopWindowListener(this);
    m_pTopWindowListener->setActionClosing(m_pCloseAction);
    registerListener(m_pTopWindowListener);
}

void SolverDialog::unregisterListeners()
{
    // Unregistering a listener object from its associated widget causes it
    // to be automatically destroyed, so that a manual delete of such object 
    // is no longer necessary.
    
    unregisterListener( ascii( "btnRangeSelect" ), m_pVarCellsRngListener );
    unregisterListener( ascii( "btnTargetCellRange" ), m_pTargetRngListener );
    unregisterListener( ascii( "btnClose" ), m_pOKListener );
    unregisterListener( ascii( "btnSave" ), m_pSaveListener );
    unregisterListener( ascii( "btnLoad" ), m_pLoadListener );
    unregisterListener( ascii( "btnReset" ), m_pResetListener );
    unregisterListener( ascii( "btnOptions" ), m_pOptionListener );

    unregisterListener(ascii("rbMax"),          m_pBtnMaxListener);
    unregisterListener(ascii("rbMin"),          m_pBtnMinListener);
    unregisterListener(ascii("rbSolveToValue"), m_pBtnToValueListener);

    unregisterListener( ascii( "btnConstAdd" ), m_pConstAddListener );
    unregisterListener( ascii( "btnConstChange" ), m_pConstChangeListener );
    unregisterListener( ascii( "btnConstDelete" ), m_pConstDeleteListener );
    unregisterListener( ascii( "lbConstraint" ), m_pConstListBoxListener );
    unregisterListener(m_pTopWindowListener);
}

void SolverDialog::setVisible( bool bVisible )
{
    if ( bVisible )
    {
        // Get currently active sheet (no use as long as the dialog is modeless).
        CalcInterface* pCalc = getSolverImpl()->getCalcInterface();

        Reference< sheet::XSpreadsheet > xSheet = pCalc->getActiveSheet();
        Reference< container::XNamed > xNamed( xSheet, UNO_QUERY );
//      printOUStr( ascii( "Current sheet is " ) + xNamed->getName() );
    }
    else
        getConstEditDialog()->setVisible( false );

    setVisibleDefault( bVisible );
}

void SolverDialog::close()
{
    getConstEditDialog()->reset();
    getConstEditDialog()->setVisible(false);
    getOptionDialog()->setVisible(false);
    setVisible(false);
}

ConstEditDialog* SolverDialog::getConstEditDialog()
{
    if (!m_pConstEditDialog.get())
    {
        m_pConstEditDialog.reset( new ConstEditDialog(getSolverImpl()) );
        m_pConstEditDialog->setRefBoundingBox(getPosSize());
    }
    
    return m_pConstEditDialog.get();
}

OptionDialog* SolverDialog::getOptionDialog()
{
    if(!m_pOptionDialog.get())
    {
        m_pOptionDialog.reset( new OptionDialog( getSolverImpl() ) );
        m_pOptionDialog->setRefBoundingBox(getPosSize());
    }

    return m_pOptionDialog.get();
}

sal_Int16 SolverDialog::getSelectedConstraintPos()
{
    Reference< uno::XInterface > oWgt = getWidgetByName( ascii( "lbConstraint" ) );
    OSL_ASSERT( oWgt != NULL );
    Reference< awt::XListBox > xLB( oWgt, UNO_QUERY );
    OSL_ASSERT( xLB != NULL );
    return xLB->getSelectedItemPos();
}

Reference< awt::XTextComponent > SolverDialog::getXTextComponentFromWidget(
    const rtl::OUString& sName ) const
{
    Reference< uno::XInterface > oRngEdit = getWidgetByName( sName );
    OSL_ASSERT( oRngEdit != NULL );
    Reference< awt::XTextComponent > xComp( oRngEdit, UNO_QUERY );
    return xComp;
}

rtl::OUString SolverDialog::getTargetCellAddress() const
{
    Reference< awt::XTextComponent > xComp = getXTextComponentFromWidget( 
            ascii( "editTargetCell" ) );
    return xComp->getText();
}

void SolverDialog::setTargetCellAddress( const rtl::OUString& sAddr )
{
    Reference< awt::XTextComponent > xComp = getXTextComponentFromWidget( 
            ascii( "editTargetCell" ) );
    xComp->setText( sAddr );
}

rtl::OUString SolverDialog::getVarCellAddress()
{
    Reference< awt::XTextComponent > xComp = getXTextComponentFromWidget( 
            ascii( "editVarCells" ) );
    return xComp->getText();
}

void SolverDialog::setVarCellAddress( const rtl::OUString& sAddr )
{
    Reference< awt::XTextComponent > xComp = getXTextComponentFromWidget( 
            ascii( "editVarCells" ) );
    xComp->setText( sAddr );
}

GoalType SolverDialog::getGoal() const
{
    Reference< uno::XInterface > xModel = getWidgetModelByName( ascii( "rbMax" ) );
    sal_Int16 nState;
    unohelper::getPropertyValue( xModel, ascii( "State" ), nState );
    if ( nState )
        return GOAL_MAXIMIZE;
    
    xModel = getWidgetModelByName( ascii( "rbMin" ) );
    unohelper::getPropertyValue( xModel, ascii( "State" ), nState );
    if ( nState )
        return GOAL_MINIMIZE;

    xModel = getWidgetModelByName(ascii("rbSolveToValue"));
    unohelper::getPropertyValue(xModel, ascii("State"), nState);
    if (nState)
        return GOAL_TOVALUE;

    return GOAL_UNKNOWN;
}

void SolverDialog::setGoal( GoalType eGoal )
{
    rtl::OUString sName = ascii( "" );
    switch ( eGoal )
    {
        case GOAL_MAXIMIZE:
            sName = ascii("rbMax");
        break;
        case GOAL_MINIMIZE:
            sName = ascii("rbMin");
        break;
        case GOAL_TOVALUE:
            sName = ascii("rbSolveToValue");
        break;    
        case GOAL_UNKNOWN:
            break;
    }

    if ( sName.getLength() > 0 )
    {
        Reference< uno::XInterface > xModel = getWidgetModelByName( sName );
        sal_Int16 nState = 1;
        uno::Any aVal;
        aVal <<= nState;
        unohelper::setPropertyValue( xModel, ascii( "State" ), aVal );
        enableWidget(ascii("editSolveToValue"), eGoal == GOAL_TOVALUE);
    }
}

double SolverDialog::getSolveToValue() const
{
    Reference< awt::XTextComponent > xComp = getXTextComponentFromWidget(
        ascii("editSolveToValue"));
    if (xComp.is())
    {
        OUString aTextVal = xComp->getText();
        return aTextVal.toDouble();
    }
    return 0.0;
}

void SolverDialog::output()
{
    cout << "----------------------------------------------" << endl;
    cout << "Subject to the constraints:" << endl;

    vector< ConstraintString >::iterator pos = m_aConstraint.begin();
    while( pos != m_aConstraint.end() )
    {
        ConstraintString aItem = *pos;
        rtl::OUString sLine = ascii( "  " ) + aItem.Left;
        switch ( aItem.Equal )
        {
            case GREATER_EQUAL:
                sLine += ascii( " >= " );
                break;
            case EQUAL:
                sLine += ascii( " = " );
                break;
            case LESS_EQUAL:
                sLine += ascii( " <= " );
                break;
            default:
                OSL_ASSERT( !"wrong equality type" );
        }
        sLine += aItem.Right;
        printOUStr( sLine );
        ++pos;
    }
    cout << "----------------------------------------------" << endl;
}

void SolverDialog::getConstraint( const sal_uInt32 nIdx, 
    rtl::OUString& sLeft, rtl::OUString& sRight, EqualityType& eEq )
{
    ConstraintString aConstItem = m_aConstraint.at( nIdx );
    sLeft = aConstItem.Left;
    sRight = aConstItem.Right;
    eEq = aConstItem.Equal;
}

vector< ConstraintString > SolverDialog::getAllConstraints() const
{
    return m_aConstraint;
}

void SolverDialog::setConstraint( 
        const rtl::OUString& sLeft, const rtl::OUString& sRight, const EqualityType eEq )
{
    ConstraintString aConstItem;
    aConstItem.Left = sLeft;
    aConstItem.Right = sRight;
    aConstItem.Equal = eEq;
    m_aConstraint.push_back( aConstItem );
    
#if SCSOLVER_DEBUG  
    output();
#endif    
    setConstraintImpl( sLeft, sRight, eEq, false );
}

void SolverDialog::editConstraint( const sal_uInt32 nId,
        const rtl::OUString& sLeft, const rtl::OUString& sRight, const EqualityType eEqVal )
{
    if ( m_aConstraint.size() <= nId )
        return;
    
    vector< ConstraintString > aConstraint;
    vector< ConstraintString >::iterator pos = m_aConstraint.begin();
    sal_uInt32 nCurId = 0;
    while ( pos != m_aConstraint.end() )
    {
        if ( nCurId == nId )
        {
            ConstraintString aConstItem;
            aConstItem.Left = sLeft;
            aConstItem.Right = sRight;
            aConstItem.Equal = eEqVal;
            m_aConstraint.erase( pos );
            m_aConstraint.insert( pos, aConstItem );
        }
        ++pos; ++nCurId;
    }
    
#if SCSOLVER_DEBUG  
    output();
#endif    
    setConstraintImpl( sLeft, sRight, eEqVal, true, nId );
}

void SolverDialog::removeConstraint( const sal_uInt32 nIdx )
{   
    if ( m_aConstraint.size() <= nIdx )
        return;
    
    sal_uInt32 nCurId = 0;
    vector< ConstraintString >::iterator pos = m_aConstraint.begin();
    while ( pos != m_aConstraint.end() )
    {
        if ( nCurId == nIdx )
        {
            m_aConstraint.erase( pos );
            break;
        }
        ++pos; ++nCurId;
    }

#if SCSOLVER_DEBUG  
    output();
#endif    
    
    removeConstraintsFromListBox( nIdx, 1 );
}

/** Remove all constraints. */
void SolverDialog::clearConstraints()
{
    m_aConstraint.clear();
    removeConstraintsFromListBox();
}

/** Remove constraint entries from the list box.

    @param nPos position of the first entry to be removed
    @param nCount number of entires to be removed (all remaining items if nCount = 0)
*/
void SolverDialog::removeConstraintsFromListBox( sal_Int16 nPos, sal_Int16 nCount )
{
    Reference< uno::XInterface > oWgt = getWidgetByName( ascii( "lbConstraint" ) );
    OSL_ASSERT( oWgt != NULL );
    Reference< awt::XListBox > xLB( oWgt, UNO_QUERY );
    OSL_ASSERT( xLB != NULL );
    if ( nCount == 0 )
    {
        sal_Int16 nItemCount = xLB->getItemCount();
        nCount = nItemCount - nPos;
    }
    xLB->removeItems( nPos, nCount );   
}

/** Set a new constraint or modify an existing one within the dialog.

    Implements displaying of a new constraint or an existing constraint that has
    been modified.  Note that this function takes care of just the view portion
    of the addition/modification, and it does NOT alter the internal constraint 
    data in a model.
*/
void SolverDialog::setConstraintImpl( 
        const rtl::OUString& sLeft, const rtl::OUString& sRight, const EqualityType eEq,
        const sal_Bool bChangeMode, const sal_uInt32 nId )
{
    rtl::OUString sEq;
    switch( eEq )
    {
        case GREATER_EQUAL:
            sEq = ascii( ">=" );
            break;
        case EQUAL:
            sEq = ascii( "=" );
            break;
        case LESS_EQUAL:
            sEq = ascii( "<=" );
            break;
        default:
            OSL_ASSERT( !"Unknown equality" );
    }
    
    rtl::OUString sWhole = sLeft + ascii( " " ) + sEq + ascii( " " ) + sRight;
    printOUStr( sWhole );
    Reference< uno::XInterface > oLB = getWidgetByName( ascii( "lbConstraint" ) );
    if ( oLB != NULL )
    {
        Reference< awt::XListBox > xLB( oLB, UNO_QUERY );
        if ( xLB != NULL )
        {
            if ( bChangeMode )
            {
                xLB->removeItems( nId, 1 );
                xLB->addItem( sWhole, nId );
            }
            else
            {
                sal_Int16 nCount = xLB->getItemCount();
                xLB->addItem( sWhole, nCount );
            }
        }
        else
            OSL_ASSERT( !"xLB is NULL" );
    }
    else
        OSL_ASSERT( !"oLB is NULL" );
}

void SolverDialog::showMessage( const rtl::OUString& sMsg )
{
    MessageDialog dlg(getSolverImpl(), sMsg);
    dlg.setRefBoundingBox(getPosSize());
    dlg.execute();
}

void SolverDialog::showSolutionInfeasible()
{
    showMessage(getResStr(SCSOLVER_STR_MSG_SOLUTION_NOT_FOUND));
}

void SolverDialog::showSolutionFound()
{
    showMessage(getResStr(SCSOLVER_STR_MSG_SOLUTION_FOUND));
}

void SolverDialog::reset()
{
    setTextByWidget( this, ascii( "editTargetCell" ), ascii( "" ) );
    setTextByWidget( this, ascii( "editVarCells" ), ascii( "" ) );
    setGoal( GOAL_UNKNOWN );
    clearConstraints();
}

void SolverDialog::saveModelToDocument()
{
    using rtl::OUString;
    using rtl::OUStringBuffer;
    
    OUStringBuffer sb;
    
    sb.append( getTextByWidget( this, ascii( "editTargetCell" ) ) );
    sb.append( Global::STRING_SEPARATOR );
    GoalType eGoal = getGoal();
    switch ( eGoal )
    {
    case GOAL_MAXIMIZE:
        sb.append( ascii( "max" ) );
        break;
    case GOAL_MINIMIZE:
        sb.append( ascii( "min" ) );
        break;
    default:
        OSL_ASSERT( eGoal == GOAL_UNKNOWN );
        sb.append( ascii( "unknown" ) );
    }
    sb.append( Global::STRING_SEPARATOR );
    sb.append( getTextByWidget( this, ascii( "editVarCells" ) ) );
    
    typedef std::vector< ConstraintString > ConStr;
    ConStr cnConStr = getAllConstraints();
    ConStr::iterator itEnd = cnConStr.end();
    for ( ConStr::iterator it = cnConStr.begin(); it != itEnd; ++it )
    {
        sb.append( Global::STRING_SEPARATOR );
        sb.append( it->Left );
        sb.append( Global::STRING_SEPARATOR );
        switch ( it->Equal )
        {
        case numeric::EQUAL:
            sb.append( ascii( "=" ) );
            break;
        case numeric::GREATER_EQUAL:
            sb.append( ascii( ">=" ) );
            break;
        case numeric::LESS_EQUAL:
            sb.append( ascii( "<=" ) );
            break;
        default:
            OSL_ASSERT( !"unknown equality sign" );
        }
        sb.append( Global::STRING_SEPARATOR );
        sb.append( it->Right );
    }

    OUString sStr = sb.makeStringAndClear();
    CalcInterface* pCalc = getSolverImpl()->getCalcInterface();
    uno::Any aVal;
    aVal <<= sStr;
    pCalc->setDocumentProperty( Global::MODEL_METADATA_NAME, aVal );
}

void SolverDialog::loadModelFromDocument()
{
    using rtl::OUString;
    typedef std::vector< rtl::OUString > ContStr;

    CalcInterface* pCalc = getSolverImpl()->getCalcInterface();
    uno::Any aVal = pCalc->getDocumentProperty( Global::MODEL_METADATA_NAME );
    OUString sVal;
    aVal >>= sVal;
    OUString sSep = Global::STRING_SEPARATOR;
    ContStr cnVal;
    split( sVal, sSep, cnVal );
    clearConstraints();

    ContStr::iterator it, itBeg = cnVal.begin(), itEnd = cnVal.end();
    for ( it = itBeg; it != itEnd; ++it )
    {
        sal_Int32 nIdx = distance( itBeg, it );
        if ( nIdx == 0 )
            setTextByWidget( this, ascii( "editTargetCell" ), *it );
        else if ( nIdx == 1 )
        {
            OUString sItem = *it;
            if ( sItem.equals( ascii( "min" ) ) )
                setGoal( GOAL_MINIMIZE );
            else if ( sItem.equals( ascii( "max" ) ) )
                setGoal( GOAL_MAXIMIZE );
        }
        else if ( nIdx == 2 )
            setTextByWidget( this, ascii( "editVarCells" ), *it );
        else if ( nIdx > 2 )
        {
            OUString sLeft = *it++;
            OSL_ASSERT( it != itEnd );
            OUString sEqual = *it++;
            OSL_ASSERT( it != itEnd );
            OUString sRight = *it;
            numeric::EqualityType eEq;
            if ( sEqual.equals( ascii( ">=" ) ) )
                eEq = GREATER_EQUAL;
            else if ( sEqual.equals( ascii( "<=" ) ) )
                eEq = LESS_EQUAL;
            else if ( sEqual.equals( ascii( "=" ) ) )
                eEq = EQUAL;
            else
                OSL_ASSERT( !"unknown equality value" );
            setConstraint( sLeft, sRight, eEq );
        }
    }
}

void SolverDialog::updateWidgets()
{
    if ( getSelectedConstraintPos() < 0 )
    {
        enableWidget(ascii("btnConstChange"), false);
        enableWidget(ascii("btnConstDelete"), false);
    }
    else
    {
        enableWidget(ascii("btnConstChange"), true);
        enableWidget(ascii("btnConstDelete"), true);
    }
}

void SolverDialog::enableAllWidgets(bool enable)
{
    if (enable)
        popWidgetState();
    else
    {
        Reference<XNameContainer> xNC(getDialogModel(), UNO_QUERY);
        if (!xNC.is())
            return;

        pushWidgetState();
        Sequence<OUString> names = xNC->getElementNames();
        sal_Int32 n = names.getLength();
        for (sal_Int32 i = 0; i < n; ++i)
            enableWidget(names[i], false);
    }
}

}
