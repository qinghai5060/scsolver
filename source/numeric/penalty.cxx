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

#include "numeric/penalty.hxx"
#include "numeric/nlpmodel.hxx"
#include <string>
#include <vector>

using ::std::string;
using ::std::vector;

namespace scsolver { namespace numeric { namespace nlp {

struct PenaltyImpl
{

    const BaseFuncObj*  pObjectiveFunc;
    vector<Constraint>  Constraints;
    double              Factor;

    PenaltyImpl();
    PenaltyImpl(const PenaltyImpl& r);
};

PenaltyImpl::PenaltyImpl() :
    pObjectiveFunc(NULL), 
    Factor(1.0)
{
}

PenaltyImpl::PenaltyImpl(const PenaltyImpl& r) :
    pObjectiveFunc(r.pObjectiveFunc), 
    Constraints(r.Constraints),
    Factor(r.Factor)
{
}

// ----------------------------------------------------------------------------

Penalty::Penalty() :
    BaseFuncObj(),
    mpImpl(new PenaltyImpl)
{
}

Penalty::Penalty(const Penalty& r) :
    BaseFuncObj(*this),
    mpImpl(new PenaltyImpl(*r.mpImpl))
{
}

Penalty::~Penalty()
{
}

double Penalty::eval()
{
    return 0.0;
}

const string Penalty::getFuncString() const
{
    string str("");
    return str;
}

void Penalty::initFromModel(Model* pModel)
{
    mpImpl->pObjectiveFunc = pModel->getFuncObject();
    const vector<Constraint>& cons = pModel->getAllConstraints();
    mpImpl->Constraints.assign(cons.begin(), cons.end());
}

void Penalty::setConstraintFactor(double factor)
{
    mpImpl->Factor = factor;
}


}}}
