#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#import <UIKit/UIKit.h>
#import "ProcessAppDelegate.h"

int main(int argc, char *argv[]) {
	NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

	// Setup various standard environment variable
	std::string pathToExecutable(argv[0]);
	std::string pathToBundle(pathToExecutable.substr(0,pathToExecutable.find_last_of("/\\")));
	std::string pathToRoot(pathToBundle.substr(0,pathToBundle.find_last_of("/\\")));
	std::stringstream pathToCache; pathToCache  << pathToRoot << "/Library/Caches";
	std::stringstream pathToDocuments; pathToDocuments  << pathToRoot << "/Documents";
	std::stringstream pathToTmp; pathToTmp << pathToRoot << "/tmp";
	setenv("CWD", pathToBundle.c_str(), 1);
	setenv("TMPDIR", pathToTmp.str().c_str(), 1);

	// Flush TMPDIR
	NSFileManager* fm = [[NSFileManager alloc] init];
	NSString* s = [[NSString alloc] initWithUTF8String:pathToTmp.str().c_str()];
	NSDirectoryEnumerator* en = [fm enumeratorAtPath:s];
	NSError* err = nil;
	BOOL res;
	while (NSString* file = [en nextObject]) {
		res = [fm removeItemAtPath:[s stringByAppendingPathComponent:file] error:&err];
		if (!res && err) {
			NSLog(@"Can't flush TMPDIR: %@", err);
		}
	}
	[s release];
	[en release];
	[fm release];
	
	// Run application
	int retVal = UIApplicationMain(argc, argv, nil, NSStringFromClass(ProcessAppDelegate.class));

	[pool release];
	return retVal;
}
