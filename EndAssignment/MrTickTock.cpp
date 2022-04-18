#include "pch.h"
#include "MrTickTock.h"
#include "Sprite.h"

MrTickTock::MrTickTock(const Point2f& location)
	: Actor{}
	, m_CameraLockLocation{location}
	, m_Sprite{0}
	, m_MaxLives{ 3 }
	, m_InvulnerabilityTimer{0.f}
	, m_GotDamaged{false}
	, m_IsDead{false}
{
	m_Lives = m_MaxLives;
	m_IsInhalable = false;
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
	// TODO: Finish Mr. TickTock
	UpdateSprite(elapsedSec);

	ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);

	if (m_IsDead)
	{
		return;
	}

	m_ArbitraryTimer += elapsedSec;
	ChangeSpriteAfterTwoSeconds();

	UpdateInvulnerability(elapsedSec);
	
	ApplyGravity(elapsedSec);
	HandleLevelCollisions();
}

void MrTickTock::DecrementHealth()
{
	if (m_InvulnerabilityTimer > 0.f)
	{
		return;
	}
	--m_Lives;
	m_GotDamaged = true;
	std::cout << "Lives of TickTock: " << m_Lives << '\n';
	if (m_Lives <= 0)
	{
		Kill();
	}
}

bool MrTickTock::IsDead() const
{
	return m_IsDead;
}

void MrTickTock::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 1, 0.f, "mrticktock_idle" });
	m_pSprites.push_back(new Sprite{ 2, 0.4f, "mrticktock_walking" });
	m_pSprites.push_back(new Sprite{ 4, 0.1f, "mrticktock_ringing" });
	m_pSprites.push_back(new Sprite{ 2, 0.1f, "mrticktock_dead" });
	CreateAltSprites();
}

void MrTickTock::Kill()
{
	m_pCurrentSprite = GetSpritePtr("mrticktock_dead");
	m_IsDead = true;
	m_IsInhalable = true;
}

void MrTickTock::UpdateInvulnerability(float elapsedSec)
{
	if (m_InvulnerabilityTimer > 2.f)
	{
		m_InvulnerabilityTimer = 0.f;
	}
	else if (m_InvulnerabilityTimer > 0.f)
	{
		m_InvulnerabilityTimer += elapsedSec;
		std::string spriteName = m_pCurrentSprite->GetName();
		Flicker(m_InvulnerabilityTimer, spriteName);
	}
	else if (m_GotDamaged)
	{
		m_GotDamaged = false;
		m_InvulnerabilityTimer += elapsedSec;
	}
}

void MrTickTock::ChangeSpriteAfterTwoSeconds()
{
	if (m_ArbitraryTimer > 2.f)
	{
		m_ArbitraryTimer = 0.f;
		++m_Sprite %= int(3);
		m_pCurrentSprite = m_pSprites[m_Sprite];
	}
}
