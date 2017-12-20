#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

#include <SDL.h>
#include <string>

#include "Common.h"

class EventHandler 
{
public:
	EventHandler(){}

	virtual ~EventHandler() noexcept {}

	// Main handler
	virtual bool OnEvent(SDL_Event& Event);

	// Window  Events
	virtual bool OnShow(int windowID)										{ return false; }

	virtual bool OnHide(int windowID)										{ return false; }

	virtual bool OnExpose(int windowID)										{ return false; }

	virtual bool OnMoveWindow(int windowID, int x, int y)					{ return false; }

	virtual bool OnResize(int windowID, int w, int h)						{ return false; }

	virtual bool OnMinimize(int windowID)									{ return false; }

	virtual bool OnMaximize(int windowID)									{ return false; }

	virtual bool OnRestore(int windowID)									{ return false; }

	virtual bool OnMouseFocus(int windowID)									{ return false; }

	virtual bool OnMouseBlur(int windowID)									{ return false; }

	virtual bool OnKeyFocus(int windowID)									{ return false; }

	virtual bool OnKeyBlur(int windowID)									{ return false; }

//	virtual bool OnClose(int windowID)										{ return false; }

	// Key Events
	virtual bool OnKeyDown(Scancode scan, Keycode key)				{ return false; }

	virtual bool OnKeyUp(Scancode scan, Keycode key)				{ return false; }

	virtual bool OnUTFCharacter(const std::string& codepoint)				{ return false; }

	// Mouse Events

	virtual bool OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle){ return false; }

	virtual bool OnMouseWheel(bool Up, bool Down)								{ return false; }    //Not implemented

	virtual bool OnLButtonDown(int mX, int mY)									{ return false; }

	virtual bool OnLButtonUp(int mX, int mY)									{ return false; }

	virtual bool OnRButtonDown(int mX, int mY)									{ return false; }

	virtual bool OnRButtonUp(int mX, int mY)									{ return false; }

	virtual bool OnMButtonDown(int mX, int mY)									{ return false; }

	virtual bool OnMButtonUp(int mX, int mY)									{ return false; }

	// Joystick Events

	virtual bool OnJoyAxis(Uint8 which, Uint8 axis, Sint16 value)				{ return false; }

	virtual bool OnJoyButtonDown(Uint8 which, Uint8 button)						{ return false; }

	virtual bool OnJoyButtonUp(Uint8 which, Uint8 button)						{ return false; }

	virtual bool OnJoyHat(Uint8 which, Uint8 hat, Uint8 value)					{ return false; }

	virtual bool OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel)	{ return false; }

	// System events

//	virtual bool OnExit()														{ return false; }

	// User events

	virtual bool OnUser(Uint8 type, int code, void* data1, void* data2)			{ return false; }
};

#endif