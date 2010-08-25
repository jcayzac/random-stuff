#ifndef __COLLECTS60APPLICATION_H__
#define __COLLECTS60APPLICATION_H__

#include <aknapp.h>
#include "CollectS60.hrh"

class CCollectS60Application : public CAknApplication {
public:
	TUid AppDllUid () const;
protected:
	CApaDocument* CreateDocumentL ();
};

#endif // __COLLECTS60APPLICATION_H__
