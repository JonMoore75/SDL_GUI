#pragma once

#include <string>
#include "GUICommon.h"
#include "GUIWidget.h"
#include "TextObject.h"

namespace SGUI
{
	class Label : public Widget
	{
	public:
		Label(Widget* parent, const std::string& caption, int fontSize = -1);

		/// Get the label's text caption
		const std::string& caption() const { return mCaption.GetText(); }
		/// Set the label's text caption
		void setCaption(const std::string& caption) { mCaption = caption; }

// 		/// Set the currently active font (2 are available by default: 'sans' and 'sans-bold')
// 		void setFont(const std::string& font) { mFont = font; }
// 		/// Get the currently active font
// 		const std::string& font() const { return mFont; }

		/// Get the label color
		Color color() const { return mColor; }
		/// Set the label color
		void setColor(const Color& color) { mColor = color; }

// 		/// Set the \ref Theme used to draw this widget
// 		virtual void setTheme(Theme *theme) override;

		/// Compute the size needed to fully display the label
		virtual Point preferredSize(Renderer& renderer) const override;

		/// Draw the label
		virtual void Render(Renderer& renderer, Point& offset) override;

// 		virtual void save(Serializer &s) const override;
// 		virtual bool load(Serializer &s) override;
	protected:
		TextObject mCaption;
//		std::string mFont;
		Color mColor;
	};
}
