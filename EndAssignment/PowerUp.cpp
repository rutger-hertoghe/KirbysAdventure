#include "pch.h"
#include "PowerUp.h"

PowerUp::PowerUp(PowerUpType type, bool hasStart, bool hasContinuous, bool hasEnd, bool hasCompleteSpriteSet)
	: m_Type{ type }
	, m_HasStart{ hasStart }
	, m_HasContinuous{ hasContinuous }
	, m_HasEnd{ hasEnd }
	, m_HasCompleteSpriteSet{hasCompleteSpriteSet}
	, m_IsActive{ false }
{
}

PowerUp::~PowerUp()
{
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
