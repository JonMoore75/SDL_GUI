#pragma once

#include <functional>
#include <sstream>

#include "utf8/utf8.h" // http://utfcpp.sourceforge.net/

#include "Texture.h"
#include "Font_TTF.h"

#include "GUICommon.h"
#include "GUIWidget.h"
#include "TimeKeeper.h"

std::string GetClipboardText();

bool SetClipboardText(const std::string& str);

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
		enum class Alignment {
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

		void setSize(const Point& size)
		{
			Widget::setSize(size);
			CalculateTextClipRect();
			CalculateInitialTextOffset();
		}

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

// 		/// Set the \ref Theme used to draw this widget
// 		virtual void setTheme(Theme *theme) override;

		/// Set the change callback
		std::function<bool(const std::string& str)> callback() const { return mCallback; }
		void setCallback(const std::function<bool(const std::string& str)> &callback) { mCallback = callback; }

		virtual bool mouseButtonEvent(const Point& p, MouseBut button, bool down, SDL_Keymod modifiers) override;

		virtual bool mouseMotionEvent(const Point& p, const Point& rel, MouseButStatus buttons, SDL_Keymod modifiers) override;

		virtual bool mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, SDL_Keymod modifiers) override;

		virtual bool focusEvent(bool focused) override;

		virtual bool keyboardEvent(SDL_Scancode scan, SDL_Keycode key, bool down, SDL_Keymod modifiers) override;

		void SelectWithShift(SDL_Keymod modifiers);

		virtual bool keyboardCharacterEvent(const std::string& codepoint) override;

		void Render(Renderer& renderer) override;

		virtual Point preferredSize(Renderer& renderer) const override;
// 		virtual void save(Serializer &s) const override;
// 		virtual bool load(Serializer &s) override;
	protected:

		void PreRenderText(Renderer& renderer);

		void CalculateTextClipRect();

		void CalculateGlyphs(FontTTF& mFont);

		void RenderBackground(Renderer& renderer);

		void RenderBorder(Renderer& renderer);

		void RenderText(Renderer& renderer);

		void CalculateInitialTextOffset();

		void CheckCursorScroll();


		bool checkFormat(const std::string& input, const std::string& format);

		bool copySelection();

		void pasteFromClipboard();

		bool deleteSelection();

		void DeleteCharacters(int begin, int end);

		void updateCursor(int posx, SDL_Keymod modifiers);

		int cursorIndex2Position(size_t index);

		int position2CursorIndex(int posx);



		/// The location (if any) for the spin area.
		enum class SpinArea { None, Top, Bottom };
		SpinArea spinArea(const Point& pos);

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
		int mUnitsImage;
		std::function<bool(const std::string& str)> mCallback;
		int mCursorPos;
		int mSelectionPos;
		double mLastClick;
		TimeKeeper mTimer;
		Texture mImageText;		// Texture with the pre-rendered text
		std::vector<GlyphPosition> mGlyphs;
		Rect mTextClipRect;		// The clipping rect for the text
		Point mTextOffset;		// Relative to top left corner of mTextClipRect
	};
}

