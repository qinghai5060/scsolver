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

#include "option.hxx"

namespace scsolver {

/**
 *  This is where the values of options settings are stored. The
 *  default values are specified in the constructor.
 */
struct OptionDataImpl
{
	OptionDataImpl() :
		VarPositive(true),
		VarInteger(false),
		ModelType(OPTMODELTYPE_LP)
	{
	}

	~OptionDataImpl() throw() {}

	bool VarPositive:1;
	bool VarInteger:1;
	OptModelType ModelType;
};

//-----------------------------------------------------------------

OptionData::OptionData() : m_pImpl( new OptionDataImpl )
{
}

OptionData::~OptionData() throw()
{
}

void OptionData::setVarPositive( bool b )
{
	m_pImpl->VarPositive = b;
}

bool OptionData::getVarPositive() const
{
	return m_pImpl->VarPositive;
}

void OptionData::setVarInteger( bool b )
{
	m_pImpl->VarInteger = b;
}

bool OptionData::getVarInteger() const
{
	return m_pImpl->VarInteger;
}

void OptionData::setModelType( OptModelType type )
{
	m_pImpl->ModelType = type;
}

OptModelType OptionData::getModelType() const
{
	return m_pImpl->ModelType;
}

}
