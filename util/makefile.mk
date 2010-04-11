EXTERNAL_WARNINGS_NOT_ERRORS := TRUE

PRJ=..

PRJNAME=scsolver
TARGET=scsolver

# --- Settings -----------------------------------------------------------

.INCLUDE :	settings.mk

# --- Allgemein ----------------------------------------------------------

LIB1TARGET= $(SLB)$/$(TARGET).uno.lib
LIB1FILES=	$(SLB)$/numeric.lib	\
		$(SLB)$/service.lib	\
		$(SLB)$/tool.lib	\
		$(SLB)$/ui.lib

SHL1TARGET= $(TARGET).uno
SHL1IMPLIB= i$(SHL1TARGET)
SHL1DEPN=	makefile.mk
SHL1VERSIONMAP=$(PRJNAME).map
SHL1DEF=$(MISC)$/$(SHL1TARGET).def
DEF1NAME=$(SHL1TARGET)
SHL1LIBS=$(LIB1TARGET)
SHL1STDLIBS= \
	$(CPPUHELPERLIB) \
	$(CPPULIB) \
	$(SALLIB)

.IF "$(SYSTEM_LPSOLVE)" != "YES"
# statically link lpsolve
.IF "$(GUI)" == "WNT"
SHL1STDLIBS += $(SOLARVER)$/$(INPATH)$/lib$/lpsolve55.lib
.ELSE
SHL1STDLIBS += $(SOLARVER)$/$(INPATH)$/lib$/liblpsolve55.a
.ENDIF
.ELSE
# system lpsolve
SHL1STDLIBS += $(LPSOLVELIB)
.ENDIF

RESLIB1NAME=scsolver
RESLIB1SRSFILES= $(SRS)$/scsolver.srs
.IF "$(GUI)" == "WNT"
SHL1TARGETPATH= $(BIN)$/$(DLLPRE)$(SHL1TARGET)$(DLLPOST)
PYTHON = unset PYTHONHOME PYTHONPATH; /usr/bin/python
.ELSE
SHL1TARGETPATH= $(LB)$/$(DLLPRE)$(SHL1TARGET)$(DLLPOST)
PYTHON = python
.ENDIF

EXTPACKFILES= \
	$(BIN)$/$(SHL1TARGET).oxt \
	$(BIN)$/$(SHL1TARGET).rdb \
	..$/ext$/translation \
	..$/ext$/help \
	..$/ext$/META-INF \
	..$/ext$/description.xml \
	..$/ext$/*.xcu \
    $(SHL1TARGETPATH)

EXTPACKPATH= \
	-p ..$/ext \
	-p $(BIN) \
	-p $(LB)

# --- Targets ------------------------------------------------------------

.INCLUDE :	target.mk

COMP=build_ext

ALLTAR: $(COMP)

build_ext:
        $(PYTHON) ..$/bin$/make_manifest.py ..$/ext$/META-INF$/manifest.xml $(DLLPRE)$(SHL1TARGET)$(DLLPOST)
        sleep 2
        $(REGCOMP) -register -wop -r $(BIN)/$(SHL1TARGET).rdb -c $(SHL1TARGETPATH)
        $(PYTHON) ..$/bin$/pack_extension.py $(EXTPACKPATH) $(EXTPACKFILES)
