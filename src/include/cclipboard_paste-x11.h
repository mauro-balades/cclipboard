#include <stdio.h> // printf
#include <string.h> // strndup
#include <xcb/xcb.h>

#ifdef CCLIPBOARD_BUILD_X11

#ifndef CCLIPBOARD_PASTE_H
#define CCLIPBOARD_PASTE_H

int XA_STRING = 31;

static Display * display;
static Window window;
static Atom UTF8;

char * XPasteType(Atom atom) {
	XEvent event;
	int format;
	unsigned long N, size;
	char * data, * s = 0;
	Atom target,
		CLIPBOARD = XInternAtom(display, "CLIPBOARD", 0),
		XSEL_DATA = XInternAtom(display, "XSEL_DATA", 0);
	XConvertSelection(display, CLIPBOARD, atom, XSEL_DATA, window, CurrentTime);
	XSync(display, 0);
	XNextEvent(display, &event);

	switch(event.type) {
		case SelectionNotify:
		if(event.xselection.selection != CLIPBOARD) break;
		if(event.xselection.property) {
			XGetWindowProperty(event.xselection.display, event.xselection.requestor,
				event.xselection.property, 0L,(~0L), 0, AnyPropertyType, &target,
				&format, &size, &N,(unsigned char**)&data);
			if(target == UTF8 || target == XA_STRING) {
				s = strndup(data, size);
				XFree(data);
			}
			XDeleteProperty(event.xselection.display, event.xselection.requestor, event.xselection.property);
		}
	}
  return s;
}

char* _paste() {
	display = XOpenDisplay(0);
	int N = DefaultScreen(display);
	window = XCreateSimpleWindow(display, RootWindow(display, N), 0, 0, 1, 1, 0,
		BlackPixel(display, N), WhitePixel(display, N)
	);

	char * c = 0;
	UTF8 = XInternAtom(display, "UTF8_STRING", True);
	if(UTF8 != None) c = XPasteType(UTF8);
	if(!c) c = XPasteType(XA_STRING);
	return c;
}

#endif /* CCLIPBOARD_PASTE_H */
#endif /* CCLIPBOARD_BUILD_X11 */
