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

#ifndef _SCSOLVER_OPTIONDLG_HXX_
#define _SCSOLVER_OPTIONDLG_HXX_

#include "basedlg.hxx"
#include "option.hxx"

#include <memory>

namespace scsolver {

class ActionObject;
class CloseBtnListener;

struct OptionDialogImpl;

class OptionDialog : public BaseDialog
{
public:
	OptionDialog( SolverImpl* p );
	virtual ~OptionDialog() throw();

	/**
	 * This method is called when a range selection is finished.  If
	 * the dialog does not contain a range selection widget pair,
	 * just leave this method empty.
	 *
	 * @return false for signaling to the calling function that
	 *  the selection is invalid and should be discarded, or true if
	 *  the selection is satisfactory.
	 */
	virtual bool doneRangeSelection() const;

	virtual void close();


	virtual void setVisible( bool b );

	OptModelType getModelType() const;
	void setModelType( OptModelType type );

    bool isVarPositive() const;
    void setVarPositive(bool b);

    bool isVarInteger() const;
    void setVarInteger(bool b);

private:
	void initialize();
    void initOptionData();
	void registerListeners();
	void unregisterListeners() throw();

	::std::auto_ptr<OptionDialogImpl> m_pImpl;
};

}

#endif
