#include "pch.h"
#include "MrTickTock.h"
#include "Sprite.h"

MrTickTock::MrTickTock(const Point2f& location)
	: Actor{}
	, m_CameraLockLocation{location}
	, m_Sprite{0}
{
	InitializeSprites();
	SetInitialSprite();
	SetDimsFromSprite();
	SetLocation(location);
}

MrTickTock::~MrTickTock()
{
	for (Sprite*& pSprite : m_pSprites)
	{
		delete pSprite;
		pSprite = nullptr;
	}
	m_pCurrentSprite = nullptr;
}

void MrTickTock::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
	m_ArbitraryTimer += elapsedSec;
	if (m_ArbitraryTimer > 2.f)
	{
		m_ArbitraryTimer = 0.f;
		++m_Sprite %= int(m_pSprites.size());
		m_pCurrentSprite = m_pSprites[m_Sprite];
	}
	
}

void MrTickTock::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 1, 0.f, "mrticktock_idle" });
	m_pSprites.push_back(new Sprite{ 2, 0.4f, "mrticktock_walking" });
	m_pSprites.push_back(new Sprite{ 4, 0.1f, "mrticktock_ringing" });
}