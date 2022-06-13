#pragma once
#include <string>

class SoundEffect final
{
public:
	explicit SoundEffect( const std::string& path, float volumeScalar = 1.f);
	~SoundEffect( );
	SoundEffect(const SoundEffect& other) = delete;
	SoundEffect& operator=(const SoundEffect& rhs) = delete;
	SoundEffect( SoundEffect&& other) = delete;
	SoundEffect& operator=( SoundEffect&& rhs) = delete;

	bool IsLoaded( ) const;
	bool Play( int loops ) const;
	void SetVolume( int value ); 
	int GetVolume( ) const; 
	static void StopAll( );
	static void PauseAll( );
	static void ResumeAll( );

private:
	Mix_Chunk* m_pMixChunk;

	float m_VolumeScalar; // Added this to scale volume relatively to other soundeffects
};
