#pragma once

#include <functional>
#include <sstream>

#include "utf8/utf8.h" // http://utfcpp.sourceforge.net/

#include "Texture.h"
#include "Font_TTF.h"

#include "Common.h"
#include "GUIWidget.h"
#include "TimeKeeper.h"

struct GlyphPosition
{
	int index;	// Index in character array for start of that character
	int x;		// Left-hand x position of glyph
	int cx;		// x position of glyph centre
	int width;	// Glyph width
};

namespace SGUI
{
	class TextBox : public Widget
	{
	public:
		/// How to align the text in the text box.
		enum class Alignment
		{
			Left,
			Center,
			Right
		};

		TextBox(Widget* parent, const std::string& value = "Untitled");

		bool editable() const { return mEditable; }
		void TextBox::setEditable(bool editable)
		{
			mEditable = editable;
			//setCursor(editable ? Cursor::IBeam : Cursor::Arrow);
		}

		void setPosition(const Point& pos)
		{
			Widget::setPosition(pos);
			CalculateTextClipRect();
		}

		void setSize(const Point& size);

		bool spinnable() const { return mSpinnable; }
		void setSpinnable(bool spinnable) { mSpinnable = spinnable; }

		const std::string& value() const { return mValue; }
		void setValue(const std::string &value) { mValue = value; }

		const std::string& defaultValue() const { return mDefaultValue; }
		void setDefaultValue(const std::string &defaultValue) { mDefaultValue = defaultValue; }

		Alignment alignment() const { return mAlignment; }
		void setAlignment(Alignment align)
		{
			mAlignment = align;
			CalculateInitialTextOffset();
		}

		const std::string& units() const { return mUnits; }
		void setUnits(const std::string &units) { mUnits = units; }

		int unitsImage() const { return mUnitsImage; }
		void setUnitsImage(int image) { mUnitsImage = image; }

		/// Return the underlying regular expression specifying valid formats
		const std::string& format() const { return mFormat; }
		/// Specify a regular expression specifying valid formats
		void setFormat(const std::string& format) { mFormat = format; }

		/// Set the \ref Theme used to draw this widget
		virtual void setTheme(Theme* theme) override;

		/// Set the change callback
		std::function<bool(const std::string& str)> callback() const { return mCallback; }
		void setCallback(const std::function<bool(const std::string& str)> &callback) { mCallback = callback; }

		virtual bool mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers) override;
		virtual bool mouseMotionEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers) override;
		virtual bool mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers) override;

		virtual bool focusEvent(bool focused) override;

		virtual bool keyboardEvent(Scancode scan, Keycode key, bool down, Keymod modifiers) override;
		virtual bool keyboardCharacterEvent(const std::string& codepoint) override;

		void Render(Renderer& renderer, Point& offset) override;

		virtual Point preferredSize(Renderer& renderer) const override;

		virtual void setEnabled(bool enabled) override;
		// 		virtual void save(Serializer &s) const override;
		// 		virtual bool load(Serializer &s) override;
	protected:
		void SelectWithShift(Keymod modifiers);
		void PreRenderText(Renderer& renderer);

		void PreRenderSpinArrows(Renderer& renderer);

		void CalculateTextClipRect();

		void CalculateGlyphs(FontTTF& mFont);

		void RenderBackground(Renderer& renderer, Point& offset);

		void RenderBorder(Renderer& renderer, Point& offset);

		void RenderText(Renderer& renderer, Point& offset);

		void CalculateInitialTextOffset();

		void CheckCursorScroll();

		bool checkFormat(const std::string& input, const std::string& format);

		bool copySelection();

		void pasteFromClipboard();

		bool deleteSelection();

		void DeleteCharacters(int begin, int end);

		void updateCursor(int posx, Keymod modifiers);

		int cursorIndex2Position(size_t index);

		int position2CursorIndex(int posx);

		/// The location (if any) for the spin area.
		enum class SpinArea { None, Top, Bottom };
		SpinArea spinArea(const Point& pos);

		void RenderSpinArrows(Renderer& renderer, Point& offset);

	protected:
		bool mEditable;
		bool mSpinnable;
		bool mCommitted;
		bool mValidFormat;
		bool mRedrawText;
		bool mDragActive;

		Alignment mAlignment;

		std::string mValue;
		std::string mValueBackup;
		std::string mDefaultValue;
		std::string mUnits;
		std::string mFormat;

		SpinArea mSpinArea{ SpinArea::None };

		std::vector<GlyphPosition> mGlyphs;

		int mUnitsImage;
		int mCursorPos;
		int mSelectionPos;
		double mLastClick;

		int mSpinnerSize{ 14 };

		TimeKeeper mTimer;

		Texture mImageText;		// Texture with the pre-rendered text
		Texture mImageSpinUp;	// Textures with the spin arrows
		Texture mImageSpinDn;	// Textures with the spin arrows

		Rect mTextClipRect;		// The clipping rect for the text
		Point mTextOffset;		// Relative to top left corner of mTextClipRect

		std::function<bool(const std::string& str)> mCallback;
	};


	/**
	* \class IntBox textbox.h nanogui/textbox.h
	*
	* \brief A specialization of TextBox for representing integral values.
	*
	* Template parameters should be integral types, e.g. ``int``, ``long``,
	* ``uint32_t``, etc.
	*/
	template <typename Scalar>
	class IntBox : public TextBox 
	{
	public:
		IntBox(Widget* parent, Scalar value = (Scalar)0) : TextBox(parent) 
		{
			setDefaultValue("0");
			setFormat(std::is_signed<Scalar>::value ? "[-]?[0-9]*" : "[0-9]*");
			setValueIncrement(1);
			setMinMaxValues(std::numeric_limits<Scalar>::lowest(), std::numeric_limits<Scalar>::max());
			setValue(value);
			setSpinnable(false);
		}

		Scalar value() const 
		{
			std::istringstream iss(TextBox::value());
			Scalar value = 0;
			iss >> value;
			return value;
		}

		void setValue(Scalar value) 
		{
			Scalar clampedValue = std::min(std::max(value, mMinValue), mMaxValue);
			TextBox::setValue(std::to_string(clampedValue));
		}

		void setCallback(const std::function<void(Scalar)> &cb) 
		{
			TextBox::setCallback(
				[cb, this](const std::string &str) 
				{
					std::istringstream iss(str);
					Scalar value = 0;
					iss >> value;
					setValue(value);
					cb(value);
					return true;
				}
			);
		}

		void setValueIncrement(Scalar incr) 
		{
			mValueIncrement = incr;
		}
		void setMinValue(Scalar minValue)
		{
			mMinValue = minValue;
		}
		void setMaxValue(Scalar maxValue) 
		{
			mMaxValue = maxValue;
		}
		void setMinMaxValues(Scalar minValue, Scalar maxValue) 
		{
			setMinValue(minValue);
			setMaxValue(maxValue);
		}

		virtual bool mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers) override
		{
			if ((mEditable || mSpinnable) && down)
			{
				mMouseDownValue = value();
				mMouseDownPos = p;
			}

			if (!down)
				mMouseDownPos = Point{ -1,-1 };

			SpinArea area = spinArea(p);
			if (mSpinnable && area != SpinArea::None && down && !focused()) 
			{
				if (area == SpinArea::Top) 
				{
					setValue(value() + mValueIncrement);
					mRedrawText = true;
					if (mCallback)
						mCallback(mValue);
				}
				else if (area == SpinArea::Bottom) 
				{
					setValue(value() - mValueIncrement);
					mRedrawText = true;
					if (mCallback)
						mCallback(mValue);
				}
				return true;
			}

			return TextBox::mouseButtonEvent(p, button, down, modifiers);
		}
		virtual bool mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers) override
		{
			if (TextBox::mouseDragEvent(p, rel, buttons, modifiers)) 
			{
				return true;
			}
			if (mSpinnable && !focused() && buttons.mRight && mMouseDownPos.x != -1) 
			{
				int valueDelta = static_cast<int>((p.x - mMouseDownPos.x) / float(10));
				setValue(mMouseDownValue + valueDelta * mValueIncrement);
				if (mCallback)
					mCallback(mValue);
				return true;
			}
			return false;
		}
		virtual bool scrollEvent(const Point& p, const Point& rel) override
		{
			if (Widget::scrollEvent(p, rel)) 
			{
				return true;
			}
			if (mSpinnable && !focused()) 
			{
				int valueDelta = (rel.y > 0) ? 1 : -1;
				setValue(value() + valueDelta * mValueIncrement);
				if (mCallback)
					mCallback(mValue);
				return true;
			}
			return false;
		}
	private:
		Scalar mMouseDownValue;
		Scalar mValueIncrement;
		Scalar mMinValue, mMaxValue;
		Point mMouseDownPos{ -1,-1 };
	};

	/**
	* \class FloatBox textbox.h nanogui/textbox.h
	*
	* \brief A specialization of TextBox representing floating point values.

	* Template parameters should be float types, e.g. ``float``, ``double``,
	* ``float64_t``, etc.
	*/
	template <typename Scalar>
	class FloatBox : public TextBox 
	{
	public:
		FloatBox(Widget* parent, Scalar value = (Scalar) 0.f) : TextBox(parent) 
		{
			mNumberFormat = sizeof(Scalar) == sizeof(float) ? "%.4g" : "%.7g";
			setDefaultValue("0");
			setFormat("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?");
			setValueIncrement((Scalar) 0.1);
			setMinMaxValues(std::numeric_limits<Scalar>::lowest(), std::numeric_limits<Scalar>::max());
			setValue(value);
			setSpinnable(false);
		}

		std::string numberFormat() const { return mNumberFormat; }
		void numberFormat(const std::string &format) { mNumberFormat = format; }

		Scalar value() const 
		{
			return (Scalar)std::stod(TextBox::value());
		}

		void setValue(Scalar value) 
		{
			Scalar clampedValue = std::min(std::max(value, mMinValue), mMaxValue);
			char buffer[50];
			NANOGUI_SNPRINTF(buffer, 50, mNumberFormat.c_str(), clampedValue);
			TextBox::setValue(buffer);
		}

		void setCallback(const std::function<void(Scalar)> &cb) 
		{
			TextBox::setCallback([cb, this](const std::string &str) 
			{
				Scalar scalar = (Scalar)std::stod(str);
				setValue(scalar);
				cb(scalar);
				return true;
			});
		}

		void setValueIncrement(Scalar incr) 
		{
			mValueIncrement = incr;
		}
		void setMinValue(Scalar minValue) 
		{
			mMinValue = minValue;
		}
		void setMaxValue(Scalar maxValue) 
		{
			mMaxValue = maxValue;
		}
		void setMinMaxValues(Scalar minValue, Scalar maxValue) 
		{
			setMinValue(minValue);
			setMaxValue(maxValue);
		}

		virtual bool mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers) override
		{
			if ((mEditable || mSpinnable) && down)
			{
				mMouseDownValue = value();
				mMouseDownPos = p;
			}

			if (!down)
				mMouseDownPos = Point{ -1,-1 };


			SpinArea area = spinArea(p);
			if (mSpinnable && area != SpinArea::None && down && !focused()) 
			{
				if (area == SpinArea::Top) 
				{
					setValue(value() + mValueIncrement);
					if (mCallback)
						mCallback(mValue);
				}
				else if (area == SpinArea::Bottom) 
				{
					setValue(value() - mValueIncrement);
					if (mCallback)
						mCallback(mValue);
				}
				return true;
			}

			return TextBox::mouseButtonEvent(p, button, down, modifiers);
		}
		virtual bool mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers) override
		{
			if (TextBox::mouseDragEvent(p, rel, button, modifiers)) 
			{
				return true;
			}
			if (mSpinnable && !focused() && button == 2 /* 1 << GLFW_MOUSE_BUTTON_2 */ && mMouseDownPos.x() != -1) 
			{
				int valueDelta = static_cast<int>((p.x - mMouseDownPos.x) / float(10));
				setValue(mMouseDownValue + valueDelta * mValueIncrement);
				if (mCallback)
					mCallback(mValue);
				return true;
			}
			return false;
		}
		virtual bool scrollEvent(const Point& p, const Point& rel) override
		{
			if (Widget::scrollEvent(p, rel)) 
			{
				return true;
			}
			if (mSpinnable && !focused()) 
			{
				int valueDelta = (rel.y > 0) ? 1 : -1;
				setValue(value() + valueDelta * mValueIncrement);
				if (mCallback)
					mCallback(mValue);
				return true;
			}
			return false;
		}

	private:
		std::string mNumberFormat;
		Scalar mMouseDownValue;
		Scalar mValueIncrement;
		Scalar mMinValue, mMaxValue;
		Point mMouseDownPos{ -1,-1 };
	};
}
