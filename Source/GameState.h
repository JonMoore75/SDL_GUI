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
	virtual QUITRESPONSE QuitDialog() { return QUIT; }

	QUITRESPONSE ThreeOptionQuitDialog(std::string new_state);
	QUITRESPONSE TwoOptionQuitDialog();

	void SetWindow(Window* pWnd) { m_pWnd = pWnd; }

	std::string ExtractNextState();

protected:
	void RequestAppQuit() const;

	Window* m_pWnd = nullptr;

	std::string m_newState;


};

#endif GameState_h__
