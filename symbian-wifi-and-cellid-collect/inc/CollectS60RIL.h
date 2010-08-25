#ifndef __COLLECTS60RIL_H__
#define __COLLECTS60RIL_H__
#include <etel3rdParty.h>

class ZoneInfo {
public:
	unsigned long m_ulCID;
	unsigned long m_ulAID;
	unsigned long m_ulNID;
	unsigned long m_ulMCC;
public:
	ZoneInfo();
	ZoneInfo(const ZoneInfo& o);
	ZoneInfo(unsigned long cid,unsigned long aid,unsigned long nid,unsigned long mcc);
	ZoneInfo& operator=(const ZoneInfo& o);
	bool operator==(const ZoneInfo& o) const;
	bool operator!=(const ZoneInfo& o) const { return !(o==*this); }
	bool Null() const { return (m_ulCID==0); }
};

class CRIL: public CActive {
public:
	static CRIL* NewL();
	static CRIL* NewLC();
	virtual ~CRIL();
	bool GetInfo(ZoneInfo& zi, unsigned long& ss);
protected:
	CRIL();
	void ConstructL();
private:
	void Start();
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
private:
	CActiveSchedulerWait				m_oWait;
	CTelephony*							m_pRIL;
	CTelephony::TNetworkInfoV1			m_oNwInfo;
	CTelephony::TNetworkInfoV1Pckg		m_oNwInfoPckg;
	CTelephony::TSignalStrengthV1		m_oSS;
	CTelephony::TSignalStrengthV1Pckg	m_oSSPckg;
	//
	bool								m_bError;
	bool								m_bReadingZone;
};

#endif // __COLLECTS60RIL_H__
