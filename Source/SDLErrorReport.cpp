#include "SDLErrorReport.h"


void MsgBoxErrorReport(std::string err_msg, std::string err_msg2)
{
	err_msg += err_msg2;
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", err_msg.c_str(), nullptr);
}