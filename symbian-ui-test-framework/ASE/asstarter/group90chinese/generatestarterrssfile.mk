# Modified by SDK_team Mkfiles.pl script
# Build resource files
#

PROJECT=starter
SOURCEDIR=.
LANGUAGE=sc

#
# Target directory
#

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
TARGETDIR=$(EPOCROOT)Epoc32\Release\$(PLATFORM)\$(CFG)\Z\System\Programs
else
TARGETDIR=$(EPOCROOT)Epoc32\Data\Z\System\Programs
endif

$(TARGETDIR) :
	@perl $(EPOCROOT)epoc32\tools\emkdir.pl $(TARGETDIR)

#
# Build the mtm resource files
#

SOURCERESOURCE=$(SOURCEDIR)\$(PROJECT).rss
TARGETRESOURCE=$(TARGETDIR)\$(PROJECT).r$(LANGUAGE)
$(TARGETRESOURCE) : $(TARGETDIR) $(SOURCERESOURCE)
	@copy $(SOURCERESOURCE) $(TARGETDIR)\$(PROJECT).rss
	@epocrc.bat -u -I. -I%EPOCROOT%epoc32\include -I%EPOCROOT%epoc32\include\internal $(TARGETDIR)\$(PROJECT).rss -o$(TARGETRESOURCE)
	@del $(TARGETDIR)\$(PROJECT).rss

do_nothing:
	rem do nothing

#
# The targets invoked by abld
#

MAKMAKE : do_nothing

RESOURCE : $(TARGETRESOURCE)

SAVESPACE : BLD

BLD : do_nothing

FREEZE : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

FINAL : do_nothing

CLEAN :
	@erase $(TARGETRESOURCE)

RELEASABLES :
	@echo $(TARGETRESOURCE)