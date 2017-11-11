#pragma once

#include "GUIWidget.h"
#include "TextObject.h"
#include "GUILayout.h"

class Renderer;

namespace SGUI
{
	class Window : public Widget
	{
	public:
		friend class GroupLayout;
		Window(Widget* parent, const std::string& title = "Untitled") : Widget{ parent }, mTitle{ title } {}
		Window(Widget* parent, const SGUI::Point& pos, const SGUI::Point& size, const std::string& title = "Untitled");
		virtual ~Window() { CleanUp(); }

		void CleanUp() {}

		/// Return the window title
		const std::string& title() const { return mTitle.GetText(); }
		/// Set the window title
		void setTitle(const std::string &title) { mTitle = title; }

		/// Is this a model dialog?
		bool modal() const { return mModal; }
		/// Set whether or not this is a modal dialog
		void setModal(bool modal) { mModal = modal; }

		/// Handle window drag events
		virtual bool mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, SDL_Keymod modifiers) override;
		/// Handle mouse events recursively and bring the current window to the top
		virtual bool mouseButtonEvent(const Point& p, MouseBut button, bool down, SDL_Keymod modifiers) override;
		/// Accept scroll events and propagate them to the widget under the mouse cursor
		virtual bool scrollEvent(const  Point& p, const  Point& rel) override;

		void SetBackgroundColor(const SGUI::Color& col) { mBackgrdColor = col; }

		void Render(Renderer& renderer, Point& offset) override;

		Point preferredSize(Renderer& renderer) const override;

	private:
		SGUI::Color mWindowFillUnfocused{43, 230};
		SGUI::Color mWindowFillFocused{45, 230};
		SGUI::Color mWindowTitleUnfocused{220, 160};
		SGUI::Color mWindowTitleFocused{ 255, 190 };
		SGUI::Color mBackgrdColor{ 255, 255 };
		SGUI::Color mEdgeColor = { 0, 0, 255, 255 };

		int mEdge = 2;

		//std::string mTitle;
		TextObject mTitle;
		bool mModal = false;
		bool mDrag = false;
		int mWindowHeaderHeight = 30;
	};

}