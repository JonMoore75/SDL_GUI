#include "EventHandler.h"



bool EventHandler::OnEvent(SDL_Event& Event) 
{
	int windowID = -1;
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

	bool eventHandled = false;

	switch (Event.type) 
	{
	case SDL_WINDOWEVENT: 
		{
			windowID = Event.window.windowID;
			switch (Event.window.event)
			{
			case SDL_WINDOWEVENT_SHOWN:
				eventHandled = OnShow(windowID);
				break;
			case SDL_WINDOWEVENT_HIDDEN:
				eventHandled = OnHide(windowID);
				break;
			case SDL_WINDOWEVENT_EXPOSED:
				eventHandled = OnExpose(windowID);
				break;
			case SDL_WINDOWEVENT_MOVED:
				x = Event.window.data1;
				y = Event.window.data2;
				eventHandled = OnMoveWindow(windowID, x, y);
				break;
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				width = Event.window.data1;
				height = Event.window.data2;
				eventHandled = OnResize(windowID, width, height);
				break;
			case SDL_WINDOWEVENT_RESIZED:
				width = Event.window.data1;
				height = Event.window.data2;
				break;
			case SDL_WINDOWEVENT_MINIMIZED:
				eventHandled = OnMinimize(windowID);
				break;
			case SDL_WINDOWEVENT_MAXIMIZED:
				eventHandled = OnMaximize(windowID);
				break;
			case SDL_WINDOWEVENT_RESTORED:
				eventHandled = OnRestore(windowID);
				break;
			case SDL_WINDOWEVENT_ENTER:
				eventHandled = OnMouseFocus(windowID);
				break;
			case SDL_WINDOWEVENT_LEAVE:
				eventHandled = OnMouseBlur(windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				eventHandled = OnKeyFocus(windowID);
				break;
			case SDL_WINDOWEVENT_FOCUS_LOST:
				eventHandled = OnKeyBlur(windowID);
				break;
			case SDL_WINDOWEVENT_CLOSE:
//				eventHandled = OnClose(windowID);
				break;
			}
		}
		break;

	case SDL_TEXTINPUT:
		eventHandled = OnUTFCharacter(std::string{ Event.text.text });
		break;

	case SDL_TEXTEDITING:
		break;
	
	case SDL_KEYDOWN: 
		eventHandled = OnKeyDown(Event.key.keysym.scancode, Event.key.keysym.sym);
		break;
	

	case SDL_KEYUP: 
		eventHandled = OnKeyUp(Event.key.keysym.scancode, Event.key.keysym.sym);
		break;
	

	case SDL_MOUSEMOTION: 
		eventHandled = OnMouseMove(Event.motion.x, Event.motion.y, Event.motion.xrel, Event.motion.yrel, (Event.motion.state&SDL_BUTTON(SDL_BUTTON_LEFT)) != 0, (Event.motion.state&SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0, (Event.motion.state&SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
		break;
	

	case SDL_MOUSEBUTTONDOWN: 
	{
		switch (Event.button.button) 
		{
		case SDL_BUTTON_LEFT: 
			eventHandled = OnLButtonDown(Event.button.x, Event.button.y);
			break;
								  
		case SDL_BUTTON_RIGHT: 
			eventHandled = OnRButtonDown(Event.button.x, Event.button.y);
			break;
								  
		case SDL_BUTTON_MIDDLE: 
			eventHandled = OnMButtonDown(Event.button.x, Event.button.y);
			break;
								  
		}
		break;
	}

	case SDL_MOUSEBUTTONUP:    
		switch (Event.button.button)
		{
		case SDL_BUTTON_LEFT:
			eventHandled = OnLButtonUp(Event.button.x, Event.button.y);
			break;

		case SDL_BUTTON_RIGHT:
			eventHandled = OnRButtonUp(Event.button.x, Event.button.y);
			break;

		case SDL_BUTTON_MIDDLE:
			eventHandled = OnMButtonUp(Event.button.x, Event.button.y);
			break;

		}
		break;

	case SDL_JOYAXISMOTION: 
		eventHandled = OnJoyAxis(Event.jaxis.which, Event.jaxis.axis, Event.jaxis.value);
		break;
	

	case SDL_JOYBALLMOTION: 
		eventHandled = OnJoyBall(Event.jball.which, Event.jball.ball, Event.jball.xrel, Event.jball.yrel);
		break;
	

	case SDL_JOYHATMOTION: 
		eventHandled = OnJoyHat(Event.jhat.which, Event.jhat.hat, Event.jhat.value);
		break;
	
	case SDL_JOYBUTTONDOWN: 
		eventHandled = OnJoyButtonDown(Event.jbutton.which, Event.jbutton.button);
		break;


	case SDL_JOYBUTTONUP: 
		eventHandled = OnJoyButtonUp(Event.jbutton.which, Event.jbutton.button);
		break;
	

	case SDL_QUIT:
//		eventHandled = OnExit();
		break;
	

	case SDL_SYSWMEVENT: 
		//Ignore
		break;


	default: 
		eventHandled = OnUser(Event.user.type, Event.user.code, Event.user.data1, Event.user.data2);
		break;

	}
	return eventHandled;
}