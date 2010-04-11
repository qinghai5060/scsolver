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

#include "numeric/nlpbase.hxx"
#include "numeric/nlpmodel.hxx"
#include "numeric/funcobj.hxx"

using ::std::vector;

namespace scsolver { namespace numeric { namespace nlp {

struct BaseAlgorithmImpl
{
	Model* pModel;
    vector<double> Solution;
    bool Debug;

    BaseAlgorithmImpl() :
        pModel(NULL),
        Debug(false)
    {
    }
};

BaseAlgorithm::BaseAlgorithm() : m_pImpl( new BaseAlgorithmImpl )
{
}

BaseAlgorithm::~BaseAlgorithm()
{
}

Model* BaseAlgorithm::getModel() const
{
	return m_pImpl->pModel;
}

void BaseAlgorithm::setModel( Model* model )
{
	m_pImpl->pModel = model;
}

void BaseAlgorithm::getSolution(vector<double>& rVars) const
{
    getModel()->getFuncObject()->getVars(rVars);
}

void BaseAlgorithm::setDebug(bool debug)
{
    m_pImpl->Debug = debug;
}

bool BaseAlgorithm::isDebug() const
{
    return m_pImpl->Debug;
}

}}}

