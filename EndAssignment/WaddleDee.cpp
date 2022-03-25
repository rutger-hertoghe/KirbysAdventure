#include "pch.h"
#include "WaddleDee.h"
#include "Sprite.h"

WaddleDee::WaddleDee(const Point2f& location)
	: Enemy{location}
{
	m_IsInhalable = true;

	SetBaseVelocity(0.f, 0.f);
	m_pSprites.push_back(new Sprite{ 2, 0.3f, "resources/sprites/waddledee.png" });
	m_pCurrentSprite = m_pSprites[0];
	m_Shape.width = m_pCurrentSprite->GetFrameDimensions().x;
	m_Shape.height = m_pCurrentSprite->GetFrameDimensions().y;

	m_XDirection = m_Velocity.x > 0.f ? -1.F : 1.f;
}

WaddleDee::~WaddleDee()
{
	for (Sprite*& pSprite : m_pSprites)
	{
		delete pSprite;
		pSprite = nullptr;
	}
	m_pCurrentSprite = nullptr;
}
