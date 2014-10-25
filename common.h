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

int shellexec(char *cmd);
int shellfork(char *cmd);
