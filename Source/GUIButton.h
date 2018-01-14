#pragma once
#include <functional>

#include "Texture.h"
#include "GUIWidget.h"
#include "TextObject.h"

#include "Common.h"

class Renderer;



namespace SGUI
{
	class Button : public Widget
	{
	public:
		/// Flags to specify the button behavior (can be combined with binary OR)
		enum Flags {
			NormalButton = (1 << 0),	///< A normal Button
			RadioButton = (1 << 1),		///< A radio Button
			ToggleButton = (1 << 2),	///< A toggle Button
			PopupButton = (1 << 3)		///< A popup Button
		};

		Button(Widget* parent, std::string text) : Widget{parent}, mImageText{ text } { }
		virtual ~Button() { CleanUp(); }

		void CleanUp();

		void PreRender(Renderer& renderer, TextObject::CLIPALIGN align = TextObject::CLIPCENTRE);

		/// Returns the caption of this Button
		const std::string& caption() const { return mImageText.GetText(); }

		/// Sets the caption of this Button
		void setCaption(const std::string& caption) { mImageText.SetText(caption); }

		/// The current flags of this Button 
		int flags() const { return mFlags; }

		/// Sets the flags of this Button 
		void setFlags(int buttonFlags) { mFlags = buttonFlags; }

		/// Whether or not this Button is currently pushed
		bool pushed() const { return mPushed; }

		/// Sets whether or not this Button is currently pushed
		void setPushed(bool pushed) { mPushed = pushed; }

		/// Set the button group (for radio buttons)
		void setButtonGroup(const std::vector<Button*>& buttonGroup) { mButtonGroup = buttonGroup; }

		/// The current button group (for radio buttons)
		const std::vector<Button*>& buttonGroup() const { return mButtonGroup; }

		/// Set the push callback (for any type of button)
		std::function<void()> callback() const { return mCallback; }
		void setCallback(const std::function<void()>& callback) { mCallback = callback; }

		/// Set the change callback (for toggle buttons)
		std::function<void(bool)> changeCallback() const { return mChangeCallback; }
		void setChangeCallback(const std::function<void(bool)>& callback) { mChangeCallback = callback; }


		bool mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers) override;

		void Render(Renderer& renderer, Point& offset) override;

		/// Compute the preferred size of the widget
		virtual Point preferredSize(Renderer& renderer) const override;


	private:
		void PopupButtonDown();
		void RadioButtonDown();

		bool mPushed{ false };

		std::function<void()>		mCallback;
		std::function<void(bool)>	mChangeCallback;

		/// The current flags of this button
		int mFlags{ NormalButton };

		/// The icon of this Button (``0`` means no icon).
		int mIcon{ 0 };

		/// The button group for radio buttons.
		std::vector<Button*> mButtonGroup;

		Texture mImageNormal;
		Texture mImagePushed;
		Texture mImageFocus;
		TextObject mImageText;
	};
}
