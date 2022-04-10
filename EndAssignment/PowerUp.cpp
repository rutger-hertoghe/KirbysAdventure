#include "pch.h"
#include "PowerUp.h"
#include "Vector2f.h"

PowerUp::PowerUp(PowerUpType type, ProjectileManager* pMgr, bool hasStart, bool hasContinuous, bool hasEnd, bool hasCompleteSpriteSet)
	: m_Type{ type }
	, m_pProjectileManager{pMgr}
	, m_HasStart{ hasStart }
	, m_HasContinuous{ hasContinuous }
	, m_HasEnd{ hasEnd }
	, m_HasCompleteSpriteSet{hasCompleteSpriteSet}
	, m_IsActive{ false }
	, m_Owner{ Projectile::ActorType::enemy}
{
}

bool PowerUp::IsOneShot()
{
	return false;
}

int PowerUp::GetPowerPanelSlot() const
{
	return int(m_Type) + 1;
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

void PowerUp::SetKirbyAsOwner()
{
	m_Owner = Projectile::ActorType::kirby;
}
