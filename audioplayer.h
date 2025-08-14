#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include <iostream>
#include <vector>
#include "../game/game_consts.h"

class AudioPlayer
{
private:
	SDL_AudioSpec m_AudioSpec;
	SDL_AudioDeviceID m_AudioDeviceID;

	Mix_Music* m_Music;
	std::vector<Mix_Chunk*> m_Chunks;
	std::vector<std::string> m_ChunkNames;

	int m_ChannelCount;
	int m_SampleRate;
	int m_Volume;

	bool m_SoundSwitched;
public:
	bool SoundOn;

public:
	AudioPlayer();
	~AudioPlayer();

	void loadMusic(const char* file_path);							// Loads a music file
	int loadChunk(const char* file_path);							// Loads a sound chunk with name "", returns its index.
	int loadChunk(const char* file_path, const std::string& name);	// Loads a sound chunk and adds a name to it, returns its index.

	void play();
	void pause();
	void stop();

	void mute();
	void unmute();

	void playChunk(const int index);
	void playChunk(const std::string& name);

	void volumeUp();
	void volumeDown();
	void setVolume(const int volume);								// Sets master volume, between 0 and 10.
	void setChunkVolume(const int channel, const int volume);		// Sets volume of the chunk.

	void Update();
};
