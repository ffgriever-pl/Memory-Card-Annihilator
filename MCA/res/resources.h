#ifndef _RESOURCES_H_
#define _RESOURCES_H_ 1

#include "GUIFramePS2Includes.h"
//#include "../../GUIFramework/Include/IGUIFrameFont.h
#include "IGUIFrameFont.h"
#include "GUITypes.h"
#include "../GUIMcaLang.h"

class CResources
{
public:
	//lang
	static CGUIMcaLang mainLang;
	static std::string boot_path;
	static bool iopreset;
	//fonts
	static unsigned int size_verdana18s_bfnk;
	static unsigned char verdana18s_bfnk[];
	static unsigned int size_verdana22_bfnk;
	static unsigned char verdana22_bfnk[];
	/*static unsigned int size_verdana38_bfnk;
	static unsigned char verdana38_bfnk[];
	static unsigned int size_verdana38p_bfnk;
	static unsigned char verdana38p_bfnk[];*/
	static unsigned int size_centurygoth38p_bfnk;
	static unsigned char centurygoth38p_bfnk[];

	static CIGUIFrameFont<CGUITexture> verdana18s;
	static CIGUIFrameFont<CGUIFrameTexturePS2> verdana22;
	/*static CIGUIFrameFont<CGUIFrameTexturePS2> verdana38;
	static CIGUIFrameFont<CGUIFrameTexturePS2> verdana38p;*/
	static CIGUIFrameFont<CGUIFrameTexturePS2> centurygoth38p;
	
	//universal
	static unsigned int size_bgimg_tm2;
	static unsigned char bgimg_tm2[];
	static unsigned int size_bgimg_ntsc_tm2;
	static unsigned char bgimg_ntsc_tm2[];
	static unsigned int size_popup_pal_tm2;
	static unsigned char popup_pal_tm2[];
	static unsigned int size_popup_pal_info_tm2;
	static unsigned char popup_pal_info_tm2[];
	static unsigned int size_popup_tip_tm2;
	static unsigned char popup_tip_tm2[];

	static unsigned int size_icon_exclam_tm2;
	static unsigned char icon_exclam_tm2[];
	static unsigned int size_icon_success_tm2;
	static unsigned char icon_success_tm2[];
	static unsigned int size_icon_fail_tm2;
	static unsigned char icon_fail_tm2[];

	static unsigned int size_vkbd_hover_tm2;
	static unsigned char vkbd_hover_tm2[];

	static unsigned int size_cursor_tm2;
	static unsigned char cursor_tm2[];

	static unsigned int size_vkbd_tm2;
	static unsigned char vkbd_tm2[];

	static CGUITexture m_bgimage;
	static CGUITexture m_popup_pal;
	static CGUITexture m_popup_pal_info;
	static CGUITexture m_popup_tip;

	static CGUITexture m_icon_exclam;
	static CGUITexture m_icon_fail;
	static CGUITexture m_icon_success;

	static CGUITexture m_vkbd;
	static CGUITexture m_cursor;
	static CGUITexture m_vkbd_hover;

	//manager
	static unsigned int size_icon_dir_tm2;
	static unsigned char icon_dir_tm2[];
	static unsigned int size_icon_file_tm2;
	static unsigned char icon_file_tm2[];
	static unsigned int size_popup_mgr_tm2;
	static unsigned char popup_mgr_tm2[];
	static unsigned int size_scroll_tm2;
	static unsigned char scroll_tm2[];

	static CGUITexture m_icon_dir;
	static CGUITexture m_icon_file;
	static CGUITexture m_popup_mgr;
	static CGUITexture m_scroll;

	//mainwnd
	static unsigned int size_slot1_tm2;
	static unsigned char slot1_tm2[];
	static unsigned int size_slot2_tm2;
	static unsigned char slot2_tm2[];
	static unsigned int size_wyb_slot_tm2;
	static unsigned char wyb_slot_tm2[];
	static unsigned int size_ikona_mc_ps2_tm2;
	static unsigned char ikona_mc_ps2_tm2[];
	static unsigned int size_ikona_mc_psx_tm2;
	static unsigned char ikona_mc_psx_tm2[];
	static unsigned int size_ikona_mc_unk_tm2;
	static unsigned char ikona_mc_unk_tm2[];

	//PS2/PSX Memory Card
	static unsigned int size_mca_logo_tm2;
	static unsigned char mca_logo_tm2[];
	static unsigned int size_mc_bg_tm2;
	static unsigned char mc_bg_tm2[];

	//progress bar
	/*static unsigned int size_progresbar_empty_tm2;
	static unsigned char progresbar_empty_tm2[];
	static CGUITexture m_bar_empty;*/
	static unsigned int size_progress_black_tm2;
	static unsigned char progress_black_tm2[];
	static CGUITexture m_bar_black;
	static unsigned int size_progress_red_tm2;
	static unsigned char progress_red_tm2[];
	static CGUITexture m_bar_red;
	static unsigned int size_progress_green_tm2;
	static unsigned char progress_green_tm2[];
	static CGUITexture m_bar_green;

	static unsigned int size_progress_lead_green_tm2;
	static unsigned char progress_lead_green_tm2[];
	static CGUITexture m_bar_lead_green;
	static unsigned int size_progress_lead_red_tm2;
	static unsigned char progress_lead_red_tm2[];
	static CGUITexture m_bar_lead_red;

	//elf loader
	static unsigned int size_loader_elf;
	static unsigned char loader_elf[];
};

#endif
