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

#include "numeric/matrix.hxx"
#include <stdio.h>

using namespace ::scsolver::numeric;

void basicIO()
{
    printf("Original empty matrix\n");
    Matrix mx(3, 3);
    mx.print();     // empty matrix
    printf("Simple assignment: (0, 0) = 4.54   (0, 2) = 12.55   (1, 1) = 1.2   (2, 1) = 35.4   (2, 2) = 5\n");
    mx(0, 0) = 4.54;
    mx(0, 2) = 12.55;
    mx(1, 1) = 1.2;
    mx(2, 1) = 35.4;
    mx(2, 2) = 5.0;
    mx.print();

    printf("Transposing matrix\n");
    mx.trans().print();

    // Automatic resizing.
    printf("Automatic resizing\n");
    mx.setResizable(true);
    mx(3, 10) = 100;
    mx.print();
    mx.setResizable(false);

    // Shrink the matrix.
    printf("Shrinking the matrix to 3 x 3\n");
    mx.resize(3, 3);
    mx.print();

    // Expanding the matrix
    printf("Expanding the matrix to 5 x 5\n");
    mx.resize(5, 5);
    mx.print();

    // Shrinking it again.
    printf("Shrinking it again\n");
    mx.resize(3, 3);
    mx.print();

    // Bad index
    mx.setResizable(false);
    try
    {
        printf("Bad index: (-1, 0) = 999\n");
        mx(-1, 0) = 999;
    }
    catch (const BadIndex&)
    {
        printf("BadIndex exception caught on assigning a new value.\n");
    }
    mx.print();

    try
    {
        printf("Bad index: (0, -1) = 999\n");
        mx(0, -1) = 999;
    }
    catch (const BadIndex&)
    {
        printf("BadIndex exception caught on assigning a new value.\n");
    }
    mx.print();

    try
    {
        printf("Bad index: r = (10, 10)\n");
        double r = mx(10, 10);
        r = 20.5;
    }
    catch (const BadIndex&)
    {
        printf("BadIndex exception caught on querying a value.\n");
    }
    mx.print();
    mx.setResizable(true);

    printf("3 x 3 identity matrix\n");
    Matrix mxi(3, 3, true);
    mxi.print();
}

int main()
{
    printf("unit test: Matrix\n");
    basicIO();
    printf("Unit test passed!\n");
}
