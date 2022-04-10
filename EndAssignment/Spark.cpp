#include "pch.h"
#include "Spark.h"

Spark::Spark(ActorType owner, const Rectf& sparkRect, const Vector2f& directionVector)
	: Projectile{owner, ProjectileType::spark, sparkRect, directionVector}
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
