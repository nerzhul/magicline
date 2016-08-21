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

#include <time.h>
#include <stdlib.h>

static void get_current_day (char* buf, size_t s)
{
	time_t now = time(0);
	struct tm t;
	t = *localtime(&now);
	strftime(buf, s, "%F", &t);
}

static void get_current_hour_min (char* buf, size_t s)
{
	time_t now = time(0);
	struct tm t;
	t = *localtime(&now);
	strftime(buf, s, "%H:%M", &t);
}

static const long get_sys_uptime ()
{
	struct timespec sp;
	(void)clock_gettime(CLOCK_MONOTONIC, &sp);
	return(sp.tv_sec);
}

static void convert_seconds_to_readable_string(uint32_t t, char* buf, size_t s)
{
	uint32_t seconds = t % 60;
	t -= seconds; t /= 60; // Convert time to minutes

	uint32_t minutes = t % 60;
	t -= minutes; t /= 60; // Convert time to hours

	uint32_t hours = t % 24;
	t -= hours; t /= 24; // Convert time to days

	snprintf(buf, s, "%dd %dh %dm %ds", t, hours, minutes, seconds);
}
