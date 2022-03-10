#include "pch.h"
#include "PowerStar.h"
#include "utils.h"

PowerStar::PowerStar(Rectf shape)
{
	m_Shape = shape;
}

PowerStar::~PowerStar()
{
}

void PowerStar::Draw() const
{
	if (m_pPowerUp)
	{
		utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
		utils::FillRect(m_Shape);
	}
	else
	{
		utils::SetColor(Color4f{ 0.5f, 0.5f, 0.5f, 1.f });
		utils::FillRect(m_Shape);
	}
}
