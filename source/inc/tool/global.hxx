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

#ifndef _GLOBAL_HXX_
#define _GLOBAL_HXX_

#include <string>
#include <iterator>
#include <iostream>
#include <vector>

#define SCSOLVER_DEBUG 0
#define SCSOLVER_UNO_COMPONENT 1
#define REWRITE_FOR_SUN_STUDIO_COMPILER 1

#if !defined(SCSOLVER_UNITTEST)
#include <osl/diagnose.h>
#endif

namespace scsolver { namespace numeric {
    class Matrix;

/** 
 * build-independent isfinite function that behaves comparable to
 * ::std::isfinite().
 */
bool isfinite(double f);

}}

namespace {

//---------------------------------------------------------------------------
// Function templates (need to be implemented in header)

template<typename Container>
void printElements( const Container& cn, const char* sep = " " )
{
	using namespace ::std;
	copy( cn.begin(), cn.end(), ostream_iterator<typename Container::value_type>( cout, sep ) );
	cout << endl;
}

}

namespace scsolver {

/**
 * Outputs debug message when SCSOLVER_DEBUG is defined.
 * 
 * @param s      message to output
 */
void Debug( const char* s );
std::string repeatString( const char*, unsigned long );

void vectorToMatrix(const ::std::vector<double>& vec, ::scsolver::numeric::Matrix& mx, bool rowMatrix = true);
void matrixToVector(const ::scsolver::numeric::Matrix& mx, ::std::vector<double>& vec);

}

#endif
