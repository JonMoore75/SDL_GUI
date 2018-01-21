
#pragma once

#include "GUIWidget.h"



/**
 * \class Slider GUISlider.h 
 *
 * \brief Fractional slider widget with mouse control.
 */
namespace SGUI
{
	class Slider : public Widget 
	{
	public:
		Slider(Widget* parent);

		float value() const { return mValue; }
		void setValue(float value) { mValue = value; }

		const Color& highlightColor() const { return mHighlightColor; }
		void setHighlightColor(const Color& highlightColor) { mHighlightColor = highlightColor; }

		std::pair<float, float> range() const { return mRange; }
		void setRange(std::pair<float, float> range) { mRange = range; }

		std::pair<float, float> highlightedRange() const { return mHighlightedRange; }
		void setHighlightedRange(std::pair<float, float> highlightedRange) { mHighlightedRange = highlightedRange; }

		std::function<void(float)> callback() const { return mCallback; }
		void setCallback(const std::function<void(float)>& callback) { mCallback = callback; }

		std::function<void(float)> finalCallback() const { return mFinalCallback; }
		void setFinalCallback(const std::function<void(float)>& callback) { mFinalCallback = callback; }

		virtual bool mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers) override;
		virtual bool mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers) override;

		virtual Point preferredSize(Renderer& renderer) const override;
		virtual void Render(Renderer& renderer, Point& offset) override;

	protected:
		void UpdateSliderPosition(const Point &p);

		float mValue{ 0.0f };

		std::function<void(float)> mCallback;
		std::function<void(float)> mFinalCallback;

		std::pair<float, float> mRange{ 0.f, 1.f };
		std::pair<float, float> mHighlightedRange{ 0.f, 0.f };

		Color mHighlightColor{ 255, 80, 80, 70 };
	};
}
