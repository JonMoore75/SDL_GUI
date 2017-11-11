#pragma once

#include "Texture.h"
#include "Font_TTF.h"

class TextObject
{
public:
	enum CLIPALIGN { CLIPLEFT, CLIPCENTRE, CLIPRIGHT };

	TextObject(const std::string& text = "") : mText{ text } {}
	~TextObject() { Release(); }

	void Release()
	{
		mTextTexture.Release();
	}

	void SetText(const std::string& text) { mText = text; }
	const std::string& GetText() const { return mText; }

	bool NeedsCreation() const
	{ 
		return !mText.empty() && mTextTexture.isNull();
	}

	void Create(Renderer& renderer, const std::string& font, int fontSize, SGUI::Color color)
	{
		FontTTF fontTTF;
		fontTTF.LoadFont(font.c_str(), fontSize, color);
		mTextTexture.CreateFromText(renderer, mText, fontTTF);
		fontTTF.Release();

		TextAlign(CLIPLEFT);
	}

	void TextAlign(CLIPALIGN align, const SGUI::Point& trgtSize = SGUI::Point{0,0})
	{
		SGUI::Point textSize{ mTextTexture.GetWidth(), mTextTexture.GetHeight() };

		mOffset.x = trgtSize.x / 2 - textSize.x / 2;
		mOffset.y = (trgtSize.y == 0) ? 0 : trgtSize.y / 2 - textSize.y / 2;

		if (align == CLIPLEFT)
			mOffset.x = 0;

		if (align == CLIPRIGHT)
			mOffset.x = trgtSize.x - textSize.x ;

		SDL_Rect clipRect{ 0, 0, textSize.x, textSize.y };

		// If text bigger in x than target rect
		if (trgtSize.x > 0 && mOffset.x < 0)
		{
			clipRect.w = trgtSize.x;

			if (align == CLIPCENTRE)
				clipRect.x = -mOffset.x;
			if (align == CLIPRIGHT)
				clipRect.x = textSize.x - trgtSize.x;

			mOffset.x = 0;
		}

		// If text bigger in y than target rect
		if (trgtSize.y > 0 && mOffset.y < 0)
		{
			clipRect.y = -mOffset.y;
			clipRect.h = trgtSize.y;
			mOffset.y = 0;
		}

		mTextTexture.SetClipRect(clipRect);
	}

	// Pass in the target position ie point to centre around if centre aligned
	// If defined by target rect then pass in the top left point of target rect
	void Render(Renderer& renderer, SGUI::Point pt = { 0,0 })
	{
		mTextTexture.Render(renderer, pt.x + mOffset.x, pt.y + mOffset.y);
	}

private:
	Texture		mTextTexture;
	SGUI::Point mOffset;
	std::string mText;
};

