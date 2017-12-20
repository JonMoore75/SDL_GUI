#include "Common.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>



#include "SDLErrorReport.h"

#include "Font_TTF.h"
#include "Renderer.h"

Point operator+(const Point& lhs, const Point& rhs)
{
	return Point{ lhs.x + rhs.x, lhs.y + rhs.y };
}

Rect operator+(const Rect& lhs, const Point& rhs)
{
	return Rect{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.w, lhs.h };
}

Point operator-(const Point& lhs, const Point& rhs)
{
	return Point{ lhs.x - rhs.x, lhs.y - rhs.y };
}

bool PointinRect(const Point& point, const Rect& rect)
{
	return (SDL_PointInRect(&point, &rect) == SDL_TRUE);
}

bool PointinRect(int x, int y, const Rect& rect)
{
	Point point{ x,y };
	return PointinRect(point, rect);
}

Point TextBounds(const std::string& fontName, int fontSize, const std::string& text, Uint32 wrapLength/* = 0*/)
{
	Point textSize{ -1, -1 };
	FontTTF fontTTF;
	fontTTF.LoadFont(fontName.c_str(), fontSize, Color{});
	fontTTF.SizeUTF8(text, textSize.x, textSize.y, wrapLength);
	fontTTF.Release();

	return textSize;
}

void StartUnicodeEvents()
{
	SDL_StartTextInput();
}

void StopUnicodeEvents()
{
	SDL_StopTextInput();
}

std::string GetClipboardText()
{
	char* cbstr{ SDL_GetClipboardText() };
	std::string ret{ cbstr };
	SDL_free(cbstr);
	return ret;
}

bool SetClipboardText(const std::string& str)
{
	return SDL_SetClipboardText(str.c_str()) >= 0;
}

bool InitSDL()
{
	// Initialise SDL, report error if it fails
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		MsgBoxErrorReport("SDL Initialisation Failed!\n", SDL_GetError());
		return false;
	}

	//Initialize PNG/JPG loading 
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		MsgBoxErrorReport("SDL_image could not initialize!\n", IMG_GetError());
		return false;
	}

	//Initialize SDL_ttf 
	if (TTF_Init() == -1)
	{
		MsgBoxErrorReport("SDL_ttf could not initialize!\n", TTF_GetError());
		return false;
	}

	//Initialize SDL_mixer 
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		MsgBoxErrorReport("SDL_mixer could not initialize!\n", Mix_GetError());
		return false;
	}

	return true;
}

void ShutdownSDL()
{
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void MainLoop(std::function<bool(SDL_Event& Event)> OnEvent, std::function<void()> Render)
{
	bool running = true;

	// Application will indicate quit by setting m_Running to false
	// until then we loop thro event handling and rendering each frame
	while (running)
	{
		SDL_Event Event;

		while (SDL_PollEvent(&Event))
		{
			// If quit has been requested (eg clicking close button) then query user for confirmation
			// via dialog box.  Then react to users choice
			if (Event.type == SDL_QUIT)
			{
				running = false;
				break;
			}

			OnEvent(Event);
		}

		// Draw our frame
		Render();
	}
}




