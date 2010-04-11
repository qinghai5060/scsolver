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

#ifndef _SCSOLVER_NUMERIC_PENALTY_HXX_
#define _SCSOLVER_NUMERIC_PENALTY_HXX_

#include "numeric/funcobj.hxx"
#include <memory>

namespace scsolver { namespace numeric { namespace nlp {

class Model;
struct PenaltyImpl;

class Penalty : public ::scsolver::numeric::BaseFuncObj
{
public:
    Penalty();
    Penalty(const Penalty& r);
    virtual ~Penalty();

    virtual double eval();
    virtual const ::std::string getFuncString() const;

    /** 
     * Initialize the penalty function from a non-linear model.  It takes the 
     * objective function and the constraint conditions from the passed model 
     * instance. 
     *
     * @param pModel non-linear model instance
     */
    void initFromModel(Model* pModel);

    /** 
     * Set the factor that multiplies the constraint terms when converting the 
     * constrained model into an unconstrained one. 
     *
     * @param factor value that multiplies the constraint terms.  All 
     *               constraint terms are multiplied by this value.  It's not
     *               possible to set different factors for different
     *               constraint terms.
     */
    void setConstraintFactor(double factor);

private:
    ::std::auto_ptr<PenaltyImpl> mpImpl;
};

}}}


#endif
