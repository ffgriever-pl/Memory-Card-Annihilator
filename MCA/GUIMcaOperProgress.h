#ifndef _GUIMCAOPERPROGRESS_H_
#define _GUIMCAOPERPROGRESS_H_

#include "GUIMcaPopup.h"
#include "GUIMcaProgressBar.h"

class CGUIMcaOperProgress :
	public CGUIMcaPopup
{
private:
	u32 m_input_state_new;
	u32 m_input_state_all;
	u32 m_ticks;
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void drawMessage(float alpha = 1.0f);
	int m_result;
	int m_default;
	bool m_return;
	bool m_locked;
	CGUIMcaProgressBar m_progressBar;
	bool checkMessages();
	int display(bool blur = false);
public:
	enum enResult
	{
		enresFail = 0,
		enresSuccess = 1,
	};
	CGUIMcaOperProgress(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y);
	~CGUIMcaOperProgress(void);
	int doFormat(int slot, bool fast, bool psx, int pagestotal, bool blur = false);
	int doUnformat(int slot, bool psx, int pagestotal, bool blur = false);
	int doCreateImage(int slot, bool psx, int pagestotal, const char* path, bool blur = false);
	int doRestoreImage(int slot, bool psx, const char *path, bool blur = false);
};

#endif //_GUIMCAOPERPROGRESS_H_
