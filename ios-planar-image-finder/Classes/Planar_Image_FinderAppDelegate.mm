//
//  Planar_Image_FinderAppDelegate.m
//  Planar Image Finder
//
//  Created by Cayzac Julien on 9/24/10.
//  Copyright 2010 -. All rights reserved.
//

#import "Planar_Image_FinderAppDelegate.h"
#import "Planar_Image_FinderViewController.h"

@implementation Planar_Image_FinderAppDelegate

@synthesize window;
@synthesize viewController;

- (void)applicationWillResignActive:(UIApplication *)application
{
	(void)application;
    [viewController stopAnimation];
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
	(void)application;
    [viewController startAnimation];
}

- (void)applicationWillTerminate:(UIApplication *)application
{
	(void)application;
    [viewController stopAnimation];
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
	(void)application;
    // Handle any background procedures not related to animation here.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
	(void)application;
    // Handle any foreground procedures not related to animation here.
}


@end
