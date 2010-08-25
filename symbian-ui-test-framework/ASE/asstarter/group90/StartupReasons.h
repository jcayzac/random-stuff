/*
NOKIA MOBILE PHONES
WIRELESS DATA
Calypso SW









            ECUST
            ---------------

            SW Module Document - C++








Module name:    -

Filename:       startupreasons.h

Working copy: 


Copyright (c) 2000-2001. Nokia Mobile Phones. All rights reserved.


Change History:

VERSION     : 5				Draft			08-Oct-2001       Ilkka Korhonen
REASON      : 
REFERENCE   : 
DESCRIPTION : Updated comments

VERSION     : 4				Draft			26-Feb-2001       Ilkka Korhonen
REASON      : Added comments
REFERENCE   : 
DESCRIPTION : 

VERSION     : 3				Draft			16-Jan-2001       Ilkka Korhonen
REASON      : Removed ","
REFERENCE   : 
DESCRIPTION : 

VERSION     : 2				Draft			09-Nov-2000       Ilkka Korhonen
REASON      : Start-up reasons updated
REFERENCE   : 
DESCRIPTION : 

VERSION     : 1				Draft			28-Feb-2000       Ilkka Korhonen
REASON      : Initial version
REFERENCE   : 
DESCRIPTION : 
----------------------------------------------------------------------------
*/

#ifndef __STARTUPREASONS__H__
#define __STARTUPREASONS__H__

//#define MTC_STARTUP_REASONS

/* Calypso specific startup reasons */
#ifndef MTC_STARTUP_REASONS
enum TWD2StartupReason
{		
	ENormal,							/* Normal start-up */
	EAlarm,								/* RTC alarm */
	EMidnightAlarm,						/* RTC midnight alarm, not available to applications */
	EHiddenReset,						/* Hidden reset, not available to applications as start-up reason*/
	ECharger,							/* Charger connected */
	ETest,								/* Test mode */
	ELocal,								/* Local mode. Not available to applications, applications will be in Test*/
	ESelftestFail,						/* Not available to applications */
	ENotReadFromHardware,				/* Not available to applications */
	ENotKnown							/* Not available to applications */
};
#else
/* New startup reasons, mapped to MTC state information defined in isi_mtc.h */
enum TWD2StartupReason  // TMTCStateSymbolInformation   // The enum name HAS TO be changed!!! 
    {
    EPowerOff,
    ENormal,
    ECharger,
    EAlarm,
    ETest,
    ELocal,
    EWarranty,
    EReliability,
    ESelftestFail,
    ESwdl,
    ERFInactive,
    EIDWrite,
    EDischarging,
    EHiddenReset
    };
#endif

// Type definition for compatibility reasons during phase-over
//typedef TMTCStateSymbolInformation TWD2StartupReason;

#endif

// End of File
