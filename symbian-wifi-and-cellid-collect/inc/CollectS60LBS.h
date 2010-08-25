#ifndef __COLLECTS60LBS_H__
#define __COLLECTS60LBS_H__
#include <lbs.h>
#include <lbssatellite.h>

// ===========================================
// LBS-related stuff
// ===========================================

class ILBSListener {
public:
	virtual void OnLBSPosition(float longitude,float latitude)=0;
};

class CLBSActive: public CActive {
public:
	static CLBSActive* NewL(ILBSListener& listener);
	static CLBSActive* NewLC(ILBSListener& listener);
	virtual ~CLBSActive();
	void Start();
protected:
	CLBSActive(ILBSListener& listener);
	void ConstructL();
private:
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
private:
	RPositionServer			m_oPositionServer;
	RPositioner				m_oPositioner;
	TPositionSatelliteInfo	m_oSatelliteInfo;
	ILBSListener&			m_oListener;
	float					m_fCurrentLongitude;
	float					m_fCurrentLatitude;
	float					m_fCurrentSpeed;
	bool					m_bPositionServerOpened;
	bool					m_bPositionerOpened;
};

#endif // __COLLECTS60LBS_H__

