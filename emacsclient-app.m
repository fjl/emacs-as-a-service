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

#include "common.h"
#include <Cocoa/Cocoa.h>

@interface AppDelegate : NSObject <NSApplicationDelegate>

- (BOOL) application:(NSApplication *)app openFile:(NSString *)filename;
- (void) applicationDidFinishLaunching:(NSNotification *)notification;

@end

@implementation AppDelegate

- (BOOL) application:(NSApplication *)app openFile:(NSString *)filename
{
	// launch with file, open the file and quit
	char *cmd;
	asprintf(&cmd, "emacsclient -nc %s", [filename UTF8String]);
	shellfork(cmd);
	free(cmd);
	[app stop: self];
	return true;
}

- (void) applicationDidFinishLaunching:(NSNotification *)notification
{
	// regular launch, show emacs and quit
	BOOL start = YES;
	NSRunningApplication *app;
	NSArray *instances = [NSRunningApplication runningApplicationsWithBundleIdentifier:@"org.gnu.Emacs"];
	if ([instances count] > 0) {
		app = [instances objectAtIndex:0];
		start = ![app activateWithOptions:NSApplicationActivateIgnoringOtherApps];
	}
	if (start) {
		shellfork("emacsclient -nc");
	}
	[(NSApplication*)notification.object stop: self];
}

@end

int main()
{
	[NSAutoreleasePool new];
	[NSApplication sharedApplication];
	[NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
	id menubar = [[NSMenu new] autorelease];
	id appMenuItem = [[NSMenuItem new] autorelease];
	[menubar addItem:appMenuItem];
	[NSApp setMainMenu:menubar];
	id appMenu = [[NSMenu new] autorelease];
	id quitMenuItem = [[[NSMenuItem alloc] initWithTitle:@"Quit"action:@selector(terminate:) keyEquivalent:@"q"] autorelease];
	[appMenu addItem:quitMenuItem];
	[appMenuItem setSubmenu:appMenu];
	[NSApp setDelegate: (id)[AppDelegate alloc]];
	[NSApp activateIgnoringOtherApps:YES];
	[NSApp run];
}
