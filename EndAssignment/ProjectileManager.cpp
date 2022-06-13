#include "pch.h"
#include <iostream>
#include "ProjectileManager.h"
#include "utils.h"
#include "Actor.h"
#include "Kirby.h"

ProjectileManager::ProjectileManager()
{
	InitializeSprites();
}

ProjectileManager::~ProjectileManager()
{
	for (Projectile* pProjectile : m_pProjectiles)
	{
		delete pProjectile;
		pProjectile = nullptr;
	}

	for (Sprite* pSprite : m_pSprites)
	{
		delete pSprite;
		pSprite = nullptr;
	}
}

void ProjectileManager::Draw() const
{
	for (Projectile* pProjectile : m_pProjectiles)
	{
		if (pProjectile) pProjectile->Draw();
	}
}

void ProjectileManager::Update(float elapsedSec)
{
	int idx{};
	if (int(m_pProjectiles.size()) > 0)
	{
		while (idx < int(m_pProjectiles.size()))
		{
			UpdateProjectile(elapsedSec, idx);
			++idx;
		}
	}
}

void ProjectileManager::Add(Projectile* projectile)
{
	m_pProjectiles.push_back(projectile);
	int lastIdx{ int(m_pProjectiles.size() - 1) };
	int typeInt{ m_pProjectiles[lastIdx]->GetTypeInt() };
	m_pProjectiles[lastIdx]->SetSprite(m_pSprites[typeInt]);
}

bool ProjectileManager::ProjectileHasHit(Actor* pActor, float& direction)
{
	for (Projectile* pProjectile : m_pProjectiles)
	{
		Actor* pOwner{ pProjectile->GetOwner() };
		bool isAnyActorKirby{ (typeid(*pOwner) == typeid(Kirby) || typeid(*pActor) == typeid(Kirby)) };

		if (isAnyActorKirby && pOwner != pActor && HasCollided(pActor, pProjectile))
		{
			if (pProjectile->IsPersistent() == false)
			{
				pProjectile->SetReadyToDestroy();
			}
			direction = pProjectile->GetDirection();
			return true;
		}
	}
	return false;
}

bool ProjectileManager::ProjectileHasHit(Actor* pActor)
{
	for (Projectile* pProjectile : m_pProjectiles)
	{
		Actor* pOwner{ pProjectile->GetOwner() };
		bool isAnyActorKirby = (typeid(*pOwner) == typeid(Kirby) || typeid(*pActor) == typeid(Kirby));

		if (isAnyActorKirby && pOwner != pActor && HasCollided(pActor, pProjectile))
		{
			if (pProjectile->IsPersistent() == false)
			{
				pProjectile->SetReadyToDestroy();
			}
			return true;
		}
	}
	return false;
}

void ProjectileManager::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 4, 0.2f, "fire" });
	m_pSprites.push_back(new Sprite{ 4, 0.2f, "star" });
	m_pSprites.push_back(new Sprite{ 1, 0.f, "puff" });
	m_pSprites.push_back(new Sprite{ 3, 0.2f, "spark" });
	m_pSprites.push_back(new Sprite{ 1, 0.f, "throwingstar" });
}

bool ProjectileManager::HasCollided(const Actor* pActor, const Projectile* pProjectile)
{
	utils::HitInfo hitInfo{};

	std::vector<Point2f> actorVertices{};

	const Rectf& actorRect{ pActor->GetShape()};
	actorVertices.push_back(Point2f{ actorRect.left, actorRect.bottom });
	actorVertices.push_back(Point2f{ actorRect.left + actorRect.width, actorRect.bottom });
	actorVertices.push_back(Point2f{ actorRect.left + actorRect.width, actorRect.bottom + actorRect.height });
	actorVertices.push_back(Point2f{ actorRect.left, actorRect.bottom + actorRect.height });

	const Rectf& projectileRect{ pProjectile->GetShape() };
	Point2f projectileLeft{ projectileRect.left, projectileRect.bottom + projectileRect.height / 2 };
	Point2f projectileRight{ projectileRect.left + projectileRect.width, projectileRect.bottom + projectileRect.height / 2 };

	if (utils::Raycast(actorVertices, projectileLeft, projectileRight, hitInfo))
	{
		return true;
	}
	return false;
}

void ProjectileManager::UpdateProjectile(float elapsedSec, int index)
{
	// Don't do anything if there's no projectile at index
	if (!m_pProjectiles[index]) return; 

	m_pProjectiles[index]->Update(elapsedSec);

	if (!m_pProjectiles[index]->IsOnScreen())
	{
		m_pProjectiles[index]->SetReadyToDestroy();
	}

	if (m_pProjectiles[index]->IsReadyToDestroy())
	{
		DeleteProjectile(index);
	}
}

void ProjectileManager::DeleteProjectile(int index)
{
	delete m_pProjectiles[index];
	m_pProjectiles[index] = nullptr;
	std::swap(m_pProjectiles[index], m_pProjectiles[m_pProjectiles.size() - 1]);
	m_pProjectiles.pop_back();
}
