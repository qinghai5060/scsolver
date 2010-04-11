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

#ifndef _NUMERIC_OPRES_NLP_NLPMODEL_HXX_
#define _NUMERIC_OPRES_NLP_NLPMODEL_HXX_

#include "numeric/type.hxx"
#include <memory>
#include <string>
#include <vector>

namespace scsolver { namespace numeric {
    class BaseFuncObj;
}}

namespace scsolver { namespace numeric { namespace nlp {

class ModelImpl;
class BaseAlgorithm;

/** 
 * Individual constraint item. 
 */
struct Constraint
{
    BaseFuncObj*    FuncObj;
    EqualityType    Equality;
    double          RightValue;

    explicit Constraint(BaseFuncObj* pFunc, EqualityType eq, double rightValue);
private:
    Constraint();
};

// ============================================================================

class Model
{
public:
    Model();
    Model( const Model& );
    ~Model() throw();

    /**
     * Print model information to standard output.
     */
    void print() const;

    /**
     * Set desired precision for optimized solution. 
     *  
     * @param n desired precision
     */
    void setPrecision(unsigned long n);

    /**
     * Get current desired precision.
     * 
     * @return unsigned long desired precision.
     */
    unsigned long getPrecision() const;

    /**
     * Set optimization goal. 
     *  
     * @param goal
     */
    void setGoal(scsolver::numeric::GoalType goal);

    /**
     * Get current optimization goal.
     * 
     * @return scsolver::numeric::Goal
     */
    scsolver::numeric::GoalType getGoal() const;

    /**
     * Control whether or not to produce verbose output to standard output during 
     * optimization run. 
     *  
     * @param b flag to control verbose ouput
     */
    void setVerbose(bool b);

    /**
     * Get the current verbose setting.
     * 
     * @return bool verbose setting flag
     */
    bool getVerbose() const;

    /**
     * Set a pointer to the function object to be used that represents the 
     * objective function in a model.  Note that <b>this model class does NOT 
     * manage the life cycle of the function object passed to this function</b>; it
     * is the responsibility of the calling function to ensure that the instance 
     * of the function object gets deleted after the optimization code has run. 
     *  
     * @param pFuncObj pointer to function object.
     */
    void setFuncObject(BaseFuncObj* pFuncObj);

    /**
     * Get a pointer to the current function object.
     * 
     * @return BaseFuncObj* pointer to current function object.
     */
    BaseFuncObj* getFuncObject() const;

    /**
     * Push a variable to the end of variable list.  Calling this method will 
     * increase the number of variables by one. 
     *  
     * @param var variable value
     */
    void pushVar( double var );

    /**
     * Get all variables that are currently in the model as an array. 
     *  
     * @return const::std::vector<double>
     */
    void getVars( ::std::vector<double>& vars ) const;

    /**
     * Get the boundary value of a variable specified by the index.  Note 
     * especially that the caller needs to ensure that the bounardy specified is
     * actually bounded by calling isVarBounded(). Calling this function for an 
     * unbounded boundary leads to an undefined behavior.
     * 
     * @param index index value for the variable (0-based).
     * @param bound BOUND_UPPER or BOUND_LOWER for upper and lower bounds, 
     *              respectrively.
     * 
     * @return double boundary value
     */
    double getVarBound( size_t index, BoundType bound ) const;

    /**
     * Set boundary value for a boundary specified by variable index and side 
     * (either upper or lower).  Setting a boundary value by calling this method 
     * will cause isVarBounded() for this boundary to return true.
     *  
     * @param index index value for the variable (0-based)
     * @param bound BOUND_UPPER or BOUND_LOWER for upper and lower bounds, 
     *              respectively.
     * @param value boundary value to be set.
     */
    void setVarBound( size_t index, BoundType bound, double value );

    /**
     * Check whether or not the specified boundary for a specified variable is 
     * bounded. 
     *  
     * @param index variable index (0-based)
     * @param bound BOUND_UPPER or BOUND_LOWER for upper and lower bounds, 
     *              respectively.
     * 
     * @return bool true if bounded, or false otherwise.
     */
    bool isVarBounded( size_t index, BoundType bound ) const;

    void pushConstraint(BaseFuncObj* pFunc, EqualityType eq, double rightValue);

    const ::std::vector<Constraint>& getAllConstraints() const;

private:
    ::std::auto_ptr<ModelImpl> m_pImpl;
};


}}}


#endif
