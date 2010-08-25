# ============================================================================
#  Name	 : build_help.mk
#  Part of  : CollectS60
# ============================================================================
#  Name	 : build_help.mk
#  Part of  : CollectS60
#
#  Description: This make file will build the application help file (.hlp)
# 
# ============================================================================

do_nothing :
	@rem do_nothing

# build the help from the MAKMAKE step so the header file generated
# will be found by cpp.exe when calculating the dependency information
# in the mmp makefiles.

MAKMAKE : CollectS60.hlp
CollectS60.hlp : CollectS60.xml CollectS60.cshlp Custom.xml
	cshlpcmp CollectS60.cshlp
ifeq (WINS,$(findstring WINS, $(PLATFORM)))
	copy CollectS60.hlp $(EPOCROOT)epoc32\$(PLATFORM)\c\resource\help
endif

BLD : do_nothing

CLEAN :
	del CollectS60.hlp
	del CollectS60.hlp.hrh

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE : do_nothing
		
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo CollectS60.hlp

FINAL : do_nothing
