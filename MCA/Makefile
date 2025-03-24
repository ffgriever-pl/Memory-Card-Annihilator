# _____     ___ ____     ___ ____
#  ____|   |    ____|   |        | |____|
# |     ___|   |____ ___|    ____| |    \    PS2DEV Open Source Project.
.SILENT:

EE_BIN = ../bin/mca2warn.elf
EE_BIN_COMPR = ../bin/mca2warn-pack.elf

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
			PS2/PS2Application.o ../PS2/cdvd_rpc.o PS2/main.o

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

all: 
	mkdir -p ../bin/
	echo "Compiling..."
	$(MAKE) $(EE_BIN)
	echo "Stripping..."
	$(EE_STRIP) --strip-all $(EE_BIN)
	echo "Compressing..."
	ps2-packer $(EE_BIN) $(EE_BIN_COMPR) | grep "File compressed"

clean:
	echo "Cleaning..."
	rm -f $(EE_BIN) $(EE_OBJS) $(EE_BIN_COMPR)

run:
	echo "Running..."
	ps2client execee host:$(EE_BIN)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal

