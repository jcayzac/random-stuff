#include "CollectS60LBS.h"

// ===========================================
// LBS-related stuff
// ===========================================
_LIT(KSrvName, "CollectS60");
const TInt KSecond = 1000000;
const TInt KUpdateInterval = KSecond;
const TInt KUpdateTimeOut = 15*KSecond;
const TInt KMaxAge = 500000;

CLBSActive* CLBSActive::NewL(ILBSListener& listener) {
	CLBSActive* self = NewLC (listener);
	CleanupStack::Pop (self);
	return self;
}
CLBSActive* CLBSActive::NewLC(ILBSListener& listener) {
	CLBSActive* self = new ( ELeave ) CLBSActive(listener);
	CleanupStack::PushL (self);
	self->ConstructL ();
	return self;
}
CLBSActive::~CLBSActive() {
	Cancel();
	if (m_bPositionerOpened) {
		m_oPositioner.Close();
		m_bPositionerOpened=false;
	}
	if (m_bPositionServerOpened) {
		m_oPositionServer.Close();
		m_bPositionServerOpened=false;
	}
}
void CLBSActive::Start() {
	TPositionUpdateOptions upOpt;
    upOpt.SetUpdateInterval(TTimeIntervalMicroSeconds(KUpdateInterval));
    upOpt.SetUpdateTimeOut(TTimeIntervalMicroSeconds(KUpdateTimeOut));
    upOpt.SetMaxUpdateAge(TTimeIntervalMicroSeconds(KMaxAge));
	m_oPositioner.SetUpdateOptions(upOpt);
	m_oPositioner.NotifyPositionUpdate(m_oSatelliteInfo, iStatus);
	SetActive();
}
CLBSActive::CLBSActive(ILBSListener& listener)
: CActive(CActive::EPriorityStandard)
, m_bPositionServerOpened(false)
, m_bPositionerOpened(false)
, m_oListener(listener)
, m_fCurrentLongitude(.0f)
, m_fCurrentLatitude(.0f)
, m_fCurrentSpeed(.0f)
{ }
void CLBSActive::ConstructL() {
	User::LeaveIfError(m_oPositionServer.Connect());
	m_bPositionServerOpened=true;
	User::LeaveIfError(m_oPositioner.Open(m_oPositionServer));
	m_bPositionerOpened=true;
	User::LeaveIfError(m_oPositioner.SetRequestor(CRequestor::ERequestorService, CRequestor::EFormatApplication, KSrvName));
	CActiveScheduler::Add(this);
}
void CLBSActive::RunL() {
	if (m_bPositionerOpened) {
		if (iStatus.Int()==KErrNone) {
			TPosition pos;
			m_oSatelliteInfo.GetPosition(pos);
			TCourse course;
			m_oSatelliteInfo.GetCourse(course);
			float longitude = (float)pos.Longitude();
			float latitude  = (float)pos.Latitude();
			float speed     = (float)course.Speed();
			if (longitude!=m_fCurrentLongitude ||
				latitude!=m_fCurrentLatitude ||
				speed!=m_fCurrentSpeed) {
				m_fCurrentLongitude=longitude;
				m_fCurrentLatitude =latitude;
				m_fCurrentSpeed=speed;
				m_oListener.OnLBSPosition(longitude,latitude);
			}
		}
		m_oPositioner.NotifyPositionUpdate(m_oSatelliteInfo, iStatus);
		SetActive();
	}
}
void CLBSActive::DoCancel() {
	iStatus=KErrNone;
	if (m_bPositionerOpened) {
		m_oPositioner.CancelRequest(EPositionerNotifyPositionUpdate);
	}
}
TInt CLBSActive::RunError(TInt aError) {
	return aError;
}

