#include "asappview.h"
#include "aseengine.h"
#include "asapp.hrh"

#include <eikenv.h>
#include <eikedwin.h>
#include <eikcfdlg.h>
#include <eikfile.rsg>
#include <sys/reent.h>
#include "asapp.rsg"

_LIT(KMutexName,		"ASEVIEW-1");
const TInt KStatusHeight = 20;

//static
CAsappView* CAsappView::NewL(const TRect& aRect)
	{
	CAsappView* self = new(ELeave) CAsappView;
	CleanupStack::PushL(self);
	self->ConstructL(aRect);
	CleanupStack::Pop(self);
	return self;
	}

CAsappView::CAsappView()
	{
	}

CAsappView::~CAsappView()
	{
	delete iEngine;
	delete iEdwin;
	// mandatory. prevents memory leaks.
	CloseSTDLIB();
	}

void CAsappView::HandleCommandL(TInt aCommand)
	{
	switch (aCommand)
		{
	case EAseCmdOpenScript:
		DoOpenL();
		break;
	case EAseCmdRunScript:
		DoRunL();
		break;
	case EAseCmdStopScript:
		if (iEngine)
			iEngine->Break();
		break;
	default:
		break;
		}
	}

void CAsappView::ConstructL(const TRect& aRect)
	{
	CreateWindowL();

	iEngine = CAseEngine2::NewL();
	iEngine->SetObserver(this);

	TRect edwinRect(TSize(aRect.Size()));
	edwinRect.iBr -= TPoint(0,KStatusHeight);

	iEdwin = new(ELeave) CEikEdwin();
	iEdwin->ConstructL(CEikEdwin::ENoWrap|CEikEdwin::ENoAutoSelection);
	iEdwin->SetContainerWindowL(*this);
	iEdwin->CreateTextViewL();
	iEdwin->CreateScrollBarFrameL()->SetScrollBarVisibilityL(CEikScrollBarFrame::EAuto,CEikScrollBarFrame::EAuto);

	ActivateL();
	SetRect(aRect);
	iEdwin->SetRect(edwinRect);
	iEdwin->MakeVisible(ETrue);
	iEdwin->SetFocusing(ETrue);
	iEdwin->SetFocus(ETrue);
	
	TRect statusRect(iSize);
	statusRect.iTl += TPoint(0,iSize.iHeight - KStatusHeight);
	const TInt bulletSize = KStatusHeight-4;
	iActivityIndicatorRect = statusRect;
	iActivityIndicatorRect.iTl += TPoint(16, 2);
	iActivityIndicatorRect.SetSize(TSize(bulletSize,bulletSize));

	TryAutoBootL();
	}


void CAsappView::LoadSourceFileL(const TFileName& aFileName)
	{
	iEdwin->SetTextL(&KNullDesC);
	iEdwin->InsertFromTextFileL(aFileName, CPlainText::EOrganiseByParagraph);
	DrawNow();
	}


void CAsappView::DoOpenL()
	{
	TFileName fileName(_L("c:\\*.ase"));
	CEikFileOpenDialog* dialog = new(ELeave) CEikFileOpenDialog(&fileName, R_ASE_OPEN_FILE_DIALOG);
	dialog->SetShowSystem(ETrue);
	_LIT(KExtension, ".ase");
	dialog->SetRequiredExtension(&KExtension);
	if (dialog->ExecuteLD(R_EIK_DIALOG_FILE_OPEN))
		{
		LoadSourceFileL(fileName);
		DrawNow();
		}
	}


void CAsappView::DoRunL()
	{
	if (iEdwin->TextLength() > 0)
		{
		HBufC* buf = iEdwin->GetTextInHBufL();
		CleanupStack::PushL(buf);
		iEngine->RunScriptL(*buf, CAseEngine2::EScriptSource);
		CleanupStack::PopAndDestroy(buf);
		}
	}


void CAsappView::TryAutoBootL()
	{
	TRAPD(error, iEngine->AutoBootL());
	if (error!=KErrNone && error!=KErrNotFound)
		User::Leave(error);
	}

// From CCoeControl
TKeyResponse CAsappView::OfferKeyEventL(const TKeyEvent& aKeyEvent,TEventCode aType)
	{
	return iEdwin->OfferKeyEventL(aKeyEvent, aType);
	}


void CAsappView::FocusChanged(TDrawNow aDrawNow)
	{
	iEdwin->SetFocus(IsFocused(), aDrawNow);
	}


void CAsappView::SizeChanged()
	{
	iEdwin->SetRect(Rect());
	}


void CAsappView::Draw(const TRect& aRect) const
	{
	CEikonEnv* env = CEikonEnv::Static();

	CWindowGc& gc = SystemGc();

	//
	TRect statusRect(iSize);
	statusRect.iTl += TPoint(0,iSize.iHeight - KStatusHeight);
	if (statusRect.Intersects(aRect))
		{
		gc.SetBrushStyle(CGraphicsContext::ESolidBrush);
		gc.SetBrushColor(env->Color(EColorControlBackground));
		gc.SetPenColor(env->Color(EColorControlSurroundBackground));
		gc.SetPenStyle(CGraphicsContext::ESolidPen);
		gc.DrawRect(statusRect);

		gc.SetPenColor(KRgbBlack);
		if (iIsRunning)
			{
			gc.SetBrushColor(KRgbGreen);
			}
		else
			{
			gc.SetBrushColor(KRgbRed);
			}
		gc.DrawEllipse(iActivityIndicatorRect);
		}
	}


TInt CAsappView::CountComponentControls() const
	{
	return 1;
	}


CCoeControl* CAsappView::ComponentControl(TInt /*aIndex*/) const
	{
	return iEdwin;
	}

// From MAseObserver
void CAsappView::HandleScriptBeginL()
	{
	iIsRunning = ETrue;
	Window().Invalidate(iActivityIndicatorRect);
	DrawDeferred();
	}

void CAsappView::HandleScriptStopL()
	{
	iIsRunning = EFalse;
	Window().Invalidate(iActivityIndicatorRect);
	DrawDeferred();
	}

void CAsappView::HandleErrorL()
	{
	}
