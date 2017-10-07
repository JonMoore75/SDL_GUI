#ifndef SoundEffect_h__
#define SoundEffect_h__
#pragma once

#include <SDL_mixer.h>
#include <string>

class  SoundEffect
{
public:
	SoundEffect();
	~SoundEffect() noexcept;

	bool CreateFromFile(std::string filename);

	void Play(int channel=-1, int loops=0) const;

	void Release();
private:
	Mix_Chunk*  m_Sound = nullptr;
};

#endif //  SoundEffect_h__
