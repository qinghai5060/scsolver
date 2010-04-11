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

#include "tool/timer.hxx"
#ifndef _WIN32
#include <sys/time.h>
#else
#include <windows.h>
#endif

namespace scsolver {

class TimerImpl
{
public:
	TimerImpl( double duration ) :
		m_fDuration(duration)
	{
	}

	~TimerImpl() throw()
	{
	}

	void init()
	{
		m_fCurTime = getTime();
	}

	bool isTimedOut()
	{
		return (getTime() - m_fCurTime) > m_fDuration;
	}

private:

	/**
     * Get current time in seconds.
	 * 
     * @return double current time in seconds
	 */
	double getTime()
	{
#ifndef _WIN32
		timeval tv;
		gettimeofday(&tv, NULL);
		return tv.tv_sec + tv.tv_usec / 1000000.0;
#else
		FILETIME ft;
		__int64 *time64 = (__int64 *) &ft;

		GetSystemTimeAsFileTime (&ft);

		/* Convert from 100s of nanoseconds since 1601-01-01
		 * to seconds since the Unix epoch.
		 */
		*time64 -= 116444736000000000i64;
		return *time64 / 10000000.0;
#endif
	}

	double m_fDuration;
	double m_fCurTime;
};

//-----------------------------------------------------------------

/*
Timer::Timer()
{
	// disabled
}
*/

Timer::Timer( double duration ) :
	m_pImpl( new TimerImpl(duration) )
{
}

Timer::~Timer() throw()
{
}

void Timer::init()
{
	m_pImpl->init();
}

bool Timer::isTimedOut() const
{
	return m_pImpl->isTimedOut();
}

}
