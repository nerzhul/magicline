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

#if defined(__FreeBSD__)
	#define _WITH_GETLINE
	#include <stdio.h>
#endif

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <regex.h>
#include "host_utils.h"

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

	static char remote_prefix[128] = "";
	if (getenv("SSH_CONNECTION")) {
		static char hostname_buf[64];
		get_hostname(hostname_buf, sizeof(hostname_buf));
		snprintf(remote_prefix, sizeof(hostname_buf) * sizeof(char),
			"%%{\u001B[0;38;5;220;48;5;166m%%} \uE0A2 %s ",
			hostname_buf);
	}

	printf("%s%%{\u001B[0;38;5;231;48;5;%s;1m%%} %s %%{\u001B[0;38;5;%s"
			";48;5;240;1m%%}\uE0B0 %s "
			"%%{\u001B[0;38;5;240;49;22m%%} %%{\u001B[0m%%}",
			remote_prefix,
			(uid == 0 ? "160" : "31"), username, (uid == 0 ? "160" : "31"),
			path_cpp11.c_str());
}

void show_rprompt(const char* last_command_rv) {
	char git_commit[48];
	bzero(git_commit, sizeof(git_commit));

	// Search for git commit
	static char path_buf[MAXPATHLEN];
	char* path = getcwd(path_buf, sizeof(path_buf));
	if (path != NULL) {
		std::string path_str(path, strlen(path));
		uint16_t slash_count = 0;
		for (char *p = path; *p; ++p) {
			if (*p == '/') {
				slash_count++;
			}
		}

		// Search .git/HEAD in current folder and upper
		for (uint16_t i = 0; i < slash_count; i++) {
			std::string git_path = path_str;
			for (uint16_t j = 0; j < i; j++) {
				git_path += "/..";
			}

			git_path +=  "/.git/HEAD";

			if (access(git_path.c_str(), F_OK) != -1) {
				FILE* f = fopen(git_path.c_str(), "r");
				if (f == NULL) {
					break;
				}

				char* line_buf;
				size_t line_len;
				ssize_t read_len;
				// Read first line
				if ((read_len = getline(&line_buf, &line_len, f)) != -1) {
					// First check it's a git branch name
					regex_t regex;
					int regex_rs = regcomp(&regex, "^ref: refs/heads/(.+)\n", REG_EXTENDED);
					if (regex_rs != 0) {
						fprintf(stderr, "Non compilable regex for git!\n");
						fclose(f);
						break;
					}

					size_t max_groups = 2;
					regmatch_t matches[max_groups];
					regex_rs = regexec(&regex, (const char*)line_buf, max_groups, matches, 0);
					// Now verify it's a branch
					if (regex_rs == 0) {
						for (uint8_t g = 1; g < max_groups && matches[g].rm_so != -1; g++) {
							int ncpy_char = matches[g].rm_eo - matches[g].rm_so;

							// We shoud not copy more than 127 characters to have a consistent result
							if (ncpy_char > 47) {
								ncpy_char = 47;
							}

							strncpy(git_commit, line_buf + matches[g].rm_so, (size_t) ncpy_char);
						}
					}
					// This is a commit ID
					else if (read_len == 41) {
						strncpy(git_commit, line_buf, 7);
					}
					regfree(&regex);
				}

				fclose(f);

				// Leave the git search loop
				break;
			}
		}
	}

	if (strcmp(last_command_rv, "0") != 0) {
		printf("%%{\u001B[0;38;5;52;49;22m%%} \uE0B2%%{\u001B[0;38;5;231;48;5;52m%%} %s"
				   "%%{\u001B[0;38;5;236;48;5;52;22m%%} ", last_command_rv);
	}
	else {
		printf("%%{\u001B[0;38;5;236;49;22m%%} ");
	}

	if (strcmp(git_commit, "") != 0) {
		printf("\uE0B2%%{\u001B[0;38;5;250;48;5;236m%%} "
				   "\uE0A0 %s ", git_commit);
	}
	printf("%%{\u001B[0m%%}");
}

void usage (const char* program_name)
{
	printf("Usage: %s [prompt,rprompt]\n", program_name);
}

int main (int argc, const char* argv[])
{
	if (strcmp(argv[1], "prompt") == 0) {
		if (argc != 2) {
			usage(argv[0]);
			return 1;
		}

		show_prompt();
		return 0;
	}
	else if (strcmp(argv[1], "rprompt") == 0) {
		if (argc != 3) {
			usage(argv[0]);
			return 1;
		}

		show_rprompt(argv[2]);
		return 0;
	}
	usage(argv[0]);
	return 2;
}

