#include "pch.h"
#include "StarBlock.h"
#include "Sprite.h"

StarBlock::StarBlock(const Point2f& location)
	: Item{true, false, true, true, true, true}
{
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	SetLocation(location);
}

void StarBlock::Update(float elapsedSec)
{
	ApplyVelocities(elapsedSec, m_Velocity.x, m_Velocity.y);
}

void StarBlock::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 1, 0.f, "starblock" });
}
