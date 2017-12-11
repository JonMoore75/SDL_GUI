#pragma once

#include <SDL_ttf.h>

static SDL_bool CharacterIsDelimiter(char c, const char *delimiters);

int TTF_SizeUTF8_Wrapped(TTF_Font *font,
	const char *text, int *w, int *h, Uint32 wrapLength);

