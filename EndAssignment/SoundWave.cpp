#include "pch.h"
#include "SoundWave.h"
#include "Sprite.h"

SoundWave::SoundWave(Orientation orientation)
	: m_Orientation{orientation}
	, m_IsActive{false}
{	
	const int nrFrames{ 2 };
	const float animationTime{ 0.1f };
	switch (orientation)
	{
	case Orientation::topLeft:
		m_pCurrentSprite = new Sprite{ nrFrames, animationTime, "Ringing_UpperLeft" };
		break;
	case Orientation::topRight:
		m_pCurrentSprite = new Sprite{ nrFrames, animationTime, "Ringing_UpperRight" };
		break;
	case Orientation::bottomLeft:
		m_pCurrentSprite = new Sprite{ nrFrames, animationTime, "Ringing_LowerLeft" };
		break;
	case Orientation::bottomRight:
		m_pCurrentSprite = new Sprite{ nrFrames, animationTime, "Ringing_LowerRight" };
		break;
	}
	SetDimsFromSprite();
}

SoundWave::~SoundWave()
{
	delete m_pCurrentSprite;
}

void SoundWave::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
	
		m_Shape.left = m_TargetLocation.x;
		m_Shape.bottom = m_TargetLocation.y;
}

void SoundWave::SetTargetLocation(const Point2f& targetLocation)
{
	m_TargetLocation = targetLocation;
}

void SoundWave::SetActive(bool active)
{
	m_IsActive = active;
}

SoundWave::Orientation SoundWave::GetOrientation() const
{
	return m_Orientation;
}

bool SoundWave::IsActive() const
{
	return m_IsActive;
}
