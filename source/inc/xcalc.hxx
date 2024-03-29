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

#ifndef _SHEET_HXX_
#define _SHEET_HXX_

#include "type.hxx"
#include <com/sun/star/table/CellAddress.hpp>
#include <com/sun/star/table/CellRangeAddress.hpp>

using namespace com::sun::star;
using com::sun::star::uno::Reference;

namespace rtl { class OUString; }

namespace com { namespace sun { namespace star {

	namespace uno
	{
		class XComponentContext;
	}

	namespace lang
	{
		class XComponent;
	}
	
	namespace sheet
	{
		class XSpreadsheet;
		class XSpreadsheets;
		class XRangeSelection;
	}
	
	namespace table
	{
		class XCellRange;
		class XCell;
	}
	
}}}


namespace scsolver {

/**
 *  This class is the only class that interfaces with the Calc
 *  document component.  If you want access to the service
 *  manager, then this class is it.
 */
class CalcInterface
{
public:

	CalcInterface( const Reference< uno::XComponentContext >& );
	~CalcInterface();

	//------------------------------------------------------------------------------
	// Basic Component Methods
	
	Reference< uno::XComponentContext > getComponentContext() const { return m_xCC; }
	Reference< lang::XComponent > getCurrentComponent() const;
	void updateCurrentComponent() const;
	Reference< lang::XMultiComponentFactory > getServiceManager() const;
	
	void setDocumentProperty( const rtl::OUString&, uno::Any );
	uno::Any getDocumentProperty( const rtl::OUString& ) const;

	//------------------------------------------------------------------------------
	// Calc Component Methods
	
	Reference< sheet::XRangeSelection > getXRangeSelection() const;

	//------------------------------------------------------------------------------
	// Sheet-Related Methods

	Reference< sheet::XSpreadsheet > getActiveSheet() const;
	Reference< sheet::XSpreadsheet > getSheetByIndex( const sal_uInt16 ) const;
	Reference< sheet::XSpreadsheet > getSheetByName( const rtl::OUString& ) const;
	
	
	rtl::OUString getCellFormula( const rtl::OUString&, const rtl::OUString& );


	table::CellAddress getCellAddress( const rtl::OUString& );
	table::CellAddress getCellAddress( const rtl::OUString&, const rtl::OUString& );

	table::CellRangeAddress getCellRangeAddress( const rtl::OUString& );
	table::CellRangeAddress getCellRangeAddress( const rtl::OUString&, const rtl::OUString& );
	Reference< table::XCell > getCell( const table::CellAddress& ) const;
	rtl::OUString getCellFormula( const table::CellAddress& );
	double getCellValue( const table::CellAddress& );

	void setCellFormula( const table::CellAddress&, const rtl::OUString& ) const;
	void setCellValue( const table::CellAddress&, double ) const;

	void disableCellUpdates() const;
	void enableCellUpdates() const;

	void addMenuItem() const;

private:
	Reference< uno::XComponentContext > m_xCC;
	mutable Reference< lang::XMultiComponentFactory > m_xSM;
	mutable Reference< lang::XComponent > m_xCurComp;
	mutable Reference< sheet::XRangeSelection > m_xRngSel;

	void splitCellRangeAddress( const rtl::OUString&, rtl::OUString&, rtl::OUString& );

};	
	
	
	
}

#endif
