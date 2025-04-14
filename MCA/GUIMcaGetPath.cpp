#include <string>
#include "GUIMcaGetPath.h"
#include "GUIFramePS2Modules.h"
#include "res/resources.h"
#include "GUIFramePS2Includes.h"
#include "GUIMcaVkbd.h"
#include "GUIMcaMan.h"

#include <libcdvd-common.h>
#define cdStatus sceCdStatus
#define SCECdvdDriveStates SCECdvdDriveState
#define CDVD_STAT_STOP     SCECdStatStop
#define CDVD_STAT_OPEN     SCECdStatShellOpen
#define CDVD_STAT_SPIN     SCECdStatSpin
#define CDVD_STAT_READ     SCECdStatRead
#define CDVD_STAT_PAUSE    SCECdStatPause
#define CDVD_STAT_SEEK     SCECdStatSeek
#define CDVD_STAT_ERROR    SCECdStatEmg

#define LIST_POSITION (11)
CGUIMcaGetPath::CGUIMcaGetPath(float x, float y, const char *defaultname)
	: m_hover_files(NULL)
	, m_tips(146, 300, 25000)
{
	CResources::m_icon_dir.loadTextureBuffer(CResources::icon_dir_tm2, CResources::size_icon_dir_tm2, true);
	CResources::m_icon_file.loadTextureBuffer(CResources::icon_file_tm2, CResources::size_icon_file_tm2, true);
	CResources::m_popup_mgr.loadTextureBuffer(CResources::popup_mgr_tm2, CResources::size_popup_mgr_tm2, true);
	CResources::m_scroll.loadTextureBuffer(CResources::scroll_tm2, CResources::size_scroll_tm2, true);

	CResources::m_icon_dir.setTexfilter(CIGUIFrameTexture::etFiltBilinear);
	CResources::m_icon_file.setTexfilter(CIGUIFrameTexture::etFiltBilinear);
	CResources::m_popup_mgr.setTexfilter(CIGUIFrameTexture::etFiltBilinear);
	CResources::m_scroll.setTexfilter(CIGUIFrameTexture::etFiltBilinear);

	m_x = x;
	m_y = y;
	m_defaultname = defaultname;
	m_save = false;
	m_curr_dir = NULL;
	m_prevBuffTex = NULL;
	m_curr_path = NULL;
	m_mask.clear();

	m_keyb_tip_shown = false;
	m_mask_enabled = false;
	m_maskItems = 1;

	m_hover_files = new CGUIMcaHover(m_x+9, m_y+33, 468, 21, 0.01f, 600, 255, 255, 255, 96, 96, 96, 0.70f, 0.20f, true);
}

CGUIMcaGetPath::CGUIMcaGetPath(void)
	: m_hover_files(NULL)
	, m_tips(146, 300, 25000)
{
	m_curr_dir = NULL;
	m_prevBuffTex = NULL;
	m_curr_path = NULL;

	m_keyb_tip_shown = false;
	m_mask_enabled = false;
	m_maskItems = 1;
	m_mask.clear();
}


CGUIMcaGetPath::~CGUIMcaGetPath(void)
{
	if (m_hover_files) delete m_hover_files;
	m_hover_files = NULL;
}

void CGUIMcaGetPath::clearMask()
{
	m_mask.clear();
}

void CGUIMcaGetPath::addMaskEntry(std::string mask)
{
	m_mask.push_back(mask);
}

void CGUIMcaGetPath::drawMngrWnd(float alpha)
{
	m_renderer->drawSpriteT(
		&CResources::m_popup_mgr
		, m_x, m_y
		, CResources::m_popup_mgr.getWidth(), CResources::m_popup_mgr.getHeight()
		, 0, 0
		, CResources::m_popup_mgr.getWidth(), CResources::m_popup_mgr.getHeight()
		, 128, 128, 128, alpha*0.90f
	);
}

bool CGUIMcaGetPath::checkMessagesTop(bool topdir)
{
	bool windowCalled = false;
	bool pos_changed = false;
	bool pos_instant = false;
	m_no_out = false;
	CGUIMcaMan::updateMca();

	if (m_curr_dir)
	{
		if (topdir)
		{
			static int massnum = 0;
			bool currmassfound = false;
			bool currcdromfound = false;
			bool currmc0found = false;
			bool currmc1found = false;

			std::list<t_fileentry>::iterator k;
			std::string massname = "mass0:";
			massname[4] = '0'+massnum;
			for (k = m_curr_dir->begin(); k != m_curr_dir->end();)
			{
				if ( (*k).name == massname)
				{
					fio_stat_t chk_stat;
					massname += "/";
					if(fioGetstat(massname.c_str(), &chk_stat) < 0)
					{
						k = m_curr_dir->erase(k);
						m_listChosen = 0;
						m_listBegin = 0;
						pos_instant = true;
						pos_changed = true;
					} else k++;
					currmassfound = true;
				} else if ( (*k).name == "cdfs:")
				{
					int type = sceCdGetDiskType();

					switch (type)
					{
						case CDVD_TYPE_PS1CD:
						case CDVD_TYPE_PS1CDDA:
						case CDVD_TYPE_PS2CD:
						case CDVD_TYPE_PS2CDDA:
						case CDVD_TYPE_PS2DVD:
							k++;
							break;
						default:
							k = m_curr_dir->erase(k);
							m_listChosen = 0;
							m_listBegin = 0;
							pos_instant = true;
							pos_changed = true;
							break;
					}
					currcdromfound = true;
				} else if ( (*k).name == "mc0:")
				{
					if(CGUIMcaMan::mce_memcards[0].type == 0)
					{
						k = m_curr_dir->erase(k);
						m_listChosen = 0;
						m_listBegin = 0;
						pos_instant = true;
						pos_changed = true;
					} else k++;
					currmc0found = true;
				} else if ( (*k).name == "mc1:")
				{
					if(CGUIMcaMan::mce_memcards[1].type == 0)
					{
						k = m_curr_dir->erase(k);
						m_listChosen = 0;
						m_listBegin = 0;
						pos_instant = true;
						pos_changed = true;
					} else k++;
					currmc1found = true;
				} else
				{
					k++;
				}
			}
			if (!currmassfound)
			{
				fio_stat_t chk_stat;
				std::string chkmassname = massname + "/";
				if(fioGetstat(chkmassname.c_str(), &chk_stat) >= 0)
				{
					m_curr_dir->push_back(t_fileentry(massname.c_str(), true));
					m_curr_dir->sort();
					m_listChosen = 0;
					m_listBegin = 0;
					pos_instant = true;
					pos_changed = true;
				}
			}
			if (!currcdromfound && !m_save)
			{
				int type = sceCdGetDiskType();

				switch (type)
				{
					case CDVD_TYPE_PS1CD:
					case CDVD_TYPE_PS1CDDA:
					case CDVD_TYPE_PS2CD:
					case CDVD_TYPE_PS2CDDA:
					case CDVD_TYPE_PS2DVD:
						m_curr_dir->push_back(t_fileentry("cdfs:", true));
						m_curr_dir->sort();
						m_listChosen = 0;
						m_listBegin = 0;
						pos_instant = true;
						pos_changed = true;
						break;
					default:
						break;
				}
			}
			if (!currmc0found)
			{
				if(CGUIMcaMan::mce_memcards[0].type != 0)
				{
					m_curr_dir->push_back(t_fileentry("mc0:", true));
					m_curr_dir->sort();
					m_listChosen = 0;
					m_listBegin = 0;
					pos_instant = true;
					pos_changed = true;
				}
			}
			if (!currmc1found)
			{
				if(CGUIMcaMan::mce_memcards[1].type != 0)
				{
					m_curr_dir->push_back(t_fileentry("mc1:", true));
					m_curr_dir->sort();
					m_listChosen = 0;
					m_listBegin = 0;
					pos_instant = true;
					pos_changed = true;
				}
			}

			massnum++;
			if (massnum > 9) massnum = 0;
		}
		int numentr = 0;
		m_maskItems = numentr = m_curr_dir->size();

		if (m_mask_enabled && m_mask.size() > 0)
		{
			std::list<t_fileentry>::const_iterator i;
			std::string currstring;
			int maskentr = 0;

			for (i = m_curr_dir->begin(); i != m_curr_dir->end(); i++)
			{
				if (!(*i).directory && m_mask_enabled)
				{
					bool mask_match = false;
					for (u32 msknum = 0; msknum < m_mask.size(); msknum++)
					{
						const std::string &filename = (*i).name;
						if (filename.size() >= m_mask[msknum].size())
						{
							if ( filename.substr( filename.size() - m_mask[msknum].size(), m_mask[msknum].size() ) == m_mask[msknum] )
							{
								mask_match = true;
								break;
							}
						}
					}
					if (!mask_match) continue;
				}
				maskentr++;
			}
			m_maskItems = numentr = maskentr;
		}

		if (numentr)
		{
			if (m_input_state_new & CIGUIFrameInput::enInL2 || m_input_state_new & CIGUIFrameInput::enInR2)
			{
				if (m_mask.size() > 0)
				{
					m_listChosen = 0;
					m_listBegin = 0;
					pos_instant = true;
					pos_changed = true;

					m_mask_enabled = !m_mask_enabled;

					if (m_mask_enabled)
					{
						std::string fullmsg = CResources::mainLang.getText("LNG_TIP_MASK_ENABLED");
						fullmsg += "\n(";
						for (u32 i = 0; i < m_mask.size(); i++)
						{
							if (i == 0) fullmsg += "*";
							else fullmsg += ", *";
							fullmsg += m_mask[i];
						}
						fullmsg += ")";
						m_tips.addTip(fullmsg, 0, 0, 0, 1.0f, 60000, true);
					} else
					{
						m_tips.addTip(CResources::mainLang.getText("LNG_TIP_MASK_DISABLED"), 0, 0, 0, 1.0f, 60000, true);
					}
				} else
				{
					m_mask_enabled = false;
				}
			}
			if (m_input_state_new & CIGUIFrameInput::enInDown)
			{
				m_listChosen++;
				if (m_listChosen >= numentr)
				{
					m_listChosen = 0;
					m_listBegin = 0;
					pos_instant = true;
				}
				if (m_listChosen - m_listBegin >= LIST_POSITION)
				{
					m_listBegin++;
				}
				pos_changed = true;
			} else if (m_input_state_new & CIGUIFrameInput::enInUp)
			{
				m_listChosen--;
				if (m_listChosen < 0)
				{
					m_listChosen = numentr-1;
					m_listBegin = m_listChosen-(LIST_POSITION-1);
					if (m_listBegin < 0) m_listBegin = 0;
					pos_instant = true;
				}
				if (m_listChosen < m_listBegin)
				{
					m_listBegin = m_listChosen;
				}
				pos_changed = true;
			} else if (m_input_state_new & CIGUIFrameInput::enInR1)
			{
				m_listChosen += LIST_POSITION;
				if (m_listChosen >= numentr)
				{
					m_listChosen = numentr-1;
				}
				m_listBegin = m_listChosen-(LIST_POSITION-1);
				if (m_listBegin < 0) m_listBegin = 0;
				pos_changed = true;
				pos_instant = true;
			} else if (m_input_state_new & CIGUIFrameInput::enInL1)
			{
				m_listChosen -= LIST_POSITION;
				if (m_listChosen < 0)
				{
					m_listChosen = 0;
				}
				m_listBegin = m_listChosen;
				pos_changed = true;
				pos_instant = true;
			} else if (m_input_state_new & CIGUIFrameInput::enInCross)
			{
				std::list<t_fileentry>::const_iterator i;
				std::string currstring;
				int pos = 0;
				for (i = m_curr_dir->begin(); i != m_curr_dir->end(); i++)
				{
					if (!(*i).directory && m_mask_enabled && m_mask.size() > 0)
					{
						bool mask_match = false;
						for (u32 msknum = 0; msknum < m_mask.size(); msknum++)
						{
							const std::string &filename = (*i).name;
							if (filename.size() >= m_mask[msknum].size())
							{
								if ( filename.substr( filename.size() - m_mask[msknum].size(), m_mask[msknum].size() ) == m_mask[msknum] )
								{
									mask_match = true;
									break;
								}
							}
						}
						if (!mask_match) continue;
					}
					pos++;
					if (pos-1 == m_listChosen) break;
				}
				if ((*i).name == "..")
				{
					m_exit_one = true;
					return false;
				}
				if ((*i).name == "mc0:" || (*i).name == "mc1:") CGUIFramePS2Modules::loadMcModules();
				std::string masslabel = "mass0:";
				for (int massnum = 0; massnum < 10; massnum++)
				{
					masslabel[4] = '0'+massnum;
					if ((*i).name == masslabel) CGUIFramePS2Modules::loadUsbModules();
				}
				if ((*i).name == "hdd0:") CGUIFramePS2Modules::loadHddModules();
				if ((*i).name == "cdfs:") CGUIFramePS2Modules::loadCdvdModules();
				
				m_all_path.push_back((*i).name);
				if ((*i).directory)
				{
					//enter
					doSubLevel(m_prevBuffTex);
					windowCalled = true;
					if (m_hover_files) m_hover_files->setDest(m_x+9, m_y+33 + (m_listChosen - m_listBegin)*21, true);
					return windowCalled;
				} else
				{
					m_exit = true;
				}
			}
			if (m_save && m_input_state_new & CIGUIFrameInput::enInStart && m_all_path.size() > 0 && !(m_all_path.size() == 1 && m_all_path.front() == "hdd0:") )
			{
				CGUIMcaVkbd myKbd(74, 170);
				std::string kbdret;
				fadeInOut(m_prevBuffTex, m_timer, 25000, true);
				if (myKbd.getEntry(m_renderer, m_input, m_timer, m_defaultname, kbdret, 32, m_prevBuffTex, true, true) == 1)
				{
					//check if proper name
					if (kbdret.size() > 0)
					{
						m_all_path.push_back(kbdret);
						m_no_out = true;
						m_exit = true;
					} else
					{
						fadeInOut(m_prevBuffTex, m_timer, 25000, false);
					}
				} else
				{
					fadeInOut(m_prevBuffTex, m_timer, 25000, false);
				}
				windowCalled = true;
				return windowCalled;
			}
			if (m_hover_files && pos_changed) m_hover_files->setDest(m_x+9, m_y+33 + (m_listChosen - m_listBegin)*21, pos_instant);
		}
	}

	if (m_input_state_new & CIGUIFrameInput::enInTriangle)
	{
		m_exit_one = true;
	}

	return windowCalled;
}

void CGUIMcaGetPath::fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out)
{
	u32 currTick = 0, oldTick = 0;
	currTick = oldTick = timer->getTicks();

	float alpha = 0.0f;
	u32 ticks = 0;
	do
	{
		ticks = currTick - oldTick;
		alpha = (float)ticks/(float)ms;
		if (alpha > 1.0f) alpha = 1.0f;
		if (out) alpha = 1.0f - alpha;

		drawAll(prevBuffTex, alpha);
		m_renderer->swapBuffers();

		currTick = timer->getTicks();
	} while (ticks <= ms);
	drawAll(prevBuffTex, alpha);
	m_renderer->swapBuffers();
}

void CGUIMcaGetPath::drawBoxArea(float alpha)
{
	bool drawbar = false;
	float barprogress = 0.0f;

	if (m_curr_dir)
	{
		int dirnum = m_curr_dir->size();
		if (m_maskItems < dirnum) dirnum = m_maskItems;
		if (dirnum > LIST_POSITION)
		{
			drawbar = true;
			barprogress = (float)m_listChosen/(float)(dirnum-1);
		}
	}

	if (drawbar)
	{
		m_renderer->drawSpriteT(
			&CResources::m_scroll
			, m_x+482, m_y+27.0f +229.0f*barprogress
			, CResources::m_scroll.getWidth(), CResources::m_scroll.getHeight()
			, 0, 0
			, CResources::m_scroll.getWidth(), CResources::m_scroll.getHeight()
			, 128, 128, 128, alpha
		);
	}

	if (m_curr_path)
	{
		m_renderer->setScissor(true, (int)m_x +4, (int)m_y+3, (int)m_x +374, (int)m_y +23);
		CResources::verdana18s.printUTF8Shadow(m_curr_path->c_str()
			, m_x + 6, m_y + 3 + CResources::verdana18s.getBasePos()
			, 00.0f, 0
			, 0, 0, 0, alpha
		);
		m_renderer->setScissor(false);
	}
}

void CGUIMcaGetPath::drawFilelist(float alpha)
{
	if (m_curr_dir)
	{
		int line = 0;
		int pos = 0;
		std::list<t_fileentry>::const_iterator i;
		std::string currstring;

		if (m_hover_files && m_curr_dir->size() > 0) m_hover_files->drawHover(m_renderer, m_ticks, alpha);
		for (i = m_curr_dir->begin(); i != m_curr_dir->end(); i++)
		{
			if (!(*i).directory && m_mask_enabled)
			{
				bool mask_match = false;
				for (u32 msknum = 0; msknum < m_mask.size(); msknum++)
				{
					const std::string &filename = (*i).name;
					if (filename.size() >= m_mask[msknum].size())
					{
						if ( filename.substr( filename.size() - m_mask[msknum].size(), m_mask[msknum].size() ) == m_mask[msknum] )
						{
							mask_match = true;
							break;
						}
					}
				}
				if (!mask_match) continue;
			}

			pos++;
			if (pos-1 < m_listBegin)
				continue;

			currstring = "";
			currstring += (*i).name;

			if ((*i).directory)
			{
				currstring += "/";

				m_renderer->drawSpriteT(
					&CResources::m_icon_dir
					, m_x + 10, m_y + 32 +3 + 21*line
					, CResources::m_icon_dir.getWidth(), CResources::m_icon_dir.getHeight()
					, 0, 0
					, CResources::m_icon_dir.getWidth(), CResources::m_icon_dir.getHeight()
					, 128, 128, 128, alpha
				);
			} else
			{
				m_renderer->drawSpriteT(
					&CResources::m_icon_file
					, m_x + 10 +4, m_y + 32 +2 + 21*line
					, CResources::m_icon_file.getWidth(), CResources::m_icon_file.getHeight()
					, 0, 0
					, CResources::m_icon_file.getWidth(), CResources::m_icon_file.getHeight()
					, 128, 128, 128, alpha
				);
			}
			
			CResources::verdana22.printASCIIShadow(currstring.c_str()
				, m_x + 10 +26, m_y + 32 + 21*line +CResources::verdana22.getBasePos()
				, 0, 0
				, 0, 0, 0, alpha
			);

			line++;
			if (line >= LIST_POSITION) break;
		}
	}
}

void CGUIMcaGetPath::drawAll(CIGUIFrameTexture *prevBuffTex, float alpha)
{
	m_renderer->setAlpha(true);
	m_renderer->setTestAlpha(true);
	m_renderer->setScissor(false);
	m_renderer->clearFrontBuffer();
	if (prevBuffTex != NULL) m_renderer->restoreFrameTex(prevBuffTex);
	drawMngrWnd(alpha);
	drawBoxArea(alpha);

	m_renderer->setScissor(true, (int)m_x +8, (int)m_y+26, (int)m_x +478, (int)m_y +270);
	drawFilelist(alpha);
	m_renderer->setScissor(false);

	m_tips.drawTip(m_renderer, m_input_state_new, m_ticks, alpha*0.90f);
}

void CGUIMcaGetPath::doTopLevel(CIGUIFrameTexture *prevBuffTex, float alpha)
{
	std::list<t_fileentry> *last_dir = m_curr_dir;
	int lastChosen = m_listChosen;
	int lastBegin = m_listBegin;
	std::string *lastPath = m_curr_path;
	std::list<t_fileentry> top_dir;
	m_listChosen = 0;
	m_listBegin = 0;
	if (m_hover_files) m_hover_files->setDest(m_x+9, m_y+33 + (m_listChosen - m_listBegin)*21, true);

	std::string caption;
	if (m_caption == NULL)
	{
		if (m_save)
			caption = CResources::mainLang.getText("LNG_FILE_CHOOSE_FILE_SAVE");
		else
			caption = CResources::mainLang.getText("LNG_FILE_CHOOSE_FILE_READ");
	} else
	{
		caption = m_caption;
	}

	m_curr_path = &caption;

	top_dir.push_back(t_fileentry("hdd0:", true));
	if (!CResources::iopreset) top_dir.push_back(t_fileentry("host:", true));

	top_dir.sort();
	m_curr_dir = &top_dir;

	u32 currTick = 0, oldTick = 0;
	currTick = oldTick = m_timer->getTicks();
	m_exit_one = false;
	do
	{
		m_ticks = currTick - oldTick;
		m_input->update();
		m_input_state_new = m_input->getNew(m_ticks);
		m_input_state_all = m_input->getAll();

		if (checkMessagesTop(true))
		{
			currTick = oldTick = m_timer->getTicks();
			if (m_exit) break;
			continue;
		}

		drawAll(prevBuffTex);
		m_renderer->swapBuffers();

		oldTick = currTick;
		currTick = m_timer->getTicks();

	} while (!m_exit && !m_exit_one);
	m_exit_one = false;
	top_dir.clear();
	m_curr_dir = last_dir;
	m_listChosen = lastChosen;
	m_listBegin = lastBegin;
	m_curr_path = lastPath;

	if (m_exit)
	{
		std::list<std::string>::const_iterator i;
		m_retpath = "";
		int level = 0;
		for (i = m_all_path.begin(); i != m_all_path.end(); i++)
		{
			m_retpath += (*i);
			if (level > 0 || ( (*i) != "host:" && (*i) != "hdd0:" )) m_retpath += "/";
			level++;
		}
		m_retpath = m_retpath.substr(0, m_retpath.length() -1);
		sio_printf("Chosen path: %s\n", m_retpath.c_str());
	}
	m_all_path.clear();
}

void CGUIMcaGetPath::doSubLevel(CIGUIFrameTexture *prevBuffTex, float alpha)
{
	std::list<t_fileentry> *last_dir = m_curr_dir;
	int lastChosen = m_listChosen;
	int lastBegin = m_listBegin;
	std::string *lastPath = m_curr_path;
	std::list<t_fileentry> sub_dir;
	m_listChosen = 0;
	m_listBegin = 0;
	if (m_hover_files) m_hover_files->setDest(m_x+9, m_y+33 + (m_listChosen - m_listBegin)*21, true);

	std::string totalpath = "";
	std::list<std::string>::const_iterator i;

	int level = 0;
	for (i = m_all_path.begin(); i != m_all_path.end(); i++)
	{
		totalpath += (*i);
		if (level > 0 || ( (*i) != "host:" && (*i) != "hdd0:" )) totalpath += "/";
		level++;
	}
	sio_printf("Total Sub Path: %s\n", totalpath.c_str());
	m_curr_path = &totalpath;

	if (m_all_path.front() == "hdd0:" && hddCheckFormatted() == 0)
	{
		if (m_all_path.size() == 1)
		{//enumerate partitions
			if (m_partitions.size() > 0)
			{
				std::list<t_fileentry>::const_iterator part;

				for (part = m_partitions.begin(); part != m_partitions.end(); part++) sub_dir.push_back( (*part) );
			} else
			{
				int fd = fileXioDopen("hdd0:");
				iox_dirent_t dirp;
				if(fd > 0)
				{
					while (fileXioDread(fd, &dirp) > 0)
					{
						if (dirp.stat.attr != ATTR_MAIN_PARTITION || dirp.stat.mode != FS_TYPE_PFS) continue;
						std::string name = dirp.name;
						if ( name.substr(0, 3) == "PP." &&  name.substr(name.length()-4, 4) == ".PCB") continue; //Codebreaker
						if ( name.substr(0, 2) == "__")
						{
							if (name != "__boot"
								&& name != "__net"
								&& name != "__system"
								&& name != "__sysconf"
								&& name != "__common"
								) continue;
						}
						m_partitions.push_back( t_fileentry( dirp.name, true ) );
						sub_dir.push_back( t_fileentry( dirp.name, true ) );
					}
					fileXioDclose(fd);
				} else sio_printf("There was an error enumerating partitions\n");
			}
		} else if (m_all_path.size() > 1)
		{//make sure it's mounted
			int levelhdd = 0;
			std::string pathpart = "", pathpfs = "";
			for (i = m_all_path.begin(); i != m_all_path.end(); i++)
			{
				if (levelhdd == 1) pathpfs = "pfs0:/";
				if (levelhdd < 2) pathpart += (*i);
				else pathpfs += (*i);
				if (levelhdd > 1) pathpfs += "/";
				levelhdd++;
			}
			if (pathpart != m_mountpath[0])
			{
				m_mountpath[0] = pathpart;
				sio_printf("Mounting %s as pfs0\n", m_mountpath[0].c_str());
				fileXioUmount("pfs0:");
				fileXioMount("pfs0:", m_mountpath[0].c_str(), FIO_MT_RDWR);
			}
			int fd = fioDopen(pathpfs.c_str());
			fio_dirent_t dirp;
			if(fd > 0)
			{
				bool backdir = false;
				while (fioDread(fd, &dirp) > 0)
				{
					if (dirp.name[0] == '.' && dirp.name[1] == 0) continue;
					if (dirp.name[0] == '.' && dirp.name[1] == '.' && dirp.name[2] == 0) backdir = true;
					sub_dir.push_back( t_fileentry( dirp.name, (FIO_SO_ISDIR(dirp.stat.mode)) ) );
				}
				fioDclose(fd);
				if (!backdir) sub_dir.push_back( t_fileentry("..", true) );
			} else sio_printf("There was an error opening directory %s\n", pathpfs.c_str());
		}
	} else if (m_all_path.front() == "cdfs:")
	{
		int status = cdStatus();
		sio_printf("Status: %d\n", status);
		if ( status != CDVD_STAT_OPEN && status != CDVD_STAT_ERROR)
		{
			//CdvdDiscType_t type;
			int type;
			bool doloop = true;
			bool valid = false;
			while(doloop)
			{
				type = sceCdGetDiskType();
				sio_printf("Disc type: %d\n", type);
				switch (type)
				{
					case CDVD_TYPE_DETECT:
					case CDVD_TYPE_DETECT_CD:
					case CDVD_TYPE_DETECT_DVDSINGLE:
					case CDVD_TYPE_DETECT_DVDDUAL:
						break;
					case CDVD_TYPE_UNKNOWN:
					case CDVD_TYPE_NODISK:
					case CDVD_TYPE_ILLEGAL:
					case CDVD_TYPE_DVDVIDEO:
					case CDVD_TYPE_CDDA:
						doloop = false;
						break;
					case CDVD_TYPE_PS1CD:
					case CDVD_TYPE_PS1CDDA:
					case CDVD_TYPE_PS2CD:
					case CDVD_TYPE_PS2CDDA:
					case CDVD_TYPE_PS2DVD:
						doloop = false;
						valid = true;
						break;
				}

				if (valid)
				{
					sio_printf("Valid disc\n");
					CDVD_FlushCache();
					CDVD_DiskReady(0);

					bool backdir = false;
					int entries = 2048;
					TocEntry *TocEntryList = new TocEntry[entries];

					int numoffiles = CDVD_GetDir(totalpath.substr(5, totalpath.length()-5).c_str(), NULL, CDVD_GET_FILES_AND_DIRS, TocEntryList, entries, NULL);
					sio_printf("Num of files in '%s': %d\n", totalpath.c_str(), numoffiles);
					for (int i = 0; i < numoffiles; i++)
					{
						if (TocEntryList[i].filename[0] == '.' && TocEntryList[i].filename[1] == 0) continue;
						if (TocEntryList[i].filename[0] == '.' && TocEntryList[i].filename[1] == '.' && TocEntryList[i].filename[2] == 0) backdir = true;

						sub_dir.push_back( t_fileentry( TocEntryList[i].filename, (TocEntryList[i].fileProperties & 0x02) ) );
					}
					if (!backdir && numoffiles > 0) sub_dir.push_back( t_fileentry("..", true) );

					delete [] TocEntryList;
				}
			}
		}
	} else
	{
		int fd = fioDopen(totalpath.c_str());
		fio_dirent_t dirp;
		if(fd > 0)
		{
			bool backdir = false;
			while (fioDread(fd, &dirp) > 0)
			{
				if (dirp.name[0] == '.' && dirp.name[1] == 0) continue;
				if (dirp.name[0] == '.' && dirp.name[1] == '.' && dirp.name[2] == 0) backdir = true;
				sub_dir.push_back( t_fileentry( dirp.name, (FIO_SO_ISDIR(dirp.stat.mode)) ) );
			}
			fioDclose(fd);
			if (!backdir) sub_dir.push_back( t_fileentry("..", true) );
		} else sio_printf("There was an error opening directory %s\n", totalpath.c_str());
	}
	sub_dir.sort();
	m_curr_dir = &sub_dir;

	if (m_curr_dir->size() > 0)
	{
		if (!m_keyb_tip_shown && m_save)
		{
			m_keyb_tip_shown = true;
			m_tips.addTip(CResources::mainLang.getText("LNG_TIP_VKBD_MASK"), 0, 0, 0, 1.0f, 25000);
		}
	}

	u32 currTick = 0, oldTick = 0;
	currTick = oldTick = m_timer->getTicks();
	m_exit_one = false;
	do
	{
		m_ticks = currTick - oldTick;
		m_input->update();
		m_input_state_new = m_input->getNew(m_ticks);
		m_input_state_all = m_input->getAll();

		if (checkMessagesTop())
		{
			currTick = oldTick = m_timer->getTicks();
			if (m_exit) break;
			continue;
		}

		drawAll(prevBuffTex);
		m_renderer->swapBuffers();

		oldTick = currTick;
		currTick = m_timer->getTicks();

	} while (!m_exit && !m_exit_one);
	if (m_exit_one) m_all_path.pop_back();
	m_exit_one = false;
	sub_dir.clear();
	m_curr_dir = last_dir;
	m_listChosen = lastChosen;
	m_listBegin = lastBegin;
	m_curr_path = lastPath;
}

int CGUIMcaGetPath::display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur)
{
	return 0;
}

int CGUIMcaGetPath::doGetName(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, std::string &ret, bool save, bool blur, const char *caption)
{
	m_input_state_new = 0;
	m_input_state_all = 0;
	m_renderer = renderer;
	m_input = input;
	m_timer = timer;

	m_mountpath[0] = ""; m_mountpath[1] = ""; m_mountpath[2] = ""; m_mountpath[3] = "";
	m_retpath = "";
	m_exit = false;
	m_exit_one = false;
	m_listBegin = 0;
	m_listChosen = 0;
	m_all_path.clear();
	m_save = save;
	m_caption = caption;

	CIGUIFrameTexture *prevBuffTex;
	if (blur)
	{
		prevBuffTex = renderer->getFrameTex(1);
		prevBuffTex->blur(0);
		prevBuffTex->blur(0);
	} else
	{
		prevBuffTex = renderer->getFrameTex();
	}
	m_prevBuffTex = prevBuffTex;
	m_ticks = 0;
	fadeInOut(prevBuffTex, timer, 25000, false);
	
	doTopLevel(prevBuffTex, 1.0f);

	if (!m_no_out) fadeInOut(prevBuffTex, timer, 25000, true);

	delete prevBuffTex;

	ret = m_retpath;

	m_mountpath[0] = ""; m_mountpath[1] = ""; m_mountpath[2] = ""; m_mountpath[3] = "";

	return 1;
}

std::list<CGUIMcaGetPath::t_fileentry> CGUIMcaGetPath::m_partitions;
