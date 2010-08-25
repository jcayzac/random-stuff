#include "AseEngineExtra.h"

#include <f32file.h>
#include <eikenv.h>

#if defined(__WINS__) && !defined(__MWERKS__)
#pragma warning(push, 1)
#endif
#include "skString.h"
#if defined(__WINS__) && !defined(__MWERKS__)
#pragma warning(pop)
#endif


_LIT(KConfigFilePaths,	"\\;\\system\\data\\");
_LIT(KConfigFile,		"ase.ini");

CAseEngine2Extra::~CAseEngine2Extra()
	{
	delete iConfig;
	delete iScreenShotBase;
	}

HBufC*	CAseEngine2Extra::ScreenShotBase() const
	{
	return iScreenShotBase;
	}

void	CAseEngine2Extra::SetScreenShotBase(HBufC* aSSB)
	{
	delete iScreenShotBase;
	iScreenShotBase = aSSB;
	iScreenShotCounter = 0;
	}

HBufC*	CAseEngine2Extra::ScreenShotFileNameLC()
	{
	TFileName format;
	if (iScreenShotBase)
		{
		format = *iScreenShotBase;
		}
	else
		{
		format = _L("c:\\MyDocs\\screen\04d.png");
		}

	TFileName fname;
	TEntry entry;

	while(true)
		{
		fname.Format(format, iScreenShotCounter);
		if (CEikonEnv::Static()->FsSession().Entry(fname, entry)!=KErrNone)
			break;
		++iScreenShotCounter;
		};

	return fname.AllocLC();
	}

void CAseEngine2Extra::ReadConfigL()
	{
	TAutoClose<RFs> fs;
	User::LeaveIfError(fs.iObj.Connect());
	fs.PushL();

	TFindFile filePath(fs.iObj);
	User::LeaveIfError(filePath.FindByPath(KConfigFile,&KConfigFilePaths));

	TAutoClose<RFile> fl;
	fl.PushL();
	User::LeaveIfError(fl.iObj.Open(fs.iObj,filePath.File(),EFileShareExclusive));

	TInt size;
	User::LeaveIfError(fl.iObj.Size(size));

	HBufC8* buf8 = HBufC8::NewL(size+1);
	CleanupStack::PushL(buf8);

	TPtr8 ptr8 = buf8->Des();
	User::LeaveIfError(fl.iObj.Read(ptr8));

	if (iConfig)
		delete iConfig;
	iConfig = 0;

	iConfig = HBufC::NewL(size+1);
	TPtr ptr = iConfig->Des();
	ptr.Copy(ptr8);

	CleanupStack::PopAndDestroy(buf8);
	CleanupStack::PopAndDestroy(2); // fl, fs
	}

void CAseEngine2Extra::ReadValueL(const TDesC& aName, TInt& aValue)
	{
	skString val;
	ReadValueL(aName, val);
	if(val.length()>0)
		{
		TLex lex(val.ptr());
		User::LeaveIfError(lex.Val(aValue));
		}
	}

void CAseEngine2Extra::ReadValueL(const TDesC& aName, skString& aValue)
	{
	if (!iConfig)
		User::Leave(KErrNotFound);

	TLexMark16 begin, end;

	TInt pos=iConfig->Find(aName);
	if (pos==KErrNotFound)
		User::Leave(KErrNotFound);
	
	TLex lex(iConfig->Mid(pos));
	lex.SkipCharacters();		 // skip keyword
	lex.SkipSpaceAndMark();		 // skip space
	lex.SkipCharacters();		 // skip =
	lex.SkipSpaceAndMark(begin); // skip space -> beginning of the field
	lex.Mark();
	lex.SkipCharacters();
	
	TInt size = lex.Offset()-lex.MarkedOffset(begin);
	
	// FIXME: it messes with space characters inside the value...
	TPtrC ptr(lex.MarkedToken().Ptr(), size);
	aValue = ptr;
	}
