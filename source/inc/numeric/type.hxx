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

#ifndef _NUMERIC_TYPE_HXX_
#define _NUMERIC_TYPE_HXX_

namespace scsolver { namespace numeric {

enum EqualityType
{
	/** [constraint] >= [rhs]
	 */
	GREATER_EQUAL,
	/** [constraint] = [rhs]
	 */
	EQUAL,
	/** [constraint] <= [rhs]
	 */
	LESS_EQUAL
};

/**
 * Upper or lower boundary for a decision variable.
 */
enum BoundType
{
	BOUND_UPPER,
	BOUND_LOWER
};

/**
 * Goal of an optimization model.
 */
enum GoalType
{
	GOAL_MINIMIZE,
	GOAL_MAXIMIZE,
    GOAL_TOVALUE,
	GOAL_UNKNOWN
};

}}

#endif
