#include "CollectS60RIL.h"
#include <eikenv.h>

ZoneInfo::ZoneInfo()
: m_ulCID(0)
, m_ulAID(0)
, m_ulNID(0)
, m_ulMCC(0)
{ }
ZoneInfo::ZoneInfo(const ZoneInfo& o)
: m_ulCID(o.m_ulCID)
, m_ulAID(o.m_ulAID)
, m_ulNID(o.m_ulNID)
, m_ulMCC(o.m_ulMCC)
{ }
ZoneInfo::ZoneInfo(unsigned long cid,unsigned long aid,unsigned long nid,unsigned long mcc)
: m_ulCID(cid)
, m_ulAID(aid)
, m_ulNID(nid)
, m_ulMCC(mcc)
{ }
ZoneInfo& ZoneInfo::operator=(const ZoneInfo& o) {
	m_ulCID=o.m_ulCID;
	m_ulAID=o.m_ulAID;
	m_ulNID=o.m_ulNID;
	m_ulMCC=o.m_ulMCC;
	return *this;
}

bool ZoneInfo::operator==(const ZoneInfo& o) const {
	return ((o.m_ulCID==m_ulCID && o.m_ulAID==m_ulAID &&
			o.m_ulNID==m_ulNID && o.m_ulMCC==m_ulMCC) ||
			(o.Null() && Null()));
}

// CellID

CRIL* CRIL::NewL() {
	CRIL* self = NewLC ();
	CleanupStack::Pop (self);
	return self;
}
CRIL* CRIL::NewLC() {
	CRIL* self = new ( ELeave ) CRIL();
	CleanupStack::PushL (self);
	self->ConstructL ();
	return self;
}
CRIL::~CRIL() {
	Cancel();
	if (m_pRIL) {
		delete m_pRIL;
		m_pRIL=0;
	}
}
void CRIL::Start() {
	Cancel();
	m_bReadingZone=true;
	m_bError=false;
	m_pRIL->GetCurrentNetworkInfo(iStatus, m_oNwInfoPckg);
	SetActive();
	m_oWait.Start();
}
CRIL::CRIL()
: CActive(EPriorityHigh)
, m_pRIL(0)
, m_oNwInfoPckg(m_oNwInfo)
, m_oSSPckg(m_oSS)
{ }
void CRIL::ConstructL() {
	m_pRIL=CTelephony::NewL();
	CActiveScheduler::Add(this);
}
void CRIL::RunL() {
	if (m_oWait.IsStarted()) {
		m_oWait.AsyncStop();
		User::LeaveIfError(iStatus.Int());
		if (m_bReadingZone) {
			m_bReadingZone=false;
			m_pRIL->GetSignalStrength(iStatus,m_oSSPckg);
			SetActive();
			m_oWait.Start();
		}
	}
}

bool CRIL::GetInfo(ZoneInfo& zi, unsigned long& ss) {
	Start();
	if (m_bError) return false;
	//
	CTelephony::TNetworkInfoV1 info = m_oNwInfoPckg();
	if (info.iAreaKnown) {
		if (info.iCountryCode.Length()) {
			TLex lex(info.iCountryCode);
			if (KErrNone!=lex.Val((TUint&)zi.m_ulMCC)) zi.m_ulMCC=0;
		}
		if (info.iNetworkId.Length()) {
			TLex lex(info.iNetworkId);
			if (KErrNone!=lex.Val((TUint&)zi.m_ulNID)) zi.m_ulNID=0;
		}
		zi.m_ulCID=info.iCellId;
		zi.m_ulAID=info.iLocationAreaCode;
	}
	else return false;
	//
	CTelephony::TSignalStrengthV1 ssv1 = m_oSSPckg();
	
	int dBm = ssv1.iSignalStrength;
	if (dBm>0) dBm=-dBm;
	int asu2 = dBm+113;
	if (asu2<0) asu2=0;
	ss = (unsigned long)asu2;
	return true;
}
void CRIL::DoCancel() {
	iStatus=KErrNone;
	if (m_pRIL) {
		if (m_bReadingZone) m_pRIL->CancelAsync(CTelephony::EGetCurrentNetworkInfoCancel);
		else m_pRIL->CancelAsync(CTelephony::EGetSignalStrengthCancel);
	}
}

TInt CRIL::RunError(TInt aError) {
	m_bError=true;
	return aError;
}
