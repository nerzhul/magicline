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


#include <iostream>
#include <unistd.h>
#include <cstring>
#include <thread>
#include "time_utils.h"

static const int map_sys_load_to_color (const double &load_value)
{
	int cpu_number = std::thread::hardware_concurrency();
	if (load_value < cpu_number * 0.2f) {
		return 76;
	}
	else if (load_value < cpu_number * 0.6f) {
		return 22;
	}
	else if (load_value < cpu_number * 0.8f) {
		return 208;
	}
	else {
		return 1;
	}
}

static const char* get_sys_load ()
{
	static const int loadavg_entries = 3;
	double loadavg[loadavg_entries];

	int res = getloadavg(loadavg, loadavg_entries);
	if (res < 0) {
		return "unknown";
	}

	// @TODO: format colors
	static char load_str_fmt[12]; // 0.0 0.0 0.0\0
	sprintf(load_str_fmt, "#[fg=colour%d]%.1f #[fg=colour%d]%.1f #[fg=colour%d]%.1f",
		map_sys_load_to_color(loadavg[0]), loadavg[0],
		map_sys_load_to_color(loadavg[1]), loadavg[1],
		map_sys_load_to_color(loadavg[2]), loadavg[2]
	);
	return load_str_fmt;
}

static const char* get_hostname ()
{
	static char hostname[64];
	int res = gethostname(hostname, sizeof(hostname));
	if (res != 0) {
		return "unk hostname";
	}

	return hostname;
}

static void print_right ()
{
	printf("#[fg=colour233,bg=default,nobold,noitalics,nounderscore] \uE0B2#[fg=colour22,bg=colour233,nobold,noitalics,nounderscore] ⇑  #[fg=colour247]");
	std::cout << convert_seconds_to_readable_string(get_sys_uptime());
	printf("#[fg=colour241,bg=colour233,nobold,noitalics,nounderscore] \uE0B3 %s"
		"#[fg=colour236,bg=colour233,nobold,noitalics,nounderscore] \uE0B2#[fg=colour247,bg=colour236,nobold,noitalics,nounderscore] %s"
		"#[fg=colour241,bg=colour236,nobold,noitalics,nounderscore] \uE0B3"
		"#[fg=colour252,bg=colour236,bold,noitalics,nounderscore] ⌚ %s"
		"#[fg=colour252,bg=colour236,nobold,noitalics,nounderscore] \uE0B2#[fg=colour16,bg=colour252,bold,noitalics,nounderscore] %s \n",
		   get_sys_load(), get_current_day(), get_current_hour_min(), get_hostname());
}

static void print_left ()
{
	printf("#{?client_prefix,#[fg=colour254]#[bg=colour31]#[bold]#[noitalics]#[nounderscore],"
		"#[fg=colour16]#[bg=colour254]#[bold]#[noitalics]#[nounderscore]} #S "
		"#{?client_prefix,#[fg=colour31],#[fg=colour254]}#[bg=colour233]#[nobold]");
}

static void print_left_tmux_v1 ()
{
	printf("#[fg=colour254]#[bg=colour31]#[bold]#[noitalics]#[nounderscore]#["
		"fg=colour16]#[bg=colour254]#[bold]#[noitalics]#[nounderscore] #S "
		"#[fg=colour254]#[bg=colour233]#[nobold]");
}

static void print_window_status_format ()
{
	printf("#[fg=colour244,bg=colour233,nobold,noitalics,nounderscore]  #I#F "
		"#[fg=colour240,bg=colour233,nobold,noitalics,nounderscore] #[default]#W   ");
}

static void print_window_status_current_format ()
{
	printf("#[fg=colour233,bg=colour31,nobold,noitalics,nounderscore] "
		"#[fg=colour117,bg=colour31,nobold,noitalics,nounderscore]#I#F  "
		"#[fg=colour231,bg=colour31,bold,noitalics,nounderscore]#W "
		"#[fg=colour31,bg=colour233,nobold,noitalics,nounderscore] ");
}

struct OptionMapper
{
	const char* option_name;
	void (*handler) ();
};

static const OptionMapper option_mappers[] = {
	{ "right", &print_right },
	{ "left", &print_left },
	{ "left-v1", &print_left_tmux_v1 },
	{ "wsf", &print_window_status_format },
	{ "wscf", &print_window_status_current_format },
};

static void usage (const char *prog_name)
{
	fprintf(stderr, "Wrong arguments.\nUsage: %s [", prog_name);

	for (uint8_t i = 0; i < 5; i++) {
		const OptionMapper &om = option_mappers[i];
		fprintf(stderr, "%s", om.option_name);
		if (i != 4) {
			fprintf(stderr, ",");
		}
	}

	fprintf(stderr, "]\n");
}

int main (int argc, const char* argv[])
{
	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}

	for (uint8_t i = 0; i < 5; i++) {
		if (strcmp(argv[1], option_mappers[i].option_name) == 0) {
			const OptionMapper &om = option_mappers[i];
			(*om.handler) ();
			return 0;
		}
	}

	// Unknown option
	usage(argv[0]);
	return 2;
}

