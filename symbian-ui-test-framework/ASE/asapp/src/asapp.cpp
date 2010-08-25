#include "asapp.h"
#include "asappui.h"

CAseDocument::CAseDocument(CEikApplication& aApp):
	CEikDocument(aApp)
	{
	}

CEikAppUi* CAseDocument::CreateAppUiL()
	{
	return new(ELeave) CAsappUi;
	}

TUid CAsapp::AppDllUid() const
	{
	return KUidAseApplication;
	}

CApaDocument* CAsapp::CreateDocumentL()
	{
	return new(ELeave) CAseDocument(*this);
	}


EXPORT_C CApaApplication* NewApplication()
	{
	CAsapp* app = new CAsapp;
	return app;
	}

GLDEF_C TInt E32Dll(TDllReason)
	{
	return(KErrNone);
	}
