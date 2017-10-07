#include "SDLErrorReport.h"


void Error2MsgBox(std::string err_msg)
{
	err_msg += SDL_GetError();
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", err_msg.c_str(), nullptr);
}