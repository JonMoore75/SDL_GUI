#pragma once
#include <functional>

#include "Texture.h"
#include "GUIWidget.h"
#include "TextObject.h"

#include "GUICommon.h"

class Renderer;



namespace SGUI
{
	class Button : public Widget
	{
	public:
		Button(Widget* parent, std::string text) : Widget{parent}, mImageText{ text } { }
		virtual ~Button() { CleanUp(); }

		void CleanUp();

		void PreRender(Renderer& renderer, TextObject::CLIPALIGN align = TextObject::CLIPCENTRE);

		/// Set the push callback (for any type of button)
		std::function<void()> callback() const { return mCallback; }
		void setCallback(const std::function<void()>& callback) { mCallback = callback; }

		/// Set the change callback (for toggle buttons)
		std::function<void(bool)> changeCallback() const { return mChangeCallback; }
		void setChangeCallback(const std::function<void(bool)>& callback) { mChangeCallback = callback; }


		bool mouseButtonEvent(const Point& p, MouseBut button, bool down, SDL_Keymod modifiers) override;

		void Render(Renderer& renderer, Point& offset) override;

		/// Compute the preferred size of the widget
		virtual Point preferredSize(Renderer& renderer) const override;


	private:
		bool mPushed{ false };

		std::function<void()>		mCallback;
		std::function<void(bool)>	mChangeCallback;

		Texture mImageNormal;
		Texture mImagePushed;
		Texture mImageFocus;
		TextObject mImageText;
	};
}



