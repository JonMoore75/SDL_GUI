#include "GUISlider.h"

#include <algorithm>
#include <cmath>

#include "Common.h"
#include "Renderer.h"
#include "GUITheme.h"

namespace SGUI
{
	Slider::Slider(Widget *parent)
		: Widget(parent)
	{
	}

	Point Slider::preferredSize(Renderer& renderer) const
	{
		return Point{ 70, 20 };
	}

	bool Slider::mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers)
	{
		if (!mEnabled)
			return false;

		UpdateSliderPosition(p);

		if (mCallback)
			mCallback(mValue);
		return true;
	}

	bool Slider::mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers)
	{
		if (!mEnabled)
			return false;

		UpdateSliderPosition(p);

		if (mCallback)
			mCallback(mValue);
		if (mFinalCallback && !down)
			mFinalCallback(mValue);
		return true;
	}

	void Slider::UpdateSliderPosition(const Point &p)
	{
		const float kr = std::floor(mSize.y * 0.4f);
		const float kshadow = 3;
		const float startX = kr + kshadow + mPos.x - 1;
		const float widthX = mSize.x - 2 * (kr + kshadow);

		float value = (p.x - startX) / widthX;
		value = value * (mRange.second - mRange.first) + mRange.first;
		mValue = std::min(std::max(value, mRange.first), mRange.second);
	}

	void Slider::Render(Renderer& renderer, Point& offset)
	{
		Point centre = mSize / 2;

		const int kr = static_cast<int>(mSize.y * 0.4f);
		const int kshadow = 3;
		const int startX = kr + kshadow - 1;
		const int widthX = mSize.x - 2 * (kr + kshadow);
		const int trackWidth = 6;
		const int startY = centre.y - trackWidth / 2;

		// Draw slider track
		Rect trackRect{ offset.x + startX, offset.y + startY, widthX, trackWidth };
		Uint8 alpha = mEnabled ? 128 : 210;
		renderer.FillRect(trackRect, Color{ 0, alpha});
		renderer.OutlineRect(trackRect, Color{ 0, 255 });

		// Draw slider knob
		int knobX = static_cast<int>(startX + (mValue - mRange.first) / (mRange.second - mRange.first) * widthX);
		Point knobPos{ knobX - kr, centre.y - kr };

		Rect knobRect{ offset + knobPos, Point{2 * kr, 2 * kr} };
		renderer.FillRect(knobRect, mTheme->mBorderLight);
		renderer.OutlineRect(knobRect, mTheme->mBorderDark);
	}
}

