#ifndef _GUIMCAOPERPROGRESS_H_
#define _GUIMCAOPERPROGRESS_H_

#include "GUIMcaPopup.h"
#include "GUIMcaProgressBar.h"
#include "GUIMcaMan.h"

class CGUIMcaOperProgress :
	public CGUIMcaPopup
{
private:
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void drawMessage(float alpha = 1.0f);
	int m_result;
	int m_default;
	bool m_locked;
	CGUIMcaProgressBar m_progressBar;
	bool checkMessages();
	int display(bool blur = false);
	void resetProgress();
public:
	enum enResult
	{
		enresFail = 0,
		enresSuccess = 1,
	};
	CGUIMcaOperProgress(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x, float y);
	~CGUIMcaOperProgress(void);
    int doOperation(const CGUIMcaMan::opParams& params, void (*operation)(const CGUIMcaMan::opParams&), float blur);
	int doFormat(int slot, bool fast, bool psx, int pagestotal, bool blur = false);
	int doUnformat(int slot, bool psx, int pagestotal, bool blur = false);
	int doCreateImage(int slot, bool psx, int pagestotal, const char* path, bool blur = false);
	int doRestoreImage(int slot, bool psx, const char *path, bool blur = false);
};

#endif //_GUIMCAOPERPROGRESS_H_
