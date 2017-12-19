#ifndef TextObject_h__
#define TextObject_h__
#pragma once

#include <vector>
#include <functional>

#include "Texture.h"
#include "Font_TTF.h"

#include "Common.h"


void StringBreaker(const std::string& str, int maxwidth, std::vector<std::string>& rows, std::function<int(const std::string&)> widthFunc);

class TextObject
{
public:
	enum CLIPALIGN { CLIPLEFT, CLIPCENTRE, CLIPRIGHT };

	TextObject(const std::string& text = "") : mText{ text } {}
	~TextObject() { Release(); }

	void Release() { mTextTexture.Release(); }

	void SetText(const std::string& text)	{ mText = text; }
	const std::string& GetText() const		{ return mText; }

	void SetAlpha(Uint8 alpha)	{ mTextTexture.SetAlpha(alpha); }

	Point getSize() const { return Point{ mTextTexture.GetWidth(), mTextTexture.GetHeight() }; }
	int getWidth() const		{ return mTextTexture.GetWidth(); }
	int getHeight() const		{ return mTextTexture.GetHeight(); }

	bool NeedsCreation() const	{ return !mText.empty() && mTextTexture.isNull(); }

	void Create(Renderer& renderer, const std::string& font, int fontSize, Color color, int maxWidth = 0);

	void TextAlign(CLIPALIGN align, const Point& trgtSize = Point{ 0, 0 });

	// Pass in the target position ie point to centre around if centre aligned
	// If defined by target rect then pass in the top left point of target rect
	void Render(Renderer& renderer, Point pt = { 0,0 })
	{
		mTextTexture.Render(renderer, pt.x + mOffset.x, pt.y + mOffset.y);
	}

private:
	Texture mTextTexture;
	Point mOffset;
	std::string mText;
};

#endif // TextObject_h__