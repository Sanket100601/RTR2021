// sh build.sh
// Header files
#import <foundation/Foundation.h>
#import <cocoa/cocoa.h>
// OpenGL
#import <QuartzCore/CVDisplayLink.h>
#import <OpenGL/gl3.h>
#import <OpenGL/gl3ext.h>

// Global Function Declaration
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef, const CVTimeStamp*, const CVTimeStamp*,
                               CVOptionFlags, CVOptionFlags*, void*);

// Global varaible declarations
FILE *gpFile = NULL;

// Interface/class declarations
@interface AppDelegate : NSObject < NSApplicationDelegate, NSWindowDelegate >
  
@end

@interface GLView : NSOpenGLView 

@end


// EntryPoint Function

int main(int argc, char* argv[])
{
    // code
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init]; // Memroy management 

    NSApp = [NSApplication sharedApplication];

    AppDelegate* appDelegate = [[AppDelegate alloc] init];

    [NSApp setDelegate: appDelegate];

    // GameLoop
    [NSApp run];
     
    // release 
    [pool release];

    return 0;
}

// implementation of AppDelegate
@implementation AppDelegate
{
    @private
    NSWindow *window;
    GLView *view;
}

- (void) applicationDidFinishLaunching: (NSNotification*) notification
{
    // code
   // NSLog(@"Application Started Successfully !!\n");
    NSBundle *appBundle = [NSBundle mainBundle];
    NSString *appDirPath = [appBundle bundlePath];
    NSString *parentDirPath = [appDirPath stringByDeletingLastPathComponent];
    NSString *logFileNameWithPath = [NSString stringWithFormat:@"%@/log.txt", parentDirPath];
    const char* pszLogFileNameWithPath = [logFileNameWithPath cStringUsingEncoding:NSASCIIStringEncoding];
   // const char* pszLogFileNameWithPath = [logFileNameWithPath UTF8String];

    gpFile = fopen(pszLogFileNameWithPath, "w");
    if(gpFile == NULL)
    {
        [self release];
        [NSApp terminate:self];
    }

    fprintf(gpFile, "program is started Successfully\n");

    NSRect rect = NSMakeRect(0.0, 0.0, 800.0, 600.0);
    window = [[NSWindow alloc] initWithContentRect:rect
                                styleMask:NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable
                                backing:NSBackingStoreBuffered
                                defer:NO];

    [window setTitle:@"macOS Sanket Pawar"];
    NSColor *backgroundColor = [NSColor blackColor];
    [window setBackgroundColor: backgroundColor];

    // create view
    view = [[GLView alloc] initWithFrame:rect];
    //set view
    [window setContentView:view];

    [window center];
    [window setDelegate:self];
    [window makeKeyAndOrderFront:self];
   



}

- (void) applicationWillTerminate: (NSNotification*) notification
{
    // code
    // NSLog(@"Application Terminate Successfully !!\n");
    if(gpFile)
    {
        fprintf(gpFile, "program is Terminate Successfully\n");
        fclose(gpFile);
        gpFile = NULL;
    }
    
}

- (void) windowWillClose: (NSNotification*) notification
{
    // code
    [NSApp terminate:self];
}

-(void) dealloc
{
    // code
    if(view)
    {
        [view release];
        view = nil;
    }

    if(window)
    {
        [window release];
        window = nil;
    }
    
    [super dealloc];
}
@end 



@implementation GLView
{
    @private
    CVDisplayLinkRef displayLink;
}

-(id) initWithFrame:(NSRect)frame
{
    // code
    self = [super initWithFrame:frame];

    if(self)
    {
        // 1. Init Array of OpenGL Pixel format attribute 
        NSOpenGLPixelFormatAttribute openGLattributes[] =
        {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion4_1Core,
            NSOpenGLPFAScreenMask, CGDisplayIDToOpenGLDisplayMask(kCGDirectMainDisplay),
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFANoRecovery,
            NSOpenGLPFAAccelerated,
            NSOpenGLPFADoubleBuffer,
            0
        };

        //2. Create OprnGL Pixel Format using attribute
        NSOpenGLPixelFormat *gLPixelFormat = [[[NSOpenGLPixelFormat alloc] initWithAttributes:openGLattributes] autorelease];
        if(gLPixelFormat == nil)
        {
            fprintf(gpFile, "Error : Failed to get OpenGL Pixel Format.\n");
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
        }
        
        //3. Create OpenGL context in above format
        NSOpenGLContext *gLContext = [[[NSOpenGLContext alloc] initWithFormat:gLPixelFormat shareContext:nil] autorelease];
        if(gLContext == nil)
        {
            fprintf(gpFile, "Error : Failed to get OpenGL Context.\n");
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
        }

        //4.  Set this view's Pixel Format using above Pixel Format 
        [self setPixelFormat:gLPixelFormat];
        
        //5. Set View's by using Above OpenGL Context
        [self setOpenGLContext:gLContext];
    }

    return (self);
}

// Define getFrameForTime() with predefine param
-(CVReturn)getFrameForTime:(const CVTimeStamp*)outputTime
{
    //code
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    [self drawView];
    [pool release];
    
    return (kCVReturnSuccess);
}

// overrideable
-(void)prepareOpenGL
{   
    // code
    [super prepareOpenGL];
    [[self openGLContext] makeCurrentContext];


    // set boudble buffer swaping interval into one
    GLint swapInterval = 1;

    [[self openGLContext] setValues:&swapInterval forParameter:NSOpenGLCPSwapInterval];

    // OpenGL Log
    fprintf(gpFile, "Renderer : %s\n", glGetString(GL_RENDERER));
    fprintf(gpFile, "Version : %s\n", glGetString(GL_VERSION));
    fprintf(gpFile, "GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    [self initialize];

    // cretae, config , start displaylink

    // create displaylink
    CVDisplayLinkCreateWithActiveCGDisplays(&displayLink);

    // set callback in displaylink
    CVDisplayLinkSetOutputCallback(displayLink, &MyDisplayLinkCallback, self);

    // convert nsopengl pixel foramt to cgl pixel format
    CGLPixelFormatObj cglPixelformat = (CGLPixelFormatObj)[[self pixelFormat] CGLPixelFormatObj];

    //convert cgl context
    CGLContextObj cglContext = (CGLContextObj)[[self openGLContext] CGLContextObj];

    // using above info set current CG display to CGL pixel format
    CVDisplayLinkSetCurrentCGDisplayFromOpenGLContext(displayLink, cglContext, cglPixelformat);

    // start displaylink
    CVDisplayLinkStart(displayLink);


}

-(void)drawRect:(NSRect)dirtyRect
{
    //code
    [self drawView];
}

-(void)drawView
{
    //code
    [[self openGLContext] makeCurrentContext];

    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

    // call ur display here
    [self display];

    // do double buffering
    CGLFlushDrawable((CGLContextObj)[[self openGLContext] CGLContextObj]);

    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);
}

-(int)initialize
{
    // code
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    return 0;
}

// override 
-(void)reshape
{
    //code
    [super reshape];
    CGLLockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

    NSRect rect = [self bounds];
    int width = rect.size.width;
    int height = rect.size.height;

    // call our resize
    [self resize:width : height];

    CGLUnlockContext((CGLContextObj)[[self openGLContext] CGLContextObj]);

}

-(void)resize: (int)width : (int)height
{
    // code
    if(height < 0)
    {
        height = 1;
    }
        
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);

}

-(void)display
{
    // code 
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

} 

-(void)myUpdate
{
    // code 
}

-(void)uninitialize
{
    // code 
}

-(BOOL)acceptsFirstResponder
{
    //code
    [[self window] makeFirstResponder:self];
        
    return (YES);
}


-(void)keyDown:(NSEvent*)event
{
    //code
    int key = [[event characters] characterAtIndex:0];
    switch(key)
    {
        case 27:
            [self uninitialize];
            [self release];
            [NSApp terminate:self];
            break;
        
        case 'F':
        case 'f':
            [[self window] toggleFullScreen:self];
            break;
        default:
        break;
    }
}

-(void)mouseDown:(NSEvent*)event
{
    // code

}

-(void)dealloc
{
    [super dealloc];
    
    if(displayLink)
    {
        CVDisplayLinkStop(displayLink);
        CVDisplayLinkRelease(displayLink);
        displayLink = nil;
    }
}

@end

// Implement MyDisplayLinkCallback Callback function
CVReturn MyDisplayLinkCallback(CVDisplayLinkRef displayLink, const CVTimeStamp *currentTime, const CVTimeStamp *outputTime,
                               CVOptionFlags flagsIn, CVOptionFlags *flagsOut, void *view)
{
    //code
    CVReturn result = [(GLView*)view getFrameForTime:outputTime];
    
    return (result);
}
