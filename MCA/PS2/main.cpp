#include "PS2Application.h"

int main(int argc, char *argv[])
{
	int retval = CPS2Application::getInstance()->main(argc, argv);
	CPS2Application::delInstance();
	return retval;
}
