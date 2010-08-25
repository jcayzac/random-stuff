#if !defined(__CONSOLE_H)
#define __CONSOLE_H
#include <windows.h>
#include "commode.h"

extern DWORD __stdcall console_main(void*);
class CConsole {
	uint32 tid;
	HANDLE console_thread;
public:
			CConsole(void);
	virtual	~CConsole(void);
	bool	Close(void);
	void	Print(string msg);
	// basic messaging. don't use directly!
	bool	quit_requested;
	HANDLE	mutex;
	string	buffer;
};

extern CConsole con;
#endif
