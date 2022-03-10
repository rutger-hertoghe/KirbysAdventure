#include "pch.h"
#include "PowerUp.h"
#include "FirePower.h"
#include "GameObject.h"

GameObject::GameObject()
	: m_IsInhalable{false}
	, m_pPowerUp{nullptr}
	, m_XDirection{1.f}
{
}

GameObject::~GameObject()
{
	if (m_pPowerUp)
	{
		delete m_pPowerUp;
		m_pPowerUp = nullptr;
	}
}

void GameObject::TransferPowerUp(GameObject* newOwner)
{
	newOwner->m_pPowerUp = m_pPowerUp;
	m_pPowerUp = nullptr;
}

bool GameObject::HasPower()
{
	if (m_pPowerUp) return true;
	return false;
}

void GameObject::SetPowerUp(PowerUp* powerUp)
{
	m_pPowerUp = powerUp;
}
