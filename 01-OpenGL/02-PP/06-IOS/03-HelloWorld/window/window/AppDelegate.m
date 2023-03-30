//
//  AppDelegate.m
//  window
//
//  Created by Admin on 25/02/23.
//
#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#import "ViewController.h"
#import "MyView.h"

@implementation AppDelegate
{
    @private
    UIWindow *window;
    ViewController *viewcontroller;
    MyView *view;
}

- (BOOL) application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    // code
    CGRect rect = window.screen.bounds;
    //CGRect rect = [[window screen] bounds];
    
    window = [UIWindow new];
    [window setBackgroundColor:[UIColor blackColor]];
    
    viewcontroller = [[ViewController alloc] init];
    [window setRootViewController: viewcontroller];
    
    view = [[MyView alloc] initWithFrame: rect];
    
    [viewcontroller setView: view];
    
    [window makeKeyAndVisible];
    
    
    [view release];
    
    
    
    
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // code
    
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    //code
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    //code
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    //code
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    //code
}
- (void)dealloc
{
    //code
    if(window)
    {
        [window release];
        window = nil;
    }
    
    if(view)
    {
        [view release];
        view = nil;
    }

    if(viewcontroller)
    {
        [viewcontroller release];
        viewcontroller = nil;
    }

    [super dealloc];
    
}

@end

