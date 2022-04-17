#include "pch.h"
#include "WaddleDee.h"
#include "Sprite.h"
#include "utils.h"

WaddleDee::WaddleDee(const Point2f& location)
	: Enemy{location}
{
	SetBaseVelocity(50.f, 0.f);

	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	m_XDirection = 1.f;
}

void WaddleDee::Update(float elapsedSec)
{
	const float jumpStrength{ 200.f };
	Jump(jumpStrength);
	StandardUpdateSequence(elapsedSec);
}

void WaddleDee::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 2, 0.3f, "waddledee" });
}
