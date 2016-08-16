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

#include <pwd.h>
#include <unistd.h>
#include <sys/param.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <stdlib.h>

/*
// normal mode
%{^[[0;38;5;231;48;5;31;1m%} nerzhul %{^[[0;38;5;31;48;5;240;22m%} %{^[[0;38;5;252;48;5;240;1m%}~ %{^[[0;38;5;240;49;22m%} %{^[[0m%}
// ssh mode
%{^[[0;38;5;220;48;5;166m%}  Nerz-PC %{^[[0;38;5;166;48;5;31;22m%} %{^[[0;38;5;231;48;5;31;1m%}nerzhul %{^[[0;38;5;31;48;5;240;22m%} %{^[[0;38;5;252;48;5;240;1m%}~ %{^[[0;38;5;240;49;22m%} %{^[[0m%}
// root mode
%{^[[0;38;5;231;48;5;160;1m%} root %{^[[0;38;5;160;48;5;240;22m%} %{^[[0;38;5;252;48;5;240;1m%}~ %{^[[0;38;5;240;49;22m%} %{^[[0m%}
// long path
%{^[[0;38;5;220;48;5;166m%}  Nerz-PC %{^[[0;38;5;166;48;5;31;22m%} %{^[[0;38;5;231;48;5;31;1m%}nerzhul %{^[[0;38;5;31;48;5;240;22m%} %{^[[0;38;5;250;48;5;240m%}⋯ %{^[[0;38;5;245;48;5;240;22m%} %{^[[0;38;5;250;48;5;240m%}lib %{^[[0;38;5;245;48;5;240;22m%} %{^[[0;38;5;250;48;5;240m%}jvm %{^[[0;38;5;245;48;5;240;22m%} %{^[[0;38;5;252;48;5;240;1m%}java-8-openjdk %{^[[0;38;5;240;49;22m%} %{^[[0m%}
*/

void show_prompt ()
{
	struct passwd *pwd;
	uid_t uid = getuid();
	const char* default_username = "?";
	char* username;

	if ((pwd = getpwuid(getuid())) != NULL) {
		username = pwd->pw_name;
	}
	else {
		username = (char*) default_username;
	}

	char path_buf[MAXPATHLEN];
	char* path = getcwd(path_buf, sizeof(path_buf));

	std::string path_cpp11 = "?";
	if (path != NULL) {
		uint16_t slash_count = 0;
		for (char *p = path; *p; ++p) {
			if (*p == '/') {
				slash_count++;
			}
		}

		// slash case is special
		if (strcmp(path, "/") == 0) {
			path_cpp11 = "%{\u001B[0;38;5;252;48;5;240;1m%}/";
		}
		else {
			path_cpp11 = std::string(path, strlen(path));
			size_t index = 0;
			while (true) {
				index = path_cpp11.find("/", index);
				if (index == std::string::npos) break;

				slash_count--;

				std::string replacement;
				if (slash_count == 0) {
					replacement = (index == 0) ?
								  "%{\u001B[0;38;5;252;48;5;240;22m%}/  %{\u001B[0;38;5;250;48;5;240;1m%}" :
								  "%{\u001B[0;38;5;252;48;5;240;22m%}  %{\u001B[0;38;5;250;48;5;240;1m%}";
				}
				else {
					replacement = (index == 0) ?
								  "%{\u001B[0;38;5;245;48;5;240;22m%}/  %{\u001B[0;38;5;250;48;5;240m%}" :
								  "%{\u001B[0;38;5;245;48;5;240;22m%}  %{\u001B[0;38;5;250;48;5;240m%}";
				}
				path_cpp11.replace(index, 1, replacement);
				index += replacement.length();
			}
		}



	}

	printf("%%{\u001B[0;38;5;231;48;5;%s;1m%%} %s %%{\u001B[0;38;5;%s"
			";48;5;240;1m%%}\uE0B0 %s "
			"%%{\u001B[0;38;5;240;49;22m%%} %%{\u001B[0m%%}",
			(uid == 0 ? "160" : "31"), username, (uid == 0 ? "160" : "31"),
		   path_cpp11.c_str());
}

void usage (const char* program_name)
{
	printf("Usage: %s [prompt,rprompt]\n", program_name);
}

int main (int argc, const char* argv[])
{
	if (argc != 2) {
		usage(argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "prompt") == 0) {
		show_prompt();
	}
	else {
		usage(argv[0]);
		return 2;
	}

	return 0;
}

