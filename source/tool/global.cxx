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

#include "tool/global.hxx"
#include "numeric/matrix.hxx"
#include <sstream>
#include <stdio.h>

#ifdef SCSOLVER_OOO_BUILD
#include "rtl/math.hxx"
#endif

using ::std::vector;
using ::std::string;
using ::std::ostringstream;
using ::scsolver::numeric::Matrix;

namespace scsolver {

namespace numeric {

bool isfinite(double f)
{
#ifdef SCSOLVER_OOO_BUILD
    return ::rtl::math::isFinite(f);
#elif defined(_MSC_VER)
    return _isfinite(f);
#else
    return ::std::isfinite(f);
#endif
}

}

#if SCSOLVER_DEBUG
void Debug( const char* s )
{
	fprintf(stdout, "%s\n", s);
	fflush(stdout);
}
#else
void Debug( const char* )
{
}
#endif

string repeatString( const char* str, unsigned long nNum )
{
    ostringstream os;
	for ( unsigned int i = 0; i < nNum; ++i )
		os << str;
	return os.str();
}

void vectorToMatrix(const vector<double>& vec, Matrix& mx, bool rowMatrix)
{
    size_t vecSize = vec.size();
    if (rowMatrix)
    {
        Matrix tmp(1, vecSize);
        for (size_t i = 0; i < vecSize; ++i)
            tmp(0, i) = vec[i];
        mx.swap(tmp);
    }
    else
    {
        Matrix tmp(vecSize, 1);
        for (size_t i = 0; i < vecSize; ++i)
            tmp(i, 0) = vec[i];
        mx.swap(tmp);
    }
}

void matrixToVector(const Matrix& mx, vector<double>& vec)
{
    bool rowMatrix = mx.cols() > 1;
    if (rowMatrix)
    {
        // single row & multi-column matrix.

        size_t n = mx.cols();
        vector<double> tmp;
        tmp.reserve(n);
        for (size_t i = 0; i < n; ++i)
            tmp.push_back(mx(0, i));
        vec.swap(tmp);
    }
    else
    {
        // single column & multi-row matrix.

        size_t n = mx.rows();
        vector<double> tmp;
        tmp.reserve(n);
        for (size_t i = 0; i < n; ++i)
            tmp.push_back(mx(i, 0));
        vec.swap(tmp);
    }
}

}
