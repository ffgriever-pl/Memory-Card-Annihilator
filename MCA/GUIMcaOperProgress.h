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
	void fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out = false);
	void drawMessage(float alpha = 1.0f);
	int m_result;
	int m_default;
	bool m_return;
	bool m_locked;
	CGUIMcaProgressBar *m_progressBar;
	CGUIMcaOperProgress(void);
	bool checkMessages();
	int display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur = false);
public:
	enum enResult
	{
		enresFail = 0,
		enresSuccess = 1,
	};
	CGUIMcaOperProgress(float x, float y);
	~CGUIMcaOperProgress(void);
	int doFormat(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, int slot, bool fast, bool psx, int pagestotal, bool blur = false);
	int doUnformat(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, int slot, bool psx, int pagestotal, bool blur = false);
	int doCreateImage(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, int slot, bool psx, int pagestotal, const char* path, bool blur = false);
	int doRestoreImage(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, int slot, bool psx, const char *path, bool blur = false);
};

#endif //_GUIMCAOPERPROGRESS_H_
