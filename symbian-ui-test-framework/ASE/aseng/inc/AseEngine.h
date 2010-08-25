#if !defined(ASEENGINE_H_INCLUDED)
#define ASEENGINE_H_INCLUDED

#include <e32std.h>

#if defined(__WINS__) && !defined(__MWERKS__)
#pragma warning(push, 1)
#endif
#include "skTreeNodeObject.h"
#include "skTraceCallback.h"
#include "skStatementStepper.h"
#include "skRValueArray.h"
#if defined(__WINS__) && !defined(__MWERKS__)
#pragma warning(pop)
#endif

#include "AseEngineObserver.h"
#include "AseUtils.h"
#include "ThreadWatcher.h"

class RFs;
class RWsSession;
class skExecutableContext;
class skInterpreter;


//
// wrapper for all CAseRunnable derived classes's methods
//
class CAseMethod: public CBase
	{
public:
	enum TFlags
		{
		ESessionSafe	= 0x1
		};
public:
	virtual void operator()(skRValueArray&, skRValue&) = 0;
	inline void SetNameL(const TDesC& aName)
		{
		delete iName;
		iName = 0;
		iName = aName.AllocL();
		}
	inline const TDesC& Name() const
		{
		if (iName) return *iName;
		return KNullDesC;
		}
	inline void SetFlags(TInt aFlags)
		{
		iFlags = aFlags;
		}
	inline TInt Flags() const
		{
		return iFlags;
		}
	virtual inline ~CAseMethod() { delete iName; }
private:
	HBufC*	iName;
	TInt	iFlags;
	};

template<class T>
class CAseMethodWrapper: public CAseMethod
	{
public:
	inline static CAseMethodWrapper* NewL(T& aInstance, const TDesC& aName, void (T::*aPtr)(skRValueArray& aArguments, skRValue& aResult))
		{
		CAseMethodWrapper* self = new(ELeave) CAseMethodWrapper(aInstance, aPtr);
		CleanupStack::PushL(self);
		self->SetNameL(aName);
		CleanupStack::Pop(self);
		return self;
		}
	inline void operator()(skRValueArray& aArgs, skRValue& aResult)
		{
		(iInstance.*iPtr)(aArgs, aResult);
		}
private:
	inline CAseMethodWrapper(T& aInstance, void (T::*aPtr)(skRValueArray&, skRValue&)): iInstance(aInstance), iPtr(aPtr) { }
private:
	T&					iInstance;
	void				(T::*iPtr)(skRValueArray&, skRValue&);
	};


//
// Named value
//
class CAseItem: public CBase
	{
public:
	inline static CAseItem* NewL(const TDesC& aName)
		{
		CAseItem* self = new(ELeave) CAseItem;
		CleanupStack::PushL(self);
		self->ConstructL(aName);
		CleanupStack::Pop(self);
		return self;
		}
	inline virtual ~CAseItem()
		{
		delete iName;
		}
	inline const TDesC& Name() const
		{
		return *iName;
		}
	inline skRValue& Value()
		{
		return iValue;
		}
private:
	inline CAseItem(const CAseItem& /*aCopy*/) { }
	inline CAseItem& operator=(const CAseItem& /*aCopy*/) { return *this; }
	inline CAseItem(): iValue(0) { }
	inline void ConstructL(const TDesC& aName)
		{
		iName = aName.AllocL();
		}
private:
	HBufC*		iName;
	skRValue	iValue;
	};

//
// Interthread message
//
class TStatement
	{
public:
	inline TStatement(): iCommand(0), iArgs(0), iResult(0) { }
	inline TStatement(CAseMethod* aCommand, skRValueArray* aArgs, skRValue* aResult): iCommand(aCommand), iArgs(aArgs), iResult(aResult) { }
	inline TStatement(const TStatement& aSt): iCommand(aSt.iCommand), iArgs(aSt.iArgs), iResult(aSt.iResult), iStatus(aSt.iStatus) { }
	inline TStatement& operator=(const TStatement& aSt)
		{
		iCommand	= aSt.iCommand;
		iArgs		= aSt.iArgs;
		iResult		= aSt.iResult;
		iStatus		= aSt.iStatus;
		return *this;
		}
	inline void ProcessL()
		{
		(*iCommand)(*iArgs, *iResult);
		}
	inline void Done()
		{
		iCommand = 0;
		}
	inline bool Exists() const
		{
		return (iCommand!=0);
		}
	inline TInt Status() const
		{
		return iStatus.Int();
		}
	inline void SetStatus(TInt aStatus)
		{
		iStatus = aStatus;
		}
public:
	CAseMethod*		iCommand;
	skRValueArray*	iArgs;
	skRValue*		iResult;
	TRequestStatus	iStatus;
	};

//
// looper interface
//
class MAseLooper
	{
public:
	virtual bool WaitForStatement(TStatement& aStatement) = 0;
	virtual bool Stopped() const = 0;
	};

//
// engine observer
//
class MAseObserver
	{
public:
	virtual void HandleScriptBeginL() = 0;
	virtual void HandleScriptStopL() = 0;
	virtual void HandleErrorL() = 0;
	};

//
// CAseRunnable
//
class CAseRunnable: public skTreeNodeObject
	{
public:
	inline CAseRunnable(MAseLooper* aLooper): iLooper(aLooper) { setAddIfNotPresent(true); }
	~CAseRunnable() { delete iTypeName; }
	skRValue& ItemL(const TDesC& aName);
	skRValue& ItemL(const skString& aName);
	template<class T>
	inline void ExportMethodL(const TDesC& aName, void (T::*aPtr)(skRValueArray&, skRValue&), TInt aFlags = 0)
		{
		CAseMethodWrapper<T>* meth = CAseMethodWrapper<T>::NewL(*static_cast<T*>(this), aName, aPtr);
		CleanupStack::PushL(meth);
		meth->SetFlags(aFlags);
		iMethodLookup.AppendL(meth);
		CleanupStack::Pop(meth);
		}
	const TDesC& Name() const { return *iTypeName; }
protected:
	void BaseConstructL();
	bool WaitForStatement(TInt aStatement, skRValueArray& aArguments, skRValue& aResult);
	void SetNameL(const TDesC& aName);
	// From skTreeNodeObject
	bool method(const skString& aMethodName, skRValueArray& aArguments, skRValue& aResult, skExecutableContext& aContext);
	bool getValue (const skString& aFieldName, const skString& aAttribute, skRValue& aValue);
	bool setValue (const skString& aFieldName, const skString& aAttribute, const skRValue& aValue);
protected:
	CArrayFixSegPtr32<CAseItem*>	iValueLookup;
	CArrayFixSegPtr32<CAseMethod*>	iMethodLookup;
	MAseLooper*						iLooper;
	HBufC*							iTypeName;
	};

//
// Point (for findtext())
//

class CAsePoint: public CAseRunnable
	{
public:
	static CAsePoint* NewL(MAseLooper* aLooper);
	TInt	X() const;
	TInt	Y() const;
	void	SetL(const TPoint& aPoint);
	void	SetXL(TInt aX);
	void	SetYL(TInt aY);
private:
	CAsePoint(MAseLooper* aLooper);
	void ConstructL();

	void cmdSetXL			(skRValueArray&, skRValue&);
	void cmdSetYL			(skRValueArray&, skRValue&);
	void cmdSetL			(skRValueArray&, skRValue&);
	};

//
// Engine
//

class CAseEngine2Extra;
class CAseEngine2: public CAseRunnable, public skTraceCallback, public MAseLooper, public MThreadWatcherObserver
	{
public:
	enum TScriptMode
		{
		EScriptFile,
		EScriptSource
		};
	enum TPathTranslationMode
		{
		EPTMNormal,
		EPTMAddSession
		};
	struct SPenState
		{
		enum TPenState
			{
			ENone,
			EPenDownBeforeClicking,
			EPenDownBeforeDragging,
			EPenDrag,
			EPenUp
			};
		struct SPoint
			{
			TPoint	iPoint;
			SPoint*	iNext;
			SPoint(): iNext(0) { }
			};
		TPenState	iNextState;
		SPoint		iOrigin;
		};
public:
	IMPORT_C static CAseEngine2*	NewL();
	virtual IMPORT_C				~CAseEngine2();
	IMPORT_C void					RunScriptL(const TDesC& aSource, TScriptMode aOpenMode = EScriptFile);
	IMPORT_C void					AutoBootL();
	IMPORT_C void					SetObserver(MAseObserver* aObserver);
	IMPORT_C TBool					IsRunning() const;
	IMPORT_C void					Break();
private:
						CAseEngine2();
	void				ConstructL();
	void				InitFontsL();
	static TInt			ThreadMain(TAny* aPtr);		
	static TInt			PeriodicBeat(TAny*);
	inline void			Acquire() { iMutex.Wait(); }
	inline void			Release() { iMutex.Signal(); }
	inline TStatement&	PendingStatement() { return iPendingStatement; }
	static void			TranslatePathL(skString& aPath, TPathTranslationMode aMode=EPTMAddSession);
	void				ReadEntryPointL();
	TPoint DoFindTextL(const TDesC& aText,const TRgb& aColor,CFbsBitmap& aBitmap,
		TRect* aSearchRect = NULL, TRect* aTextRect = NULL,
		TSize* aFontSize = NULL,bool aCheckAllFonts = false, bool aSpaceSlashes = false);
		
	void				ReadIniL(); // todo
	void				WriteIniL(); // todo

	static void			SendStdKey(TInt aKey,TBool aDown);
	static void			SendStdKey(TInt aKey);
	static void			SendKeyL(const TDesC& aKey,TBool aDown);
	static void			SendKeyL(const TDesC& aKey);
	static void			SendCharL(TInt aCode);
	static void			SendCTRLNumberCharL(TInt aCode);
	void				SendNextCharInTextL();
	const skString&		Text() const;

	bool				TryToSendPenEvent();
	void				PushPointL(const TPoint& aPoint);
	bool				PopPoint(TPoint& aPoint);

	void				SaveBitmapL(const TFileName& aFileName, const CFbsBitmap& aBitmap);
	void				DoRunScriptL();
	CAseMethod*			TraceCallback();

	static void			FindFileL(const TDesC& aPath, const TDesC& aName, TFileName& aMatch);
	static void			FindDirVolumeL(const TDesC& aPath, TFileName& aFullPath);
	static void			FindMidletL(const TDesC& aPath, const TDesC& aName, TFileName& aMatch);

	void				CloseApps();
	void				HandleThreadExit(CThreadWatcher* aWatcher);
	
	// From skTraceCallback
	void				trace(const skString& aMessage);	
	// From MAseLooper
	bool WaitForStatement(TStatement& aStatement);
	bool Stopped() const;
	
private:
	// loggers
//	bool	isFileLogL(const TDesC& aUri, RWriteStream*& aStreamPtr);

private:
	// native methods
//	void cmdAddLogL			(skRValueArray&, skRValue&); // todo
	void cmdBreakL			(skRValueArray&, skRValue&);
	void cmdClickL			(skRValueArray&, skRValue&);
	void cmdCloseL			(skRValueArray&, skRValue&);
	void cmdCreateL			(skRValueArray&, skRValue&);
	void cmdDragL			(skRValueArray&, skRValue&);
	void cmdFindTextL		(skRValueArray&, skRValue&);
	void cmdHintL			(skRValueArray&, skRValue&);
	void cmdKeyL			(skRValueArray&, skRValue&);
//	void cmdLogL			(skRValueArray&, skRValue&); // todo
//	void cmdLogResetL		(skRValueArray&, skRValue&); // todo
	void cmdMakeDirL		(skRValueArray&, skRValue&);		
	void cmdOpenL			(skRValueArray&, skRValue&);
	void cmdRebootL			(skRValueArray&, skRValue&);
	void cmdScreenShotBaseL	(skRValueArray&, skRValue&);
	void cmdScreenShotL		(skRValueArray&, skRValue&);
	void cmdSleepL			(skRValueArray&, skRValue&);
	void cmdTextL			(skRValueArray&, skRValue&);
	void cmdTraceL			(skRValueArray&, skRValue&);
	void cmdValidDriveL		(skRValueArray&, skRValue&);

private:
	RMutex							iMutex;
	RThread							iScriptThread;
	bool							iScriptThreadLaunched;
	CArrayPtrFlat<CThreadWatcher>	iWatcherArray;
	CPeriodic*						iPeriodic;
	TStatement						iPendingStatement;
	skString						iSource;
	HBufC*							iFileName;
	skRValueArray*					iPrivateMethodsArgs;
	CAseEngine2Extra*				iExtra;
	CArrayPtrFlat<const CFont>*			iFontArray;
	skString						iCurrentText;
	SPenState						iCurrentPenState;
	skString						iEntryPoint;
//	CArrayPtrFlat<RWriteStream>*	iLogArray;	
	};

inline bool CAseEngine2::Stopped() const
	{
	return (IsRunning()==EFalse);
	}

#endif
