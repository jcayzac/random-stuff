#ifndef __COLLECTS60APPUI_h__
#define __COLLECTS60APPUI_h__
#include <aknappui.h>
#include <eikedwin.h>
//#include "CollectS60RIL.h"
//#include "CollectS60LBS.h"
//#include "CollectS60WiFi.h"
#include "CollectS60DataCollector.h"


// ===========================================
// UI-related stuff
// ===========================================
class CEikConsoleScreen;
class CConsoleControl: public CCoeControl {
public:
	CConsoleControl();
	virtual ~CConsoleControl();
	void ConstructL();
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	void HandleCommandL(TInt aCommand);
	void ActivateL();
	inline CEikConsoleScreen* Screen() { return iConsole; }
protected:
	void FocusChanged(TDrawNow aDrawNow);
private:
	CEikConsoleScreen* iConsole;
	TInt iAllPrintable,iScrollLock,iIgnoreCursor,iHideCursor;
	TDesC* iSelBufPtr;
	TInt iSmallScreen;
	TInt iHighCursor;
	TBuf<256> iLine;
	TInt iCursorPos;
};

class CCollectS60AppUi
: public CAknAppUi
//, public IRILListener
//, public ILBSListener
//, public IWiFiListener
{
public:
	void ConstructL ();
	CCollectS60AppUi ();
	virtual ~CCollectS60AppUi ();
protected:
//	void OnRILZoneEntered(const ZoneInfo& zi);
//	void OnRILZoneLeft(const ZoneInfo& zi);
//	void OnRILSignalStrength(int ss,int bars);
//	void OnLBSPosition(float,float);
//	void OnWIFIAccessPointDetected(const WiFiInfo&);
//	void OnWIFIAccessPointUpdate(const WiFiInfo&);
//	void OnWIFIAccessPointLost(const WiFiInfo&);
private:
	void HandleCommandL (TInt aCommand);
	void HandleStatusPaneSizeChange ();
private:
	CConsoleControl*	m_pConsole;
	CDataCollector*		m_pDataCollector;
	//
//	CRIL*				m_pRIL;
//	CLBSActive*			m_pLBSActive;
//	CWiFiActive*		m_pWIFIActive;
};

#endif // __COLLECTS60APPUI_h__
// End of File
