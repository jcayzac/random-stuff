#if !defined(ASEENGINEEXTRA_H_INCLUDED)
#define ASEENGINEEXTRA_H_INCLUDED

#include <e32std.h>
#include <e32base.h>

class skString;
class MAseObserver;

class CAseEngine2Extra: public CBase
	{
	public:
		void	ReadConfigL();
		void	ReadValueL(const TDesC& aName, TInt& aValue);
		void	ReadValueL(const TDesC& aName, skString& aValue);
		
		HBufC*	ScreenShotFileNameLC();
		HBufC*	ScreenShotBase() const;
		void	SetScreenShotBase(HBufC*);

		inline TBool	IsRunning() const;
		inline void		SetRunning(TBool aStatus);

		inline void SetObserver(MAseObserver* aObserver);
		inline MAseObserver* Observer();

		~CAseEngine2Extra();
	private:
		// Config
		HBufC*			iConfig;
		// ScreenShotBase
		HBufC*			iScreenShotBase;
		TInt			iScreenShotCounter;
		// script status
		TBool			iRunning;
		// observer
		MAseObserver*	iObserver;
	};

inline TBool CAseEngine2Extra::IsRunning() const
	{
	return iRunning;
	}

inline void CAseEngine2Extra::SetRunning(TBool aStatus)
	{
	iRunning = aStatus;
	}

inline void CAseEngine2Extra::SetObserver(MAseObserver* aObserver)
	{
	iObserver = aObserver;
	}

inline MAseObserver* CAseEngine2Extra::Observer()
	{
	return iObserver;
	}

#endif
