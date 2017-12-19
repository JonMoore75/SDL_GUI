#include "GameState.h"



GameState::QUITRESPONSE GameState::ThreeOptionQuitDialog(std::string menuStateID)
{
	int buttonid = -1;

	const SDL_MessageBoxButtonData buttons[] = {
		{ 0, 0, "Cancel" },
		{ 0, 1, "Menu" },
		{ 0, 2, "Quit" },
	};

	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION, /* .flags */
		NULL,						/* .window */
		"Quit",						/* .title */
		"Would you like to Return to Menu or Quit to Desktop?",	/* .message */
		SDL_arraysize(buttons),		/* .numbuttons */
		buttons,					/* .buttons */
		NULL						/* .colorScheme */
	};

	int ret = SDL_ShowMessageBox(&messageboxdata, &buttonid);

	if (buttonid == 1)
	{
		// Return to main menu
		m_newState = menuStateID;
		return GameState::NEWSTATE;
	}

	if (buttonid == 2)
		return GameState::QUIT;

	return GameState::CANCEL;
}

GameState::QUITRESPONSE GameState::TwoOptionQuitDialog()
{
	int buttonid = -1;

	const SDL_MessageBoxButtonData buttons[] = {
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes" },
		{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "No" },
	};

	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION, /* .flags */
		NULL,						/* .window */
		"Quit",						/* .title */
		"Would you like to Quit?",	/* .message */
		SDL_arraysize(buttons),		/* .numbuttons */
		buttons,					/* .buttons */
		NULL						/* .colorScheme */
	};

	int ret = SDL_ShowMessageBox(&messageboxdata, &buttonid);

	if (buttonid == 1)
		return GameState::QUIT;

	return GameState::CANCEL;
}

std::string GameState::CheckIfChangetoNewState()
{

	std::string newState = m_newState;
	m_newState.clear();

	return newState;
}

void GameState::RequestAppQuit() const
{
	SDL_Event Event;
	Event.type = SDL_QUIT;
	SDL_PushEvent(&Event);
}
