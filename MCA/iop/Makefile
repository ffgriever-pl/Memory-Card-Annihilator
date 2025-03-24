# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
#-----------------------------------------------------------------------
# Copyright 2001-2004, ps2dev - http://www.ps2dev.org
# Licenced under Academic Free License version 2.0

.SILENT:
IOP_BIN  = mca.irx
IOP_HEADER = mca_irx
IOP_OBJS = mca.o imports.o formatter.o

IOP_CFLAGS  += -Wall -fno-builtin -G0 -O2
IOP_LDFLAGS += -s

#PS2SDK seems to rely on it
ECHO=echo

all:
	echo "Compiling ${IOP_BIN} module..."
	$(MAKE) $(IOP_BIN)
	bin2c $(IOP_BIN) $(IOP_HEADER).h $(IOP_HEADER)
	sed -i 's/size_$(IOP_HEADER)/CGUIMcaMan::size_$(IOP_HEADER)/g' $(IOP_HEADER).h
	sed -i 's/$(IOP_HEADER)\[\]/CGUIMcaMan::$(IOP_HEADER)\[\]/g' $(IOP_HEADER).h

clean:
	echo "Cleaning ${IOP_BIN}..."
	rm -f -r $(IOP_OBJS) $(IOP_BIN) $(IOP_HEADER).h

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.iopglobal

%.o: $(IOP_SRC_DIR)%.c
	@printf "\033[1m CC  - $@\033[0m\n"
	$(IOP_CC) $(IOP_CFLAGS) $(IOP_INCS) -c $< -o $@

%.o: $(IOP_SRC_DIR)%.s
	@printf "\033[1m ASM - $@\033[0m\n"
	$(IOP_AS) $(IOP_ASFLAGS) $< -o $@
