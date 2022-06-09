#include "pch.h"
#include "PowerUp.h"
#include "Vector2f.h"

PowerUp::PowerUp(PowerUpType type, Actor* pActor, bool hasStart, bool hasContinuous, bool hasEnd, bool hasCompleteSpriteSet)
	: m_Type{ type }
	, m_HasStart{ hasStart }
	, m_HasContinuous{ hasContinuous }
	, m_HasEnd{ hasEnd }
	, m_HasCompleteSpriteSet{hasCompleteSpriteSet}
	, m_pOwner{ pActor}
	, m_pProjectileManager{pActor->GetProjectileManager()}
	, m_IsActive{ false }
{
}

int PowerUp::GetPowerPanelSlot() const
{
	return int(m_Type) + 1;
}

PowerUp::PowerUpType PowerUp::GetType() const
{
	return m_Type;
}

bool PowerUp::HasStart() const
{
	return m_HasStart;
}

bool PowerUp::HasContinuous() const
{
	return m_HasContinuous;
}

bool PowerUp::HasEnd() const
{
	return m_HasEnd;
}

bool PowerUp::HasCompleteSpriteSet() const
{
	return m_HasCompleteSpriteSet;
}

void PowerUp::SetOwner(Actor* pOwner)
{
	m_pOwner = pOwner;
}

void PowerUp::SetInactive()
{
	m_IsActive = false;
}
