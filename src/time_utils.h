/**
	* Copyright (c) 2016, Loïc BLOT <loic.blot@unix-experience.fr>
	* All rights reserved.
	*
	* Redistribution and use in source and binary forms, with or without
	* modification, are permitted provided that the following conditions are met:
	*
	* 1. Redistributions of source code must retain the above copyright notice, this
	*    list of conditions and the following disclaimer.
	* 2. Redistributions in binary form must reproduce the above copyright notice,
	*    this list of conditions and the following disclaimer in the documentation
	*    and/or other materials provided with the distribution.
	*
	* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
	* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
	* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
	* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
	* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
	* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
	* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
	* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	*
	* The views and conclusions contained in the software and documentation are those
	* of the authors and should not be interpreted as representing official policies,
	* either expressed or implied, of the FreeBSD Project.
**/

#pragma once

#include <sys/time.h>
#include <sstream>

static const std::string get_current_day ()
{
	time_t now = time(0);
	struct tm t;
	char buf[11];
	t = *localtime(&now);
	strftime(buf, sizeof(buf), "%F", &t);
	return std::string(buf, strlen(buf));
}

static const std::string get_current_hour_min ()
{
	time_t now = time(0);
	struct tm t;
	char buf[6];
	t = *localtime(&now);
	strftime(buf, sizeof(buf), "%H:%M", &t);
	return std::string(buf, strlen(buf));
}

static const long get_sys_uptime ()
{
	struct timespec sp;
	(void)clock_gettime(CLOCK_MONOTONIC, &sp);
	return(sp.tv_sec);
}

static const std::string convert_seconds_to_readable_string(uint32_t t)
{
	uint32_t seconds = t % 60;
	t -= seconds; t /= 60; // Convert time to minutes

	uint32_t minutes = t % 60;
	t -= minutes; t /= 60; // Convert time to hours

	uint32_t hours = t % 24;
	t -= hours; t /= 24; // Convert time to days

	std::stringstream ss;

	if (t > 0) {
		ss << t << "d ";
	}

	if (hours > 0) {
		ss << hours << "h ";
	}

	if (minutes > 0) {
		ss << minutes << "m ";
	}

	if (seconds > 0) {
		ss << seconds << "s";
	}

	return ss.str();
}
