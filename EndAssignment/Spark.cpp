#include "pch.h"
#include "Spark.h"

Spark::Spark(Actor* pOwner, const Rectf& sparkRect, const Vector2f& directionVector)
	: Projectile{pOwner, ProjectileType::spark, sparkRect, directionVector}
{
}

void Spark::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
	ApplyVelocities(elapsedSec);
	if (m_HasLooped)
	{
		SetReadyToDestroy();
	}
}
