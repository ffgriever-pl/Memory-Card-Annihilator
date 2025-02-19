#ifndef _GUIFRAMEINPUTPS2_H_
#define _GUIFRAMEINPUTPS2_H_

#include "GUIFramePS2Includes.h"
#include "GUIFramePS2Modules.h"
#include "IGUIFrameInput.h"

class CGUIFrameInputPS2 :
	public CIGUIFrameInput
{
private:
	static bool m_pad_init_done;
	static char m_padBuf[2][256] __attribute__((aligned(64)));
	static struct padButtonStatus m_buttons[2] __attribute__((aligned(64)));
	static bool m_pad_detected;
	bool initializePad(int port, int slot);
	bool waitPadReady(int port, int slot);
	u32 updateInternal();
public:
	CGUIFrameInputPS2(void);
	~CGUIFrameInputPS2(void);

	bool getSystemState();
	bool initInput();
	void getAdditive(float &x, float &y);
};

#endif //_GUIFRAMEINPUTPS2_H_
