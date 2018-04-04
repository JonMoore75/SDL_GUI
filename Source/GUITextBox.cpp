#include "GUITextBox.h"

#include <assert.h>
#include <regex>
#include <algorithm>

#include "Renderer.h"
#include "GUITheme.h"
#include "entypo.h"


namespace SGUI
{
	TextBox::TextBox(Widget* parent, const std::string& value /*= "Untitled"*/) :
		Widget{ parent },
		mEditable(false),
		mSpinnable(false),
		mCommitted(true),
		mDragActive(false),
		mValue(value),
		mDefaultValue(""),
		mAlignment(Alignment::Center),
		mUnits(""),
		mFormat(""),
		mUnitsImage(-1),
		mValidFormat(true),
		mRedrawText(true),
		mValueBackup(value),
		mCursorPos(-1),
		mSelectionPos(-1),
		mLastClick(0)
	{
		mTimer.Initialize();
		if (mTheme)
			mFontSize = mTheme->mTextBoxFontSize;
	}

	void TextBox::setSize(const Point& size)
	{
		Widget::setSize(size);
		CalculateTextClipRect();
		CalculateInitialTextOffset();
	}

	void TextBox::updateCursor(int posx, Keymod modifiers)
	{
		/// Handle mouse cursor events
		if (!mDragActive)
		{
			if (modifiers & KMOD_SHIFT)
			{
				if (mSelectionPos == -1)
					mSelectionPos = mCursorPos;
			}
			else
				mSelectionPos = -1;

			mCursorPos = position2CursorIndex(posx);
			mDragActive = true;
		}

		if (mCursorPos == mSelectionPos)
			mSelectionPos = -1;
	}

	void TextBox::setTheme(Theme* theme)
	{
		Widget::setTheme(theme);
		if (mTheme)
			mFontSize = mTheme->mTextBoxFontSize;
	}

	bool TextBox::mouseButtonEvent(const Point& p, MouseBut button, bool down, Keymod modifiers)
	{
		if (button == LEFT && down && !mFocused)
		{
			if (!mSpinnable || spinArea(p) == SpinArea::None) /* not on scrolling arrows */
				requestFocus();
		}

		if (down)
		{
			if (mEditable && focused() && PointinRect(p, mTextClipRect + mPos))
			{
				/// Double-click: select all text 
				double time = mTimer.GetCurrentTime();
				if (time - mLastClick < 0.25)
				{
					mSelectionPos = 0;
					mCursorPos = utf8::distance(mValue.begin(), mValue.end());
				}
				else /// Single click, update cursor position
					updateCursor(p.x, modifiers);
				mLastClick = time;

				return true;
			}
		}
		else /// Button up so cancel any dragging
			mDragActive = false;

		return false;
	}

	bool TextBox::mouseMotionEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers)
	{
		// 			if (!mEditable)
		// 				setCursor(Cursor::Arrow);
		// 			else if (mSpinnable && !focused() && spinArea(p) != SpinArea::None) /* scrolling arrows */
		// 				setCursor(Cursor::Hand);
		// 			else
		// 				setCursor(Cursor::IBeam);

		mSpinArea = spinArea(p);

		if (mEditable && focused())
			return true;

		return false;
	}

	bool TextBox::mouseDragEvent(const Point& p, const Point& rel, MouseButStatus buttons, Keymod modifiers)
	{
		if (mSelectionPos < 0)
			mSelectionPos = mCursorPos;

		mCursorPos = position2CursorIndex(p.x);

		if (mCursorPos == mSelectionPos)
			mSelectionPos = -1;

		if (mEditable && focused())
			return true;

		return false;
	}

	bool TextBox::focusEvent(bool focused)
	{
		Widget::focusEvent(focused);

		if (mEditable)
		{
			if (focused) // Gained focus
			{
				mValueBackup = mValue;
				mCommitted = false;
				mCursorPos = 0;
				mRedrawText = true;
			}
			else // Lost focus ie text committed via enter key etc
			{
				if (!mValidFormat) // if invalid text revert to previous text
					mValue = mValueBackup;
				else if (mValue == "") // if no value set to default
					mValue = mDefaultValue;

				// Callback to pass edited text to main code
				// if rejected then reset the value to previous one
				if (mCallback && !mCallback(mValue))
					mValue = mValueBackup;

				mCommitted = true;
				mCursorPos = -1;
				mSelectionPos = -1;
			}

			mValidFormat = (mValue == "") || checkFormat(mValue, mFormat);
		}

		return true;
	}

	bool TextBox::keyboardEvent(Scancode scan, Keycode key, bool down, Keymod modifiers)
	{
		if (!mEditable || !focused())
			return false;

		if (down)
		{
			switch (key)
			{
			case SDLK_LEFT:
				SelectWithShift(modifiers);
				if (mCursorPos > 0)
				{
					mCursorPos--;
					CheckCursorScroll();
				}
				break;
			case SDLK_RIGHT:
				SelectWithShift(modifiers);
				if (mCursorPos < utf8::distance(mValue.begin(), mValue.end()))
				{
					mCursorPos++;
					CheckCursorScroll();
				}
				break;
			case SDLK_HOME:
				SelectWithShift(modifiers);
				mCursorPos = 0;
				break;
			case SDLK_END:
				SelectWithShift(modifiers);
				mCursorPos = utf8::distance(mValue.begin(), mValue.end());
				break;
			case SDLK_BACKSPACE:
				if (!deleteSelection())
				{
					if (mCursorPos > 0)
					{
						DeleteCharacters(mCursorPos - 1, mCursorPos);

						mCursorPos--;
					}
				}
				break;
			case SDLK_DELETE:
				if (!deleteSelection())
				{
					if (mCursorPos < utf8::distance(mValue.begin(), mValue.end()))
						DeleteCharacters(mCursorPos, mCursorPos + 1);
				}
				break;
			case SDLK_RETURN:

				if (!mCommitted)
					focusEvent(false);
				break;
			case SDLK_a:
				if (modifiers & KMOD_CTRL)
				{
					mCursorPos = utf8::distance(mValue.begin(), mValue.end());
					mSelectionPos = 0;
				}
				break;
			case SDLK_x:
				if (modifiers & KMOD_CTRL)
				{
					copySelection();
					deleteSelection();
				}
				break;
			case SDLK_c:
				if (modifiers & KMOD_CTRL)
				{
					copySelection();
				}
				break;
			case SDLK_v:
				if (modifiers & KMOD_CTRL)
				{
					deleteSelection();
					pasteFromClipboard();
				}
				break;
			}

			mValidFormat = (mValue == "") || checkFormat(mValue, mFormat);
		}

		return true;
	}

	void TextBox::SelectWithShift(Keymod modifiers)
	{
		if (modifiers & KMOD_SHIFT)
		{
			if (mSelectionPos == -1)
				mSelectionPos = mCursorPos;
		}
		else
			mSelectionPos = -1;
	}

	bool TextBox::keyboardCharacterEvent(const std::string& codepoint)
	{
		if (!mEditable || !focused())
			return false;

		deleteSelection();

		// Find current position in the string
		auto insertPosIt = mValue.begin();
		if (mValue.size())
			utf8::advance(insertPosIt, mCursorPos, mValue.end());

		// insert it into string
		mValue.insert(insertPosIt, codepoint.begin(), codepoint.end());

		// Move to next position
		mCursorPos++;
		mRedrawText = true;

		mValidFormat = (mValue == "") || checkFormat(mValue, mFormat);

		return true;
	}

	void TextBox::PreRenderText(Renderer& renderer)
	{
		Color textColor = mEnabled ? mTheme->mTextColor : mTheme->mDisabledTextColor;

		// Render text to a texture
		FontTTF mFont;
		mFont.LoadFont(mTheme->mFontNormal.c_str(), fontSize(), textColor);
		if (mValue.size())
			mImageText.CreateFromText(renderer, mValue, mFont);
		else
			mImageText.Release();

		if (mImageText.GetWidth() < mTextClipRect.w)
			CalculateInitialTextOffset();

		CalculateGlyphs(mFont);

		// Make sure cursor hasn't scrolled out of clipping rect, 
		// if so shift text so it remains visible
		CheckCursorScroll();

		mFont.Release();
		
		PreRenderSpinArrows(renderer);
	}

	void TextBox::PreRenderSpinArrows(Renderer& renderer)
	{
		FontTTF mFont;
		int spinFontSize{ static_cast<int>(((mFontSize < 0) ? mTheme->mButtonFontSize : mFontSize)) };
		mFont.LoadFont(mTheme->mFontIcons.c_str(), spinFontSize, mTheme->mDisabledTextColor);

		auto upIcon = int_to_utf8(ENTYPO_ICON_CHEVRON_UP);
		auto dnIcon = int_to_utf8(ENTYPO_ICON_CHEVRON_DOWN);

		mImageSpinUp.SetAlphaBlend();
		mImageSpinDn.SetAlphaBlend();

		mImageSpinUp.CreateFromText(renderer, upIcon, mFont);
		mImageSpinDn.CreateFromText(renderer, dnIcon, mFont);

		int xSpacing = static_cast<int>(mSize.y * 0.3f);
		mSpinnerSize = mImageSpinUp.GetWidth() + xSpacing;
	}

	void TextBox::CalculateTextClipRect()
	{
		float xSpacing = mSize.y * 0.3f;
		float spinArrowsWidth = (mSpinnable) ? mSpinnerSize: 0.f;
		float unitWidth = 0;

		// clip visible text area
		int clipX = static_cast<int>(xSpacing + spinArrowsWidth - 1.0f);
		int clipY = static_cast<int>(1.0f);
		int clipWidth = static_cast<int>(mSize.x - unitWidth - spinArrowsWidth - 2 * xSpacing + 2.0f);
		int clipHeight = static_cast<int>(mSize.y - 3.0f);

		mTextClipRect = Rect{ clipX, clipY, clipWidth, clipHeight };
	}


	void TextBox::CalculateGlyphs(FontTTF& mFont)
	{
		int numCharacters = utf8::distance(mValue.begin(), mValue.end());
		mGlyphs.clear();

		auto it = mValue.begin();
		for (auto i = 0; i < numCharacters; i++)
		{
			int w = 0;
			int h = 0;
			int cw = 0;

			auto prev_it = it;
			assert(it != mValue.end());
			utf8::next(it, mValue.end());

			std::string utf8Char{ prev_it, it };
			std::string substr{ mValue.begin(), it };

			mFont.SizeUTF8(utf8Char, cw, h);
			mFont.SizeUTF8(substr, w, h);

			mGlyphs.emplace_back(GlyphPosition{ i, w - cw, w - cw / 2, cw });
		}
	}

	void TextBox::RenderBackground(Renderer& renderer, Point& offset)
	{
		Rect fillRect{ offset.x + 1, offset.y + 2,  mSize.x - 2, mSize.y - 2 };

		Color bg = Color(255, 32);			// Default
		Color fg1 = Color(150, 32);			// Focused
		Color fg2 = Color(255, 0, 0, 100);	// Invalid text

		Color fillCol = bg;

		if (mEditable && focused())
			fillCol = (mValidFormat) ? fg1 : fg2;
		else if (mSpinnable)
			fillCol = fg1;

		renderer.FillRect(fillRect, fillCol);
	}

	void TextBox::RenderBorder(Renderer& renderer, Point& offset)
	{
		Rect fillRect{ offset,  Point{mSize.x - 1, mSize.y - 1} };
		renderer.OutlineRect(fillRect, Color(0, 48));
	}

	void TextBox::RenderText(Renderer& renderer, Point& offset)
	{
		if (mRedrawText)
		{
			PreRenderText(renderer);
			mRedrawText = false;
		}

		Rect oldViewport = renderer.GetViewport();
		Rect textAbsClipRect = mTextClipRect + absolutePosition();

		Rect intersectRect = IntersectRect(oldViewport, textAbsClipRect);

		renderer.SetViewport(intersectRect);

		Point newOffset = textAbsClipRect.getPosition() - intersectRect.getPosition();

		// Draw background for highlighted text
		if (focused() && mSelectionPos > -1)
		{
			Color hiliteColor{ 255, 80 };
			int cursorx = cursorIndex2Position(mCursorPos) - mTextClipRect.x - mPos.x;
			int selectx = cursorIndex2Position(mSelectionPos) - mTextClipRect.x - mPos.x;
			int xMin = std::min(cursorx, selectx);
			int selWidth = abs(cursorx - selectx);
			Rect hilite_box{ newOffset.x + xMin, newOffset.y, newOffset.x + selWidth, newOffset.y + mTextClipRect.h };

			renderer.FillRect(hilite_box, hiliteColor);
		}

		mImageText.Render(renderer, newOffset + mTextOffset);

		// draw cursor
		if (focused() && !mCommitted)
		{
			Color cursorColor{ 255, 192, 0, 255 };
			int caretx = cursorIndex2Position(mCursorPos) - mTextClipRect.x - mPos.x;
			renderer.Line(newOffset.x + caretx, newOffset.y, newOffset.x + caretx, newOffset.y + mTextClipRect.h, cursorColor);
		}

		renderer.SetViewport(oldViewport);
	}

	void TextBox::CalculateInitialTextOffset()
	{
		// Find the size of the rendered text and store width and height
		// uses the SizeUTF8 function which sizes without rendering
		assert(mTheme);
		Point textSize = TextBounds(mTheme->mFontNormal, mFontSize, mValue);

		// x coord
		switch (mAlignment)
		{
		case Alignment::Left:
			mTextOffset.x = 0;
			break;
		case Alignment::Right:
			// subtract 1 at end to make sure we always have room to draw cursor on right aligned 
			// text boxes ie shifts left by 1 pixel
			mTextOffset.x = static_cast<int>(mTextClipRect.w - textSize.x - 1);
			break;
		case Alignment::Center:
			mTextOffset.x = static_cast<int>(mTextClipRect.w * 0.5f - textSize.x / 2);
			break;
		}

		// y coord
		mTextOffset.y = (mTextClipRect.h - textSize.y) / 2;
	}

	void TextBox::CheckCursorScroll()
	{
		if (mCursorPos > 0)
		{
			int numGlyphs = utf8::distance(mValue.begin(), mValue.end());
			int prevCPos = mCursorPos > 0 ? mCursorPos - 1 : 0;
			int nextCPos = mCursorPos < numGlyphs ? mCursorPos + 1 : numGlyphs;
			int prevCX = cursorIndex2Position(prevCPos) - mTextClipRect.x - mPos.x;
			int nextCX = cursorIndex2Position(nextCPos) - mTextClipRect.x - mPos.x;

			// If cursor has moved out of clipping rect to right, 
			// shift text left so it's visible
			if (nextCX >= mTextClipRect.w)
				mTextOffset.x += mTextClipRect.w - nextCX - 1;

			// If cursor has moved out of clipping rect to left, 
			// shift text right so it's visible
			if (prevCX <= 0)
				mTextOffset.x += -prevCX + 1;
		}
	}



	void TextBox::Render(Renderer& renderer, Point& offset)
	{
		Widget::Render(renderer, offset);

		SDL_BlendMode oldMode = renderer.SetRenderDrawMode(SDL_BLENDMODE_BLEND);

		RenderBackground(renderer, offset);
		RenderBorder(renderer, offset);
		RenderText(renderer, offset);

		/// Display spin arrows except when editing text
		if (mSpinnable && !focused())
			RenderSpinArrows(renderer, offset);

		renderer.SetRenderDrawMode(oldMode);


		// 			if (mUnitsImage > 0) {
		// 				int w, h;
		// 				nvgImageSize(ctx, mUnitsImage, &w, &h);
		// 				float unitHeight = mSize.y() * 0.4f;
		// 				unitWidth = w * unitHeight / h;
		// 				NVGpaint imgPaint = nvgImagePattern(
		// 					ctx, mPos.x() + mSize.x() - xSpacing - unitWidth,
		// 					drawPos.y() - unitHeight * 0.5f, unitWidth, unitHeight, 0,
		// 					mUnitsImage, mEnabled ? 0.7f : 0.35f);
		// 				nvgBeginPath(ctx);
		// 				nvgRect(ctx, mPos.x() + mSize.x() - xSpacing - unitWidth,
		// 					drawPos.y() - unitHeight * 0.5f, unitWidth, unitHeight);
		// 				nvgFillPaint(ctx, imgPaint);
		// 				nvgFill(ctx);
		// 				unitWidth += 2;
		// 			}
		// 			else if (!mUnits.empty()) {
		// 				unitWidth = nvgTextBounds(ctx, 0, 0, mUnits.c_str(), nullptr, nullptr);
		// 				nvgFillColor(ctx, Color(255, mEnabled ? 64 : 32));
		// 				nvgTextAlign(ctx, NVG_ALIGN_RIGHT | NVG_ALIGN_MIDDLE);
		// 				nvgText(ctx, mPos.x() + mSize.x() - xSpacing, drawPos.y(),
		// 					mUnits.c_str(), nullptr);
		// 				unitWidth += 2;
		// 			}
	}

	Point TextBox::preferredSize(Renderer& renderer) const
	{
		int uw = 0;
		if (mUnitsImage > 0)
		{
			// 		int w, h;
			// 		nvgImageSize(ctx, mUnitsImage, &w, &h);
			// 		float uh = size(1) * 0.4f;
			// 		uw = w * uh / h;
		}
		else if (!mUnits.empty())
		{
			//		uw = nvgTextBounds(ctx, 0, 0, mUnits.c_str(), nullptr, nullptr);
		}
		int sw = (mSpinnable) ? mSpinnerSize : 0;

		Point size = TextBounds(mTheme->mFontNormal, mFontSize, mValue);

		size.y = static_cast<int>(size.y * 1.4f);
		size.x = size.y + size.x + uw + sw;
		return size;
	}

	void TextBox::setEnabled(bool enabled)
	{
		if (enabled != mEnabled)
			mRedrawText = true;
		SGUI::Widget::setEnabled(enabled);
	}

	bool TextBox::checkFormat(const std::string& input, const std::string& format)
	{
		if (format.empty())
			return true;
		try
		{
			std::regex regex(format);
			return regex_match(input, regex);
		}
		catch (const std::regex_error &)
		{
			throw;
		}
	}

	bool TextBox::copySelection()
	{
		if (mSelectionPos > -1)
		{
			int begin = mCursorPos;
			int end = mSelectionPos;

			if (begin > end)
				std::swap(begin, end);

			// As utf8 for some languages have more than one char
			// per character then use ut8 advance to find correct locations
			auto beginIt = mValue.begin();
			utf8::advance(beginIt, begin, mValue.end());

			auto endIt = mValue.begin();
			utf8::advance(endIt, end, mValue.end());

			if (!SetClipboardText(std::string{ beginIt, endIt }))
			{
				std::string errorStr{ SDL_GetError() };
				// TODO maybe report error to user
			}
		}

		return false;
	}

	void TextBox::pasteFromClipboard()
	{
		int numChars = utf8::distance(mValue.begin(), mValue.end());

		auto insertPosIt = mValue.begin();

		if (mCursorPos < numChars)
			utf8::advance(insertPosIt, mCursorPos, mValue.end());
		else
			insertPosIt = mValue.end();

		std::string& pasteStr = GetClipboardText();
		mValue.insert(insertPosIt, pasteStr.begin(), pasteStr.end());
		mRedrawText = true;
	}

	bool TextBox::deleteSelection()
	{
		// Return if no selection
		if (mSelectionPos <= -1)
			return false;

		int begin = mCursorPos;
		int end = mSelectionPos;

		if (begin > end)
			std::swap(begin, end);

		if (begin != end)
			DeleteCharacters(begin, end);

		mCursorPos = begin;
		mSelectionPos = -1;

		return true;
	}

	void TextBox::DeleteCharacters(int begin, int end)
	{
		assert(begin < end);

		// As utf8 for some languages have more than one char
		// per character then use ut8 advance to find correct locations
		auto beginIt = mValue.begin();
		utf8::advance(beginIt, begin, mValue.end());

		auto endIt = mValue.begin();
		utf8::advance(endIt, end, mValue.end());

		mValue.erase(beginIt, endIt);

		mRedrawText = true;
	}

	int TextBox::cursorIndex2Position(size_t index)
	{
		// Calculates the absolute screen position of the glyph at the index
		int offset = mTextOffset.x + mTextClipRect.x + mPos.x;

		if (mGlyphs.empty())
			return offset;

		if (index >= mGlyphs.size())
			return mGlyphs[mGlyphs.size() - 1].x + mGlyphs[mGlyphs.size() - 1].width + offset;
		else
			return mGlyphs[index].x + offset;
	}

	int TextBox::position2CursorIndex(int posx)
	{
		// Convert from absolute coords to relative to text clipping rect
		posx -= (mTextOffset.x + mTextClipRect.x + mPos.x);

		int mCursorId = 0;

		for (auto glyph : mGlyphs)
		{
			if (posx < glyph.cx)
				return mCursorId;

			mCursorId++;
		}

		return mCursorId;
	}

	TextBox::SpinArea TextBox::spinArea(const Point& pos)
	{
		if (0 <= pos.x - mPos.x && pos.x - mPos.x < mSpinnerSize)
		{ /* on scrolling arrows */
			if (mSize.y >= pos.y - mPos.y && pos.y - mPos.y <= mSize.y / 2) /* top part */
				return SpinArea::Top;
			else if (0 <= pos.y - mPos.y && pos.y - mPos.y > mSize.y / 2)  /* bottom part */
				return SpinArea::Bottom;
		}
		return SpinArea::None;
	}

	void TextBox::RenderSpinArrows(Renderer& renderer, Point& offset)
	{
		int xSpacing = static_cast<int>(mSize.y * 0.3f);

		Point upPos{ xSpacing, -mImageSpinUp.GetHeight() / 5 };
		mImageSpinUp.SetAlpha((mSpinArea == SpinArea::Top) ? 160 : 80 );
		mImageSpinUp.Render(renderer, offset + upPos);

		Point dnPos{ xSpacing, mSize.y - 4 * mImageSpinDn.GetHeight() / 5 };
		
		mImageSpinDn.SetAlpha((mSpinArea == SpinArea::Bottom)? 160 : 80 );
		mImageSpinDn.Render(renderer, offset + dnPos);
	}

}

