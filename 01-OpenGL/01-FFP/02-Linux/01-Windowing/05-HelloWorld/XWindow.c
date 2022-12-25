// standard headers files
#include <stdio.h> 			// for standard IO
#include <stdlib.h>      		// for use of exit 
#include <memory.h>			// for memset
#include <string.h>			// for strlen

// X11 headers
#include <X11/Xlib.h>			
#include <X11/Xutil.h>			// for use of XVisualInfo
#include <X11/XKBlib.h>		// for use of keyboard

// macros
#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// global variables
Display *display = NULL;
XVisualInfo visualInfo;		// structure has 10 members and one of its member "visual" has 8 members 
Colormap colormap;
Window window;
Bool fullscreen = False;

// entry point function
int main(void){
	
	// function declarations
	void uninitialize();
	void toggleFullscreen(void);
	
	// local variables
	int defaultScreen;
	int defaultDepth;
	Status status;
	XSetWindowAttributes windowAttributes;
	int styleMask;
	Atom wm_delete_window_atom;
	XEvent event;
	KeySym keySym;
	int screenWidth;
	int screenHeight;
	char keys[26]; 
	static GC gc;			// graphic context
	XGCValues gcValues;
	static XFontStruct *fontStruct = NULL;
	XColor fontColor;
	char str[] = "Hello World !!!";
	int strLength;
	int strWidth;
	int fontHeight;
	static int winWidth;
	static int winHeight;
	
	// code
	display = XOpenDisplay(NULL);
	if(display == NULL){
		printf("ERROR-XOpenDisplay failed");
		uninitialize();
		exit(1);
	}	
	
	defaultScreen = XDefaultScreen(display);
	defaultDepth = XDefaultDepth(display, defaultScreen);
	status = XMatchVisualInfo(display, defaultScreen, defaultDepth, TrueColor, &visualInfo);
	if(status == 0){
		printf("ERROR-XMatchVisualInfo failed");
		uninitialize();
		exit(1);
	}
	
	memset(&windowAttributes, 0, sizeof(XSetWindowAttributes));
	windowAttributes.border_pixel = 0;
	windowAttributes.background_pixel = XBlackPixel(display, defaultScreen);
	windowAttributes.background_pixmap = 0;
	windowAttributes.colormap = XCreateColormap(display, 
						     RootWindow(display, visualInfo.screen),
						     visualInfo.visual,
						     AllocNone);
	windowAttributes.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask;
	
	colormap = windowAttributes.colormap;
	styleMask = CWBorderPixel | CWBackPixel | CWColormap | CWEventMask;
	
	window = XCreateWindow(display,
			       RootWindow(display, visualInfo.screen),
			       0,
			       0,
			       WIN_WIDTH,
			       WIN_HEIGHT,
			       0,
			       visualInfo.depth,
			       InputOutput,
			       visualInfo.visual,
			       styleMask,
			       &windowAttributes);
			       
	if(!window){
		printf("ERROR-XCreateWindow failed");
		uninitialize();
		exit(1);
	}
	
	XStoreName(display, window, "SSP-Window");
	
	wm_delete_window_atom = XInternAtom(display, "WM_DELETE_WINDOW", True);
	XSetWMProtocols(display, window, &wm_delete_window_atom, 1);
	XMapWindow(display, window);
	
	// centering of window
	screenWidth = XWidthOfScreen(XScreenOfDisplay(display, defaultScreen));
	screenHeight = XHeightOfScreen(XScreenOfDisplay(display, defaultScreen));
	XMoveWindow(display, window, screenWidth/2 - WIN_WIDTH/2, screenHeight/2 - WIN_HEIGHT/2);
	
	// message loop
	while(1){
		XNextEvent(display, &event);
		
		switch(event.type){
			case MapNotify:					// WM_CREATE 
				fontStruct = XLoadQueryFont(display, "fixed");
				break;
				
			case KeyPress:
				keySym = XkbKeycodeToKeysym(display, event.xkey.keycode, 0, 0);
				
				switch(keySym){
					case XK_Escape:
						XUnloadFont(display, fontStruct->fid);
						XFreeGC(display, gc);
						uninitialize();
						exit(0);
						break;
				}
				
				XLookupString(&event.xkey, keys, sizeof(keys), NULL, NULL);
				
				switch(keys[0]){
					case 'F':
					case 'f':
						if(fullscreen == False){
							toggleFullscreen();
							fullscreen = True;
						}
						else{
							toggleFullscreen();
							fullscreen = False;
						}
						break;
				}
				
				break;
				
			case ConfigureNotify:						// this is analogous to WM_SIZE in Win32SKD
				winWidth =  event.xconfigure.width;
				winHeight = event.xconfigure.height;
				break;
				
			case Expose:
				gc = XCreateGC(display, window, 0, &gcValues);
				
				XSetFont(display, gc, fontStruct->fid);
				
				XAllocNamedColor(display, colormap, "green", &fontColor, &fontColor);
				XSetForeground(display, gc, fontColor.pixel);				
				
				strLength = strlen(str);
				strWidth = XTextWidth(fontStruct, str, strLength);
				fontHeight = fontStruct->ascent + fontStruct->descent;
				XDrawString(display, window, gc, winWidth/2 - strWidth/2, winHeight/2 - fontHeight/2, str, strLength);
				break;
			
			case 33:			// created using atom
				XUnloadFont(display, fontStruct->fid);
				XFreeGC(display, gc);
				uninitialize();
				exit(0);
				break;
		}
	}
	
	uninitialize();
		
	return 0;
}

void toggleFullscreen(void){
	
	// local variables declarations
	Atom wm_current_state_atom;
	Atom wm_fullscreen_state_atom;
	XEvent event;
	
	// code
	wm_current_state_atom = XInternAtom(display, "_NET_WM_STATE", False);
	wm_fullscreen_state_atom = XInternAtom(display, "_NET_WM_STATE_FULLSCREEN", False);
	
	memset(&event, 0, sizeof(XEvent));
	event.type = ClientMessage;
	event.xclient.window = window;
	event.xclient.message_type = wm_current_state_atom;
	event.xclient.format = 32;
	event.xclient.data.l[0] = fullscreen ? 0 : 1;
	event.xclient.data.l[1] = wm_fullscreen_state_atom;
	
	XSendEvent(display,
		   RootWindow(display, visualInfo.screen),
		   False,
		   SubstructureNotifyMask,
		   &event);
		   
}

void uninitialize(void){
	
	//code
	if(window){
		XDestroyWindow(display, window);
	}
	
	if(colormap){
		XFreeColormap(display, colormap);
	}
	
	if(display){
		XCloseDisplay(display);
		display = NULL;
	}
	
}


