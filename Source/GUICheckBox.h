#pragma once

#include <string>
#include <functional>

#include "Common.h"
#include "GUIWidget.h"
#include "TextObject.h"


/**
* \class CheckBox checkbox.h 
*
* \brief Two-state check box widget.
*
* \remark
*     This class overrides \ref nanogui::Widget::mIconExtraScale to be ``1.2f``,
*     which affects all subclasses of this Widget.  Subclasses must explicitly
*     set a different value if needed (e.g., in their constructor).
*/
namespace SGUI
{
	class CheckBox : public Widget
	{
	public:
		/**
		* Adds a CheckBox to the specified ``parent``.
		*
		* \param parent
		*     The Widget to add this CheckBox to.
		*
		* \param caption
		*     The caption text of the CheckBox (default ``"Untitled"``).
		*
		* \param callback
		*     If provided, the callback to execute when the CheckBox is checked or
		*     unchecked.  Default parameter function does nothing.  See
		*     \ref nanogui::CheckBox::mPushed for the difference between "pushed"
		*     and "checked".
		*/ 	
		CheckBox(Widget* parent, const std::string& caption = "Untitled",
			const std::function<void(bool)>& callback = std::function<void(bool)>());

		void PreRender(Renderer& renderer, TextObject::CLIPALIGN align = TextObject::CLIPCENTRE);

		/// Whether or not this CheckBox is currently checked.
		const bool& checked() const { return mChecked; }

		/// Sets whether or not this CheckBox is currently checked.
		void setChecked(const bool& checked) { mChecked = checked; }


		/// Returns the current callback of this CheckBox.
		std::function<void(bool)> callback() const { return mCallback; }

		/// Sets the callback to be executed when this CheckBox is checked / unchecked.
		void setCallback(const std::function<void(bool)> &callback) { mCallback = callback; }

		/**
		* The mouse button callback will return ``true`` when all three conditions are met:
		*
		* 1. This CheckBox is "enabled" (see \ref nanogui::Widget::mEnabled).
		* 2. ``p`` is inside this CheckBox.
		* 3. ``button`` is ``GLFW_MOUSE_BUTTON_1`` (left mouse click).
		*
		* Since a mouse button event is issued for both when the mouse is pressed, as well
		* as released, this function sets \ref nanogui::CheckBox::mPushed to ``true`` when
		* parameter ``down == true``.  When the second event (``down == false``) is fired,
		* \ref nanogui::CheckBox::mChecked is inverted and \ref nanogui::CheckBox::mCallback
		* is called.
		*
		* That is, the callback provided is only called when the mouse button is released,
		* **and** the click location remains within the CheckBox boundaries.  If the user
		* clicks on the CheckBox and releases away from the bounds of the CheckBox,
		* \ref nanogui::CheckBox::mPushed is simply set back to ``false``.
		*/
		virtual bool mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers) override;

		/// The preferred size of this CheckBox
		virtual Point preferredSize(Renderer& renderer) const override;

		/// Draws this CheckBox
		virtual void Render(Renderer& renderer, Point& offset) override;

	protected:
		Point SizeCheck() const;
		Point SizeText() const;

// 		/// The caption text of this CheckBox.
// 		std::string mCaption;

		/**
		* Internal tracking variable to distinguish between mouse click and release.
		* \ref nanogui::CheckBox::mCallback is only called upon release.  See
		* \ref nanogui::CheckBox::mouseButtonEvent for specific conditions.
		*/
		bool mPushed{ false };

		/// Whether or not this CheckBox is currently checked or unchecked.
		bool mChecked{ false };

		Texture mCheckMark; // TODO centralize the icons in a texture sheet?  Only issue is sizing
		Texture mImageNormal;
		Texture mImagePushed;
		TextObject mImageText;

		int textYPad{ 10 };
		int tickPad{ 10 };
		Point mTextPos{ 0, 0 };
		Point mTickBoxPos{ 0, 0 };
		Point mCheckPos{ 0, 0 };

		/// The function to execute when \ref nanogui::CheckBox::mChecked is changed.
		std::function<void(bool)> mCallback;
	};

}