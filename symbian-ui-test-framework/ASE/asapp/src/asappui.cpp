#include "asappui.h"
#include "asappview.h"

//#include <asapp.rsg>

#include <eikenv.h>

CAsappUi::CAsappUi()
	{}

CAsappUi::~CAsappUi()
	{
	if(iView)
		RemoveFromStack(iView);
	delete iView;
	}

void CAsappUi::HandleForegroundEventL(TBool aForeground)
	{
	CEikAppUi::HandleForegroundEventL(aForeground);
	}

void CAsappUi::ConstructL()
	{
    BaseConstructL();
	iView = CAsappView::NewL(ClientRect());
	AddToStackL(iView);
//	RWindowGroup& groupWin=iEikonEnv->RootWin();
//	groupWin.CaptureKey(EKeyEscape, EModifierLeftShift, EModifierLeftShift);
//	groupWin.CaptureKey(CTRL('p'), EModifierLeftShift, EModifierLeftShift);
	}

void CAsappUi::HandleCommandL(TInt aCommand)
	{
	iView->HandleCommandL(aCommand);
	if(aCommand==EEikCmdExit)
		Exit();
	}


