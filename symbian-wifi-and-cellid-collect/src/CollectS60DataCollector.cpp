#include "CollectS60DataCollector.h"
#include <eikenv.h>

CDataCollector* CDataCollector::NewL() {
	CDataCollector* self = NewLC ();
	CleanupStack::Pop (self);
	return self;
}
CDataCollector* CDataCollector::NewLC() {
	CDataCollector* self = new ( ELeave ) CDataCollector();
	CleanupStack::PushL (self);
	self->ConstructL ();
	return self;
}
CDataCollector::~CDataCollector() {
	Stop();
	if (m_pRIL) {
		delete m_pRIL;
		m_pRIL=0;
	}
	if (m_pLBSActive) {
		delete m_pLBSActive;
		m_pLBSActive=0;
	}
	if (m_pDB) {
		delete 	m_pDB;
		m_pDB=0;
	}
}
CDataCollector::CDataCollector()
: m_pLBSActive(0)
, m_pRIL(0)
, m_pWifi(0)
, m_pDB(0)
{ }
void CDataCollector::ConstructL() {
	m_pRIL=CRIL::NewL();
	m_pWifi=CWifi::NewL();
	m_pLBSActive=CLBSActive::NewL(*this);
	m_pDB = new(ELeave) CGeoSignalDB;
}
void CDataCollector::Start(bool useExternalGPSApi)		{ if (!useExternalGPSApi) m_pLBSActive->Start(); }
void CDataCollector::Stop()								{ m_pLBSActive->Cancel(); }
void CDataCollector::OnLBSPosition(float lon,float lat)	{ SetCurrentPosition(lon,lat); }

void CDataCollector::SetCurrentPosition(float longitude, float latitude) {
	GeoPosition pos;
	pos.m_fLongitude=longitude;
	pos.m_fLatitude =latitude;
	ZoneInfo zi;
	unsigned long ss;
	if (m_pRIL->GetInfo(zi, ss)) {
		TRawRow gsmRow(EDBTypeGSM);
		gsmRow.SetZoneInfo(zi);
		gsmRow.SetPosition(pos);
		gsmRow.Set2ASU(ss);
		m_pDB->AddRow(gsmRow);
	}
	//
	RArray<WiFiInfo> wia;
	if (m_pWifi->GetInfo(wia)) {
		unsigned long foundNetworks=wia.Count();
		unsigned long index=0;
		while(index<foundNetworks) {
			WiFiInfo& wi=wia[index++];
			TRawRow wifiRow(EDBTypeWLAN);
			wifiRow.SetHMAC(wi.m_oHMAC);
			wifiRow.SetPosition(pos);
			wifiRow.Set2ASU(ss);
			m_pDB->AddRow(wifiRow);			
		}
	}
}
