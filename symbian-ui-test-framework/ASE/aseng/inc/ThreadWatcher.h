//
// ThreadWatcher.h
//
#if !defined(__THREADWATCHER_H__)
#define __THREADWATCHER_H__

#include <e32base.h>

/**
 * class MThreadWatcherObserver
 *
 * observer base class for the thread watcher
 */
class CThreadWatcher;
class MThreadWatcherObserver
	{
public:
	virtual void HandleThreadExit(CThreadWatcher* aWatcher)=0;
	};

/**
 * class CThreadWatcher
 *
 * used to watch an application and return information when
 * the application closes, panics, is killed or dies
 */
class CThreadWatcher : public CActive
	{
public:
	static CThreadWatcher* NewL(TThreadId aThreadId,MThreadWatcherObserver* aObserver);
	~CThreadWatcher();
//
	inline TThreadId ThreadId() const { return iThreadId; };
	inline const RThread& Thread() const { return iThread; };
	inline RThread& Thread() { return iThread; };
private:
	CThreadWatcher(TThreadId aThreadId,MThreadWatcherObserver* aObserver);
	void ConstructL();

// from CActive
	void RunL();
	void DoCancel();
private:
	TThreadId iThreadId;
	MThreadWatcherObserver* iObserver;
	RThread iThread;
	};

#endif // __THREADWATCHER_H__
