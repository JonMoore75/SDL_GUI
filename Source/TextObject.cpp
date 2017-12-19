#include "TextObject.h"

#include "utf8/utf8.h" // http://utfcpp.sourceforge.net/

// My code to wrap a string, not strictly needed in this project
void StringBreaker(const std::string& str, int maxwidth, std::vector<std::string>& rows, std::function<int(const std::string&)> widthFunc)
{
	auto beginIt = str.begin();
	auto endIt = str.end();

	// Strip leading white-space 
	while (beginIt != endIt && utf8::peek_next(beginIt, endIt) == 0x0020)
		utf8::next(beginIt, endIt);

	int stringlen = utf8::distance(beginIt, endIt);

	if (stringlen < 1)
		return;

	if ( widthFunc(std::string{ beginIt, endIt }) <= maxwidth )
	{
		rows.push_back(std::string{ beginIt, endIt });
		return;
	}

	// Move iterator to maxwidth position
	auto Iter = beginIt;
	while (Iter != endIt && widthFunc(std::string{ beginIt, Iter }) < maxwidth)
		utf8::next(Iter, endIt);
	utf8::prior(Iter, beginIt);
	auto maxWidthIter = Iter;

	do
	{
		// If this or previous character is whitespace break the string
		if (utf8::peek_next(Iter, endIt) == 0x0020 || utf8::prior(Iter, beginIt) == 0x0020) // Space
		{
			rows.push_back(std::string{ beginIt, Iter });
			StringBreaker(std::string{ Iter, endIt }, maxwidth, rows, widthFunc);
			return;
		}

		stringlen = utf8::distance(beginIt, Iter);
	} while (stringlen > 0);

	// If reach here then no space in current row so break at maxwidth
	rows.push_back(std::string{ beginIt, maxWidthIter });
	StringBreaker(std::string{ maxWidthIter, endIt }, maxwidth, rows, widthFunc);
}

void TextObject::Create(Renderer& renderer, const std::string& font, int fontSize, Color color, int maxWidth/* = 0*/)
{
	FontTTF fontTTF;
	fontTTF.LoadFont(font.c_str(), fontSize, color);
	mTextTexture.CreateFromText(renderer, mText, fontTTF, maxWidth);
	fontTTF.Release();

	TextAlign(CLIPLEFT);
	mTextTexture.SetAlphaBlend();
}

void TextObject::TextAlign(CLIPALIGN align, const Point& trgtSize /*= Point{ 0, 0 }*/)
{
	Point textSize{ mTextTexture.GetWidth(), mTextTexture.GetHeight() };

	mOffset.x = trgtSize.x / 2 - textSize.x / 2;
	mOffset.y = (trgtSize.y == 0) ? 0 : trgtSize.y / 2 - textSize.y / 2;

	if (align == CLIPLEFT)
		mOffset.x = 0;

	if (align == CLIPRIGHT)
		mOffset.x = trgtSize.x - textSize.x;

	Rect clipRect{ 0, 0, textSize.x, textSize.y };

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