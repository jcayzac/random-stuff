//
//  Planar_Image_FinderAppDelegate.h
//  Planar Image Finder
//
//  Created by Cayzac Julien on 9/24/10.
//  Copyright 2010 -. All rights reserved.
//

#import <UIKit/UIKit.h>

@class Planar_Image_FinderViewController;

@interface Planar_Image_FinderAppDelegate : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    Planar_Image_FinderViewController *viewController;
}

@property (nonatomic, strong) IBOutlet UIWindow *window;
@property (nonatomic, strong) IBOutlet Planar_Image_FinderViewController *viewController;

@end

