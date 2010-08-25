#if !defined(__COMMODE_H)
#define __COMMODE_H

// #include utiles
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <time.h>
using namespace std;

#if !defined(WIN32)
#pragma message("This program has been written for a WIN32 machine!\n")
#endif

// Runtime(classname): cette macro cree un membre <Class>
// contenant des infos.
#define Runtime(s) \
class myCruntime{\
private:\
string	_name;\
int		_size;\
public:\
inline myCruntime(void) {\
_name	= #s;\
_size	= sizeof(s) - sizeof(myCruntime);\
}\
inline int size(void) { return _size; }\
inline string& name(void) { return _name; }\
} Class;

// pour creer des objets avec le constructeur par defaut
// declare et cree
#define myNewx(s,t)	t * s = new t
// cree
#define myNew(s,t)		s = new t

// types plus sympa
typedef unsigned long		uint32;
typedef signed long			int32;
typedef unsigned short		uint16;
typedef signed short		int16;
typedef unsigned char		uint8;
typedef signed char			int8;

// math const
#define epsilon		1.0e-10f
#define infinity	1.0e+17f
#define pi			3.1415926535897932384626f
#define inch		25.4000843476f

// j'aime pas les maj, ni les comparaisons avec NULL
#define null	NULL
#define is(s)	((s)!=NULL)
#define no(s)	((s)==NULL)

// log vers un fichier: debug(string&) ou debug(char*)
#define DEBUGFILE "D:\\infographie\\debug.log"
inline void debug(string& msg) {
	static bool init=true;
	ofstream logfile(DEBUGFILE,ios::app);
	if (init) {
		time_t	curtime;
		tm		*local;
		time(&curtime);
		local = localtime(&curtime);
		logfile << "*** File launched on " << asctime(local);
		init = false;
	}
	logfile << msg;
	logfile.flush();
}
inline void debug(char *s) { debug(string(s)); }
inline void debug(const char *s) { debug(string(s)); }
#endif
