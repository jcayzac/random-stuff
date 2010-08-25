#include <eikenv.h>
#include "CollectS60WiFi.h"

HMAC::HMAC() {
	m_pValue[5]=m_pValue[4]=m_pValue[3]=m_pValue[2]=m_pValue[1]=m_pValue[0]=0;
}
HMAC::HMAC(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f) {
	m_pValue[0]=a;
	m_pValue[1]=b;
	m_pValue[2]=c;
	m_pValue[3]=d;
	m_pValue[4]=e;
	m_pValue[5]=f;
}
HMAC::HMAC(const unsigned char* value) {
	for (int i=0; i<6; ++i)
		m_pValue[i]=value[i];
}
HMAC::HMAC(const HMAC& o) {
	for (int i=0; i<6; ++i)
		m_pValue[i]=o.m_pValue[i];
}
bool HMAC::operator==(const HMAC& o) const {
	bool res=true;
	for (int i=0;i<6;++i) res|=(o.m_pValue[i]==m_pValue[i]);
	return res;
}


WiFiInfo::WiFiInfo()
: m_iSignalStrength(0)
{ }
WiFiInfo::WiFiInfo(const HMAC& hmac, int ss)
: m_oHMAC(hmac)
, m_iSignalStrength(ss)
{ }
WiFiInfo::WiFiInfo(const WiFiInfo& o)
: m_oHMAC(o.m_oHMAC)
, m_iSignalStrength(o.m_iSignalStrength)
{ }
bool WiFiInfo::operator==(const WiFiInfo& o) const {
	return (m_oHMAC==o.m_oHMAC && m_iSignalStrength==o.m_iSignalStrength);
}


CWifi* CWifi::NewL() {
	CWifi* self = NewLC ();
	CleanupStack::Pop (self);
	return self;
}
CWifi* CWifi::NewLC() {
	CWifi* self = new ( ELeave ) CWifi();
	CleanupStack::PushL (self);
	self->ConstructL ();
	return self;
}
CWifi::~CWifi() {
	if (m_pClient) {
		delete m_pClient;
		m_pClient=0;
	}
	if (m_pScanInfo) {
		delete m_pScanInfo;
		m_pScanInfo=0;
	}
}
CWifi::CWifi()
: m_pScanInfo(0)
, m_pClient(0)
{ }

void CWifi::ConstructL() {
	//
	m_pScanInfo=CWlanScanInfo::NewL();
	m_pClient=CWlanMgmtClient::NewL();
}

bool CWifi::GetInfo(RArray<WiFiInfo>& wia) {
	wia.Reset();
	// Find access points
	m_pClient->GetScanResults(*m_pScanInfo);
	for(m_pScanInfo->First(); !m_pScanInfo->IsDone(); m_pScanInfo->Next() ) {
		bool isInfrastructure=(m_pScanInfo->Capability()&1)?true:false;
		if (!isInfrastructure) continue;
		TWlanBssid bssid;
		m_pScanInfo->Bssid(bssid);
		
		int dBm = m_pScanInfo->RXLevel();
		if (dBm>0) dBm=-dBm;
		int asu2 = dBm+113;
		if (asu2<0) asu2=0;
		if (KErrNone!=wia.Append(WiFiInfo(HMAC((const unsigned char*)&bssid[0]),asu2))) {
			wia.Reset();
			return false;
		}
	}
	return true;
}
