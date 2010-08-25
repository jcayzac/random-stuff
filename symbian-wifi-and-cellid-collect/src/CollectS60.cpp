#include "CollectS60Application.h"

#ifdef EKA2
#include <eikstart.h>
LOCAL_C CApaApplication* NewApplication() {
	return new CCollectS60Application;
}
GLDEF_C TInt E32Main() {
	return EikStart::RunApplication( NewApplication );
}
#else
EXPORT_C CApaApplication* NewApplication() {
	return new CCollectS60Application;
}

GLDEF_C TInt E32Dll(TDllReason) {
	return KErrNone;
}
#endif
