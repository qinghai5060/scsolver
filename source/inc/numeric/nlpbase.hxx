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

#ifndef OPRES_NLP_NLPBASE_HXX_
#define OPRES_NLP_NLPBASE_HXX_

#include <vector>
#include <memory>

namespace scsolver { namespace numeric {

namespace nlp {

class Model;
struct BaseAlgorithmImpl;

class BaseAlgorithm
{
public:
	BaseAlgorithm();
	virtual ~BaseAlgorithm() = 0;

    /**
     * Call this method to solve for optimized solution.
     */
	virtual void solve() = 0;

    /**
     * Set NLP model to be optimized.  <b>Note that this class does NOT manage the
     * life cycle of this model instance;</b> it is the responsibility of the 
     * calling function to ensure that it gets deleted after the optimization code 
     * has run. 
     *  
     * @param model pointer to NLP model.
     */
	void setModel( Model* model );

    /**
     * Get the optimized solution.  Call this method after solve() returns.
     * 
     * @return ::std::vector<double> optimized solution
     */
	void getSolution(::std::vector<double>& rVars) const;

    void setDebug(bool debug);

    bool isDebug() const;

protected:
    /**
     * Get current NLP model.
     * 
     * @return Model* pointer to current model.
     */
	Model* getModel() const;

private:
	::std::auto_ptr<BaseAlgorithmImpl> m_pImpl;
};

}}}


#endif
