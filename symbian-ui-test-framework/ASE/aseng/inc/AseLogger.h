#if !defined(ASELOGGER_H_INCLUDED)
#define ASELOGGER_H_INCLUDED


#include <e32std.h>
#include <e32base.h>
#include "AseUtils.h"

class MAseLogChannel
	{
	virtual void LogL(const TDesC& aString) = 0;
	};

class CAseLogger: public CBase
	{
public:
	void AddChannelL(const TDesC& aURI);
	void ResetChannels();
	void LogL(const TDesC& aString);
private:
private:
	CArrayFixSegPtr32<MAseLogChannel*>	iChannelLookup;
	};









#endif // ASELOGGER_H_INCLUDED
