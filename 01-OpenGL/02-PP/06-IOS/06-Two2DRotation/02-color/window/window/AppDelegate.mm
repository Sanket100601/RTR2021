//
//  AppDelegate.m
//  window
//
//  Created by Admin on 25/02/23.
//
#import "AppDelegate.h"
#import "ViewController.h"
#import "GLESView.h"

@implementation AppDelegate
{
    @private
    UIWindow *window;
    ViewController *viewController;
    GLESView *view;
}

- (BOOL)application:(UIApplication *)application willFinishLaunchingWithOptions:(NSDictionary<UIApplicationLaunchOptionsKey,id> *)launchOptions
{
    // code
    window = [UIWindow new];
    
    [window setBackgroundColor:[UIColor blackColor]];
    
    viewController = [[ViewController alloc]init];
    
    [window setRootViewController:viewController];
    
    CGRect rect=window.screen.bounds;
    //objective-c
   // CGRect rect = [[window screen]bounds];
    view = [[GLESView alloc]initWithFrame:rect];
    
    [viewController setView:view];
    
    [view release];
    
    [window makeKeyAndVisible];
    
    [view startAnimation];
    
    return YES;
    
    
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    //code
    [view stopAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    //code
  
}

- (void) applicationWillEnterForeground:(UIApplication *)application
{
    //code
   
}

- (void) applicationDidBecomeActive:(UIApplication *)application
{
    //code
    [view startAnimation];
}


-(void)applicationWillTerminate:(UIApplication *)application
{
    [view stopAnimation];
}

- (void)dealloc
{
  //code
  
  [super dealloc];
    
    if(view)
    {
        [view release];
        view=nil;
    }
    
    if(viewController)
    {
        [viewController release];
        viewController=nil;
    }
    
    if(window)
    {
        [window release];
        window=nil;
    }
}

@end

