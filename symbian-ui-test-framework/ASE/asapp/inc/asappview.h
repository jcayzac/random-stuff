#if !defined(ASAPPVIEW_H_INCLUDED)
#define ASAPPVIEW_H_INCLUDED

#include <coecntrl.h>
#include "AseEngine.h"

class CAseEngine2;
class CEikEdwin;

class CAsappView : public CCoeControl, public MAseObserver
	{
public:
	static CAsappView* NewL(const TRect& aRect);
	~CAsappView();
	void HandleCommandL(TInt aCommand);

private:
	CAsappView();
	void ConstructL(const TRect& aRect);
	void TryAutoBootL();
	void DoOpenL();
	void DoRunL();
	void LoadSourceFileL(const TFileName& aFileName);
		
private:
	// From CCoeControl
	TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType);
	CCoeControl* ComponentControl(TInt aIndex) const;
	void FocusChanged(TDrawNow aDrawNow);
	void SizeChanged();
	TInt CountComponentControls() const;
	void Draw(const TRect& aRect) const;
	
private:
	// From MAseObserver
	void HandleScriptBeginL();
	void HandleScriptStopL();
	void HandleErrorL();
	
private:
	CAseEngine2*	iEngine;
	CEikEdwin*		iEdwin;
	TBool			iIsRunning;
	TRect			iActivityIndicatorRect;
	};

#endif
