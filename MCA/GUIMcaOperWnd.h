#ifndef _GUIMCAOPERWND_H_
#define _GUIMCAOPERWND_H_

#include "IGUIFrameWindow.h"
#include "GUIMcaBaseWindow.h"
#include "GUITypes.h"
#include "GUIMcaHover.h"

class CGUIMcaOperWnd :
	public CIGUIFrameWindow,
	public CGUIMcaBaseWindow
{
private:
	int m_menu_item;
	int m_menu_item_format;
	int m_oper_slot;
	bool m_psx_mode;

	CGUITexture m_mca_logo;
	CGUITexture m_mc_bg;

	CGUIMcaHover m_hover_menu;

	static const char *m_menu_options[];
	static const char *m_menu_format_type[];


	bool checkMessages();

	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void drawBackground(float alpha = 1.0f);
	void drawStatic(float alpha = 1.0f);
	void drawMenu(float alpha = 1.0f);
public:
	CGUIMcaOperWnd(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer);
	~CGUIMcaOperWnd(void);
	int display(bool blur = false);
	void setPsxMode(bool psxmode = false);
	void setOperSlot(int operslot);
};

#endif //_GUIMCAOPERWND_H_
