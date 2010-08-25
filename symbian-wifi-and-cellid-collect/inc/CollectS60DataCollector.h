#ifndef __COLLECTS60DATACOLLECTOR__
#define __COLLECTS60DATACOLLECTOR__

#include "CollectS60RIL.h"
#include "CollectS60LBS.h"
#include "CollectS60WiFi.h"
#include "GeoSignalDB.h"

class CDataCollector
: public ILBSListener
{
public:
	static CDataCollector* NewL();
	static CDataCollector* NewLC();
	virtual ~CDataCollector();
	void Start(bool useExternalGPSApi=false);
	void Stop();
	// Use this with Start(true)
	void SetCurrentPosition(float longitude, float latitude);
private:
	CDataCollector();
	void ConstructL();
private:
	void OnLBSPosition(float longitude,float latitude);

private:
	CLBSActive*			m_pLBSActive;
	CRIL*				m_pRIL;
	CWifi*				m_pWifi;
	CGeoSignalDB*		m_pDB;
};

#endif // __COLLECTS60DATACOLLECTOR__
