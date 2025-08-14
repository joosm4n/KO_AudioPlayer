
#include "audioplayer.h"

AudioPlayer::AudioPlayer()
{
	m_SoundSwitched = false;
	SoundOn = true;
	m_AudioDeviceID = 0;
	m_ChannelCount = 0;
	m_SampleRate = 48000;
	m_Volume = 5;

	m_Music = nullptr;

	SDL_zero(m_AudioSpec);
	m_AudioSpec.format = SDL_AUDIO_F32;
	m_AudioSpec.channels = m_ChannelCount;
	m_AudioSpec.freq = m_SampleRate;

	m_AudioDeviceID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &m_AudioSpec);
	if (m_AudioDeviceID == 0)
	{
		SDL_Log("Unable to open audio! SDL error: %s\n", SDL_GetError());
		return;
	}
	else
	{
		//Initialize SDL_mixer
		if (Mix_OpenAudio(m_AudioDeviceID, &m_AudioSpec) == false)
		{
			SDL_Log("SDL_mixer could not initialize! SDL_mixer error: %s\n", SDL_GetError());
			return;
		}
	}

	Mix_AllocateChannels(m_ChannelCount);
	int version = Mix_Version();
	std::cout << " SDL_Mixer Loaded. Version : " << version << std::endl;
}

AudioPlayer::~AudioPlayer()
{
	m_Music = nullptr;

	SDL_CloseAudioDevice(m_AudioDeviceID);
	m_AudioDeviceID = 0;

	Mix_Quit();
}

void AudioPlayer::loadMusic(const char* file_path)
{
	if (m_Music != nullptr)
		Mix_FreeMusic(m_Music);

	// Load audio
	if (m_Music = Mix_LoadMUS(file_path); m_Music == nullptr)
	{
		SDL_Log("Unable to load music! SDL_mixer error: %s\n", SDL_GetError());
		return;
	}
}

int AudioPlayer::loadChunk(const char* file_path)
{
	m_Chunks.push_back(Mix_LoadWAV(file_path));
	if (m_Chunks[m_ChannelCount] == nullptr)
	{
		SDL_Log("Unable to load music! SDL_mixer error: %s\n", SDL_GetError());
		return-1;
	}
	m_ChunkNames.push_back("");
	int index = m_ChannelCount;

	m_ChannelCount++;
	Mix_AllocateChannels(m_ChannelCount);
	int channel_count_check;
	Mix_QuerySpec(nullptr, nullptr, &channel_count_check);
	if (m_ChannelCount !=  channel_count_check)
	{
		m_ChannelCount -= 1;
		SDL_Log("Unable to allocate channels! SDL_mixer error: %s\n", SDL_GetError());
		return -1;
	}
	return index;
}
int AudioPlayer::loadChunk(const char* file_path, const std::string& give_name)
 {
	m_Chunks.push_back(Mix_LoadWAV(file_path));
	if (m_Chunks[m_ChannelCount] == nullptr)
	{
		SDL_Log("Unable to load chunk! SDL_mixer error: %s\n", SDL_GetError());
		return -1;
	}
	m_ChunkNames.push_back(give_name);

	int index = m_ChannelCount;

	m_ChannelCount++;
	Mix_AllocateChannels(m_ChannelCount);
	int channel_count_check;
	Mix_QuerySpec(nullptr, nullptr, &channel_count_check);
	return index;
}

void AudioPlayer::playChunk(const int index)
{
	Mix_PlayChannel(index, m_Chunks[index], 0);
}
void AudioPlayer::playChunk(const std::string& name)
{
	for (int i = 0; i < m_Chunks.size(); i++)
	{
		if (m_ChunkNames[i] == name)
		{
			Mix_PlayChannel(i, m_Chunks[i], 0);
			return;
		}
	}
}

void AudioPlayer::play()
{
	if (!Mix_PlayingMusic())
	{ 
		Mix_PlayMusic(m_Music, -1);
	}
	else
	{
		if (Mix_PausedMusic())
			Mix_ResumeMusic();
	}
}

void AudioPlayer::pause()
{
	if (Mix_PlayingMusic())
		Mix_PauseMusic();
}

void AudioPlayer::stop()
{
	if (Mix_PlayingMusic())
		Mix_HaltMusic();
}

void AudioPlayer::mute()
{
	if (SoundOn != m_SoundSwitched) {
		Mix_MasterVolume(0);
		Mix_VolumeMusic(0);
	}
	Update();
}
void AudioPlayer::unmute()
{
	if (SoundOn != m_SoundSwitched) {
		int normalizedVolume = int(MIX_MAX_VOLUME * float(m_Volume) / 10.0f);
		Mix_MasterVolume(normalizedVolume);
		Mix_VolumeMusic(normalizedVolume);
	}
	Update();
}	

void AudioPlayer::volumeUp()
{
	if (m_Volume <= 10)
	{
		m_Volume++;
		Mix_MasterVolume(m_Volume);
		Mix_VolumeMusic(m_Volume);
	}
}

void AudioPlayer::volumeDown()
{
	if (m_Volume >= 0 && m_Volume <= 128)
	{
		m_Volume++;
		setVolume(m_Volume);
	}
}

void AudioPlayer::setVolume(int volume)
{
	if (volume > 10)
	{
		m_Volume = 10;
		Mix_MasterVolume(MIX_MAX_VOLUME);
		Mix_VolumeMusic(MIX_MAX_VOLUME);
	}
	else if (volume < 0)
	{
		m_Volume = 0;
		Mix_MasterVolume(0);
		Mix_VolumeMusic(0);
	}
	else
	{
		m_Volume = volume;
		int normalizedVolume = int(MIX_MAX_VOLUME * float(m_Volume)/10.0f);
		Mix_MasterVolume(normalizedVolume);
		Mix_VolumeMusic(normalizedVolume);
	}
}

void AudioPlayer::setChunkVolume(const int channel, const int volume)
{
	if (volume > 10)
	{
		Mix_VolumeChunk(m_Chunks[channel], 128);
	}
	else if (volume < 0)
	{
		Mix_VolumeChunk(m_Chunks[channel], 0);
	}
	else
	{
		int normalizedVolume = int(MIX_MAX_VOLUME * float(volume) / 10.0f);
		Mix_VolumeChunk(m_Chunks[channel], normalizedVolume);
	}
}

void AudioPlayer::Update()
{
	m_SoundSwitched = SoundOn;
}

