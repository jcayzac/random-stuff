#if !defined(ASAPPUI_H_INCLUDED)
#define ASAPPUI_H_INCLUDED

#include <EikAppUi.h>

class CAsappView;
class CAsappUi: public CEikAppUi
	{
public:
	CAsappUi();
	~CAsappUi();

private:
//from CEikAppUi
	void HandleForegroundEventL(TBool aForeground);
	void HandleCommandL(TInt aCommand);

	void ConstructL();

private:
	CAsappView* iView;
	};

#endif
