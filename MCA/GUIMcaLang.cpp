#include <sstream>
#include <vector>

#include "GUIMcaLang.h"
#include "helpers.h"

CGUIMcaLang::CGUIMcaLang(void)
{
	for (std::map<const std::string, const std::string>::const_iterator i = m_default_lang.begin(); i != m_default_lang.end(); i++)
	{
		m_curr_lang.insert(*i);
	}
	//std::map<const std::string, const std::string>::iterator ilang;
	//if ( (ilang = m_curr_lang.find("LNG_PHRASE_ETC")) != m_curr_lang.end() )
	//but here easier to use: m_curr_lang["LNG_PHRASE_ETC"];
}

void CGUIMcaLang::trimLeft(std::string& str, const char* chars2remove)
{
	if (!str.empty())
	{
		std::string::size_type pos = str.find_first_not_of(chars2remove);

		if (pos != std::string::npos)
			str.erase(0,pos);
		else
			str.erase( str.begin() , str.end() ); // make empty
	}
}

void CGUIMcaLang::trimRight(std::string& str, const char* chars2remove)
{
	if (!str.empty())
	{
		std::string::size_type pos = str.find_last_not_of(chars2remove);

		if (pos != std::string::npos)
			str.erase(pos+1);
		else
			str.erase( str.begin() , str.end() ); // make empty
	}
}
//#define sio_printf printf
void CGUIMcaLang::initLang(std::string input)
{
	replace(&input, "\r\n", "\n");
	replace(&input, "\r", "\n");
	std::istringstream instream(input);
	std::string line;
	std::vector<std::string> all_lines;

	while(std::getline(instream, line, '\n'))
	{
		trimLeft(line, " ");
		trimRight(line, " ");
		if (line.empty()) continue;
		if (line[0] == '#' || (line[0] == '/' && line[1] == '/')) continue;
		if (line.find("=") == std::string::npos) continue;

		all_lines.push_back(line);
		//sio_printf("%s\n", line.c_str());
	}

	std::string::size_type subpos;
	std::string::size_type laspos;
	for (std::map<const std::string, const std::string>::const_iterator i = m_default_lang.begin(); i != m_default_lang.end(); i++)
	{
		for (u32 li = 0; li < all_lines.size(); li++)
		{
			subpos = all_lines[li].find((*i).first);
			if (subpos != std::string::npos)
			{
				if (all_lines[li][subpos+(*i).first.length()] == ' ' || all_lines[li][subpos+(*i).first.length()] == '=')
				{
					subpos = all_lines[li].find("\"");
					laspos = all_lines[li].rfind("\"");
					if (subpos != laspos)
					{
						m_curr_lang[(*i).first] = all_lines[li].substr(subpos+1, (laspos)-(subpos+1));
						replace(&m_curr_lang[(*i).first], "\\\"", "\"");
						replace(&m_curr_lang[(*i).first], "\\n", "\n");
						//sio_printf("Def repl: %s=%s\n", (*i).first.c_str(), m_curr_lang[(*i).first].c_str());
					}
				}
			}
		}
	}

	all_lines.clear();
}

CGUIMcaLang::~CGUIMcaLang(void)
{
	m_curr_lang.clear();
}

const char *CGUIMcaLang::getText(const char *label)
{
	return m_curr_lang[label].c_str();
}

void CGUIMcaLang::replace(std::string *base, const std::string find, const std::string repl)
{
	int lenrepl = repl.length();
	int lenfind = find.length();

	u32 i = 0;
	while ( std::string::npos != (i = base->find(find, i)) )
	{
		base->replace( i, lenfind, repl);
		i += lenrepl;
	}
}

void CGUIMcaLang::replace(std::string *base, const std::string find, const long long repln)
{
	std::ostringstream conv;

	conv << repln;
	std::string repl = conv.str();

	int lenrepl = repl.length();
	int lenfind = find.length();

	u32 i = 0;
	while ( std::string::npos != (i = base->find(find, i)) )
	{
		base->replace( i, lenfind, repl);
		i += lenrepl;
	}
}

const CGUIMcaLang::langmapRaw CGUIMcaLang::m_default_lang_raw[] = {
   CGUIMcaLang::langmapRaw("LNG_MWND_CHOOSE_SLOT", "Select slot"),
   CGUIMcaLang::langmapRaw("LNG_OPER_PS2_MEMORY_CARD", "PS2 Memory Card"),
   CGUIMcaLang::langmapRaw("LNG_OPER_PSX_MEMORY_CARD", "PS1 Memory Card"),
   CGUIMcaLang::langmapRaw("LNG_OPER_FORMAT", "Format"),
   CGUIMcaLang::langmapRaw("LNG_OPER_UNFORMAT", "Unformat"),
   CGUIMcaLang::langmapRaw("LNG_OPER_MAKE_IMAGE", "Create MC image"),
   CGUIMcaLang::langmapRaw("LNG_OPER_RESTORE_IMAGE", "Restore MC image"),
   CGUIMcaLang::langmapRaw("LNG_OPER_INFO", "Memory Card Info"),
   CGUIMcaLang::langmapRaw("LNG_OPER_FAST", "Fast"),
   CGUIMcaLang::langmapRaw("LNG_OPER_FULL", "Full"),
   CGUIMcaLang::langmapRaw("LNG_OPER_CHOOSE_MC_SIZE", "\nSelect your memory card size"),
   CGUIMcaLang::langmapRaw("LNG_OPER_MEGABYTE", "{CARDSIZE} MB"),
   CGUIMcaLang::langmapRaw("LNG_OPER_YES", "Yes"),
   CGUIMcaLang::langmapRaw("LNG_OPER_NO", "No"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_FORMAT_DATA_LOST", "Are you sure you want to format the memory card?\n\nFormatting will destroy all the data stored on the card!"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_UNFORMAT_DATA_LOST", "Are you sure you want to unformat the memory card?\n\nUnormatting will destroy all the data stored on the card!"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_RESTORE_DATA_LOST", "Are you sure you want to restore the memory card image?\n\nRestoring the image will destroy all the data stored on the card!"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_OVERWRITE", "Selected file already exists! Are you sure you want to overwrite it?\n\nOverwriting will cause all the file contents to be lost."),
   CGUIMcaLang::langmapRaw("LNG_WARN_CAP", "Warning..."),
   CGUIMcaLang::langmapRaw("LNG_WARN_NO_CARD_IN_SLOT", "Memory card in slot {SLOTNUM} has been pulled out or changed. Program will exit to main menu.\n\n\nPress X to continue."),
   CGUIMcaLang::langmapRaw("LNG_PROGRESS_DO_NOT_REMOVE", "\nDo not remove the memory card, reset or switch off the console"),
   CGUIMcaLang::langmapRaw("LNG_PROGRESS_SUCCESS", "\nOperation completed successfully\nPress X to continue"),
   CGUIMcaLang::langmapRaw("LNG_PROGRESS_FAIL", "\nOperation has failed\nPress X to continue"),
   CGUIMcaLang::langmapRaw("LNG_FILE_CHOOSE_FILE_READ", "Select a file to read"),
   CGUIMcaLang::langmapRaw("LNG_FILE_CHOOSE_FILE_SAVE", "Select a file to write"),
   CGUIMcaLang::langmapRaw("LNG_INFO_CAPTION", "Memory Card Info"),
   CGUIMcaLang::langmapRaw("LNG_INFO_UNIT_MB", "MB"),
   CGUIMcaLang::langmapRaw("LNG_INFO_UNIT_KB", "kB"),
   CGUIMcaLang::langmapRaw("LNG_INFO_MC_PSX", "PS1"),
   CGUIMcaLang::langmapRaw("LNG_INFO_MC_PS2", "PS2"),
   CGUIMcaLang::langmapRaw("LNG_INFO_SLOT", "Memory card slot:"),
   CGUIMcaLang::langmapRaw("LNG_INFO_TYPE", "Card type:"),
   CGUIMcaLang::langmapRaw("LNG_INFO_PAGESIZE", "Page size:"),
   CGUIMcaLang::langmapRaw("LNG_INFO_PAGESINBLOCK", "Pages per block:"),
   CGUIMcaLang::langmapRaw("LNG_INFO_PAGESTOTAL", "Total pages:"),
   CGUIMcaLang::langmapRaw("LNG_INFO_THX_TXT", "\nSony for the console that has entertained us during long evenings, that has kept us awake during countless nights and which made our girlfriends to break up with us ;).\n\nPiotrB for taking a shot of his PS1 memory card and everyone else that has been involved in the PS1 MC photo session."),
   CGUIMcaLang::langmapRaw("LNG_INFO_THX_CAP", "We'd like to thank:"),
   CGUIMcaLang::langmapRaw("LNG_TIP_VKBD_MASK", "START - create new file\nL2/R2 - enable/disable file mask"),
   CGUIMcaLang::langmapRaw("LNG_TIP_MASK_ENABLED", "File mask enabled"),
   CGUIMcaLang::langmapRaw("LNG_TIP_MASK_DISABLED", "File mask disabled"),
   CGUIMcaLang::langmapRaw("LNG_VKBD_WARN_WRONG_NAME", "The specified filename contains invalid characters. Change the filename to continue.\n\n\nPress any button to continue."),
   CGUIMcaLang::langmapRaw("LNG_EXIT_SELECT_ELF", "\n\nSelect a file to execute..."),
   CGUIMcaLang::langmapRaw("LNG_EXIT_INVALID", "\n\nThe selected file is not a valid ELF executable."),
   CGUIMcaLang::langmapRaw("LNG_EXIT_FAILED", "\n\nThe selected file couldn't be opened."),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_SIZE_MISMATCH", "You've selected a size bigger than default. This could damage the memory card!\n\nDo you want to continue?"),
};
const int CGUIMcaLang::m_numof_entries = countof(CGUIMcaLang::m_default_lang_raw);
const std::map<const std::string, const std::string> CGUIMcaLang::m_default_lang(CGUIMcaLang::m_default_lang_raw, CGUIMcaLang::m_default_lang_raw + CGUIMcaLang::m_numof_entries);
