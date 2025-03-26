# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
.SILENT:
EE_BIN_DIR = ../bin
ifdef MGMODE
MGMODEN=_$(MGMODE)
endif
EE_BIN = $(EE_BIN_DIR)/mca$(MGMODEN).elf
EE_BIN_COMPR = $(EE_BIN_DIR)/mca-packed$(MGMODEN).elf

#El_isra: define 'MGMODE' to either ARCADE, DEVELOPER or PROTOTYPE

# 'HOMEBREW' or 'XOSD'
MCMAN_TYPE ?= HOMEBREW

EE_OBJS =	res/resources.o ../PS2/GUIFramePS2Modules.o GUIMcaMan.o GUIMcaLang.o \
			../GUIFramework/IGUIFrameTimer.o ../GUIFramework/IGUIFrameRenderer.o \
			../GUIFramework/IGUIFrameTexture.o ../GUIFramework/IGUIFrameFont.o \
			../GUIFramework/IGUIFrameFont.o ../GUIFramework/IGUIFrameInput.o \
			../GUIFramework/IGUIFrameWindow.o ../GUIFramework/IGUIFrameControl.o \
			../PS2/GUIFrameTimerPS2.o \
			../PS2/GUIFrameTexturePS2.o ../PS2/GUIFrameRendererPS2.o \
			../PS2/GUIFrameInputPS2.o \
			GUIMcaHover.o \
			GUIMcaOperWnd.o GUIMcaPopup.o GUIMcaWarrningNoCard.o \
			GUIMcaGetSize.o GUIMcaGetYesNo.o GUIMcaOperProgress.o \
			GUIMcaProgressBar.o GUIMcaGetPath.o GUIMcaVkbd.o \
			GUIMcaDisplayMessage.o GUIMcaAbout.o GUIMcaCardInfo.o \
			GUIMcaTip.o GUIMcaMainWnd.o \
			PS2/PS2Application.o ../PS2/cdvd_rpc.o ../PS2/new_iop_modules.o PS2/main.o

EE_INCS =	-I../PS2/Include -I../GUIFramework/Include -I./GUIFramework/Include -I./PS2/Include \
			-I./ -I../ -I$(PS2SDK)/ee/include -I$(PS2SDK)/common/include \
			-I$(PS2DEV)/gsKit/include -I$(PS2SDK)/ports/include
EE_LIBS =	-Wl,--gc-sections \
			-T../PS2/linkfile \
			-L./ -L$(PS2SDK)/ee/lib -L$(PS2DEV)/gsKit/lib \
			-lstdc++ -lgskit -ldmakit -lpatches -lmf -lcdvd -lhdd -lfileXio -lpoweroff -lg -lpadx -lkernel#  -lcdvdfs integrated directly, borrowed from wle ISR
EE_CFLAGS = -G0 -O2 -D__LINUX__ -fno-builtin-printf -ffunction-sections -fdata-sections -Wall -Wno-deprecated
EE_CXXFLAGS = $(EE_CFLAGS)

EE_LDFLAGS += -L$(PS2DEV)/gsKit/lib -L$(PS2SDK)/ports/lib

IOP_DIRS = iop
IRXTAG = $(notdir $(addsuffix _irx, $(basename $<)))

ifeq ($(MCMAN_TYPE), HOMEBREW)
  EE_CXXFLAGS += -DUSE_HOMEBREW_MODULES
  EE_OBJS += $(addprefix ../PS2/, mcman.o sio2man.o padman.o mcserv.o)
else ifeq ($(MCMAN_TYPE), XOSD)
else
  $(error invalid MCMAN type chosen '$(MCMAN_TYPE)')
endif

ifeq ($(EXFAT),1)
  $(info -- EXFAT Support)
  EE_CFLAGS += -DEXFAT
endif

all: $(IOP_DIRS)
	mkdir -p $(EE_BIN_DIR)/
	echo "Compiling..."
	$(MAKE) $(EE_BIN)
	echo "Stripping..."
	$(EE_STRIP) --strip-all $(EE_BIN)
	echo "Compressing..."
	ps2-packer $(EE_BIN) $(EE_BIN_COMPR) | grep "File compressed"

packlang:
	cp lng/*.lng $(EE_BIN_DIR)/

ifdef MGMODE
$(info -- Building for $(MGMODE) MemoryCards)
EE_CFLAGS += -DMGMODE=\"$(MGMODE)\"
EE_LIBS += -liopreboot
EE_OBJS += ../PS2/ioprp_$(MGMODE).o

../PS2/ioprp_$(MGMODE).c: ../PS2/IOPRP_$(MGMODE).IMG
	bin2c $< $@ ioprp
endif

ifdef VERSION
#quotes just in case there are spaces there
EE_CFLAGS += -DVERSION="\"$(VERSION)\""
endif

clean:
	echo "Cleaning..."
	rm -f $(EE_BIN) $(EE_OBJS) $(EE_BIN_COMPR)
	@for dir in $(IOP_DIRS); do \
		$(MAKE) -C $$dir clean; \
	done

run:
	echo "Running..."
	ps2client execee host:$(EE_BIN)

allkeys:
	$(MAKE) clean all MGMODE=DEVELOPER
	$(MAKE) clean all MGMODE=ARCADE
	$(MAKE) clean all MGMODE=PROTOTYPE

$(IOP_DIRS):
	$(MAKE) -C $@

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal

vpath %.irx $(PS2SDK)/iop/irx/
../PS2/%.c: %.irx
	bin2c $< $@ $(IRXTAG)

DIR_GUARD = @mkdir -p $(@D)
%.o: $(EE_SRC_DIR)%.c
	@printf "\033[1m CC  - $@\033[0m\n"
	$(DIR_GUARD)
	$(EE_CC) $(EE_CFLAGS) $(EE_INCS) -c $< -o $@

%.o: $(EE_ASM_DIR)%.s
	@printf "\033[1m ASM - $@\033[0m\n"
	$(DIR_GUARD)
	$(EE_AS) $(EE_ASFLAGS) $< -o $@

%.o: $(EE_SRC_DIR)%.cpp
	@printf "\033[1m CXX - $@\033[0m\n"
	$(DIR_GUARD)
	$(EE_CXX) $(EE_CXXFLAGS) $(EE_INCS) -c $< -o $@


.PHONY: all $(IOP_DIRS)