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

#ifndef _SCSOLVER_NUMERIC_EXCEPTION_HXX_
#define _SCSOLVER_NUMERIC_EXCEPTION_HXX_

#include <exception>
#include <string>

namespace scsolver { namespace numeric {

/// Generic exception, with a customisable error message.
class Exception : public ::std::exception
{
public:
    explicit Exception(const char* msg): m_msg(msg) {}
    explicit Exception(const ::std::string& msg) : m_msg(msg) {}
    virtual ~Exception() throw() {}

    virtual const char* what() const throw() { return m_msg.c_str(); }

private:
    ::std::string m_msg;
};

/// Exception for errors in constraint specification.
class ConstraintError : public ::std::exception
{
public:
    virtual const char* what() const throw() { return "Constraint error"; }
};

/// Exception for things that should never happen (programming errors).
class AssertionWrong : public ::std::exception
{
public:
    virtual const char* what() const throw() {
        return "Wrong assertion! (tell Kohei immediately)";
    }
};

/// Exception for ill-posed problems.
class ModelInfeasible : public ::std::exception
{
public:
    virtual const char* what() const throw() { return "Model infeasible"; }
};

/// Exception for non-converging series of iterations.
class MaxIterationReached : public ::std::exception
{
public:
    virtual const char* what() const throw() {
        return "Maximum iteration reached";
    }
};

/// Exception for series of iterations taking too much time.
class IterationTimedOut : public ::std::exception
{
public:
    virtual const char* what() const throw() { return "Iteration timed out"; }
};

/// Exception for request of a non-existing boundary value.
/**
 * This exception is thrown when the caller attempts to retrieve a boundary
 * value of a decision variable when it's not bounded.
 */
class UnboundedException : public ::std::exception
{
public:
    virtual const char * what() const throw() { return "Unbounded exception"; }
};

}}


#endif
