#ifndef _GUIMCAOPERWND_H_
#define _GUIMCAOPERWND_H_

#include "IGUIFrameWindow.h"
#include "GUITypes.h"
#include "GUIMcaHover.h"

class CGUIMcaOperWnd :
	public CIGUIFrameWindow
{
private:
	int m_menu_item;
	int m_menu_item_format;
	int m_oper_slot;
	u32 m_input_state_new;
	u32 m_input_state_all;
	u32 m_ticks;
	bool m_psx_mode;
	CIGUIFrameRenderer *m_renderer;
	CIGUIFrameInput *m_input;
	CIGUIFrameTimer *m_timer;
	bool m_exit_now;

	//CGUITexture m_bgimage;
	CGUITexture m_mca_logo;
	CGUITexture m_mc_bg;

	CGUIMcaHover m_hover_menu;

	static const char *m_menu_options[];
	static const char *m_menu_format_type[];


	bool checkMessages();

	void fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out = false);
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void drawBackground(float alpha = 1.0f);
	void drawStatic(float alpha = 1.0f);
	void drawMenu(float alpha = 1.0f);
	void drawHover(float x, float y, float w, float h, float alpha = 1.0f);
public:
	CGUIMcaOperWnd(void);
	~CGUIMcaOperWnd(void);
	int display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur = false);
	void setPsxMode(bool psxmode = false);
	void setOperSlot(int operslot);
};

#endif //_GUIMCAOPERWND_H_
