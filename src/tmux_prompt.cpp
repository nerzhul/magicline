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
#include <stdlib.h>
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

static const std::string get_sys_load ()
{
	static const int loadavg_entries = 3;
	double loadavg[loadavg_entries];

	int res = getloadavg(loadavg, loadavg_entries);
	if (res < 0) {
		return "unknown";
	}

	// @TODO: format colors
	char load_str_fmt[12]; // 0.0 0.0 0.0\0
	sprintf(load_str_fmt, "#[fg=colour%d]%.1f #[fg=colour%d]%.1f #[fg=colour%d]%.1f",
		map_sys_load_to_color(loadavg[0]), loadavg[0],
		map_sys_load_to_color(loadavg[1]), loadavg[1],
		map_sys_load_to_color(loadavg[2]), loadavg[2]
	);
	return std::string(load_str_fmt, strlen(load_str_fmt));
}

static const std::string get_hostname ()
{
	char hostname[64];
	int res = gethostname(hostname, sizeof(hostname));
	if (res != 0) {
		return "unk hostname";
	}

	return std::string(hostname, strlen(hostname));
}

static void print_right ()
{
	std::cout << "#[fg=colour233,bg=default,nobold,noitalics,nounderscore] \uE0B2#[fg=colour22,bg=colour233,nobold,noitalics,nounderscore] ⇑  #[fg=colour247]"
		<< convert_seconds_to_readable_string(get_sys_uptime())
		<< "#[fg=colour241,bg=colour233,nobold,noitalics,nounderscore] \uE0B3 "
		<< get_sys_load()
		<< "#[fg=colour236,bg=colour233,nobold,noitalics,nounderscore] \uE0B2#[fg=colour247,bg=colour236,nobold,noitalics,nounderscore] "
		<< get_current_day()
		<< "#[fg=colour241,bg=colour236,nobold,noitalics,nounderscore] \uE0B3"
		<< "#[fg=colour252,bg=colour236,bold,noitalics,nounderscore] ⌚ "
		<< get_current_hour_min()
		<< "#[fg=colour252,bg=colour236,nobold,noitalics,nounderscore] \uE0B2#[fg=colour16,bg=colour252,bold,noitalics,nounderscore] "
		<< get_hostname() << " "
		<< std::endl;
}

static void print_left ()
{
	std::cout << "#{?client_prefix,#[fg=colour254]#[bg=colour31]#[bold]#[noitalics]#[nounderscore],"
		"#[fg=colour16]#[bg=colour254]#[bold]#[noitalics]#[nounderscore]} #S "
		"#{?client_prefix,#[fg=colour31],#[fg=colour254]}#[bg=colour233]#[nobold]"
		<< std::endl;
}

static void print_window_status_format ()
{
	std::cout << "#[fg=colour244,bg=colour233,nobold,noitalics,nounderscore]  #I#F "
		"#[fg=colour240,bg=colour233,nobold,noitalics,nounderscore] #[default]#W   "
		<< std::endl;
}

static void print_window_status_current_format ()
{
	std::cout << "#[fg=colour233,bg=colour31,nobold,noitalics,nounderscore] "
		"#[fg=colour117,bg=colour31,nobold,noitalics,nounderscore]#I#F  "
		"#[fg=colour231,bg=colour31,bold,noitalics,nounderscore]#W "
		"#[fg=colour31,bg=colour233,nobold,noitalics,nounderscore] "
		<< std::endl;
}

static void usage (const char *prog_name)
{
	std::cerr << "Wrong arguments. " << std::endl << "Usage: " << prog_name
		<< " [left,right]" << std::endl;
}

int main (int argc, const char* argv[])
{
	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "right") == 0) {
		print_right();
	}
	else if (strcmp(argv[1], "left") == 0) {
		print_left();
	}
	else if (strcmp(argv[1], "wsf") == 0) {
		print_window_status_format();
	}
	else if (strcmp(argv[1], "wscf") == 0) {
		print_window_status_current_format();
	}
	else {
		usage(argv[0]);
		return 2;
	}
	return 0;
}

