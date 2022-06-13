#include "pch.h"
#include "SoundFXManager.h"
#include "SoundEffect.h"

SoundFXManager* SoundFXManager::m_pSoundFXManager{ nullptr };

SoundFXManager::SoundFXManager()
	: m_GeneralVolume{128}
{
	if (m_pSoundFXManager)
	{
		delete m_pSoundFXManager;
	}
	m_pSoundFXManager = this;

	LoadSoundFX();
}

SoundFXManager::~SoundFXManager()
{
	for (std::pair <std::string, SoundEffect*> fXPair : m_pSoundEffectMap)
	{
		delete fXPair.second;
	}
}

void SoundFXManager::Play(const std::string& name)
{
	if (m_pSoundFXManager->m_pSoundEffectMap[name])
	{
		m_pSoundFXManager->m_pSoundEffectMap[name]->Play(0);
	}
	else // Notify programmer that sound doesn't exist
	{
		std::cout << "SoundEffect " << name << " not found\n";
	}
}

void SoundFXManager::StopAll()
{
	SoundEffect::StopAll();
}

void SoundFXManager::SetVolume(int volume)
{
	for (std::pair<std::string, SoundEffect*> soundFx : m_pSoundFXManager->m_pSoundEffectMap)
	{
		soundFx.second->SetVolume(volume);
	}
}

void SoundFXManager::LoadSoundFX()
{
	AddSoundFX("inflate");
	AddSoundFX("inhale");
	AddSoundFX("spitstar");
	AddSoundFX("puff");
	AddSoundFX("jump");
	AddSoundFX("land");
	AddSoundFX("fire", 0.2f);
	AddSoundFX("snare2");
	AddSoundFX("door");
	AddSoundFX("bloat");
	AddSoundFX("damage1");
	AddSoundFX("damage2");
	AddSoundFX("shock");
	AddSoundFX("ProjectileHitsWall");
	AddSoundFX("chirp3");
	AddSoundFX("shake");
	AddSoundFX("noteshot");
	AddSoundFX("notehit");
	AddSoundFX("kick", 0.3f);
}

void SoundFXManager::AddSoundFX(const std::string& name, float volumeScalar)
{
	const std::string path{ "resources/sounds/" };
	m_pSoundEffectMap.insert(std::pair<std::string, SoundEffect*>(name, new SoundEffect{ path + name + ".wav", volumeScalar }));
	m_pSoundEffectMap[name]->SetVolume(m_GeneralVolume); // Set volume to max, which internally gets scaled by the supplied volumeScalar
}
