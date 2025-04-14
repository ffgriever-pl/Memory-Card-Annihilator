#ifndef _GUIMCAGETPATH_H_
#define _GUIMCAGETPATH_H_

#include <string>
#include <list>
#include <locale>
#include "GUIMcaPopup.h"
#include "GUIMcaHover.h"
#include "GUIMcaTip.h"

template <typename Elem, typename Traits, typename Ax>
std::basic_string<Elem, Traits, Ax> tolower(std::basic_string<Elem, Traits, Ax> str)
{
	std::use_facet<std::ctype<Elem> >(std::locale()).tolower(&*str.begin(), &*str.end());
	return str;
}

template <typename Elem, typename Traits, typename Ax>
std::basic_string<Elem, Traits, Ax> toupper(std::basic_string<Elem, Traits, Ax> str)
{
	std::use_facet<std::ctype<Elem> >(std::locale()).toupper(&*str.begin(), &*str.end());
	return str;
}

class CGUIMcaGetPath :
	public CGUIMcaPopup
{
private:
	struct t_fileentry
	{
		std::string name;
		bool directory;
		bool operator < (const t_fileentry& rhs)
		{
			if (directory == true && rhs.directory == false) return true;
			if (directory == false && rhs.directory == true) return false;
			return tolower(name) < tolower(rhs.name);
		}
		t_fileentry(std::string _name, bool _dir)
		{
			name = _name;
			directory = _dir;
		}
	};
	CIGUIFrameTexture *m_prevBuffTex;
	std::list<t_fileentry> *m_curr_dir;
	std::list<std::string> m_all_path;
	static std::list<t_fileentry> m_partitions;
	std::string *m_curr_path;
	std::string m_retpath;
	std::string m_mountpath[4];
	u32 m_input_state_new;
	u32 m_input_state_all;
	u32 m_ticks;
	const char *m_defaultname;
	bool m_save;
	bool m_exit;
	bool m_exit_one;
	bool m_no_out;
	int m_listChosen;
	int m_listBegin;
	int m_maskItems;
	CGUIMcaHover *m_hover_files;
	CGUIMcaTip m_tips;
	bool m_keyb_tip_shown;
	bool m_mask_enabled;
	std::vector<std::string> m_mask;
	const char* m_caption;
	void drawAll(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void fadeInOut(CIGUIFrameTexture *prevBuffTex, CIGUIFrameTimer *timer, u32 ms, bool out = false);
	void drawMngrWnd(float alpha = 1.0f);
	void drawBoxArea(float alpha = 1.0f);
	void drawFilelist(float alpha = 1.0f);
	void doTopLevel(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	void doSubLevel(CIGUIFrameTexture *prevBuffTex = NULL, float alpha = 1.0f);
	bool checkMessagesTop(bool topdir = false);
	CGUIMcaGetPath(void);
	int display(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, bool blur = false);
public:
	CGUIMcaGetPath(float x, float y, const char *defaultname);
	~CGUIMcaGetPath(void);
	void enableMask(bool enable) { m_mask_enabled = enable; }
	void clearMask();
	void addMaskEntry(std::string mask);
	int doGetName(CIGUIFrameRenderer *renderer, CIGUIFrameInput *input, CIGUIFrameTimer *timer, std::string &ret, bool save = false, bool blur = false, const char *caption = NULL);
};

#endif //_GUIMCAGETPATH_H_
