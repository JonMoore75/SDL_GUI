#include "SDL_TTF_utf8wrapsize.h"

#include <SDL2\SDL_ttf.h>

SDL_bool CharacterIsDelimiter(char c, const char *delimiters)
{
	while (*delimiters) {
		if (c == *delimiters) {
			return SDL_TRUE;
		}
		++delimiters;
	}
	return SDL_FALSE;
}

int TTF_SizeUTF8_Wrapped(TTF_Font *font, const char *text, int *w, int *h, Uint32 wrapLength)
{
	int status = 0;
	int width, height;
	const int lineSpace = 2;
	int numLines;
	char *str, **strLines;

	/* Get the dimensions of the text surface */
	if ((TTF_SizeUTF8(font, text, &width, &height) < 0) || !width) {
		TTF_SetError("Text has zero width");
		return -1;
	}

	numLines = 1;
	str = NULL;
	strLines = NULL;
	if (wrapLength > 0 && *text) {
		const char *wrapDelims = " \t\r\n";
		int w, h;
		int line = 0;
		char *spot, *tok, *next_tok, *end;
		char delim;
		size_t str_len = SDL_strlen(text);

		numLines = 0;

		str = SDL_stack_alloc(char, str_len + 1);
		if (str == NULL) {
			TTF_SetError("Out of memory");
			return -1;
		}

		SDL_strlcpy(str, text, str_len + 1);
		tok = str;
		end = str + str_len;
		do {
			strLines = (char **)SDL_realloc(strLines, (numLines + 1) * sizeof(*strLines));
			if (!strLines) {
				TTF_SetError("Out of memory");
				return -1;
			}
			strLines[numLines++] = tok;

			/* Look for the end of the line */
			if ((spot = SDL_strchr(tok, '\r')) != NULL ||
				(spot = SDL_strchr(tok, '\n')) != NULL) {
				if (*spot == '\r') {
					++spot;
				}
				if (*spot == '\n') {
					++spot;
				}
			}
			else {
				spot = end;
			}
			next_tok = spot;

			/* Get the longest string that will fit in the desired space */
			for (; ; ) {
				/* Strip trailing whitespace */
				while (spot > tok &&
					CharacterIsDelimiter(spot[-1], wrapDelims)) {
					--spot;
				}
				if (spot == tok) {
					if (CharacterIsDelimiter(*spot, wrapDelims)) {
						*spot = '\0';
					}
					break;
				}
				delim = *spot;
				*spot = '\0';

				TTF_SizeUTF8(font, tok, &w, &h);
				if ((Uint32)w <= wrapLength) {
					break;
				}
				else {
					/* Back up and try again... */
					*spot = delim;
				}

				while (spot > tok &&
					!CharacterIsDelimiter(spot[-1], wrapDelims)) {
					--spot;
				}
				if (spot > tok) {
					next_tok = spot;
				}
			}
			tok = next_tok;
		} while (tok < end);
	}

	*w = (numLines > 1) ? wrapLength : width;
	*h = height * numLines + (lineSpace * (numLines - 1));

	return status;
}
