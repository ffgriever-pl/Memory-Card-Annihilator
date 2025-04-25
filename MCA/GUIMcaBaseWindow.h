#ifndef _GUIMCABASEWINDOW_H_
#define _GUIMCABASEWINDOW_H_

#include "IGUIFrameRenderer.h"
#include "IGUIFrameInput.h"
#include "IGUIFrameTimer.h"
#include "GUITypes.h"

class CGUIMcaBaseWindow
{
public:
	virtual void drawAll(CIGUIFrameTexture* prevBuffTex = NULL, float alpha = 1.0f) = 0;
	virtual void fadeInOut(CIGUIFrameTexture* prevBuffTex, u32 ms, bool out = false);
protected:
	float m_x;
	float m_y;
	CIGUIFrameRenderer* m_renderer;
	CIGUIFrameInput* m_input;
	CIGUIFrameTimer* m_timer;
	u32 m_input_state_new;
	u32 m_input_state_all;
	u32 m_ticks;
	bool m_exit;

	CGUIMcaBaseWindow(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer, float x = 0, float y = 0);
	virtual ~CGUIMcaBaseWindow();
	virtual void drawLoop(CIGUIFrameTexture* prevBuffTex = NULL, u32 exit_buttons = 0xFFFFFFFF, float alpha = 1.0f);
	/**
	* @brief Check and process input messages
	* 
	* @return true if a new window was called (requires one frame advance)
	*/
	virtual bool checkMessages();
};

#endif //_GUIMCABASEWINDOW_H_
