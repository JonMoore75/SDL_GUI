#ifndef GameState_h__
#define GameState_h__
#pragma once

#include <memory>
#include <string>

#include "EventHandler.h"

class Renderer;
class Window;

class GameState : public EventHandler
{
public:
	GameState() {}
	virtual ~GameState() noexcept { }

	enum QUITRESPONSE { QUIT, CANCEL, NEWSTATE};

	virtual bool Initialise() = 0;
	virtual void Update(double dt) = 0;
	virtual void Render(Renderer& renderer) = 0;
	virtual void CleanUp() = 0;

	// Default behaviour is to just quit without checking with user
	// If want user confirmation then override this to call ThreeOptionQuitDialog
	// or TwoOptionQuitDialog
	virtual QUITRESPONSE QuitDialog() { return QUIT; }

	// This will give user option to return to menu, to quit to desktop or
	// to cancel and carry on
	QUITRESPONSE ThreeOptionQuitDialog(std::string new_state);

	// Dialog asking user if really wants to quit
	QUITRESPONSE TwoOptionQuitDialog();

	void SetWindow(Window* pWnd) { m_pWnd = pWnd; }

	std::string CheckIfChangetoNewState();

protected:
	void RequestAppQuit() const;

	Window* m_pWnd = nullptr;

	// Usually blank string, when not tells app to change game state
	std::string m_newState;


};

#endif GameState_h__
