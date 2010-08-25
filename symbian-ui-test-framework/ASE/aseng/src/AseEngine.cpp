#include <bautils.h>
#include <stringLoader.h>
#include <ImageConversion.h>
#include <apgcli.h>
#include <apgtask.h>
#include <eikapp.h>
#include <eikappui.h>
#include <gdi.h>

#if defined(PLATFORM_S90)
#include <SysStartup.h>
#endif

#include "AseEngine.h"
#include "AseEngineExtra.h"
#include "AseCommands.h"
#include <Aseng.rsg>

#if defined(__WINS__) && !defined(__MWERKS__)
#pragma warning(push, 1)
#endif
#include "skTreeNode.h"
#include "skInterpreter.h"
#include "skInputSource.h"
#include "skRValueArray.h"
#include "skScriptError.h"
#if defined(__WINS__) && !defined(__MWERKS__)
#pragma warning(pop)
#endif

#if defined(PLATFORM_S80)
// header not available for LafEnv
class LafEnv
	{
	public:
		IMPORT_C static void CreateSystemFontsL(CCoeEnv& aEnv,CArrayPtr<CFont>& aFonts);
	};
#endif

// For CKON only
#if defined(PLATFORM_S90) || defined(PLATFORM_S80)

const TInt KLafFontUid[] = {
TCknLogicalFont::EAppTitle,
TCknLogicalFont::EViewSelectorFont,
TCknLogicalFont::ECommandButtonFontNormal, 
TCknLogicalFont::ECommandButtonFontSmall, 
TCknLogicalFont::ECBBClock,
TCknLogicalFont::EPlainTextEditorFont,
TCknLogicalFont::ERichTextEditorFont,
TCknLogicalFont::EViewLevel1ZoomDefaultFont, 
TCknLogicalFont::EViewLevel2ZoomDefaultFont, 
TCknLogicalFont::EViewLevel3ZoomDefaultFont,
TCknLogicalFont::EViewLevel4ZoomDefaultFont,
TCknLogicalFont::EViewLevel1ZoomSmallFont,
TCknLogicalFont::EViewLevel2ZoomSmallFont,
TCknLogicalFont::EViewLevel3ZoomSmallFont,
TCknLogicalFont::EViewLevel4ZoomSmallFont,
TCknLogicalFont::EViewLevel1ZoomBigFont,
TCknLogicalFont::EViewLevel2ZoomBigFont,
TCknLogicalFont::EViewLevel3ZoomBigFont,
TCknLogicalFont::EViewLevel4ZoomBigFont, 
TCknLogicalFont::EDialogTitleFont,
TCknLogicalFont::EDialogTabFont, 
TCknLogicalFont::EDialogCaptionFont,
TCknLogicalFont::EDialogTrailerFont,
TCknLogicalFont::EDialogLabelFont, 
TCknLogicalFont::EDialogButtonFont,
TCknLogicalFont::EDialogSubTitleFont,
TCknLogicalFont::EMenuItemFont,
TCknLogicalFont::EMenuItemFontSecondLevel,
TCknLogicalFont::EMenuItemFontThirdLevel,
TCknLogicalFont::EMenuShortcut,
TCknLogicalFont::EBannerFont,
TCknLogicalFont::EProgressBarFont,
TCknLogicalFont::ECalendarMonthNameFont,
TCknLogicalFont::ECalendarDayInitialFont,
TCknLogicalFont::ECalendarWeekNumberFont,
TCknLogicalFont::ECalendarDayNumberFont,
TCknLogicalFont::EGridFieldFont,
TCknLogicalFont::EGridLabelFont,
TCknLogicalFont::ECharmapFont,
TCknLogicalFont::ETimePicker,
TCknLogicalFont::ESymbolFont,
TCknLogicalFont::ENoteFont,
TCknLogicalFont::EViewButtonFont,
TCknLogicalFont::EMonospaceFont,
TCknLogicalFont::EBigDigitalAmPmFont,
TCknLogicalFont::EBigDigitalNumberFont,
TCknLogicalFont::ESmallDigitalAmPmFont,
TCknLogicalFont::ESmallDigitalNumberFont,
TCknLogicalFont::EBigAnalogFont,
TCknLogicalFont::ESmallAnalogFont,
TCknLogicalFont::ECalendarAppointmentDayInitialFont,
TCknLogicalFont::EListItemRemovedDefault,
TCknLogicalFont::EListItemRemovedSmall,
TCknLogicalFont::EListItemRemovedBig,
TCknLogicalFont::EListItemWaitingDefault,
TCknLogicalFont::EListItemWaitingSmall,
TCknLogicalFont::EListItemWaitingBig,
TCknLogicalFont::ECalculatorBasicCalculationLine,
TCknLogicalFont::ECallHeaderButton,
TCknLogicalFont::ETimePickerFormatFont,
TCknLogicalFont::EVirtualKeyboardZoomDefault,
TCknLogicalFont::EVirtualKeyboardZoomSmall,
TCknLogicalFont::EVirtualKeyboardZoomBig,
TCknLogicalFont::EPartialModeDateTime,
TCknLogicalFont::EPartialModeProfileText,
TCknLogicalFont::EMarkedItemZoomDefault,
TCknLogicalFont::EMarkedItemZoomSmallFont,
TCknLogicalFont::EMarkedItemZoomBigFont,
TCknLogicalFont::ECalculatorFunctionalIndicator,
TCknLogicalFont::ECalculatorTillRollCalculationLine,
TCknLogicalFont::ECallStatusText,
TCknLogicalFont::EDialerButtonNumber,
TCknLogicalFont::EDialerButtonLetter,
TCknLogicalFont::EDialerInputZoomBig,
TCknLogicalFont::EDialerInputZoomMiddleSize,
TCknLogicalFont::EDialerInputZoomSmall,
TCknLogicalFont::EDialerIndicator,
TCknLogicalFont::EDialerOperatorName,
TCknLogicalFont::EWebView,
TCknLogicalFont::EToolbar,
TCknLogicalFont::EPlainTextEditorBold,
TCknLogicalFont::EViewLevel1ZoomDefaultBold,
TCknLogicalFont::EViewLevel1ZoomSmallBold,
TCknLogicalFont::EViewLevel1ZoomBigBold,
TCknLogicalFont::ECalendarDayEntry,
TCknLogicalFont::EWeekdayPicker,
TCknLogicalFont::ESeekBarFont,
TCknLogicalFont::ERadioStationNameFont,
TCknLogicalFont::ERadioFrequencyFont,
TCknLogicalFont::EPeninputButtonDefault,
TCknLogicalFont::EPeninputButtonSmall,
TCknLogicalFont::EPeninputButton,
TCknLogicalFont::EPeninputLangDefault,
TCknLogicalFont::EPeninputLangSmall,
TCknLogicalFont::EPeninputLangBig,
TCknLogicalFont::EDialogMarked,
TCknLogicalFont::EMMSTextEditorTitleZoomDefault,
TCknLogicalFont::EMMSTextEditorTitleZoomSmall,
TCknLogicalFont::EMMSTextEditorTitleZoomBig,
TCknLogicalFont::EDialogHeader,
TCknLogicalFont::EBrowserProgressBannerFont,
TCknLogicalFont::EMainTitleFont,
TCknLogicalFont::ENoteTitleFont,
TCknLogicalFont::EAnnotationFont,
TCknLogicalFont::EListItemDescriptive,
TCknLogicalFont::EIconCaptionFont
};

#endif

const TInt KAseUid				= 0xDEADA42E;
const TInt KAseIniBootStreamUid	= 0xB00000B0; 

_LIT(KAseInternal,	"Internal");

const TInt KWatcherArrayGranularity = 4;

const TInt KKeyPressTime = 20000;
_LIT(KLitShift,		"shift");
_LIT(KLitCtrl,		"ctrl");
_LIT(KLitFn,		"fn");
_LIT(KLitSpace,		" ");

const TInt KPenDelay = 100000;

// special keys
_LIT(KLitBackspaceWord,		"backspace");
_LIT(KLitDelWord,			"del");
_LIT(KLitDeskWord,			"desk");
_LIT(KLitDownWord,			"down");
_LIT(KLitEndWord,			"end");
_LIT(KLitEnterWord,			"enter");
_LIT(KLitEscWord,			"esc");
_LIT(KLitHomeWord,			"home");
_LIT(KLitInsertWord,		"insert");
_LIT(KLitLeftWord,			"left");
_LIT(KLitMenuWord,			"menu");
_LIT(KLitPageUpWord,		"pageup");
_LIT(KLitPageDownWord,		"pagedown");
_LIT(KLitPauseWord,			"pause");
_LIT(KLitPrintWord,			"print");
_LIT(KLitRightWord,			"right");
_LIT(KLitRockerWord,		"rocker");
_LIT(KLitSpaceWord,			"space");
_LIT(KLitTabWord,			"tab");
_LIT(KLitUpWord,			"up");
_LIT(KLitCBA1,				"cba1");
_LIT(KLitCBA2,				"cba2");
_LIT(KLitCBA3,				"cba3");
_LIT(KLitCBA4,				"cba4");

// coordinates
_LIT(KLitPointSet,			"set");
_LIT(KLitPointSetX,			"setx");
_LIT(KLitPointSetY,			"sety");
_LIT(KLitX,					"x");
_LIT(KLitY,					"y");

///////////////////////////////////////////////////////////////////////
class TKeyConvLookup
	{
public:
	TInt iKeyCode;
	TInt iScanCode;
	TInt iModifiers;
	};

// This converts Unicode codes to key sequences, eg % is Shift-5
// Will need different tables for difference target hardware
const TKeyConvLookup lookupTable[]=
	{
		{ EKeySpace,							EStdKeySpace,		0 },
		{ EKeyTab,								EStdKeyTab,			0 },
		{ EKeyEnter,							EStdKeyEnter,		0 },
		{ CEditableText::EParagraphDelimiter,	EStdKeyEnter,		0 },
		{ CEditableText::ENonBreakingSpace,		EStdKeySpace,		EModifierShift },
		{ EKeyBackspace,						EStdKeyBackspace,	0 },
		{ EKeyDelete,							EStdKeyDelete,		0 },

		{ '!', '1', EModifierShift },
		{ '"', '2', EModifierShift }, // " hack codewarrior to have comments working
		{ 163, '3', EModifierShift }, // £ sign
		{ '$', '4', EModifierShift },
		{ '%', '5', EModifierShift },
		{ '^', '6', EModifierShift },
		{ '&', '7', EModifierShift },
		{ '*', '8', EModifierShift },
		{ '(', '9', EModifierShift },
		{ ')', '0', EModifierShift },

		{ ',',	EStdKeyComma,				0 },
		{ '.',	EStdKeyFullStop,			0 },
		{ '/',	EStdKeyForwardSlash,		0 },
		{ '\\',	EStdKeyBackSlash,			0 },
		{ ';',	EStdKeySemiColon,			0 },
		{ '\'',	EStdKeySingleQuote,			0 },
		{ '#',	EStdKeyHash,				0 },
		{ '[',	EStdKeySquareBracketLeft,	0 },
		{ ']',	EStdKeySquareBracketRight,	0 },
		{ '-',	EStdKeyMinus,				0 },
		{ '=',	EStdKeyEquals,				0 },
		{ '_',	EStdKeyMinus,				EModifierShift },
		{ '+',	EStdKeyEquals,				EModifierShift },
		{ ':',	EStdKeySemiColon,			EModifierShift },
		{ '@',	EStdKeySingleQuote,			EModifierShift },
		{ '~',	EStdKeyHash,				EModifierShift },
		{ '<',	EStdKeyComma,				EModifierShift },
		{ '>',	EStdKeyFullStop,			EModifierShift },
		{ '?',	EStdKeyForwardSlash,		EModifierShift },
		{ '{',	EStdKeySquareBracketLeft,	EModifierShift },
		{ '}',	EStdKeySquareBracketRight,	EModifierShift },
		{ '|',	EStdKeyBackSlash,			EModifierShift },
		{ -1,	-1,							-1 },
	};

/**
Convert a Unicode keycode into a standard key plus modifiers.
E.g., ^ is shift-6, + is shift-=.
*/
static TInt ConvertToStdKey(TInt aKey,TInt& aModifiers)
	{
	TInt stdKey = aKey;
	aModifiers = 0;
	if (aKey >= 'A' && aKey <= 'Z')
		aModifiers = EModifierShift;
	else if (aKey >= 'a' && aKey <= 'z')
		stdKey = aKey - 32;
	else
		{
		TInt count=0;
		TInt key;
		do
			{
			if((key=lookupTable[count].iKeyCode)==aKey)
				{
				aModifiers=lookupTable[count].iModifiers;
				stdKey=lookupTable[count].iScanCode;
				break;
				}
			count++;
			} while(key!=-1);
		}
	return stdKey;
	}


///////////////////////////////////////////////////////////////////////


// CAseRunnable


skRValue& CAseRunnable::ItemL(const TDesC& aName)
	{
	for (int i=0; i<iValueLookup.Count(); ++i)
		{
		CAseItem* member = iValueLookup[i];
		if (aName==member->Name())
			{
			return member->Value();
			}
		}
	// not found, create
	CAseItem* member = CAseItem::NewL(aName);
	CleanupStack::PushL(member);
	iValueLookup.AppendL(member);
	CleanupStack::Pop(member);
	return member->Value();
	}

skRValue& CAseRunnable::ItemL(const skString& aName)
	{
	return ItemL(aName.ptr());
	}

bool CAseRunnable::WaitForStatement(TInt aStatement, skRValueArray& aArguments, skRValue& aResult)
	{
	TStatement statement(iMethodLookup[aStatement], &aArguments, &aResult);
	return iLooper->WaitForStatement(statement);
	}

void CAseRunnable::SetNameL(const TDesC& aName)
	{
	if (iTypeName)
		delete iTypeName;
	iTypeName = 0;
	iTypeName = aName.AllocL();
	}

void CAseRunnable::BaseConstructL()
	{
//	skTreeNode* treeNode = new(ELeave) skTreeNode();
//	CleanupStack::PushL(treeNode);
//	setNode(_L("CAseRunnable"), treeNode, true);
//	CleanupStack::Pop(treeNode);
	}

// From skTreeNodeObject
bool CAseRunnable::method(const skString& aMethodName, skRValueArray& aArguments, skRValue& aResult, skExecutableContext& aContext)
	{
	if (iLooper->Stopped())
		{
		return false;
		}

	// poorman's rtti
	if (aMethodName == KTypeNameCmd)
		{
		skString skname;
		if (iTypeName) skname = *iTypeName; // leaves
		aResult = skname;
		return true;
		}

	bool res = false;
	
	for (int i=0; i<iMethodLookup.Count(); ++i)
		{
		CAseMethod& meth = *iMethodLookup[i];
		if (aMethodName == meth.Name())
			{
			TInt error = KErrNone;
			if (meth.Flags() & CAseMethod::ESessionSafe)
				{
				TStatement statement(&meth, &aArguments, &aResult);
				TRAP(error, statement.ProcessL());
				}
			else
				{
				error = WaitForStatement(i, aArguments, aResult);
				}
			if (error!=KErrNone)
				{
				// TODO: report the error
				}
			res = true;
			break;
			}
		}
	if (!res)
		{
		res = skTreeNodeObject::method(aMethodName, aArguments, aResult, aContext);
		}
	return res;
	}

bool CAseRunnable::getValue (const skString& aFieldName, const skString& /*aAttribute*/, skRValue& aValue)
	{
	for (int i=0; i<iValueLookup.Count(); ++i)
		{
		CAseItem* member = iValueLookup[i];
		if (aFieldName==member->Name())
			{
			aValue = member->Value();
			return true;
			}
		}
	return false;
//	return skTreeNodeObject::getValue(aFieldName, aAttribute, aValue);
	}

bool CAseRunnable::setValue (const skString& aFieldName, const skString& /*aAttribute*/, const skRValue& aValue)
	{
	for (int i=0; i<iValueLookup.Count(); ++i)
		{
		CAseItem* member = iValueLookup[i];
		if (aFieldName==member->Name())
			{
			member->Value() = aValue;
			return true;
			}
		}
	ItemL(aFieldName) = aValue;
	return true;
//	return skTreeNodeObject::setValue(aFieldName, aAttribute, aValue);
	}



// CAsePoint

// static
CAsePoint* CAsePoint::NewL(MAseLooper* aLooper)
	{
	CAsePoint* self = new(ELeave) CAsePoint(aLooper);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

TInt	CAsePoint::X() const
	{
	CAsePoint* volatileThis = const_cast<CAsePoint*>(this);
	skRValue& val = volatileThis->ItemL(KLitX); // doesn't leave
	return val.intValue();
	}

TInt	CAsePoint::Y() const
	{
	CAsePoint* volatileThis = const_cast<CAsePoint*>(this);
	skRValue& val = volatileThis->ItemL(KLitY); // doesn't leave
	return val.intValue();
	}

void	CAsePoint::SetL(const TPoint& aPoint)
	{
	SetXL(aPoint.iX);
	SetYL(aPoint.iY);
	}

void	CAsePoint::SetXL(TInt aX)
	{
	ItemL(KLitX) = aX;
	}

void	CAsePoint::SetYL(TInt aY)
	{
	ItemL(KLitY) = aY;
	}

CAsePoint::CAsePoint(MAseLooper* aLooper): CAseRunnable(aLooper)
	{
	}

void CAsePoint::ConstructL()
	{
	SetNameL(KPointObj);
//	SetXL(0);
//	SetYL(0);
	
	// methods
//	ExportMethodL(KLitPointSet,		&CAsePoint::cmdSetL);
//	ExportMethodL(KLitPointSetX,	&CAsePoint::cmdSetXL);
//	ExportMethodL(KLitPointSetY,	&CAsePoint::cmdSetYL);
	
	// variables
	ItemL(KLitX) = 0;
	ItemL(KLitY) = 0;
	}

void CAsePoint::cmdSetXL(skRValueArray& aArguments, skRValue& /*aResult*/)
	{
	if (aArguments.entries() > 0)
		{
//		SetXL(aArguments[0].intValue());
		ItemL(KLitX) = aArguments[0].intValue();
		return;
		}
	ItemL(KLitX) = 0;
	}

void CAsePoint::cmdSetYL(skRValueArray& aArguments, skRValue& /*aResult*/)
	{
	if (aArguments.entries() > 0)
		{
//		SetYL(aArguments[0].intValue());
		ItemL(KLitY) = aArguments[0].intValue();
		return;
		}
	ItemL(KLitY) = 0;
	}

void CAsePoint::cmdSetL(skRValueArray& aArguments, skRValue& /*aResult*/)
	{
	if (aArguments.entries() > 0)
		{
		CAsePoint* arg = static_cast<CAsePoint*>(aArguments[0].obj());
		if (arg)
			{
//			SetXL(arg->X());
//			SetYL(arg->Y());
			ItemL(KLitX) = arg->X();
			ItemL(KLitY) = arg->Y();
			return;
			}
		}
	ItemL(KLitX) = 0;
	ItemL(KLitY) = 0;
	}



// CAseEngine2



CAseEngine2::CAseEngine2()
: CAseRunnable(this)
, iWatcherArray(KWatcherArrayGranularity)
	{
	}

// static
EXPORT_C CAseEngine2* CAseEngine2::NewL()
	{
	CAseEngine2* self = new(ELeave) CAseEngine2;
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

EXPORT_C CAseEngine2::~CAseEngine2()
	{
	CloseApps();
	delete iPeriodic;
	iMutex.Close();
	if (iScriptThreadLaunched)
		{
		iScriptThread.Kill(0);
		iScriptThread.Close();
		}
	delete iFileName;
	delete iPrivateMethodsArgs;
	delete iExtra;
	if (iFontArray)
		{
		iFontArray->Reset();
		delete iFontArray;
		}
	}

void CAseEngine2::ConstructL()
	{
	iExtra = new(ELeave) CAseEngine2Extra;

	User::LeaveIfError(iMutex.CreateLocal());
	iPeriodic = CPeriodic::NewL(CActive::EPriorityLow);
	iPeriodic->Start(0, 50000, TCallBack(PeriodicBeat, this));
	iPrivateMethodsArgs = new(ELeave) skRValueArray;
	iPrivateMethodsArgs->growTo(5);

	ExportMethodL(KPrivateTraceCmd, &CAseEngine2::cmdTraceL);

	InitFontsL();
	ReadIniL();

	// methods
	ExportMethodL(KBreakCmd,			&CAseEngine2::cmdBreakL);
	ExportMethodL(KClickCmd,			&CAseEngine2::cmdClickL);
	ExportMethodL(KCloseCmd,			&CAseEngine2::cmdCloseL);
	ExportMethodL(KCreateCmd,			&CAseEngine2::cmdCreateL);
	ExportMethodL(KDragCmd,				&CAseEngine2::cmdDragL);
	ExportMethodL(KFindTextCmd,			&CAseEngine2::cmdFindTextL);
	ExportMethodL(KHintCmd,				&CAseEngine2::cmdHintL);
	ExportMethodL(KKeyCmd,				&CAseEngine2::cmdKeyL);
	ExportMethodL(KMakeDirCmd,			&CAseEngine2::cmdMakeDirL);
	ExportMethodL(KOpenCmd,				&CAseEngine2::cmdOpenL);
	ExportMethodL(KRebootCmd,			&CAseEngine2::cmdRebootL);
	ExportMethodL(KInfoPrintCmd,		&CAseEngine2::cmdTraceL);
	ExportMethodL(KScreenShotCmd,		&CAseEngine2::cmdScreenShotL);
	ExportMethodL(KScreenShotBaseCmd,	&CAseEngine2::cmdScreenShotBaseL);
	ExportMethodL(KSleepCmd,			&CAseEngine2::cmdSleepL,		CAseMethod::ESessionSafe);
	ExportMethodL(KTextCmd,				&CAseEngine2::cmdTextL);
	ExportMethodL(KValidDriveCmd,		&CAseEngine2::cmdValidDriveL);

	// variables

	// hints
	ItemL(KFindTextFirstOnlyHint) = 0;
	}

void CAseEngine2::InitFontsL()
	{
	if (iFontArray)
		iFontArray->Reset();
	else
		iFontArray = new(ELeave) CArrayPtrFlat<const CFont>(5);

	CEikonEnv& env = *CEikonEnv::Static();

	// Add LAF fonts
#if defined(PLATFORM_S90) || defined(PLATFORM_S80)
	for(int ii=0; ii < sizeof(KLafFontUid)/sizeof(int); ++ii)
		{
		TCknLogicalFont logicalFont;
		logicalFont.SetCategory((TCknLogicalFont::TFontCategory)(KLafFontUid[ii]));
		const CFont* font = env.Font(logicalFont);		
		if (font)
			{
			bool add=true;
			for (int jj=0; jj< iFontArray->Count(); ++jj)
				{
				if ((*iFontArray)[jj]==font) add=false;
				}
			if (add)
				iFontArray->AppendL(font);
			}
		}
#else
//	CWsScreenDevice& device = *env.ScreenDevice();
//	int numTypefaces = device.NumTypefaces();
//	for(int i=0; i<numTypefaces; ++i)
//		{
//		TTypefaceSupport myTypefaceSupport;
//		env.TypefaceSupport(myTypefaceSupport,i);
//		for (int h=0;h<myTypefaceSupport.iNumHeights; ++h)
//			{
//			height = device.FontHeightInTwips(i,h);
//			TFontSpec spec;
//			spec.iTypeface = myTypefaceSupport.iTypeface;
//			spec.iHeight = height;
//			}
//		} 
#endif
	}

void CAseEngine2::ReadIniL()
	{
	TRAPD(err, iExtra->ReadConfigL());
	}

void CAseEngine2::WriteIniL()
	{
	// TODO
	}

//static
TInt CAseEngine2::ThreadMain(TAny* aPtr)
	{
	CTrapCleanup* cleanupStack = CTrapCleanup::New();
	CAseEngine2* engine = reinterpret_cast<CAseEngine2*>(aPtr);
	TRAPD(error, engine->DoRunScriptL());
	delete cleanupStack;
	return error;
	}


void CAseEngine2::DoRunScriptL()
	{
	Acquire();

	// input string
	skInputString* in = new(ELeave) skInputString(iSource);
	CleanupStack::PushL(in);
	
	// source reader
	skTreeNodeReader* reader = new(ELeave) skTreeNodeReader;
	CleanupStack::PushL(reader);
	
	// interpreter
	skInterpreter* interpreter = new(ELeave) skInterpreter;
	CleanupStack::PushL(interpreter);
	interpreter->setTraceCallback(this);
	
	// context
	skExecutableContext* context = new(ELeave) skExecutableContext(interpreter);
	CleanupStack::PushL(context);
	
	// root node
	skTreeNode* node = reader->read(in, *iFileName, *context);
	if (!node)
		{
//		trace(_L("Bad file type."));
		User::Leave(KErrArgument);
		}
	CleanupStack::PushL(node);
	
	// Arguments and return value
	skRValueArray* args = new(ELeave) skRValueArray;
	CleanupStack::PushL(args);
	skRValue* ret = new(ELeave) skRValue;
	CleanupStack::PushL(ret);
	
	// Release mutex before running the script!
	Release();
	
	// Run node
	setNode(*iFileName, node, false);
	ReadEntryPointL();

	iExtra->SetRunning(ETrue);
	method(iEntryPoint, *args, *ret, *context);
	
	// Catch error
	skScriptError& scriptError = context->getError(); 
	TInt errType = scriptError.getErrorCode();
	if (errType)
		{
		skString errString = scriptError.getException()->toString();
		TPtrC p = errString.ptr();
		User::InfoPrint(p);			
		}
	

	// Cleanup
	CleanupStack::PopAndDestroy(ret);
	CleanupStack::PopAndDestroy(args);
	CleanupStack::PopAndDestroy(node);
	CleanupStack::PopAndDestroy(context);
	CleanupStack::PopAndDestroy(interpreter);
	CleanupStack::PopAndDestroy(reader);
	CleanupStack::PopAndDestroy(in);

	if (iExtra->Observer())
		{
		Acquire();
		iExtra->Observer()->HandleScriptStopL();
		Release();
		}
	}

void CAseEngine2::ReadEntryPointL()
	{
	skString buf;
	TRAPD(error, iExtra->ReadValueL(KIniEntryPoint, buf));
	if (error==KErrNone)
		{
		iEntryPoint = buf;
		}
	else
		{
		iEntryPoint = KMain; // leaves
		}
	}

// static
TInt CAseEngine2::PeriodicBeat(TAny* aArg)
	{
	CAseEngine2* self = reinterpret_cast<CAseEngine2*>(aArg);
	if (self)
		{
		self->Acquire();
		TStatement& statement = self->PendingStatement();
		const skString& currentText = self->Text();
		bool ended = false;
		TInt errCode = KErrNone;

		if (currentText.length() > 0)
			{
			TRAP(errCode, self->SendNextCharInTextL());
			// TODO: report error
			ended = (currentText.length() == 0);
			}
		else if (!self->TryToSendPenEvent() && statement.Exists())
			{
			// execute statement
			TRAP(errCode, statement.ProcessL());
			if (currentText.length() == 0)
				ended = true;
			}

		if (ended)
			{
			statement.Done();
			if (errCode!=KErrNone)
				{
				User::InfoPrint(_L("ERROR!"));
				// TODO: report error
				}
			statement.SetStatus(errCode);
			}
		
		self->Release();
		return KErrNone;
		}
	return KErrGeneral;
	}

// static
void CAseEngine2::TranslatePathL(skString& aPath, TPathTranslationMode aMode)
	{
	// TODO: handle ROM:, MMC:, etc? (symbolic names)
	aPath.replaceInPlace('/', '\\'); // leaves

	if (aMode==EPTMAddSession && ((aPath.length() < 2) || (aPath.at(1) != ':')))
		{
		RFs& fs = CEikonEnv::Static()->FsSession();
		TFileName sessionPath;
		fs.SessionPath(sessionPath);
		if ((sessionPath[sessionPath.Length() - 1] == '\\') && (aPath.at(0) == '\\'))
			sessionPath.SetLength(sessionPath.Length() - 1);
		sessionPath.Append(aPath.ptr());
		aPath = sessionPath;
		}
	}

// static
void CAseEngine2::SendStdKey(TInt aKey,TBool aDown)
	{
	TRawEvent event;
	event.Set(aDown?TRawEvent::EKeyDown:TRawEvent::EKeyUp,aKey);
	CEikonEnv::Static()->WsSession().SimulateRawEvent(event);
	}

// static
void CAseEngine2::SendStdKey(TInt aKey)
	{
	SendStdKey(aKey,ETrue);
	User::After(KKeyPressTime);
	SendStdKey(aKey,EFalse);
	}

// static
void CAseEngine2::SendKeyL(const TDesC& aKey,TBool aDown)
	{
	TPtrC key(aKey);
	TInt modifiers=0;
	for (;;)
		{
		TInt locate = key.Locate('+');
		if (locate == KErrNotFound)
			break;
		TPtrC mod(key.Left(locate));
		if (mod.CompareF(KLitShift) == 0)
			modifiers |= EModifierShift;
		else if(mod.CompareF(KLitCtrl) == 0)
			modifiers |= EModifierCtrl;
		else if(mod.CompareF(KLitFn) == 0)
			modifiers |= EModifierFunc;
		else
			User::Leave(KErrArgument);
		key.Set(key.Mid(locate + 1));
		}
	
	// Allow 'space' as a synonym for an actual space to make it easier to avoid confusion.
	if (key.CompareF(KLitSpaceWord) == 0)
		key.Set(KLitSpace);

	TInt sendKey = key[0];
	if (key.Length() == 1)
		{
		TInt unused;
		sendKey = ConvertToStdKey(sendKey, unused);
		}
	else
		{
		if (key.CompareF(KLitBackspaceWord) == 0)
			{
			sendKey = EStdKeyBackspace;
			}
		else if (key.CompareF(KLitDelWord) == 0)
			{
			sendKey = EStdKeyDelete;
			}
		else if (key.CompareF(KLitDeskWord) == 0)
			{
			sendKey = EStdKeyApplication0;
			}
		else if (key.CompareF(KLitDownWord) == 0)
			{
			sendKey = EStdKeyDownArrow;
			}
		else if (key.CompareF(KLitEndWord) == 0)
			{
			sendKey = EStdKeyEnd;
			}
		else if (key.CompareF(KLitEnterWord) == 0)
			{
			sendKey = EStdKeyEnter;
			}
		else if (key.CompareF(KLitEscWord) == 0)
			{
			sendKey = EStdKeyEscape;
			}
		else if (key.CompareF(KLitHomeWord) == 0)
			{
			sendKey = EStdKeyHome;
			}
		else if (key.CompareF(KLitInsertWord) == 0)
			{
			sendKey = EStdKeyInsert;
			}
		else if (key.CompareF(KLitLeftWord) == 0)
			{
			sendKey = EStdKeyLeftArrow;
			}
		else if (key.CompareF(KLitMenuWord) == 0)
			{
			sendKey = EStdKeyMenu;
			}
		else if (key.CompareF(KLitPageUpWord) == 0)
			{
			sendKey = EStdKeyPageUp;
			}
		else if (key.CompareF(KLitPageDownWord) == 0)
			{
			sendKey = EStdKeyPageDown;
			}
		else if (key.CompareF(KLitPauseWord) == 0)
			{
			sendKey = EStdKeyPause;
			}
		else if (key.CompareF(KLitPrintWord) == 0)
			{
			sendKey = EStdKeyPrintScreen;
			}
		else if (key.CompareF(KLitRightWord) == 0)
			{
			sendKey = EStdKeyRightArrow;
			}
		else if (key.CompareF(KLitRockerWord) == 0)
			{
			sendKey = EStdKeyDevice7;
			}
		else if (key.CompareF(KLitTabWord) == 0)
			{
			sendKey = EStdKeyTab;
			}
		else if (key.CompareF(KLitUpWord) == 0)
			{
			sendKey = EStdKeyUpArrow;
			}
		else if (key.CompareF(KLitCBA1) == 0)
			{
			// TODO: this value is for s80, check this value for s60 & s90
			sendKey = EStdKeyDevice0;
			}
		else if (key.CompareF(KLitCBA2) == 0)
			{
			// TODO: this value is for s80, check this value for s60 & s90
			sendKey = EStdKeyDevice1;
			}
		else if (key.CompareF(KLitCBA3) == 0)
			{
			// TODO: this value is for s80, check this value for s60 & s90
			sendKey = EStdKeyDevice2;
			}
		else if (key.CompareF(KLitCBA4) == 0)
			{
			// TODO: this value is for s80, check this value for s60 & s90
			sendKey = EStdKeyDevice3;
			}
		else
			User::Leave(KErrArgument);
		}


	/*
	Shift needs to be done in a different sequence depending on
	whether down or up, otherwise the InsertSpecialChar
	dialog pops up (bug in the code that launches that dialog, it
	doesn't check carefully enough what modifers are down, etc.).
	*/
	if (aDown && (modifiers & EModifierShift))
		SendStdKey(EStdKeyLeftShift,aDown);
	if (modifiers & EModifierCtrl)
		SendStdKey(EStdKeyLeftCtrl,aDown);
	if (modifiers & EModifierFunc)
		SendStdKey(EStdKeyLeftFunc,aDown);
	if (!aDown && (modifiers & EModifierShift))
		SendStdKey(EStdKeyLeftShift,aDown);

	SendStdKey(sendKey, aDown);
	}

// static
void CAseEngine2::SendKeyL(const TDesC& aKey)
	{
	SendKeyL(aKey, ETrue);
	User::After(KKeyPressTime);
	SendKeyL(aKey, EFalse);
	}

// static
void CAseEngine2::SendCharL(TInt aCode)
	{
	TInt modifiers;
	
	// find out the required key combinations to send aCode, eg:
	// if aCode is % then we need to simulate Shift-5
	TInt stdKey=ConvertToStdKey(aCode,modifiers);
	
	// set modifier keys...could check current state and only set those that have changed
	// we could split these SendStdKeyL calls across several RunL's if necessary...add a SendKeySequence state?
	if(modifiers&EModifierCtrl)
		SendStdKey(EStdKeyLeftCtrl,ETrue);
	if(modifiers&EModifierShift)
		SendStdKey(EStdKeyLeftShift,ETrue);
	if(modifiers&EModifierFunc)
		SendStdKey(EStdKeyLeftFunc,ETrue);
	
	// send the key down/up event
	SendStdKey(stdKey);
	
	// reset modifier keys...could leave them and only unset on next key event?
	if(modifiers&EModifierCtrl)
		SendStdKey(EStdKeyLeftCtrl,EFalse);
	if(modifiers&EModifierShift)
		SendStdKey(EStdKeyLeftShift,EFalse);
	if(modifiers&EModifierFunc)
		SendStdKey(EStdKeyLeftFunc,EFalse);
	
	// allow time for multiple key presses.
	static const TInt KCloseTime = 50000; // 20th of a second
	User::After(KCloseTime);
	}

// static
void CAseEngine2::SendCTRLNumberCharL(TInt aCode)
	{
	TBuf<10> number;
	number.Format(_L("%d"), aCode);
	SendStdKey(EStdKeyLeftCtrl, ETrue);
	for (TInt i = 0; i < number.Length(); i++)
		{
		SendStdKey(number[i]);
		}
	SendStdKey(EStdKeyLeftCtrl, EFalse);

	// allow time for multiple key presses.
	static const TInt KCloseTime = 50000; // 20th of a second
	User::After(KCloseTime);
	}

void CAseEngine2::SendNextCharInTextL()
	{
	TInt code = iCurrentText.at(0);
	if (code)
		{
		if (code < 128)
			SendCharL(code);
		else
			SendCTRLNumberCharL(code);		
		}
		iCurrentText = iCurrentText.substr(1, iCurrentText.length()-1); // leaves
	}

const skString& CAseEngine2::Text() const
	{
	return iCurrentText;
	}

bool CAseEngine2::TryToSendPenEvent()
	{
	TRawEvent event;
	event.Set(TRawEvent::ENone);

	switch (iCurrentPenState.iNextState)
		{
		case SPenState::EPenDownBeforeClicking:
			event.Set(TRawEvent::EButton1Down, iCurrentPenState.iOrigin.iPoint.iX, iCurrentPenState.iOrigin.iPoint.iY);
			iCurrentPenState.iNextState = SPenState::EPenUp;
			break;
		case SPenState::EPenDownBeforeDragging:
			{
			TPoint p;
			bool cont = PopPoint(p);
			event.Set(TRawEvent::EButton1Down, p.iX,p.iY);
			iCurrentPenState.iNextState = cont?SPenState::EPenDrag:SPenState::EPenUp;
			}
			break;
		case SPenState::EPenDrag:
			{
			TPoint p;
			bool cont = PopPoint(p);
			event.Set(TRawEvent::EPointerMove, p.iX, p.iY);
			iCurrentPenState.iNextState = cont?SPenState::EPenDrag:SPenState::EPenUp;
			}
			break;
		case SPenState::EPenUp:
			event.Set(TRawEvent::EButton1Up, iCurrentPenState.iOrigin.iPoint.iX, iCurrentPenState.iOrigin.iPoint.iY);
			iCurrentPenState.iNextState = SPenState::ENone;
			break;
		case SPenState::ENone:
		default:
			break;
		}

	if (event.Type() == TRawEvent::ENone)
		return false;

	RWsSession& ws = CEikonEnv::Static()->WsSession();
	ws.SimulateRawEvent(event);
	User::After(KPenDelay);
	return true;
	}

void CAseEngine2::PushPointL(const TPoint& aPoint)
	{
	SPenState::SPoint* point = new(ELeave) SPenState::SPoint;
	CleanupStack::PushL(point);

	point->iPoint	= iCurrentPenState.iOrigin.iPoint;
	point->iNext	= iCurrentPenState.iOrigin.iNext;
	
	iCurrentPenState.iOrigin.iNext	= point;
	iCurrentPenState.iOrigin.iPoint	= aPoint;

	CleanupStack::Pop(point);
	}

bool CAseEngine2::PopPoint(TPoint& aPoint)
	{
	aPoint = iCurrentPenState.iOrigin.iPoint;
	// shift list
	bool remaining(false);
	SPenState::SPoint* point = &iCurrentPenState.iOrigin;
	while (point->iNext)
		{
		remaining = true;
		point->iPoint	= point->iNext->iPoint;
		point->iNext	= point->iNext->iNext;
		delete point->iNext;
		}
	return remaining;
	}

/** Does aText1 match aText2 for aBits, starting aShift bits into aText2? */
static bool ShiftedStringCompare(const TUint8* aText1,const TUint8* aText2,int aBits,int aShift)
	{
	ASSERT(aShift >= 0);
	aText2 += aShift / 8;
	aShift = aShift % 8;
	
	TUint8 c1;
	TUint8 c2;
	TUint8 mask;
	
	int next_shift = 8 - aShift;
	while (aBits > 0)
		{
		c1 = *aText1;
		c2 = *aText2;
		if (aShift)
			{
			c2 >>= aShift;
			c2 |= aText2[1] << next_shift;
			}
		if (aBits < 8)
			{
			mask = (TUint8)((1 << aBits) - 1);
			c1 &= mask;
			c2 &= mask;
			}
		if (c1 != c2)
			return false;
		aText1++;
		aText2++;
		aBits -= 8;
		}
	return true;
	}

/**
Find a bit string inside another bit string,
Return the bit offset of the first instance of aTarget in aText at bit offset aOffset or after,
or KErrNotFound (-1) if aTarget is not found.

Bits are treated as little-endian: that is, the first bit of a byte has the value 1,
the second 2, and the last 128.
*/
static int FindBitString(const TUint8* aTarget,int aTargetBits,
						 const TUint8* aText,int aTextBits,int aOffset)
	{
	int start_bit = aOffset;
	int end_bit = aTextBits - aTargetBits;
	while (start_bit <= end_bit)
		{
		if (ShiftedStringCompare(aTarget,aText,aTargetBits,start_bit))
			return start_bit;
		start_bit++;
		}
	return KErrNotFound;
	}

/**
Compares aBitmap2 with the aRect1 area of aBitmap1
Both bitmaps are assumend to be mono (1bpp)
*/
static bool MonoRectCompareL(const CFbsBitmap& aBitmap1,const TRect& aRect1,
						   const CFbsBitmap& aBitmap2)
	{
	if (aRect1.Size() != aBitmap2.SizeInPixels())
		return false;

	int width = aRect1.Width();
	if (width == 0)
		return true;

	int height = aRect1.Height();

	TInt bitmap1ScanLineBytes = ((aBitmap1.SizeInPixels().iWidth + 31) / 32) * 4;
	TInt bitmap2ScanLineBytes = ((width + 31) / 32) * 4;
	TInt byteOffset = aRect1.iTl.iX % 8;

	TUint8* bitmap1Offset = ((TUint8*)aBitmap1.DataAddress()) + (aRect1.iTl.iY * bitmap1ScanLineBytes) + (aRect1.iTl.iX / 8);
	TUint8* bitmap2Offset = (TUint8*)aBitmap2.DataAddress();
	for (int y = 0; y < height; y++)
		{
		if (!ShiftedStringCompare(bitmap2Offset, bitmap1Offset, width, byteOffset))
			return false;
		bitmap1Offset += bitmap1ScanLineBytes;
		bitmap2Offset += bitmap2ScanLineBytes;
		}

	return true;
	}

void CAseEngine2::SaveBitmapL(const TFileName& aFileName, const CFbsBitmap& aBitmap)
	{
	RFs& fsSession = CEikonEnv::Static()->FsSession();
	fsSession.Delete(aFileName);
	TBuf8<256> mime((const TUint8*)"image/png");
	CImageEncoder* pngEncoder = CImageEncoder::FileNewL(fsSession, aFileName, mime, CImageEncoder::EOptionAlwaysThread);
	CleanupStack::PushL(pngEncoder);
	TRequestStatus status;
	pngEncoder->Convert(&status, aBitmap);
	User::WaitForRequest(status);
	if(status != KErrNone)
	{
		User::InfoPrint(_L("Error saving screenshot"));
	}
	CleanupStack::PopAndDestroy(pngEncoder);
	}

EXPORT_C void CAseEngine2::AutoBootL()
	{
	skString bootfile;
	iExtra->ReadValueL(KIniBootFile, bootfile);

	TEntry entry;
	if (CEikonEnv::Static()->FsSession().Entry(bootfile.ptr(), entry)==KErrNone)
		{
		TFileName fileName(bootfile.ptr());
//		LoadSourceFileL(fileName);
		RunScriptL(fileName, EScriptFile);
		}
	else
		User::Leave(KErrNotFound);
	}


EXPORT_C void CAseEngine2::SetObserver(MAseObserver* aObserver)
	{
	iExtra->SetObserver(aObserver);
	}

EXPORT_C TBool CAseEngine2::IsRunning() const
	{
	return iExtra->IsRunning();
	}

EXPORT_C void CAseEngine2::Break()
	{
	iExtra->SetRunning(EFalse);
	}

EXPORT_C void CAseEngine2::RunScriptL(const TDesC& aSource, TScriptMode aOpenMode)
	{
	if (iExtra->Observer())
		iExtra->Observer()->HandleScriptBeginL();
	
	if (aOpenMode == EScriptFile)
		{
		// save file name for further use
		if (iFileName)
			{
			delete iFileName;
			iFileName = 0;		
			}
		iFileName = aSource.AllocL();
		
		// read all in a string, because I do not think STDLIB is thread safe
		// so I don't want to use skInputFile in our child thread...
		skInputFile* in = new(ELeave) skInputFile;
		CleanupStack::PushL(in);
		skString str;
		str = aSource; // leaves
		in->open(str);
		if (in->eof())
			{
			HBufC* message = CEikonEnv::Static()->AllocReadResourceL(R_ASE_ERR_CANNOT_OPEN);
			CleanupStack::PushL(message);
	//		trace(*message);
			User::Leave(KErrNotFound);
			}
		iSource = in->readAllToString(); // leaves
		CleanupStack::PopAndDestroy(in);
		}
	else
		{
		TBuf<256> buf(KAseInternal);
		iFileName = buf.AllocL();
		iSource = aSource;
		}

	// create and starts a thread for the interpreter
	RHeap* heap = RThread().Heap();
	if (iScriptThreadLaunched)
		{
		iScriptThread.Kill(0);
		iScriptThread.Close();
		}
	User::LeaveIfError(iScriptThread.Create(_L("ASESCRIPT"), ThreadMain, KDefaultStackSize, heap, this));
	iScriptThreadLaunched=true;
	iScriptThread.SetPriority(EPriorityNormal);
	iScriptThread.Resume();
	}

// From MAseLooper
bool CAseEngine2::WaitForStatement(TStatement& aStatement)
	{
	TInt res=KErrNone;
	aStatement.SetStatus(KRequestPending);
	// load statement
	Acquire();
	iPendingStatement = aStatement;
	Release();
	// wait till it's complete
	while(true)
		{
		Acquire();
		res = iPendingStatement.Status();
		Release();
		if (res!=KRequestPending) break;
		User::After(50000);
		}
	return (res==KErrNone);
	}

CAseMethod* CAseEngine2::TraceCallback()
	{
	for (int i=0; i<iMethodLookup.Count(); ++i)
		{
		CAseMethod* meth = iMethodLookup[i];
		if (meth->Name() == KPrivateTraceCmd)
			{
			return meth;
			}
		}
	return 0;
	}

// can leave
void CAseEngine2::trace(const skString& aMessage)
	{
	Acquire();
	skRValueArray& args = *iPrivateMethodsArgs;
	args[0] = aMessage;
	TStatement statement(TraceCallback(), iPrivateMethodsArgs, 0);
	Release();
	
	WaitForStatement(statement);
	}


void CAseEngine2::CloseApps()
	{
	TInt count=iWatcherArray.Count();
	while(--count>=0)
		{
		iWatcherArray[count]->Thread().Kill(0);
		}
	iWatcherArray.ResetAndDestroy();
	}

void CAseEngine2::HandleThreadExit(CThreadWatcher* aWatcher)
	{
	// An application thread we started has exited...log the reason
	TInt count=iWatcherArray.Count();
	while(--count>=0)
		{
		if(iWatcherArray[count]!=aWatcher)
			continue;
//		const RThread& thread=aWatcher->Thread();
//		TExitCategoryName exitName=thread.ExitCategory();
//		TExitType exitType=thread.ExitType();
//		TName name=thread.Name();
//		TInt exitReason=thread.ExitReason();
//		const TDesC* frmStr=NULL;
//		switch(exitType)
//			{
//			case EExitKill:
//				break; // don't log normal termination
//			case EExitTerminate:
//				frmStr=&KLitThreadTerminate;
//				break;
//			case EExitPanic:
//				frmStr=&KLitThreadPanic;
//				break;
//			case EExitPending:
//				frmStr=&KLitThreadPending;
//				break;
//			}
//		if(frmStr)
//			{
//			TInt ignored;
//			TRAP(ignored,LogScriptPositionL(KErrNone));
//			TRAP(ignored,LogToFileL(*frmStr,&name,&exitName,exitType,exitReason));
//			}
		delete iWatcherArray[count];
		iWatcherArray.Delete(count);
		break;
		}
	}

// methods

void CAseEngine2::cmdTraceL(skRValueArray& aArguments, skRValue& /*aUnused*/)
	{
	if (aArguments.entries() > 0)
		{
		// TODO: log to file, log to serial, log to [...]
		User::InfoPrint(aArguments[0].str().ptr());
		}
	}

/*void CAseEngine2::cmdAddLogL(skRValueArray& aArguments, skRValue& aResult)
	{
	}
*/

void CAseEngine2::cmdBreakL(skRValueArray& /*aArguments*/, skRValue& /*aResult*/)
	{
	Break();
	}

// click(x,y);
// click(myPoint);
// - Click at the specified position
void CAseEngine2::cmdClickL(skRValueArray& aArguments, skRValue& /*aResult*/)
	{
	const TPoint nowhere(-1, -1);
	TPoint origin(nowhere);
	if (aArguments.entries() == 2)
		{
		origin.iX = aArguments[0].intValue();
		origin.iY = aArguments[1].intValue();
		}
	else if (aArguments.entries() == 1 && aArguments[0].type() == skRValue::T_Object)
		{
		CAseRunnable* runnable = static_cast<CAseRunnable*>(aArguments[0].obj());
		if (runnable)
			{
			// if there's a point, this is the origin
			if (runnable->Name() == KPointObj)
				{
				CAsePoint* point = static_cast<CAsePoint*>(runnable);
				origin.iX = point->X();
				origin.iY = point->Y();
				}
			}
		}
	if (origin != nowhere)
		{
		// crop
		const TSize maxSize(CEikonEnv::Static()->ScreenDevice()->SizeInPixels());
		if (origin.iX >= maxSize.iWidth)	origin.iX = maxSize.iWidth-1;
		if (origin.iY >= maxSize.iHeight)	origin.iY = maxSize.iHeight-1;
		if (origin.iX < 0)					origin.iX = 0;
		if (origin.iY < 0)					origin.iY = 0;
		iCurrentPenState.iOrigin.iPoint	= origin;
		iCurrentPenState.iNextState		= SPenState::EPenDownBeforeClicking;
		}
	}

// close(uid);
// close("name");
// - Close and application
void CAseEngine2::cmdCloseL(skRValueArray& aArguments, skRValue& /*aResult*/)
	{
	if (aArguments.entries() > 0)
		{
		if (aArguments[0].type() == skRValue::T_Int)
			{
			TThreadId tid = aArguments[0].intValue();
			TInt count = iWatcherArray.Count();
			while (--count>=0)
				{
				if (iWatcherArray[count]->ThreadId() == tid)
					{
					iWatcherArray[count]->Thread().Kill(0);
					}
				}
			}
		else
			{
			TApaTaskList taskList(CEikonEnv::Static()->WsSession());
			TApaTask task = taskList.FindApp(aArguments[0].str().ptr());
			if (task.Exists())
				{
				task.KillTask();
				}
			}
		}
	}

void CAseEngine2::cmdCreateL(skRValueArray& aArguments, skRValue& aResult)
	{
	if (aArguments.entries() < 1)
		User::Leave(KErrArgument);

	skString className = aArguments[0].str(); // leaves

	// Create a new CAsePoint
	if (className == KPointObj)
		{
		aResult.assignObject(CAsePoint::NewL(this), true);
		return;
		}

	// ...
	}

// drag(x1, y1, x2, y2, x3, y3, x4, y4, ...);
// drag(x1, y1, x2, y2, myPoint, x3, y3, ...);
// drag(point1, point2, point3, point4, ...);
// - Issue a pen drag motion
void CAseEngine2::cmdDragL(skRValueArray& aArguments, skRValue& /*aResult*/)
	{
	const TInt argcount = aArguments.entries();
	TInt index = 0;
	TPoint currentPoint;
	bool pointComplete(false);
	bool firstPoint = true;
	while (index < argcount)
		{
		skRValue& val = aArguments[index++];
		if (val.type() == skRValue::T_Int)
			{
			if (!pointComplete)
				{
				currentPoint.iY = val.intValue();
				pointComplete = true;
				}
			else
				{
				currentPoint.iX = val.intValue();
				pointComplete = false;
				}
			}
		else if (val.type() == skRValue::T_Object)
			{
			CAseRunnable* runnable = static_cast<CAseRunnable*>(val.obj());
			if (runnable && (runnable->Name() == KPointObj))
				{
				CAsePoint* point = static_cast<CAsePoint*>(runnable);
				currentPoint.iX = point->X();
				currentPoint.iY = point->Y();
				pointComplete = true;
				}
			}
		else continue;
		//
		if (pointComplete)
			{
			if (firstPoint)
				{
				iCurrentPenState.iOrigin.iPoint = currentPoint;
				ASSERT(iCurrentPenState.iOrigin.iNext == 0);
				firstPoint = false;
				}
			else
				PushPointL(currentPoint);
			}
		}
		iCurrentPenState.iNextState = SPenState::EPenDownBeforeDragging;
	}

TPoint CAseEngine2::DoFindTextL(const TDesC& aText,const TRgb& aColor,CFbsBitmap& aBitmap,
							   TRect* aSearchRect, TRect* aTextRect,TSize* aFontSize,bool aCheckAllFonts,bool aSpaceSlashes)
	{
	TPoint text_position(-1,-1);

	// remove all non text-color pixels from the bitmap
	CFbsBitmap* bitmap = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(bitmap);
	TSize bitmapSize(aBitmap.SizeInPixels());

	// By default, use the whole bitmap, and start at the top left
	TRect searchRect(bitmapSize);

	if (aSearchRect)
		{
		// Ensure the aSearchRect is valid, ie within the bitmap
		// Must do this, see TRect.Contains documentation
		TPoint justWithinBr(aSearchRect->iBr.iX - 1,
			aSearchRect->iBr.iY - 1);
		if (!searchRect.Contains(aSearchRect->iTl))
			User::Leave(KErrNotFound);
		else if (!searchRect.Contains(justWithinBr))
			User::Leave(KErrNotFound);

		searchRect.SetRect(aSearchRect->iTl, aSearchRect->iBr);
		}

	// reset size and topleft
	bitmapSize = searchRect.Size();

	User::LeaveIfError(bitmap->Create(bitmapSize, aBitmap.DisplayMode()));

	CFbsBitmapDevice* device = CFbsBitmapDevice::NewL(bitmap);
	CleanupStack::PushL(device);
	CFbsBitGc* bitGc = NULL;
	User::LeaveIfError(device->CreateContext(bitGc));
	CleanupStack::PushL(bitGc);

	TRgb textColor(aColor);

	// Normalise the text colour
	TUint32 pixel_value = 0;
	switch (aBitmap.DisplayMode())
		{
		case ENone:
			User::Leave(KErrNotSupported);
			break;
		case EGray2:
			pixel_value = textColor.Gray2();
			break;
		case EGray4:
			pixel_value = textColor.Gray4();
			break;
		case EGray16:
			pixel_value = textColor.Gray16();
			break;
		case EGray256:
			pixel_value = textColor.Gray256();
			break;
		case EColor16:
			pixel_value = textColor.Color16();
			break;
		case EColor256:
			pixel_value = textColor.Color256();
			break;
		case EColor64K:
			pixel_value = textColor.Color64K();
			break;
		case EColor16M:
			pixel_value = textColor.Color16M();
			break;
		case ERgb:
			// already in the right format
			break;
		case EColor4K:
			pixel_value = textColor.Color4K();
			break;
		default:
			User::Leave(KErrNotSupported);
			break;
		}

	TRgb color[4];
	color[0] = textColor;
	color[1] = ~textColor;
	bitGc->BitBlt(TPoint(0, 0), &aBitmap, searchRect);
	bitGc->MapColors(TRect(TPoint(0 ,0), bitmapSize), color, 1, ETrue);	// invert text-color pixels
	bitGc->SetDrawMode(CGraphicsContext::EDrawModeNOTXOR);
	bitGc->BitBlt(TPoint(0, 0), &aBitmap, searchRect); // NOTXOR to wipe out non text-color pixels

	// generate a monochrome version of the bitmap
	CFbsBitmap* monoBitmap = new (ELeave) CFbsBitmap();
	CleanupStack::PushL(monoBitmap);
	User::LeaveIfError(monoBitmap->Create(bitmapSize, EGray2));

	CFbsBitmapDevice* monoDevice = CFbsBitmapDevice::NewL(monoBitmap);
	CleanupStack::PushL(monoDevice);
	CFbsBitGc* monoBitGc = NULL;
	User::LeaveIfError(monoDevice->CreateContext(monoBitGc));
	CleanupStack::PushL(monoBitGc);
	monoBitGc->BitBlt(TPoint(0, 0), bitmap);
	CleanupStack::PopAndDestroy(monoBitGc);
	CleanupStack::PopAndDestroy(monoDevice);

	TInt fontIndex = 0;
	bool quit_font_loop = false;

	while (!quit_font_loop && (fontIndex < iFontArray->Count()))
		{
		const CFont* font = (*iFontArray)[fontIndex++];
		if (!font)
			continue;

		// create the text bitmap
		TSize textSize(font->TextWidthInPixels(aText), font->HeightInPixels());
		if ((textSize.iWidth < aText.Length()) || (textSize.iHeight == 0))
			continue;

		CFbsBitmap* textBitmap = new (ELeave) CFbsBitmap();
		CleanupStack::PushL(textBitmap);
		User::LeaveIfError(textBitmap->Create(textSize, EGray2));
		CFbsBitmapDevice* textDevice = CFbsBitmapDevice::NewL(textBitmap);
		CleanupStack::PushL(textDevice);

		CFbsBitGc* textBitGc = NULL;
		User::LeaveIfError(textDevice->CreateContext(textBitGc));
		CleanupStack::PushL(textBitGc);
		textBitGc->UseFont(font);
		textBitGc->SetPenStyle(CGraphicsContext::ESolidPen);
		textBitGc->SetPenColor(KRgbBlack);

		if (aSpaceSlashes)
			{
#ifdef PLATFORM_S90
			// S90 has strange kerning around slash characters
			// This needs to be duplicated in the text bitmap
			TInt slashIndex = aText.Locate('/');
			if ((slashIndex == KErrNotFound) || !aSpaceSlashes)
				textBitGc->DrawText(aText, TPoint(0, Min(font->AscentInPixels(), font->HeightInPixels())));
			else
				{
				TPtrC text(aText);
				TInt xPos = 0;
				while (slashIndex != KErrNotFound)
					{
					TPtrC leftText(text.Left(slashIndex));
					textBitGc->DrawText(leftText, TPoint(xPos, Min(font->AscentInPixels(), font->HeightInPixels())));
					xPos += font->TextWidthInPixels(leftText) + 1;

					textBitGc->DrawText(_L("/"), TPoint(xPos, Min(font->AscentInPixels(), font->HeightInPixels())));
					xPos += font->TextWidthInPixels(_L("/")) - 1;

					text.Set(text.Right(text.Length() - slashIndex - 1));
					slashIndex = text.Locate('/');
					}

				if (text.Length())
					{
					textBitGc->DrawText(text, TPoint(xPos, Min(font->AscentInPixels(), font->HeightInPixels())));
					xPos += font->TextWidthInPixels(text);
					}
				}
#else
			textBitGc->DrawText(aText, TPoint(0, Min(font->AscentInPixels(), font->HeightInPixels())));
#endif
			}
		else
			textBitGc->DrawText(aText, TPoint(0, Min(font->AscentInPixels(), font->HeightInPixels())));

		textBitGc->DiscardFont();
		CleanupStack::PopAndDestroy(textBitGc);
		CleanupStack::PopAndDestroy(textDevice);

		/*
		Strip columns of white pixels at the right. If this is not
		done it will sometimes be impossible to find text ending in, say '/',
		immediately before, say 'w', because the text width will overlap the
		next letter and cause the text bitmap and the bitmap on the screen
		shot not to match.
		*/
		TInt scanLineOffset = textSize.iHeight / 2;

		TBitmapUtil textUtil(textBitmap);
		TSize new_size(textSize);
		TPoint p(0, 0);
		textUtil.Begin(p);
		for (p.iX = new_size.iWidth - 1; p.iX >= 0; p.iX--)
			{
			bool all_white = true;
			p.iY = 0;
			textUtil.SetPos(p);
			for (p.iY = 0; p.iY < new_size.iHeight; p.iY++)
				{
				TUint32 pixel_value = textUtil.GetPixel();
				if (pixel_value != 1)
					{
					scanLineOffset = p.iY;
					all_white = false;
					break;
					}
				textUtil.IncYPos();
				}
			if (!all_white)
				break;
			new_size.iWidth--;
			}
		textUtil.End();

		// S60, possibly others, can produce totally white bitmaps. Discard them.
		if (new_size.iWidth < aText.Length())
			{
			CleanupStack::PopAndDestroy(textBitmap);
			continue;
			}

		// Remove underline area and below.
		if (font->AscentInPixels() + 1 < textSize.iHeight)
			new_size.iHeight = font->AscentInPixels() + 1;

		if (new_size != textSize)
			{
			textBitmap->Resize(new_size);
			textSize = new_size;
			}


		/*
		Search all scanlines for occurrences of a chosen text scanline using EGray2 (1bpp) display mode.
		When a match occurs check using rect compare to eliminate false matches.
		*/

		TUint8* target = (TUint8*)(textBitmap->DataAddress() + (scanLineOffset * ((textSize.iWidth + 31) / 32)));
		TUint32* startText = monoBitmap->DataAddress();

		TInt scanRange(bitmapSize.iHeight - textSize.iHeight);
		TPoint scanPoint(0,0);
		for (scanPoint.iY = 0; scanPoint.iY < scanRange; scanPoint.iY++)
			{
			TUint8* text = (TUint8*)(startText + ((scanLineOffset + scanPoint.iY) * ((bitmapSize.iWidth + 31) / 32)));
			int bit = 0;
			bool found = false;
			for (;;)
				{
				bit = FindBitString(target,textSize.iWidth,text,bitmapSize.iWidth,bit);
				if (bit == KErrNotFound)
					break;
				scanPoint.iX = bit;

				if (MonoRectCompareL(*monoBitmap, TRect(scanPoint, textSize), *textBitmap))
					{
					found = true;
					// move matched font to start of array to speed up next find
					const CFont* currentFont = (*iFontArray)[fontIndex - 1];
					if (currentFont && (fontIndex - 1 != 0))
						{
						iFontArray->Delete(fontIndex - 1);
						iFontArray->InsertL(0, currentFont);
						}
					break;
					}
				bit++;
				}

			if (found)
				{
				bool set_return_values = false;
				if (aCheckAllFonts)
					{
					// Is this the 'top-left-most' find so far?
					if ((text_position.iX == -1 && text_position.iY == -1) ||
						scanPoint.iY < text_position.iY ||
						(scanPoint.iY == text_position.iY && scanPoint.iX < text_position.iX))
						set_return_values = true;
					}
				else
					set_return_values = true;
				if (set_return_values)
					{
					text_position = scanPoint;
					if (aTextRect)
						aTextRect->SetRect(scanPoint,textSize);
					if (aFontSize)
						{
						aFontSize->iWidth  = font->CharWidthInPixels('w');
						aFontSize->iHeight = font->HeightInPixels();
						}
					}
				if (!aCheckAllFonts)
					quit_font_loop = true;
				break;
				}
			}

		CleanupStack::PopAndDestroy(textBitmap);
		}

	CleanupStack::PopAndDestroy(monoBitmap);
	CleanupStack::PopAndDestroy(bitGc);
	CleanupStack::PopAndDestroy(device);
	CleanupStack::PopAndDestroy(bitmap);

#ifdef PLATFORM_S90
	if (!aSpaceSlashes && (text_position == TPoint(-1, -1)) && (aText.Locate('/') != KErrNotFound))
		return DoFindTextL(aText, aColor, aBitmap, aSearchRect, aTextRect, aFontSize, aCheckAllFonts, true);
	else
		return text_position;
#else
	return text_position;
#endif
	}

// coords = findtext("toto");
// infoprint("X = " # coords.x);
// infoprint("Y = " # coords.y);
// - Find some text on the screen
void CAseEngine2::cmdFindTextL(skRValueArray& aArguments, skRValue& aResult)
	{
	if (aArguments.entries() > 0)
		{
		const TPtrC& text = aArguments[0].str().ptr();
		TPoint position(-1,-1);
	
		// create screenshot
		CEikonEnv& env = *CEikonEnv::Static();
		CWsScreenDevice& screen = *env.ScreenDevice();
		CFbsBitmap* screenshot = new(ELeave) CFbsBitmap;
		CleanupStack::PushL(screenshot); //
		User::LeaveIfError(screenshot->Create(screen.SizeInPixels(),screen.DisplayMode()));
		User::LeaveIfError(screen.CopyScreenToBitmap(screenshot));

		TRAPD(err, (position = DoFindTextL(text, KRgbBlack, *screenshot)));

		CleanupStack::PopAndDestroy(screenshot);

		CAsePoint* point = CAsePoint::NewL(this);
		CleanupStack::PushL(point);
		point->SetL(position);
		aResult.assignObject(point, true);
		CleanupStack::Pop(point);
		}
	}
	
void CAseEngine2::cmdHintL(skRValueArray& aArguments, skRValue& aResult)
	{
	if (aArguments.entries() > 0)
		{
		TPtrC hintName	= aArguments[0].str().ptr();
		aResult	= ItemL(hintName);
		if (aArguments.entries() > 1)
			{
			// if a second argument exist, change current value
			ItemL(hintName) = aArguments[1];
			aResult	= ItemL(hintName);
			}
		}
	}

void CAseEngine2::cmdKeyL(skRValueArray& aArguments, skRValue& /*aResult*/)
	{
	if (aArguments.entries() > 0)
		{
		TInt repeat = 1;
		if (aArguments.entries() > 1)
			repeat = aArguments[1].intValue();

		TPtrC keyname = aArguments[0].str().ptr();
		for (; repeat>0; --repeat)
			SendKeyL(keyname);
		}
	}

/*
void CAseEngine2::cmdLogL(skRValueArray& aArguments, skRValue& aResult)
	{
	}
*/

/*
void CAseEngine2::cmdLogResetL(skRValueArray& aArguments, skRValue& aResult)
	{
	}
*/

// success = makedir("c:/mydir");
void CAseEngine2::cmdMakeDirL(skRValueArray& aArguments, skRValue& aResult)
	{
	if (aArguments.entries() > 0)
		{
		RFs& fs = CEikonEnv::Static()->FsSession();
		
		skString arg0;
		arg0 = aArguments[0].str(); // leaves

		TranslatePathL(arg0);
		
		TFileName dirName = arg0.ptr();
		if (dirName[dirName.Length()-1] != '\\')
			dirName.Append(_L("\\"));
			
		TInt error = fs.MkDirAll(dirName);
		aResult = (error==KErrNone || error==KErrAlreadyExists);
		}
	}

// open an application or a document
// uid = open("Try");
// uid = open("Try", timeout);
// uid = open("c:/toto.png");
// uid = open("z:/system/apps/baak/baak.app");
void CAseEngine2::cmdOpenL(skRValueArray& aArguments, skRValue& aResult)
	{
	if (aArguments.entries() > 0)
		{
		TTimeIntervalMicroSeconds32 timeout = 10000000;
		RFs& fs = CEikonEnv::Static()->FsSession();
		
		RApaLsSession session;
		TApaAppInfo info;
		User::LeaveIfError(session.Connect());
		CleanupClosePushL(session);
		User::LeaveIfError(session.GetAllApps());
		
		skString arg0;
		arg0 = aArguments[0].str(); // leaves
		TranslatePathL(arg0, EPTMNormal);
		const TDesC& name = arg0.ptr();

		if (aArguments.entries() > 1)
			timeout = aArguments[1].intValue()*1000000;
		
		// try to convert the caption name into a valid filename
		TFileName filename;
		if(name.Length()>2 && name[1]==':')
			{
			filename.Copy(name);
			}
		else
			{
			bool found = false;
			while (session.GetNextApp(info)==KErrNone)
				{
				if (info.iCaption.Compare(name) == 0)
					{
					found = true;
					break;
					}
				}
			if (!found)
				{
				User::Leave(KErrNotFound);
				}
			filename.Copy(info.iFullName);
			}

		// find application
		TEntry entry;
		User::LeaveIfError(fs.Entry(filename, entry));

		TBool isProgram = EFalse;
		User::LeaveIfError(session.IsProgram(filename, isProgram));

		TUid uid;
		if (isProgram)
			{
			uid = entry.iType[2];
			}
		else
			{
			TDataType type;
			User::LeaveIfError(session.AppForDocument(filename, uid, type));
			}

		TApaTaskList taskList(CEikonEnv::Static()->WsSession());
		TApaTask task = taskList.FindApp(uid);

		TThreadId tid;
		bool addWatcher=true;
		
		if (task.Exists())
			{
			task.BringToForeground();
			addWatcher=false;
			}
		else if (isProgram)
			{
			// run the application
			CApaCommandLine* cmdLine = CApaCommandLine::NewLC();
			cmdLine->SetLibraryNameL(filename);
			User::LeaveIfError(session.StartApp(*cmdLine, tid));
			CleanupStack::PopAndDestroy(cmdLine);
			}

		if (!isProgram)
			{
			// open the document (even if the application already exists)
			User::LeaveIfError(session.StartDocument(filename, uid, tid));
			}

		// wait for application to initialize
		while (timeout.Int()>0 && !taskList.FindApp(uid).Exists())
			{
			User::After(100000);
			timeout = timeout.Int()-100000;
			}
		

		if (addWatcher)
			{
			// add watcher
			bool alreadythere = false;
			TInt count = iWatcherArray.Count();
			while (--count>=0)
				{
				if (iWatcherArray[count]->ThreadId() == tid)
					{
					alreadythere = true;
					break;
					}
				}
			if (!alreadythere)
				{
				CThreadWatcher* watcher = NULL;
				TRAPD(error, (watcher = CThreadWatcher::NewL(tid,this)));
				if (error==KErrNone)
					{
					CleanupStack::PushL(watcher);
					iWatcherArray.AppendL(watcher);
					CleanupStack::Pop(watcher);
					}
				}
			aResult = (int)tid;
			}

		CleanupStack::PopAndDestroy(); // session
		}
		
	}

void CAseEngine2::cmdRebootL(skRValueArray& /*aArguments*/, skRValue& /*aResult*/)
	{
#if defined(PLATFORM_S90) && !defined(__WINS__) && !defined (__WINSCW__)
	TUid uid = {0};
	SysStartup::ShutdownAndRestart(uid, ESWNone);
	return;
#endif
	
	// make the current thread a system thread
	RThread().SetSystem(ETrue);
	// panic ourselves. This should bring down everything...
	User::Panic(_L("ASE::Reboot"), -1);	
	}

void CAseEngine2::cmdScreenShotBaseL(skRValueArray& aArguments, skRValue& /*aResult*/)
	{
	if (aArguments.entries() == 0)
		{
		iExtra->SetScreenShotBase(0);
		return;
		}

	skString format;
	format = aArguments[0].str();
	TranslatePathL(format);
	HBufC* buf = format.ptr().AllocL();
	iExtra->SetScreenShotBase(buf);
	}


// Save a screenshot to a file
void CAseEngine2::cmdScreenShotL(skRValueArray& aArguments, skRValue& /*aResult*/)
	{
	skString skfname;

	TFileName fileName;
	if (aArguments.entries() == 1)
		{
		skfname = aArguments[0].str();
		}
	else
		{
		HBufC* buf = iExtra->ScreenShotFileNameLC();
		skfname = *buf;
		CleanupStack::PopAndDestroy(buf);
		}
	TranslatePathL(skfname);
	fileName = skfname.ptr();

	CEikonEnv&			env       = *CEikonEnv::Static();
	CWsScreenDevice&	screen    = *env.ScreenDevice();

	CFbsBitmap* bitmap=new(ELeave) CFbsBitmap;
	CleanupStack::PushL(bitmap);
	User::LeaveIfError(bitmap->Create(screen.SizeInPixels(),screen.DisplayMode()));
	User::LeaveIfError(screen.CopyScreenToBitmap(bitmap));

	SaveBitmapL(fileName, *bitmap);
	CleanupStack::PopAndDestroy(bitmap);
	}

// suspend execution for a given number of seconds
void CAseEngine2::cmdSleepL(skRValueArray& aArguments, skRValue& /*aResult*/)
	{
	TInt seconds = 1;
	if (aArguments.entries()>0)
		{
		seconds = aArguments[0].intValue();
		}

	RTimer timer;
	TRequestStatus timerStatus;
	timer.CreateLocal();


	TInt64 interval = 1000000;
	interval *= seconds;
	timer.After(timerStatus, interval.Low());
	User::WaitForRequest(timerStatus);
	
	timer.Close();
	}

void CAseEngine2::cmdTextL(skRValueArray& aArguments, skRValue& /*aResult*/)
	{
	if (aArguments.entries()>0)
		{
		iCurrentText = aArguments[0].str(); // leaves
		// does nothing
		}
	}

void CAseEngine2::cmdValidDriveL(skRValueArray& aArguments, skRValue& aResult)
	{
	if (aArguments.entries()>0)
		{
		TPtrC arg = aArguments[0].str().ptr(); // leaves
		TInt drive;
		RFs& fs = CEikonEnv::Static()->FsSession();
		if (fs.CharToDrive(arg[0], drive) == KErrNone)
			{
			TDriveInfo info;
			if (fs.Drive(info, drive)==KErrNone)
				{
				aResult = 1;
				}
			}
		}
	aResult = 0;
	}

///////////////////////////////////////////////////////

GLDEF_C TInt E32Dll(TDllReason /*aReason*/)
	{
	return (KErrNone);
	}

