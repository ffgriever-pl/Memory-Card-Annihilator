#include "GUIMcaLang.h"
#include <sstream>
#include <vector>


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
   CGUIMcaLang::langmapRaw("LNG_MWND_CHOOSE_SLOT", "Wybierz slot"),
   CGUIMcaLang::langmapRaw("LNG_OPER_PS2_MEMORY_CARD", "Karta pamięci PS2"),
   CGUIMcaLang::langmapRaw("LNG_OPER_PSX_MEMORY_CARD", "Karta pamięci PSX"),
   CGUIMcaLang::langmapRaw("LNG_OPER_FORMAT", "Formatowanie"),
   CGUIMcaLang::langmapRaw("LNG_OPER_UNFORMAT", "Odformatowanie"),
   CGUIMcaLang::langmapRaw("LNG_OPER_MAKE_IMAGE", "Tworzenie obrazu karty"),
   CGUIMcaLang::langmapRaw("LNG_OPER_RESTORE_IMAGE", "Wgrywanie obrazu karty"),
   CGUIMcaLang::langmapRaw("LNG_OPER_INFO", "Informacje o karcie"),
   CGUIMcaLang::langmapRaw("LNG_OPER_FAST", "Szybkie"),
   CGUIMcaLang::langmapRaw("LNG_OPER_FULL", "Pełne"),
   CGUIMcaLang::langmapRaw("LNG_OPER_CHOOSE_MC_SIZE", "\nWybierz wielkość swojej karty"),
   CGUIMcaLang::langmapRaw("LNG_OPER_MEGABYTE", "{CARDSIZE} MB"),
   CGUIMcaLang::langmapRaw("LNG_OPER_YES", "Tak"),
   CGUIMcaLang::langmapRaw("LNG_OPER_NO", "Nie"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_FORMAT_DATA_LOST", "Czy na pewno chcesz sformatować kartę pamięci?\n\nFormatowanie spowoduje utratę wszystkich danych zapisanych na karcie!"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_UNFORMAT_DATA_LOST", "Czy na pewno chcesz odformatować kartę pamięci?\n\nOdformatowanie spowoduje utratę wszystkich danych zapisanych na karcie!"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_RESTORE_DATA_LOST", "Czy na pewno chcesz wgrać obraz karty pamięci?\n\nWgranie obrazu spowoduje utratę wszystkich danych zapisanych na karcie!"),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_OVERWRITE", "Wskazany plik już istnieje! Na pewno chcesz go nadpisać?\n\nNadpisanie pliku spowoduje utratę zapisanych w nim danych."),
   CGUIMcaLang::langmapRaw("LNG_WARN_CAP", "Ostrzeżenie..."),
   CGUIMcaLang::langmapRaw("LNG_WARN_NO_CARD_IN_SLOT", "Karta pamięci w slocie {SLOTNUM} została wyciągnięta lub zmieniona na inną. Nastąpi wyjście do menu głównego.\n\n\nWciśnij X, aby kontynuować."),
   CGUIMcaLang::langmapRaw("LNG_PROGRESS_DO_NOT_REMOVE", "\nProszę nie wyjmować karty pamięci, ani nie wyłączać lub resetować konsoli"),
   CGUIMcaLang::langmapRaw("LNG_PROGRESS_SUCCESS", "\nOperacja zakończona powodzeniem\nWciśnij X, aby kontynuować"),
   CGUIMcaLang::langmapRaw("LNG_PROGRESS_FAIL", "\nOperacja zakończona niepowodzeniem\nWciśnij X, aby kontynuować"),
   CGUIMcaLang::langmapRaw("LNG_FILE_CHOOSE_FILE_READ", "Wskaż plik do odczytu"),
   CGUIMcaLang::langmapRaw("LNG_FILE_CHOOSE_FILE_SAVE", "Wskaż plik do zapisu"),
   CGUIMcaLang::langmapRaw("LNG_INFO_CAPTION", "Informacje o karcie"),
   CGUIMcaLang::langmapRaw("LNG_INFO_UNIT_MB", "MB"),
   CGUIMcaLang::langmapRaw("LNG_INFO_UNIT_KB", "kB"),
   CGUIMcaLang::langmapRaw("LNG_INFO_MC_PSX", "PSX"),
   CGUIMcaLang::langmapRaw("LNG_INFO_MC_PS2", "PS2"),
   CGUIMcaLang::langmapRaw("LNG_INFO_SLOT", "Slot karty pamięci:"),
   CGUIMcaLang::langmapRaw("LNG_INFO_TYPE", "Typ karty:"),
   CGUIMcaLang::langmapRaw("LNG_INFO_PAGESIZE", "Wielkość strony:"),
   CGUIMcaLang::langmapRaw("LNG_INFO_PAGESINBLOCK", "Stron w bloku:"),
   CGUIMcaLang::langmapRaw("LNG_INFO_PAGESTOTAL", "Ilość stron:"),
   CGUIMcaLang::langmapRaw("LNG_INFO_THX_TXT", "\nSony za konsolę, przy której spędziliśmy niejeden wieczór, nie przespaliśmy niejednej nocy i przez którą rzuciło nas wiele dziewczyn ;).\n\nPiotrowiB za wykonanie zdjęcia memorki od PSXa oraz pozostałym osobom, które zaangażowały się w sesję zdjęciową swoich kart pamięci."),
   CGUIMcaLang::langmapRaw("LNG_INFO_THX_CAP", "Dziękujemy:"),
   CGUIMcaLang::langmapRaw("LNG_TIP_VKBD_MASK", "START - utwórz nowy plik\nL2/R2 - włącz/wyłącz filtr rozszerzeń"),
   CGUIMcaLang::langmapRaw("LNG_TIP_MASK_ENABLED", "Maska włączona"),
   CGUIMcaLang::langmapRaw("LNG_TIP_MASK_DISABLED", "Maska wyłączona"),
   CGUIMcaLang::langmapRaw("LNG_VKBD_WARN_WRONG_NAME", "Wprowadzona nazwa pliku zawiera nieprawidłowe znaki. Zmień nazwę pliku na prawidłową.\n\n\nWciśnij X, aby kontynuować."),
   CGUIMcaLang::langmapRaw("LNG_EXIT_SELECT_ELF", "Wybierz plik do uruchomienia..."),
   CGUIMcaLang::langmapRaw("LNG_EXIT_INVALID", "\n\nWskazany plik nie jest prawidłowym plikiem wykonywalnym."),
   CGUIMcaLang::langmapRaw("LNG_EXIT_FAILED", "\n\nNie można otworzyć wskazanego pliku."),
   CGUIMcaLang::langmapRaw("LNG_OPER_QUESTION_SIZE_MISMATCH", "Wybrano większy niż domyślny rozmiar. Może to doprowadzić do uszkodzenia karty!\n\nNa pewno chcesz kontynuować?"),
};
const int CGUIMcaLang::m_numof_entries = sizeof(CGUIMcaLang::m_default_lang_raw) / sizeof CGUIMcaLang::m_default_lang_raw[0];
const std::map<const std::string, const std::string> CGUIMcaLang::m_default_lang(CGUIMcaLang::m_default_lang_raw, CGUIMcaLang::m_default_lang_raw + CGUIMcaLang::m_numof_entries);
