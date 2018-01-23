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
		Window(Widget* parent, const Point& pos, const Point& size, const std::string& title = "Untitled");
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
		virtual bool mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers) override;
		/// Handle mouse events recursively and bring the current window to the top
		virtual bool mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers) override;
		/// Accept scroll events and propagate them to the widget under the mouse cursor
		virtual bool scrollEvent(const  Point& p, const  Point& rel) override;

//		void SetBackgroundColor(const Color& col) { mBackgrdColor = col; }

		void Render(Renderer& renderer , Point& offset) override;

		Point preferredSize(Renderer& renderer) const override;

		virtual void refreshRelativePlacement() {
			/* Overridden in \ref Popup */
		}

	private:
// 		Color mWindowFillUnfocused{43, 230};
// 		Color mWindowFillFocused{45, 230};
// 		Color mWindowTitleUnfocused{220, 160};
// 		Color mWindowTitleFocused{ 255, 190 };
// 		Color mBackgrdColor{ 255, 255 };
// 		Color mEdgeColor = { 0, 0, 255, 255 };

		int mEdge = 2;

		//std::string mTitle;
		TextObject mTitle;
		bool mModal = false;
		bool mDrag = false;
//		int mWindowHeaderHeight = 30;
	};

}