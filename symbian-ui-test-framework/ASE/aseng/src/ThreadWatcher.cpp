//
// ThreadWatcher.cpp
// Copyright (c) 2001 Psion Computers PLC
//
#include "ThreadWatcher.h"

CThreadWatcher* CThreadWatcher::NewL(TThreadId aThreadId,MThreadWatcherObserver* aObserver)
	{
	CThreadWatcher* watcher=new(ELeave) CThreadWatcher(aThreadId,aObserver);
	CleanupStack::PushL(watcher);
	watcher->ConstructL();
	CleanupStack::Pop(watcher);
	return watcher;
	}

CThreadWatcher::CThreadWatcher(TThreadId aThreadId,MThreadWatcherObserver* aObserver)
	: CActive(CActive::EPriorityStandard), iThreadId(aThreadId), iObserver(aObserver)
	{
	CActiveScheduler::Add(this);
	}

CThreadWatcher::~CThreadWatcher()
	{
	if(IsActive())
		Cancel();
	iThread.Close();
	}

void CThreadWatcher::ConstructL()
	{
	User::LeaveIfError(iThread.Open(iThreadId));
	iThread.Logon(iStatus);
	SetActive();
	}

void CThreadWatcher::RunL()
	{
	iObserver->HandleThreadExit(this);
	}

void CThreadWatcher::DoCancel()
	{
	iThread.LogonCancel(iStatus);
	}
