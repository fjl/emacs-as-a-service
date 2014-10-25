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

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <util.h>
#include "common.h"

void readwait(int fd)
{
	char buf[1024];
	while (read(fd, &buf, sizeof(buf)) > 0) { }
}

void printstatus(int status)
{
	if (WIFEXITED(status)) {
		printf("terminated with status %d\n", WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		printf("terminated by signal %d\n", WTERMSIG(status));
	} else {
		printf("terminated for unknown reason\n");
	}
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s \"<command>\"\n", argv[0]);
		exit(1);
	}

	int ptyfd, status;
	char* cmd = argv[1];
	pid_t child = forkpty(&ptyfd, NULL, NULL, NULL);
	if (child == 0) {
		// in child
		shellexec(cmd);
	}
	// in parent
	if (child == -1) {
		fprintf(stderr, "forkpty() failed: %s", strerror(errno));
		exit(1);
	} else {
		printf("started %s (pid %d)\n", cmd, child);
		readwait(ptyfd);
		waitpid(child, &status, 0);
		printstatus(status);
	}
}
