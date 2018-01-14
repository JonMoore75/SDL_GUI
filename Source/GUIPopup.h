#pragma once

#include "Common.h"
#include "GUIWindow.h"

class Renderer;

/**
* \class Popup popup.h 
*
* \brief Popup window for combo boxes, popup buttons, nested dialogs etc.
*
* Usually the Popup instance is constructed by another widget (e.g. PopupButton)
* and does not need to be created by hand.
*/
namespace SGUI
{
	class Widget;

	class Popup : public Window
	{
	public:
		enum Side { Left = 0, Right };

		/// Create a new popup parented to a screen (first argument) and a parent window
		Popup(Widget* parent, Window* parentWindow) : Window(parent, ""), mParentWindow(parentWindow) {}

		/// Return the anchor position in the parent window; the placement of the popup is relative to it
		void setAnchorPos(const Point& anchorPos) { mAnchorPos = anchorPos; }
		/// Set the anchor position in the parent window; the placement of the popup is relative to it
		const Point &anchorPos() const { return mAnchorPos; }

		/// Set the anchor height; this determines the vertical shift relative to the anchor position
		void setAnchorHeight(int anchorHeight) { mAnchorHeight = anchorHeight; }
		/// Return the anchor height; this determines the vertical shift relative to the anchor position
		int anchorHeight() const { return mAnchorHeight; }

		/// Set the side of the parent window at which popup will appear
		void setSide(Side popupSide) { mSide = popupSide; }
		/// Return the side of the parent window at which popup will appear
		Side side() const { return mSide; }

		/// Return the parent window of the popup
		Window* parentWindow() { return mParentWindow; }
		/// Return the parent window of the popup
		const Window* parentWindow() const { return mParentWindow; }

		/// Invoke the associated layout generator to properly place child widgets, if any
		virtual void performLayout(Renderer& renderer);

		/// Draw the popup window
		void Render(Renderer& renderer, Point& offset) override;
	protected:
		/// Internal helper function to maintain nested window position values
		virtual void refreshRelativePlacement() override;

	protected:
		Window * mParentWindow{ nullptr };
		Point mAnchorPos;
		int mAnchorHeight{ 30 };
		Side mSide{ Side::Right };
	};
}
