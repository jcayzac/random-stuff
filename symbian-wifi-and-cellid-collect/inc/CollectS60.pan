/*
 ============================================================================
 Name		: CollectS60.pan
 Author	  : Mappy
 Copyright   : Confidential
 Description : This file contains panic codes.
 ============================================================================
 */

#ifndef __COLLECTS60_PAN__
#define __COLLECTS60_PAN__

/** CollectS60 application panic codes */
enum TCollectS60Panics
	{
	ECollectS60Ui = 1
	// add further panics here
	};

inline void Panic (TCollectS60Panics aReason)
	{
	_LIT (applicationName, "CollectS60");
	User::Panic (applicationName, aReason);
	}

#endif // __COLLECTS60_PAN__
