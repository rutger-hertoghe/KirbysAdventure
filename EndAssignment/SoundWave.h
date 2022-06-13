#pragma once
#include "GameObject.h"

class SoundWave final : public GameObject
{
public:
	enum class Orientation
	{
		topLeft,
		topRight,
		bottomLeft,
		bottomRight
	};

	explicit SoundWave(Orientation orientation);
	SoundWave& operator=(const SoundWave& other) = delete;
	SoundWave(const SoundWave& other) = delete;
	SoundWave& operator=(SoundWave&& other) = delete;
	SoundWave(SoundWave&& other) = delete;
	~SoundWave();

	void virtual Update(float elapsedSec) override;

	void SetTargetLocation(const Point2f& targetLocation);
	void SetActive(bool active);

	Orientation GetOrientation() const;
	bool IsActive() const;

private:
	bool m_IsActive;
	const Orientation m_Orientation;
	Point2f m_TargetLocation;
	
};

