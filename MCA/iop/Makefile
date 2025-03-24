# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0

.SILENT:
IOP_BIN  = mca.irx
IOP_OBJS = mca.o imports.o formatter.o

IOP_CFLAGS  += -Wall -fno-builtin -G0 -O2
IOP_LDFLAGS += -s

#PS2SDK seems to rely on it
ECHO=echo

all:
	echo "Compiling ${IOP_BIN} module..."
	$(MAKE) $(IOP_BIN)

clean:
	echo "Cleaning..."
	rm -f -r $(IOP_OBJS) $(IOP_BIN)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.iopglobal
