#include "pch.h"
#include "RemovalFX.h"
#include "Sprite.h"
#include "TextureManager.h"

RemovalFX::RemovalFX(const Point2f& location, RemovalFX::FXType type)
	: GameObject{}
	, m_IsReadyToDestroy{false}
	, m_Type{type}
{
	m_Shape.left = location.x;
	m_Shape.bottom = location.y;
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
}

RemovalFX::~RemovalFX()
{
	for (Sprite*& pSprite : m_pSprites)
	{
		if (pSprite)
		{
			delete pSprite;
			pSprite = nullptr;
		}
	}
	m_pSprites.clear();
}

void RemovalFX::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
	
	if (m_HasLooped)
	{
		m_IsReadyToDestroy = true;
	}
}

bool RemovalFX::IsReadyToDestroy() const
{
	return m_IsReadyToDestroy;
}

void RemovalFX::InitializeSprites()
{
	switch (m_Type)
	{
	case FXType::item:
		m_pSprites.push_back(new Sprite{ 3, 0.15f, "explosion", TextureManager::Get("explosion")});
		break;
	case FXType::enemy:
		m_pSprites.push_back(new Sprite{ 4, 0.3f, "removalfx_star" });
		break;
	}
}
