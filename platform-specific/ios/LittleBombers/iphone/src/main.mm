#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/EAGLDrawable.h>

#include <string>
#include <sstream>
#include <vector>
#include <queue>

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>

#include <stdlib.h>
#include <assert.h>

#include "Game.h"
#include "IPod.h"
#include "Platform.h"
#include "FileSystem.h"

#define NEED_RUNFAST_ACTIVATION 1
//#define GAME_USE_THREADS 1
#define GAME_SKIP_FRAMES_RUNNING 0
#define GAME_SKIP_FRAMES_IDLE 3
#define GAME_MAX_FPS 30

//////////////////////// OBJECTIVE-C INTERFACES /////////////////////

// For OS <3.1
#if USE_DISPLAY_LINK_API
@interface NSObject(CADisplayLink)
+ (id)displayLinkWithTarget:(id)target selector:(SEL)sel;
- (void)addToRunLoop:(NSRunLoop *)runloop forMode:(NSString *)mode;
- (void)removeFromRunLoop:(NSRunLoop *)runloop forMode:(NSString *)mode;
- (void)invalidate;
@property(readonly, nonatomic) CFTimeInterval timestamp, duration;
@property(getter=isPaused, nonatomic) BOOL paused;
@property(nonatomic) NSInteger frameInterval;
@end
#endif

@interface MainView : UIView
+ (Class) layerClass;
- (void) dealloc;
- (void) layoutSubviews;	
@end

@interface AppDelegate : NSObject <UIApplicationDelegate>
- (void) verticalBlank;
@end

//////////////////////// IMPLEMENTATION /////////////////////

struct App {
	struct MultiTouchEvent {
		enum What { BEGAN, ENDED, MOVED };
		What what;
		std::vector<TouchEvent> touches;
	};
	
	// Objective-C objects
	AppDelegate* delegate;
	UIWindow*    window;
	MainView*    view;
	CGRect       bounds;
	id           timer;
	
	// OpenGL
	EAGLContext* context;
	GLuint viewRenderbuffer;
	GLuint viewFramebuffer;
	GLuint depthRenderbuffer;
	
	// the game instance
	boost::shared_ptr<Game> game;
	
	// Should we start to draw?
	volatile bool updated;
	
	// Audio
	bool iPodIsPlaying;
	
	// Framerate management
	float oldTime;
	bool idle;
	bool started;
	bool displayLinkSupported;
	
	std::queue<MultiTouchEvent> pendingMultiTouchEvents;
	
	App(AppDelegate* x);
	~App();
	void startTimer();
	void stop();
	void onIdle(bool value);
	bool createFramebuffer();
	void destroyFramebuffer();
	void postMultitouchEvents(const MultiTouchEvent::What& what, UIEvent* event);
	void onVerticalBlank();
	void onLayout();
	void onApplicationStart();
	void onApplicationQuit();
	
	void render();
	void update();
	
	// Multithread support
	boost::thread gameThread;
	boost::mutex eventMutex, renderMutex;
	volatile bool startupCompleted;
	volatile bool running;
	volatile bool needFrame;
	void operator()();
};

boost::shared_ptr<App> app;

App::App(AppDelegate* x) {
	delegate=x;
	window=0;
	view=0;
	timer=0;
	context=0;
	updated=false;
	iPodIsPlaying=false;
	oldTime=.0f;
	idle=false;
	started=false;
	displayLinkSupported=false;
	needFrame=true;
}
App::~App() {
	if (gameThread.get_id()!=boost::thread::id()) {
		running=false;
		gameThread.join();
	}
	stop();
	if (game) game.reset();
	[EAGLContext setCurrentContext:context];
	destroyFramebuffer();
	[EAGLContext setCurrentContext:nil];
	[context release];
	[view release];
	view=0;
	[window release];
	window=0;		
}
void App::startTimer() {
	if (!started) {
		oldTime=Platform::timeAsSeconds();
		int framesSpan=1+idle?GAME_SKIP_FRAMES_IDLE:GAME_SKIP_FRAMES_RUNNING;
		
		if (displayLinkSupported) {
			DEBUG_LOG("Creating new display link");
			timer = [CADisplayLink displayLinkWithTarget:delegate selector:@selector(verticalBlank)];
			[timer setFrameInterval:framesSpan];
			[timer addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
		}
		else {
			DEBUG_LOG("Creating new display timer");
			timer = [NSTimer scheduledTimerWithTimeInterval:(NSTimeInterval)(((double)framesSpan)/((double)GAME_MAX_FPS)) target:delegate selector:@selector(verticalBlank) userInfo:nil repeats:TRUE];
		}
		
		if (iPodIsPlaying!=isIPodPlaying()) {
			iPodIsPlaying=!iPodIsPlaying;
			DEBUG_LOG("iPod state changed. New state=" << (iPodIsPlaying?"playing":"stopped"));
			if (game) game->externalAudio(iPodIsPlaying);
		}
		started = true;
	}
}
void App::stop() {
	if (started) {
		DEBUG_LOG("Deleting display link or timer");
		[timer invalidate];
		timer = nil;
		started = false;
	}
}
void App::onIdle(bool value) {
	if (idle!=value) {
		idle=value;
		stop();
		startTimer();
	}
}
bool App::createFramebuffer() {
	glGenFramebuffersOES(1, &viewFramebuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, viewFramebuffer);
	glGenRenderbuffersOES(1, &viewRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	[context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(CAEAGLLayer*) [view layer]];
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, viewRenderbuffer);
	GLint _backingWidth;
	GLint _backingHeight;
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_WIDTH_OES, &_backingWidth);
	glGetRenderbufferParameterivOES(GL_RENDERBUFFER_OES, GL_RENDERBUFFER_HEIGHT_OES, &_backingHeight);
	glGenRenderbuffersOES(1, &depthRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, depthRenderbuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_DEPTH_COMPONENT16_OES, _backingWidth, _backingHeight);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, depthRenderbuffer);
	[EAGLContext setCurrentContext:context];
	if(glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES) != GL_FRAMEBUFFER_COMPLETE_OES) {
		const char* err="<unknown>";
		switch (glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES)) {
#define XYZABC(x) case GL_FRAMEBUFFER_##x##_OES: err=#x; break;
				XYZABC(COMPLETE)
				XYZABC(INCOMPLETE_ATTACHMENT)
				XYZABC(INCOMPLETE_MISSING_ATTACHMENT)
				XYZABC(INCOMPLETE_DIMENSIONS)
				XYZABC(INCOMPLETE_FORMATS)
				XYZABC(UNSUPPORTED)
#undef XYZABC(x)
		}
		DEBUG_LOG("[GLES] Error making FBO: « " << err << " »");
		return false;
	}
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, viewRenderbuffer);
	return true;
}
void App::destroyFramebuffer() {
	glDeleteFramebuffersOES(1, &viewFramebuffer); 
	viewFramebuffer = 0;
	glDeleteRenderbuffersOES(1, &viewRenderbuffer);
	viewRenderbuffer = 0;
	glDeleteRenderbuffersOES(1, &depthRenderbuffer);
	depthRenderbuffer = 0;
}
void App::postMultitouchEvents(const MultiTouchEvent::What& what, UIEvent* event) {
	MultiTouchEvent ev;
	ev.what = what;
	
	NSSet* _from = [event touchesForView:view];
	std::vector<TouchEvent>& _to = ev.touches;
	int count = [_from count];
	NSEnumerator* en = [_from objectEnumerator];
	
	try { _to.resize(count); }
	catch(...) { return; }
	
	size_t i=0;
	for (id t in en) {
		UITouch* touch = (UITouch*)t;
		TouchEvent te;
		te.timestamp = touch.timestamp;
		te.tapCount  = touch.tapCount;
		CGPoint pos  = [touch locationInView:view];
		CGPoint prev = [touch previousLocationInView:view];
		te.screenX = pos.x;
		te.screenY = pos.y;
		te.previousX = prev.x;
		te.previousY = prev.y;
		switch([touch phase]) {
			case UITouchPhaseBegan: te.phase=TouchEvent::BEGAN; break;
			case UITouchPhaseMoved: te.phase=TouchEvent::MOVED; break;
			case UITouchPhaseStationary: te.phase=TouchEvent::STATIONARY; break;
			case UITouchPhaseEnded: te.phase=TouchEvent::ENDED; break;
			case UITouchPhaseCancelled: te.phase=TouchEvent::CANCELLED; break;
		}
		_to[i]=te;
		++i;
	}
	eventMutex.lock();
	try { pendingMultiTouchEvents.push(ev); } catch(...) { assert(false); }
	eventMutex.unlock();
}
void App::update() {
	// Send events to game
	eventMutex.lock();
	while(!pendingMultiTouchEvents.empty()) {
		const MultiTouchEvent& ev=pendingMultiTouchEvents.front();
		switch (ev.what) {
			case MultiTouchEvent::BEGAN: game->touchesBegan(ev.touches); break;
			case MultiTouchEvent::ENDED: game->touchesEnded(ev.touches); break;
			case MultiTouchEvent::MOVED: game->touchesMoved(ev.touches); break;
		}
		pendingMultiTouchEvents.pop();
	}
	eventMutex.unlock();
	// Update game
	float now=Platform::timeAsSeconds();
	float delta=now-oldTime;
	if (delta>10.f) delta=1.f/60.f; // DST, etc
	oldTime=now;
	game->update(delta);
	updated=true;
}
void App::render() {
	renderMutex.lock();
	if (updated) {
		game->draw();
	}
	renderMutex.unlock();
}
void App::onVerticalBlank() {
	bool isthreaded=(gameThread.get_id()!=boost::thread::id());
	/*if (!isthreaded)*/ render();
	renderMutex.lock();
	[context presentRenderbuffer:GL_RENDERBUFFER_OES];
	needFrame=true;
	renderMutex.unlock();
	if (!isthreaded) update();
}
void App::onLayout() {
	[EAGLContext setCurrentContext:context];
	destroyFramebuffer();
	createFramebuffer();
	onVerticalBlank();
}
void App::onApplicationStart() {
	bool b;
	
	UIApplication* application = [UIApplication sharedApplication];
	assert(application);
	
	[application setStatusBarHidden:YES animated:NO];
	//application.statusBarOrientation = UIInterfaceOrientationLandscapeLeft;
	application.idleTimerDisabled=true;
	
	bounds = [[UIScreen mainScreen] applicationFrame];
	window = [[[UIWindow alloc] initWithFrame:bounds] retain];
	assert(window);

	view = [[[MainView alloc] initWithFrame:bounds] retain];
	assert(view);
	view.multipleTouchEnabled=true;
	view.opaque=true;
	
	game=Game::instance();
	assert(game);
	
	b=initIPod();
	assert(b);
	
	CAEAGLLayer* eaglLayer = (CAEAGLLayer *)view.layer;
	eaglLayer.opaque=true;
	eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
									kEAGLColorFormatRGB565, kEAGLDrawablePropertyColorFormat,
									nil];
	
	context=[[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1] autorelease];
	assert(context);
	[context retain];
	
	b=[EAGLContext setCurrentContext:context];
	assert(b);
	
#ifdef USE_DISPLAY_LINK_API
	displayLinkSupported = ([[[UIDevice currentDevice] systemVersion] compare:@"3.1" options:NSNumericSearch] != NSOrderedAscending);
#endif
	
	[window addSubview:view];
	[window makeKeyAndVisible];

	startTimer();

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	while(glGetError()) assert(false);
	
b=game->init(bounds.size.width, bounds.size.height);
assert(b);
#if GAME_USE_THREADS
	startupCompleted=false;
	running=true;
	gameThread=boost::thread(boost::ref(*this));
	while( !startupCompleted ) usleep(1000);
#else
	onVerticalBlank();
#endif
}

void App::onApplicationQuit() {
}

void App::operator()() {
    NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
	DEBUG_LOG("Tadaaaaaa!");
	[EAGLContext setCurrentContext:context];
//	bool b=game->init(bounds.size.width, bounds.size.height);
//	assert(b);
	startupCompleted=true;
	while(running) {
		/*if (needFrame) {
			render();
			needFrame=false;
		}*/
		update();
		boost::this_thread::yield();
	}
    [pool release];
}

////////////////////// OBJECTIVE-C OBJECTS (CONT'D) /////////////////
@implementation MainView
+ (Class)layerClass  { return [CAEAGLLayer class]; }
- (void) dealloc { [super dealloc]; }
- (void) layoutSubviews { app->onLayout(); }
- (void) touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event { app->postMultitouchEvents(App::MultiTouchEvent::BEGAN, event); }
- (void) touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event { app->postMultitouchEvents(App::MultiTouchEvent::ENDED, event); }
- (void) touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event { app->postMultitouchEvents(App::MultiTouchEvent::MOVED, event); }
@end
@implementation AppDelegate
- (id) init {
	self = [super init];
	if (self) {
		app.reset(new (nothrow) App(self));
		if (!app) {
			[self release];
			return nil;
		}
	}
	return self;
}
- (void) verticalBlank { app->onVerticalBlank(); }
- (void) applicationDidFinishLaunching:(UIApplication*)application { app->onApplicationStart(); }
- (void) applicationDidBecomeActive:(  UIApplication*)application { app->onIdle(false); }
- (void) applicationWillResignActive:( UIApplication*)application { app->onIdle(true); }
- (void) applicationWillTerminate:(    UIApplication*)application { app->onApplicationQuit(); app.reset(); }
- (void) dealloc { [super dealloc]; }
@end

#include <iostream>

int main(int argc, char *argv[]) {
	// Setup app environment
	std::string pathToExecutable(argv[0]);
	std::string pathToBundle(pathToExecutable.substr(0,pathToExecutable.find_last_of("/\\")));
	std::string pathToRoot(pathToBundle.substr(0,pathToBundle.find_last_of("/\\")));
	std::stringstream pathToCache; pathToCache  << pathToRoot << "/Library/Caches";
	std::stringstream pathToDocuments; pathToDocuments  << pathToRoot << "/Documents";
	std::stringstream pathToTmp; pathToTmp << pathToRoot << "/tmp";
	FileSystem::set(pathToRoot, pathToCache.str(), pathToTmp.str(), pathToBundle,
					pathToDocuments.str(), pathToBundle);
	
	// Set CWD in case we use some C api
	setenv("CWD", pathToBundle.c_str(), 1);
	setenv("TMPDIR", pathToTmp.str().c_str(), 1);
	
#ifdef __VFP_FP__
//#if NEED_RUNFAST_ACTIVATION
	static const unsigned int X = 0x04086060;
	static const unsigned int Y = 0x03000000;
	int R;
	asm volatile (
				  "fmrx	%0, fpscr			\n\t"	//r0 = FPSCR
				  "and	%0, %0, %1			\n\t"	//r0 = r0 & 0x04086060
				  "orr	%0, %0, %2			\n\t"	//r0 = r0 | 0x03000000
				  "fmxr	fpscr, %0			\n\t"	//FPSCR = r0
				  : "=r"(R)
				  : "r"(X), "r"(Y)
				  );
//#endif
#endif

	// Launch app
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    int retVal = UIApplicationMain(argc, argv, nil, nil);
    [pool release];
    return retVal;
}
