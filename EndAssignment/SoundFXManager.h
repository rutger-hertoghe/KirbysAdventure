#pragma once
#include <unordered_map>

class SoundEffect;

class SoundFXManager
{
public:
	explicit SoundFXManager();
	SoundFXManager(const SoundFXManager& other) = delete;
	SoundFXManager& operator=(const SoundFXManager& other) = delete;
	SoundFXManager(SoundFXManager&& other) = delete;
	SoundFXManager& operator=(SoundFXManager&& other) = delete;
	~SoundFXManager();

	static void Play(const std::string& name);
	static void StopAll();
	static void SetVolume(int volume);

private:
	static SoundFXManager* m_pSoundFXManager;

	int m_GeneralVolume;

	std::unordered_map<std::string, SoundEffect*> m_pSoundEffectMap;

	void LoadSoundFX();
	void AddSoundFX(const std::string & name, float volumeScalar = 1.f);
};

