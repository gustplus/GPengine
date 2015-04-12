//
//  GPAppDelegate.m
//  GPEngine
//
//  Created by shizhan on 14-8-30.
//  Copyright (c) 2014年 ___GUSTPLUS___. All rights reserved.
//

#import "GPAppDelegate.h"
#import "GPEAGLView.h"
#import "GPGLViewImp_iOS.h"
#import "GPRootViewController.h"

#import "GPDirector.h"
#import "GPApplication.h"

@implementation GPAppDelegate

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    CGRect frame = [UIScreen mainScreen].bounds;
    _window = [[UIWindow alloc] initWithFrame: frame];
    GPEAGLView *view = [[GPEAGLView alloc] initWithFrame:frame PixelFormat:RGBA8 DepthFormat:DEPTH24_STENCIL8 multiSampling:NO WithNumOfSamples:0];
    GPRootViewController *controller = [[GPRootViewController alloc] initWithNibName:nil bundle:nil];
    [_window setRootViewController:controller];
    controller.view = view;
    //    controller.wantsFullScreenLayout = true;
    
    _window.backgroundColor = [UIColor whiteColor];
    [_window makeKeyAndVisible];
    [[UIApplication sharedApplication] setStatusBarHidden:YES];
    
    GPEngine3D::GLView *glview = new GPEngine3D::GLViewImp();
    glview->initWithEAGLView((__bridge void *) view);
    
    GPEngine3D::Director::getInstance()->setGLView(glview);
    
    GPEngine3D::Application::getInstance()->run();
    return YES;
}
							
- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
    GPEngine3D::Application::getInstance()->applicationDidEnterBackGround();
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
    GPEngine3D::Application::getInstance()->applicationWillEnterForeGround();
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
    GPEngine3D::Application::getInstance()->applicationWillTerminate();
}

@end
