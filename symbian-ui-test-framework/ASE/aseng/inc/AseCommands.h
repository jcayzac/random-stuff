#if !defined(ASE_COMMANDS_H_INCLUDED)
#define ASE_COMMANDS_H_INCLUDED


// main function
_LIT(KMain,					"main");

// private functions
_LIT(KPrivateTraceCmd,		"privatetrace");

// typename() method for all objects
_LIT(KTypeNameCmd,			"typename");

/////// Public API ////////

// addlog("file://c:/toto.txt");
// addlog("com://1");
// addlog("sms://0684755420");
// addlog("infoprint");
// - Add a channel to the log facility
_LIT(KAddLogCmd,			"addlog");

// break()
// - Stop the script execution
_LIT(KBreakCmd,				"break");

// click(x, y);
// click(point);
// - Click at specified position
_LIT(KClickCmd,				"click");

// close(uid);
// close("name");
// - Close an application
_LIT(KCloseCmd,				"close");

// myPoint = create("point");
// - Create an object
_LIT(KCreateCmd,			"create");

// drag(x0, y0, point1, x2, y2, x3, y3, point4, point5, ...);
// - Drag the mouse around
_LIT(KDragCmd,				"drag");

// coords = findtext("toto");
// infoprint("X = " # coords.x);
// infoprint("Y = " # coords.y);
// - Find some text on the screen
_LIT(KFindTextCmd,			"findtext");

// hint("findtext_firstonly", 1);
// hint("findtext_firstonly", 0);
// currentVal = hint("findtext_firstonly");
// - Changes the behaviour of certain commands
_LIT(KHintCmd,				"hint");

// infoprint("toto");
// - Displays an infoprint
_LIT(KInfoPrintCmd,			"infoprint");

// key("x");
// key("ctrl+x");
// key("shift+x");
// key("shift+ctrl+x");
// key("shift+ctrl+fn+x");
// key("home");
// key("del");
_LIT(KKeyCmd,				"key");

// log("tot");
// - Send a string to the log facility
_LIT(KLogCmd,				"log");

// success = makedir("c:/mydir");
_LIT(KMakeDirCmd,			"makedir");

// uid = open("myapp");
// uid = open("z:/system/apps/myapp/myapp.app");
// uid = open("c:/toto.png");
// - Open an application/a document
_LIT(KOpenCmd,				"open");

// reboot()
// - Restarts the device
_LIT(KRebootCmd,			"reboot");

// resetlog();
// - Reset the channel list of the log facility
_LIT(KResetLogCmd,			"resetlog");

// screenshotbase("c:/path/base%03d.png");
_LIT(KScreenShotBaseCmd,	"screenshotbase");

// screenshot();
// screenshot(filename);
_LIT(KScreenShotCmd,		"screenshot");

// sleep();
// sleep(seconds);
_LIT(KSleepCmd,				"sleep");

// text("abcdefghijklmnopqrstuvwxyz");
_LIT(KTextCmd,				"text");

// success = validdrive("e");
_LIT(KValidDriveCmd,		"validdrive");

///////////////// special variables /////////////////
_LIT(KSessionPathVar,		"sessionpath");

///////////////// objects names /////////////////////

_LIT(KPointObj,				"point");
_LIT(KColorObj,				"color");
_LIT(KFontObj,				"font");

////////////////// hints /////////////////////////////

_LIT(KFindTextFirstOnlyHint,	"findtext_firstonly");

////////////////// .INI file /////////////////////////

_LIT(KIniBootFile,				"bootfile");
_LIT(KIniEntryPoint,			"entrypoint");

/*

  Commands:

  - close
  Syntax:
	close(uid);
	close("AppCaption");

  - create
  Syntax:
	obj = create("typename");
	valid type names: "point" (TODO: "font", "color").

  - findtext
  Syntax:
	findtext("string");
	findtext("string", x0, y0);
	findtext("string", origin_point); where origin_point is a "point" object

  - hint
  Syntax:
	hint("hintname") = intvalue;
	valid hints:
		"findtext_firstonly"
			1 means we only want the *closer next match* from the origin,
			0 is way faster and return one match if found.

  - infoprint
  Syntax:
	infoprint("string");

  - key
  Syntax:
	key("keyspec");

  - makedir
  Syntax:
	success = makedir("f:/ull/path");

  - open
  Syntax:
	uid = open("myapp");
	uid = open("z:/system/apps/myapp/myapp.app");
	uid = open("c:/toto.png");

  - screenshot
  Syntax:
	screenshot("f:/ile/name.png");
	screenshot();

  - sleep
  Syntax:
	sleep();
	sleep(seconds);

  - text
  Syntax:
	text("string");


 */
#endif
