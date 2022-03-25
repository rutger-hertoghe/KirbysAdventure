#include "pch.h"
#include "HudElement.h"

HudElement::HudElement(Sprite* pSprite)
{
	m_pCurrentSprite = pSprite;
}

HudElement::~HudElement()
{
	delete m_pCurrentSprite;
	m_pCurrentSprite = nullptr;
}

void HudElement::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
}

void HudElement::SetPosition(const Point2f& bottomLeft)
{
	m_Shape.left = bottomLeft.x;
	m_Shape.bottom = bottomLeft.y;
}

float HudElement::GetWidth()
{
	return m_pCurrentSprite->GetFrameDimensions().x;
}

float HudElement::GetHeight()
{
	return m_pCurrentSprite->GetFrameDimensions().y;
}
