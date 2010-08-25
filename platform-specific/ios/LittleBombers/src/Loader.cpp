#include "Loader.h"


boost::shared_ptr<Loader> Loader::instance() {
	static boost::weak_ptr<Loader> _instance;
	if(boost::shared_ptr<Loader> r = _instance.lock()) return r;
	boost::shared_ptr<Loader> x(new (nothrow) Loader);
	_instance = x;
	return x;
}

Loader::Loader(): mQuitRequested(false) {
	mBackgroundThread=boost::thread(boost::bind(&Loader::run, this));
}

Loader::~Loader() {
	mQuitRequested=true;
	mBackgroundThread.join();
}
bool Loader::enqueue(const boost::function<bool()>& doLoad, const boost::function<void(bool)>& loadComplete) {
	bool added=false;
	task_t task(doLoad, loadComplete);
	do {
		boost::mutex::scoped_lock locked(mMutex);
		try { mFifo.push(task); added=true; } catch(...) { }
	} while(0);
	return added;
}

static void nop(bool) { void(); }
bool Loader::enqueue(const boost::function<bool()>& doLoad) {
	return enqueue(doLoad, boost::bind(nop, _1));
}

void Loader::run() {
	DEBUG_LOG("[Loader] Spawned background thread…");
	while(!mQuitRequested) {
		usleep(1000);
		if (!mFifo.empty()) {
			task_t task;
			do {
				boost::mutex::scoped_lock locked(mMutex);
				task=mFifo.front();
				mFifo.pop();
			} while(false);
			
			if (!task.first) continue;
			bool result = task.first();
				
			if (!task.second) continue;
			task.second(result);
		}
	}
}
