#include "Common/Utilities.h"

int main()
{
#if defined(DEBUG) || defined(_DEBUG)
	Utilities::ShowConsole();
#else
	Utilities::HideConsole();
#endif

	Utilities::Write("Running...", Utilities::NORMAL_LEVEL);
	Utilities::PauseConsole();

	return 0;
}