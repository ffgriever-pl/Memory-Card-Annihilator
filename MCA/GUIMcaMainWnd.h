#ifndef _GUIMCAMAINWND_H_
#define _GUIMCAMAINWND_H_

#include "IGUIFrameWindow.h"
#include "GUIMcaBaseWindow.h"
#include "GUITypes.h"
#include "GUIMcaHover.h"
#include "GUIMcaOperWnd.h"

class CGUIMcaMainWnd :
	public CIGUIFrameWindow,
	public CGUIMcaBaseWindow
{
private:
	int m_slot_chosen;
	u32 m_input_state_new;
	u32 m_input_state_all;
	u32 m_ticks;

	//CGUITexture m_bgimage;
	CGUITexture m_slot[2];
	CGUITexture m_wyb_slot;
	CGUITexture m_icon_ps2;
	CGUITexture m_icon_psx;
	CGUITexture m_icon_unk;

	CGUIMcaHover m_hover_slots;
	CGUIMcaOperWnd m_oper_wnd;

	#define ELF_MAGIC		0x464c457f
	#define ELF_PT_LOAD		1

	//------------------------------
	typedef struct
	{
		u8	ident[16];			// struct definition for ELF object header
		u16	type;
		u16	machine;
		u32	version;
		u32	entry;
		u32	phoff;
		u32	shoff;
		u32	flags;
		u16	ehsize;
		u16	phentsize;
		u16	phnum;
		u16	shentsize;
		u16	shnum;
		u16	shstrndx;
	} elf_header_t;
	//------------------------------
	typedef struct
	{
		u32	type;				// struct definition for ELF program section header
		u32	offset;
		void	*vaddr;
		u32	paddr;
		u32	filesz;
		u32	memsz;
		u32	flags;
		u32	align;
	} elf_pheader_t;

	bool checkMessages();

	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void drawBackground(float alpha = 1.0f);
	void drawSlots(float alpha = 1.0f);
	void drawMcIcons(float alpha = 1.0f);
	void drawChoseSlot(float alpha = 1.0f);
public:
	CGUIMcaMainWnd(CIGUIFrameRenderer* renderer, CIGUIFrameInput* input, CIGUIFrameTimer* timer);
	~CGUIMcaMainWnd(void);
	int display(bool blur = false);
};

#endif //_GUIMCAMAINWND_H_
