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

#ifndef _SCSOLVER_NUMERIC_MATRIX_HXX_
#define _SCSOLVER_NUMERIC_MATRIX_HXX_

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/matrix_proxy.hpp>

#include <vector>
#include <memory>

namespace scsolver { namespace numeric {

/**
 * This exception is thrown when an element being accessed is
 * outside the current matrix boundary.
 */
class BadIndex : public ::std::exception 
{
public:
    virtual const char* what() const throw();
};

class MatrixSizeMismatch : public ::std::exception 
{
public:
    virtual const char* what() const throw();
};

class MatrixNotDiagonal : public ::std::exception
{
public:
    virtual const char* what() const throw();
};

class OperationOnEmptyMatrix : public ::std::exception
{
public:
    virtual const char* what() const throw();
};

class SingularMatrix : public ::std::exception
{
public:
    virtual const char* what() const throw();
};

class NonSquareMatrix : public ::std::exception
{
public:
    virtual const char* what() const throw();
};

class Matrix
{
public:
    typedef ::boost::numeric::ublas::matrix<double, ::boost::numeric::ublas::row_major, ::std::vector<double> > 
        NumericMatrixType;
    typedef ::boost::numeric::ublas::matrix< ::std::string > StringMatrixType;

    Matrix();
    Matrix(size_t row, size_t col, bool identity_matrix = false);
    Matrix( const Matrix& );
    Matrix( const Matrix* );
    Matrix( ::boost::numeric::ublas::matrix<double> );
    ~Matrix() throw();
    
    void setResizable(bool resizable);
    void swap( Matrix& ) throw();
    void clear();
    void copy( const Matrix& );
    Matrix clone() const;

    const double getValue(size_t row, size_t col) const;
    double& getValue(size_t row, size_t col);
    void setValue(size_t row, size_t col, double val);

    StringMatrixType getDisplayElements(int prec, size_t colspace, bool formula) const;    

    /**
     * Print the content of a matrix in to standard output. 
     *  
     * @param prec desired precision
     * @param colspace desired column space for each column.
     */
    void print(size_t prec = 2, size_t colspace = 1) const;
    
    Matrix getColumn(size_t col);
    Matrix getRow(size_t row);

    void deleteColumn( size_t );
    void deleteColumns( const std::vector<size_t>& );
    void deleteRow( size_t );
    void deleteRows( const std::vector<size_t>& );
    
    /**
     * This method calculates the determinant of a square matrix. 
     * It throws a NonSquareMatrix exception of the matrix being 
     * operated upon is not square. 
     * 
     * @return double determinant.
     */
    double det() const;
    const Matrix inverse() const;
    const Matrix trans() const;
    double minors( size_t, size_t ) const;
    void resize(size_t row, size_t col);

    /**
     * @return size_t current row size
     */
    size_t rows() const;

    /**
     * @return size_t current column size
     */
    size_t cols() const;
    
    bool empty() const;
    bool isRowEmpty( size_t ) const;
    bool isColumnEmpty( size_t ) const;
    bool isSameSize( const Matrix& ) const;
    bool isSquare() const;
    
    // Overloaded Operators
    
    Matrix& operator=( const Matrix& );
    const Matrix operator+( const Matrix& ) const;
    const Matrix operator-( const Matrix& ) const;
    const Matrix operator*( double ) const;
    const Matrix operator*( const Matrix& ) const;
    const Matrix operator/( double ) const;
    Matrix& operator+=( const Matrix& );
    Matrix& operator+=( double );
    Matrix& operator-=( const Matrix& );
    Matrix& operator*=( double );
    Matrix& operator/=( double );

    const double operator()(size_t row, size_t col) const;
    double& operator()(size_t row, size_t col);
    
    bool operator==( const Matrix& ) const;
    bool operator!=( const Matrix& ) const;
    
private:
    const Matrix adj() const;
    double cofactor( size_t, size_t ) const;
    void maybeExpand(size_t row, size_t col);
    void throwIfEmpty() const;

    bool m_bResizable;
    NumericMatrixType m_aArray;
};

const Matrix operator+(const Matrix& mx, double scalar);
const Matrix operator+(double scalar, const Matrix& mx);
const Matrix operator*(double scalar, const Matrix& mx);

}}

#endif //_MATRIX_HXX_
