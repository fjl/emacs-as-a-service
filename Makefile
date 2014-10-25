LDFLAGS = -lc
EMACSCLIENT_APP = emacsclient.app/Contents/MacOS/emacsclient-app

.PHONY: all install clean

all: ptylaunch $(EMACSCLIENT_APP)

install: all
	install -C ptylaunch /usr/local/bin
	install -C com.twurst.emacsd.plist ~/Library/LaunchAgents
	install -d ~/Applications/emacsclient.app/Contents/{MacOS,Resources}
	install -C emacsclient.app/Contents/{PkgInfo,Info.plist} ~/Applications/emacsclient.app/Contents/
	install -C emacsclient.app/Contents/Resources/Emacs.icns ~/Applications/emacsclient.app/Contents/Resources
	install -C $(EMACSCLIENT_APP) ~/Applications/emacsclient.app/Contents/MacOS
	@echo ""
	@echo "You can now start the server using:"
	@echo "   launchctl load /Users/fjl/Library/LaunchAgents/com.twurst.emacsd.plist"

clean:
	rm -f *.o ptylaunch $(EMACSCLIENT_APP)

ptylaunch: ptylaunch.o common.o
	$(CC) $(LDFLAGS) -o $@ $^

$(EMACSCLIENT_APP): emacsclient-app.o common.o
	$(CC) $(LDFLAGS) -framework Cocoa -o $@ $^

ptylaunch.o: common.h
common.o: common.h
emacsclient-app.o: common.h
