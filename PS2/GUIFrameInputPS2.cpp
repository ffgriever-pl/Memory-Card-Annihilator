#include "Include/GUIFrameInputPS2.h"


CGUIFrameInputPS2::CGUIFrameInputPS2(void)
{
	initInput();
}

bool CGUIFrameInputPS2::initInput()
{
	if (!m_pad_init_done)
	{
		CGUIFramePS2Modules::loadPadModules();
		padInit(0);

		int ret;
		if((ret = padPortOpen(0, 0, m_padBuf[0])) == 0)
		{
			printf("CGUIFrameInputPS2: padPortOpen failed: %d\n", ret);
		} else
		{
			if(!initializePad(0, 0))
			{
				printf("CGUIFrameInputPS2: Pad initalization failed!\n");
			} else
			{
				m_pad_init_done = true;
				m_pad_detected = true;
			}
		}
		
	}
	return m_pad_detected;
}

bool CGUIFrameInputPS2::waitPadReady(int port, int slot)
{
    int state;

    state = padGetState(port, slot);
	while((state != PAD_STATE_DISCONN) && (state != PAD_STATE_STABLE) && (state != PAD_STATE_FINDCTP1)) {
        state=padGetState(port, slot);
    }
    return true;
}

bool CGUIFrameInputPS2::initializePad(int port, int slot)
{
	int state;

	m_buttons[0].btns = 0xffff;
	m_buttons[1].btns = 0xffff;

	waitPadReady(port, slot);
    state = padGetState(port, slot);
	
	if (state != PAD_STATE_DISCONN)
	{
		padSetMainMode(port, slot, PAD_MMODE_DUALSHOCK, PAD_MMODE_UNLOCK);
		return true;
	}
    return false;
}

CGUIFrameInputPS2::~CGUIFrameInputPS2(void)
{
}

bool CGUIFrameInputPS2::getSystemState()
{
	return m_pad_detected;
}

u32 CGUIFrameInputPS2::updateInternal()
{
	int ret;

	m_pad_detected = false;
	waitPadReady(0,0);
	ret=padGetState(0, 0);
	m_addy = 0;
	m_addx = 0;
	
	if((ret == PAD_STATE_STABLE) || (ret == PAD_STATE_FINDCTP1)) {
		ret = padRead(0, 0, &m_buttons[0]); // port, slot, buttons
		if (ret != 0) {
			u32 paddata;
			u32 returndata = 0;
			paddata = 0xffff ^ m_buttons[0].btns;

			if (paddata & PAD_TRIANGLE)	returndata |= enInCancel;
			if (paddata & PAD_CROSS)	returndata |= enInOk;
			if (paddata & PAD_DOWN)		returndata |= enInDown;
			if (paddata & PAD_UP)		returndata |= enInUp;
			if (paddata & PAD_LEFT)		returndata |= enInLeft;
			if (paddata & PAD_RIGHT)	returndata |= enInRight;
			if (paddata & PAD_R1)		returndata |= enInTabFwd;
			if (paddata & PAD_L1)		returndata |= enInTabBck;
			if (paddata & PAD_R2)		returndata |= enInTab2Fwd;
			if (paddata & PAD_L2)		returndata |= enInTab2Bck;
			if (paddata & PAD_SELECT)	returndata |= enInSwitch;
			if (paddata & PAD_START)	returndata |= enInMenu;
			if (paddata & PAD_SQUARE)	returndata |= enInNeutral;
			if (paddata & PAD_CIRCLE)	returndata |= enInPrev;

			int xadd = 0;
			int yadd = 0;

			if (m_buttons[0].mode == 65)
			{
				m_buttons[0].ljoy_h = 128;
				m_buttons[0].ljoy_v = 128;
				m_buttons[0].rjoy_h = 128;
				m_buttons[0].rjoy_v = 128;
			}
			//sio_printf("Pad mode = %d\n", m_buttons[0].mode);

			if (m_buttons[0].ljoy_h <= 95) xadd = -(95-m_buttons[0].ljoy_h);
			else if (m_buttons[0].ljoy_h >= 160) xadd = m_buttons[0].ljoy_h -160;
			if (m_buttons[0].ljoy_v <= 95) yadd = -(95-m_buttons[0].ljoy_v);
			else if (m_buttons[0].ljoy_v >= 160) yadd = m_buttons[0].ljoy_v -160;
			
			m_addx = (float)xadd/95.0f;
			m_addy = (float)yadd/95.0f;

			m_pad_detected = true;
			return returndata;
			//if (paddata & PAD_SELECT && paddata & PAD_START) screenshot();
		}
	}
	return 0;
}


bool CGUIFrameInputPS2::m_pad_init_done = false;
char CGUIFrameInputPS2::m_padBuf[2][256] __attribute__((aligned(64)));
struct padButtonStatus CGUIFrameInputPS2::m_buttons[2] __attribute__((aligned(64)));
bool CGUIFrameInputPS2::m_pad_detected = false;
