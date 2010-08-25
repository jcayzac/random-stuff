#include <strstream>
#include "console.h"

CConsole::CConsole(void): quit_requested(false) {
	mutex  = CreateMutex(NULL,FALSE,NULL);
	CreateThread(NULL,0,console_main,(void*)this,0,&tid);
	ostrstream ostr;
	ostr.clear();
	ostr << "Console thread 0x" << hex << tid << " launched." << endl;
	ostr.flush();
	// FUCK VISUAL STUDIO
	string a = ostr.str();
	int bug=a.find_first_of("\n");
	if (bug>0) {
		char* buff = strdup(a.c_str());
		buff[bug+1]=0;
		Print(buff);
	}
	else Print(a);
}

CConsole::~CConsole(void) {
	Close();
}

bool CConsole::Close(void) {
	uint32 result = WaitForSingleObject(mutex,INFINITE);
	if (result!=WAIT_FAILED) {
		quit_requested=true;
		debug("Console closed.\n");
		if (ReleaseMutex(mutex)) return true;
	}
	return false;
}

void CConsole::Print(string msg) {
	uint32 result = WaitForSingleObject(mutex,INFINITE);
	if (result!=WAIT_FAILED) {
		buffer += msg;
		ReleaseMutex(mutex);
	}
}

DWORD __stdcall console_main(void* inst) {
	CConsole *console = reinterpret_cast<CConsole*>(inst);
	while (true) {
		uint32 result = WaitForSingleObject(console->mutex,INFINITE);
		if (result!=WAIT_FAILED) {
			if (console->quit_requested) {
				CloseHandle(console->mutex);
				break;
			}
			if (console->buffer.size()>0) {
				debug(console->buffer);
				console->buffer = "";
			}
			while (!ReleaseMutex(console->mutex));
		}
	}
	return 0;
}
