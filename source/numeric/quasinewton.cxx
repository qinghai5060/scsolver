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

#include "numeric/quasinewton.hxx"
#include "numeric/exception.hxx"
#include "numeric/diff.hxx"
#include "numeric/nlpmodel.hxx"
#include "numeric/funcobj.hxx"
#include "numeric/matrix.hxx"
#include "numeric/quadfitlinesearch.hxx"
#include "tool/timer.hxx"
#include "tool/global.hxx"

#include <boost/shared_ptr.hpp>
#include <memory>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace scsolver::numeric;
using scsolver::numeric::Matrix;
using boost::shared_ptr;
using ::std::vector;
using ::std::string;
using ::std::cout;
using ::std::endl;
using ::std::distance;
using ::std::setprecision;

namespace scsolver { namespace numeric { namespace nlp {

class QuasiNewtonImpl
{
	/**
	 * Calculate the norm of a set of variables.
	 * 
	 * @param mxX one-column matrix representing delta f(x) vector
	 * 
	 * @return double calculated norm
	 */
	static double norm( const Matrix& mxX )
	{
		double fNorm = 0.0;
		for ( size_t i = 0; i < mxX.rows(); ++i )
		{
			double f = mxX( i, 0 );
			fNorm += f*f;
		}
#ifdef _MSC_VER
		return ::sqrt( fNorm );
#else
		return std::sqrt( fNorm );
#endif
	}

	static double evalF( BaseFuncObj& oF, const Matrix& mxVars, vector<double>& fVars )
	{
		size_t nRows = mxVars.rows();
		fVars.reserve( nRows );
		for ( size_t i = 0; i < nRows; ++i )
			fVars.push_back( mxVars( i, 0 ) );

        oF.setVars(fVars);
        return oF.eval();
	}

	static double evalF( BaseFuncObj& oF, const Matrix& mxVars )
	{
		vector<double> fVars;
		return evalF( oF, mxVars, fVars );
	}

public:
	QuasiNewtonImpl( QuasiNewton* p ) :
		m_pSelf(p),
		m_nIter(0),
		m_pModel(NULL),
		m_mxVars(0, 0),
		m_mxdVars(0, 0),
		m_mxVarsOld(0, 0),
		m_mxdF(0, 0),
		m_mxdFOld(0, 0),
		m_mxD(0, 0),
		m_mxDOld(0, 0),
		m_fF(0.0),
		m_fFOld(0.0),
		m_fNorm(0.0),
		m_fTolerance(0.0),
        m_debug(false)
	{
	}

	~QuasiNewtonImpl() throw() {}

	void solve()
	{
		// Initialize relevant data members.
		m_pModel = m_pSelf->getModel();
		vector<double> cnVars;
        m_pModel->getVars(cnVars);

		vector<double>::const_iterator it, itBeg = cnVars.begin(), itEnd = cnVars.end();
		m_mxVars.clear();
		for ( it = itBeg; it != itEnd; ++it )
			m_mxVars( distance( itBeg, it ), 0 ) = *it;

		m_mxdVars.clear();
		m_mxVarsOld.clear();
		m_mxdF.clear();
		m_mxdFOld.clear();
		m_mxD.clear();
		m_mxDOld.clear();
		m_fTolerance = 0.1;
		for ( unsigned long i = 0; i < m_pModel->getPrecision(); ++i )
			m_fTolerance *= 0.1;

		m_fF = 0.0;
		m_fFOld = m_fTolerance * 100.0;
		m_fNorm = m_fTolerance * 100.0;

        m_pFuncObj = m_pModel->getFuncObject();

		m_nIter = 0;
		cout << setprecision( m_pModel->getPrecision() );
		::scsolver::Timer mytimer(20); // set timer to 20 sec
		mytimer.init();
		while ( !runIteration(mytimer) );
        
		cout << "f(x) = " << m_fF << endl;
	}

private:
	QuasiNewton* m_pSelf;

	// These data members are expected to be initialized at start of 'void solve()' call.
	unsigned long m_nIter;
	Model* m_pModel;
	Matrix m_mxVars;      // variable matrix is single-column.
	Matrix m_mxdVars;
	Matrix m_mxVarsOld;
	Matrix m_mxdF;
	Matrix m_mxdFOld;
	Matrix m_mxD;
	Matrix m_mxDOld;
	double m_fF;
	double m_fFOld;
	double m_fNorm;
	double m_fTolerance;

    BaseFuncObj* m_pFuncObj;

    bool m_debug;

	bool evaluateFunc()
	{
        using ::std::vector;
        using ::std::cout;
        using ::std::endl;

		// Solve f(x) given the x vector

		vector<double> fVars;
		m_fF = QuasiNewtonImpl::evalF( *m_pFuncObj, m_mxVars, fVars );
		size_t nRows = fVars.size();
        if (m_debug)
        {
            fprintf(stdout, "QuasiNewtonImpl::evaluateFunc:   F = %g; var size = %d\n", 
                    m_fF, nRows);
        }

		// Calculate df(x) gradient array
        for (size_t i = 0; i < nRows; ++i)
        {
            m_pFuncObj->setVars(fVars);
            SingleVarFuncObj& rSingleFunc = m_pFuncObj->getSingleVarFuncObj(i);
            NumericalDiffer diff;
            diff.setFuncObject(&rSingleFunc);
            diff.setVariable(fVars.at(i));
            diff.setPrecision(5);
            m_mxdF(i, 0) = diff.run();
        }

		m_fNorm = QuasiNewtonImpl::norm( m_mxdF );
        if (m_debug)
        {
            fprintf(stdout, "QuasiNewtonImpl::evaluateFunc:   df(x) gradient array: ");
            m_mxdF.trans().print();
            fprintf(stdout, "QuasiNewtonImpl::evaluateFunc:   norm = %g\n", m_fNorm);
        }

        double delta = fabs(m_fF - m_fFOld);
		if (delta < m_fTolerance )
		{
			if (m_debug)
            {
                fprintf(stdout, "QuasiNewtonImpl::evaluateFunc:   desired precision has been reached (delta = %g)\n", 
                        delta);
            }
			return true;
		}

		if ( m_fNorm < m_fTolerance )
		{
			if (m_debug)
                fprintf(stdout, "QuasiNewtonImpl::evaluateFunc:   norm is below specified tolerance limit\n");
			return true;
		}

		return false;
	}

	/**
	 * Calculate or initialize deflection matrix (D).
	 * 
	 * @return bool true if the iteration should stop, or false
	 *         otherwise.
	 */
	bool calcDefMatrix()
	{
		if ( m_mxD.empty() )
		{
			// Deflection matrix is empty.  Initialize it.
			size_t nX = m_mxVars.rows();
			Matrix mxTemp( nX, nX, true );
			m_mxD.swap( mxTemp );
			if ( m_pModel->getGoal() == GOAL_MAXIMIZE )
				m_mxD *= -1.0;
		}
		else
		{
			// Both mxD and mxG are columnar matrix.
			Matrix mxD = m_mxVars - m_mxVarsOld;
			Matrix mxG = m_mxdF - m_mxdFOld;
			double fDG = ( mxD.trans()*mxG )( 0, 0 );
			if ( fDG == 0.0 )
			{
				if (m_debug)
					cout << "dg is zero!" << endl;
				return true;
			}

			// Calculate new deflection matrix via BFGS formula
			Matrix mxA = mxG.trans() * m_mxDOld * mxG;
			double fA = 1.0 + mxA( 0, 0 ) / fDG;
			Matrix mxB = mxD * mxD.trans() / fDG;
			Matrix mxC = ( m_mxDOld * mxG * mxD.trans() + mxD * mxG.trans() * m_mxDOld ) / fDG;
			m_mxD = m_mxDOld + mxB*fA - mxC;
		}

		m_mxdVars = m_mxD * m_mxdF*(-1.0);

		if (m_debug && false)
		{
			unsigned long nPrec = m_pModel->getPrecision();
			cout << setprecision( nPrec );
			cout << "f(x) = " << m_fF << endl;
			cout << "df(x) = ";
			m_mxdF.trans().print( nPrec );
			cout << "|| df(x) || = " << m_fNorm << endl;
			cout << "D:";
			m_mxD.print( nPrec );
			cout << "dx = ";
			m_mxdVars.trans().print( nPrec );
		}

		return false;
	}

	void runLinearSearch( const ::scsolver::Timer& timer )
	{
		// m_mxVars  : Original X (column matrix)
		// m_mxdVars : dX (column matrix)
		// m_fF      : Original f(x)

        vector<double> vars, dx;
        matrixToVector(m_mxVars, vars);
        matrixToVector(m_mxdVars, dx);
        m_pFuncObj->setVars(vars);
        SingleVarFuncObj& singleFunc = m_pFuncObj->getSingleVarFuncObjByRatio(dx);
        QuadFitLineSearch qfit;
        qfit.setTimer(&timer);
        qfit.setDebug(false);
        qfit.setFuncObj(&singleFunc);
        qfit.setGoal(m_pModel->getGoal());
        qfit.solve();
        m_pFuncObj->getVars(vars);

        // Update the stored variables.
        m_mxVarsOld = m_mxVars;
        vectorToMatrix(vars, m_mxVars, false);
	}

	bool runIteration( const ::scsolver::Timer& timer )
	{
		if ( timer.isTimedOut() )
			throw IterationTimedOut();

		if ( m_nIter > 500 )
			throw MaxIterationReached();

		if (m_debug)
		{
			cout << repeatString( "-", 70 ) << endl;
			cout <<	"Iteration " << m_nIter << endl;
			cout << repeatString( "-", 70 ) << endl;
			cout << "x = ";
			m_mxVars.trans().print( m_pModel->getPrecision() );
		}

		if ( evaluateFunc() )
        {
            if (m_debug)
                fprintf(stdout, "QuasiNewtonImpl::runIteration:   evaluateFunc returned true\n");
            return true;
        }

		if ( calcDefMatrix() )
        {
            if (m_debug)
                fprintf(stdout, "QuasiNewtonImpl::runIteration:   calcDefMatrix returned true\n");
            return true;
        }

		runLinearSearch(timer);

		// Update parameters for next iteration

		m_mxdFOld = m_mxdF;
		m_mxDOld = m_mxD;
		m_fFOld = m_fF;
		++m_nIter;

		return false;
	}
};

//-----------------------------------------------------------------

QuasiNewton::QuasiNewton() : m_pImpl( new QuasiNewtonImpl( this ) )
{
}

QuasiNewton::~QuasiNewton() throw()
{
}

void QuasiNewton::solve()
{
	m_pImpl->solve();
}

}}}

