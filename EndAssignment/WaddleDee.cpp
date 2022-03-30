#include "pch.h"
#include "WaddleDee.h"
#include "Sprite.h"
#include "utils.h"

WaddleDee::WaddleDee(const Point2f& location)
	: Enemy{location}
{
	m_IsInhalable = true;

	SetBaseVelocity(50.f, 0.f);
	m_pSprites.push_back(new Sprite{ 2, 0.3f, "resources/sprites/waddledee.png" });
	m_pCurrentSprite = m_pSprites[0];
	m_Shape.width = m_pCurrentSprite->GetFrameDimensions().x;
	m_Shape.height = m_pCurrentSprite->GetFrameDimensions().y;

	m_XDirection = 1.f;
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

void WaddleDee::Update(float elapsedSec)
{
	utils::HitInfo hitInfo{};
	Point2f rayStart, rayEnd;
	if (m_XDirection < 0.f)
	{
		rayStart.x = m_Shape.left;
		rayEnd.x = m_Shape.left - m_Shape.width;
	}
	else
	{
		rayStart.x = m_Shape.left + m_Shape.width;
		rayEnd.x = m_Shape.left + 2 * m_Shape.width;
	}
	rayStart.y = m_Shape.bottom;
	rayEnd.y = m_Shape.bottom - m_Shape.height;



	Enemy::Update(elapsedSec);
}
