#if !defined(ASAPP_H_INCLUDED)
#define ASAPP_H_INCLUDED

#include <EikDoc.h>
#include <EikApp.h>

const TUid KUidAseApplication = { 0x1000b00f };

class CAseDocument : public CEikDocument
	{
public:
	CAseDocument(CEikApplication& aApp);
	CEikAppUi* CreateAppUiL();
	};

class CAsapp : public CEikApplication
	{
public:
	TUid AppDllUid() const;
	CApaDocument* CreateDocumentL();
	};

#endif
