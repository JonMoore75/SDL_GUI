#include "SoundEffect.h"

#include <SDL.h>

SoundEffect::SoundEffect()
{

}

SoundEffect::~SoundEffect()
{
	Release();
}

void SoundEffect::Release()
{
	Mix_FreeChunk(m_Sound);
	m_Sound = nullptr;
}

bool SoundEffect::CreateFromFile(std::string filename)
{
	Release();

	m_Sound = Mix_LoadWAV(filename.c_str());

	if (m_Sound == nullptr)
	{
		// Failed to load from file
		std::string err_msg = "Could not load sound from file.\n";
		err_msg += filename + "\n" + Mix_GetError();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error!", err_msg.c_str(), nullptr);
	}

	return (m_Sound != nullptr);
}

void SoundEffect::Play(int channel/*=-1*/, int loops/*=0*/) const
{
	Mix_PlayChannel(channel, m_Sound, loops);
}

