//
//  main.m
//  Planar Image Finder
//
//  Created by Cayzac Julien on 9/24/10.
//  Copyright 2010 -. All rights reserved.
//

#include <string>
#include "SystemPaths.h"
#import <UIKit/UIKit.h>

int main(int argc, char *argv[]) {
	SystemPaths::init(argv[0]);
	
	@autoreleasepool {
	
	// Flush TMPDIR
		NSString* tmpdir = [[NSString alloc] initWithUTF8String:SystemPaths::get(SystemPaths::TEMP).c_str()];
		NSFileManager* fm = [[NSFileManager alloc] init];
		NSDirectoryEnumerator* en = [fm enumeratorAtPath:tmpdir];
		while (NSString* file = [en nextObject]) {
			NSError* err = nil;
			BOOL res = [fm removeItemAtPath:[tmpdir stringByAppendingPathComponent:file] error:&err];
			if (!res && err) NSLog(@"Can't flush TMPDIR: %@", err);
		}
		
		// Run application
    int retVal = UIApplicationMain(argc, argv, nil, nil);
		
    return retVal;
    }
}
