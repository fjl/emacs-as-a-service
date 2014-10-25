/* -*- c-file-style:"linux" indent-tabs-mode:t tab-width:8 -*-

Copyright (C) 2014 Felix Lange

This file is part of emacs-as-a-service.

emacs-as-a-service is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

emacs-as-a-service is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with emacs-as-a-service.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include "common.h"

int shellexec(char *cmd)
{
	char tmp[2048];
	if (confstr(_CS_DARWIN_USER_TEMP_DIR, (char*)&tmp, sizeof(tmp)) == -1) {
		return -1;
	}
	
	struct passwd *pw = getpwuid(getuid());
	char *home, *logname, *tmpdir, *shell;
	asprintf(&home, "HOME=%s", pw->pw_dir);
	asprintf(&logname, "LOGNAME=%s", pw->pw_name);
	asprintf(&shell, "SHELL=%s", pw->pw_shell);
	asprintf(&tmpdir, "TMPDIR=%s", (char*)&tmp);

	char *env[6] = {home, logname, tmpdir, shell, "TERM=xterm", NULL};
	char *argv[5] = {pw->pw_shell, "-l", "-c", cmd, NULL};
	chdir(pw->pw_dir);
	return execve(argv[0], argv, env);
}

pid_t shellfork(char *cmd)
{
	pid_t child = vfork();
	if (child == 0) {
		// in child
		shellexec(cmd);
	}
	return child;
}
