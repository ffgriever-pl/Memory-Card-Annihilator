#include "resources.h"

#include "verdana18s_bfnk.h"
#include "verdana22_bfnk.h"
//#include "verdana38_bfnk.h"
//#include "verdana38p_bfnk.h"
#include "centurygoth38p_bfnk.h"
#include "urwmed20_bfnk.h"

#include "bgimg_tm2.h"
#include "bgimg_ntsc_tm2.h"

#include "popup_pal_tm2.h"
#include "popup_pal_info_tm2.h"
#include "icon_exclam_tm2.h"
#include "icon_fail_tm2.h"
#include "icon_success_tm2.h"
#include "vkbd_tm2.h"
#include "cursor_tm2.h"
#include "vkbd_hover_tm2.h"
#include "icon_dir_tm2.h"
#include "icon_file_tm2.h"
#include "popup_mgr_tm2.h"
#include "scroll_tm2.h"
#include "popup_tip_tm2.h"

#include "slot1_tm2.h"
#include "slot2_tm2.h"
#include "wyb_slot_tm2.h"
#include "ikona_mc_ps2_tm2.h"
#include "ikona_mc_psx_tm2.h"
#include "ikona_mc_unk_tm2.h"

#include "mca_logo_tm2.h"
#include "mc_bg_tm2.h"

//#include "progresbar_empty_tm2.h"
#include "progress_black_tm2.h"
#include "progress_green_tm2.h"
#include "progress_red_tm2.h"
#include "progress_lead_green_tm2.h"
#include "progress_lead_red_tm2.h"

#include "loader_elf.h"

std::string CResources::boot_path = "";
bool CResources::iopreset = false;

CGUIMcaLang CResources::mainLang;
CGUITexture CResources::m_bgimage;
CGUITexture CResources::m_popup_pal;
CGUITexture CResources::m_popup_pal_info;
CGUITexture CResources::m_popup_tip;

CGUITexture CResources::m_icon_exclam;
CGUITexture CResources::m_icon_fail;
CGUITexture CResources::m_icon_success;

CGUITexture CResources::m_vkbd;
CGUITexture CResources::m_cursor;
CGUITexture CResources::m_vkbd_hover;

CGUITexture CResources::m_icon_dir;
CGUITexture CResources::m_icon_file;
CGUITexture CResources::m_popup_mgr;
CGUITexture CResources::m_scroll;

//CGUITexture CResources::m_bar_empty;
CGUITexture CResources::m_bar_black;
CGUITexture CResources::m_bar_red;
CGUITexture CResources::m_bar_green;
CGUITexture CResources::m_bar_lead_green;
CGUITexture CResources::m_bar_lead_red;

CIGUIFrameFont<CGUITexture> CResources::verdana18s;
CIGUIFrameFont<CGUITexture> CResources::verdana22;
//CIGUIFrameFont<CGUITexture> CResources::verdana38;
//CIGUIFrameFont<CGUITexture> CResources::verdana38p;
CIGUIFrameFont<CGUITexture> CResources::centurygoth38p;
CIGUIFrameFont<CGUITexture> CResources::urwmed20;
