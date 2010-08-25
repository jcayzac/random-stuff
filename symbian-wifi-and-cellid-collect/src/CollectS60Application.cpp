#include "CollectS60.hrh"
#include "CollectS60Document.h"
#include "CollectS60Application.h"

CApaDocument* CCollectS60Application::CreateDocumentL() {
	return static_cast<CApaDocument*>(CCollectS60Document::NewL(*this));
}
TUid CCollectS60Application::AppDllUid() const {
	return TUid::Uid(_UID3);
}
