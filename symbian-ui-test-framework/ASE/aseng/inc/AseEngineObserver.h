#if !defined(ASEENGINEOBSERVER_H_INCLUDED)
#define ASEENGINEOBSERVER_H_INCLUDED

class MAseEngineObserver
	{
public:
	virtual void trace(const TDesC& aMessage)=0;
	};

#endif
