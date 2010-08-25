/*
 ============================================================================
 Name		: CollectS60AppUi.cpp
 Author	  : Mappy
 Copyright   : Confidential
 Description : CCollectS60AppUi implementation
 ============================================================================
 */

// INCLUDE FILES
#include <avkon.hrh>
#include <aknmessagequerydialog.h>
#include <aknnotewrappers.h>
#include <stringloader.h>
#include <f32file.h>
#include <s32file.h>
#include <hlplch.h>
#include <e32des16.h>
#include <e32property.h>
#include <w32std.h>
#include <eikconso.h>

#include <CollectS60.rsg>
#include "CollectS60.hrh"
#include "CollectS60.pan"
#include "CollectS60Application.h"
#include "CollectS60AppUi.h"

// =============================================================================

CConsoleControl::CConsoleControl() {
	iConsole=0;
	iSelBufPtr=0;
	
}
CConsoleControl::~CConsoleControl() {
	if (iSelBufPtr) {
		delete iSelBufPtr;
		iSelBufPtr=0;
	}
	if (iConsole) {
		delete iConsole;
		iConsole=0;
	}
}
void CConsoleControl::ConstructL() {
	CreateWindowL();
	Window().SetShadowDisabled(ETrue);
	Window().SetBackgroundColor(KRgbGray);
	SetBlank();
	iConsole=new(ELeave) CEikConsoleScreen;
	iConsole->ConstructL(_L("CollectS60"),0);
	iConsole->SetHistorySizeL(10,10);
	iLine.Zero();
	// Change font so that's the normal Eikon font, but twice as small
	TFontSpec font(CEikonEnv::Static()->NormalFont()->FontSpecInTwips());
	font.iHeight=font.iHeight>>1;
	iConsole->SetFontL(font);
}
TKeyResponse CConsoleControl::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType) {
	return EKeyWasConsumed;
}
void CConsoleControl::HandleCommandL(TInt aCommand) { }
void CConsoleControl::ActivateL() {
	CCoeControl::ActivateL();
	iConsole->SetKeepCursorInSight(TRUE);
	iConsole->SetAtt(4,15);
	iConsole->Printf(_L("Collect for S60\r\n"));
	iConsole->FlushChars();
	iConsole->DrawCursor();
	iConsole->SetAtt(ATT_NORMAL);
	iConsole->DrawCursor();
}
void CConsoleControl::FocusChanged(TDrawNow aDrawNow) {
	iConsole->ConsoleControl()->SetFocus(IsFocused(), aDrawNow);
}

// =============================================================================

void CCollectS60AppUi::ConstructL () {
	BaseConstructL (CAknAppUi::EAknEnableSkin);
	// Create console
	m_pConsole = new (ELeave) CConsoleControl;
	m_pConsole->ConstructL();
	m_pConsole->SetRect(ClientRect());
	m_pConsole->Screen()->ConsoleControl()->SetRect(ClientRect());
	m_pConsole->ActivateL();
	// Start
	m_pDataCollector=CDataCollector::NewL();
	m_pDataCollector->Start();
}

CCollectS60AppUi::CCollectS60AppUi () {
	m_pConsole=0;
	m_pDataCollector=0;
}

CCollectS60AppUi::~CCollectS60AppUi () {
	if (m_pDataCollector) {
		delete m_pDataCollector;
		m_pDataCollector=0;
	}
	if (m_pConsole) {
		delete m_pConsole;
		m_pConsole=0;
	}
}

void CCollectS60AppUi::HandleCommandL (TInt aCommand) {
	switch (aCommand) {
	case EEikCmdExit:
	case EAknSoftkeyExit:
		m_pDataCollector->Stop();
		Exit ();
		break;
	case EStart:
		//m_pDataCollector->SetCurrentPosition(2.33333333333f,48.888888888f);
		break;
	default: break;
	}
}

void CCollectS60AppUi::HandleStatusPaneSizeChange () {
	if (m_pConsole) {
		m_pConsole->SetRect(ClientRect());
		if (m_pConsole->Screen())
			if (m_pConsole->Screen()->ConsoleControl())
				m_pConsole->Screen()->ConsoleControl()->SetRect(ClientRect());
	}
}

// End of File
