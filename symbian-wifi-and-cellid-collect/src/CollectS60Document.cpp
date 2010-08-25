#include "CollectS60AppUi.h"
#include "CollectS60Document.h"

CCollectS60Document* CCollectS60Document::NewL(CEikApplication& aApp) {
	CCollectS60Document* self = NewLC(aApp);
	CleanupStack::Pop(self);
	return self;
}
CCollectS60Document* CCollectS60Document::NewLC(CEikApplication& aApp) {
	CCollectS60Document* self = new (ELeave) CCollectS60Document( aApp );
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
}
void CCollectS60Document::ConstructL() { }
CCollectS60Document::CCollectS60Document(CEikApplication& aApp): CAknDocument(aApp) { }
CCollectS60Document::~CCollectS60Document() { }
CEikAppUi* CCollectS60Document::CreateAppUiL() {
	return static_cast<CEikAppUi*>(new (ELeave) CCollectS60AppUi);
}
