#include "pch.h"
#include "MrTickTock.h"
#include "Sprite.h"
#include "Camera.h"

MrTickTock::MrTickTock(const Point2f& location, Kirby* pKirby, LevelManager* pLevelManager, ProjectileManager* pProjectileManager)
	: Actor{pLevelManager, pProjectileManager}
	, m_pKirby{pKirby}
	, m_CameraLockLocation{location}
	, m_Sprite{0}
	, m_MaxLives{ 3 }
	, m_InvulnerabilityTimer{0.f}
	, m_GotDamaged{false}
	, m_ActionState{ActionState::idle}
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
	HandleLevelCollisions();

	if (m_ActionState  == ActionState::dead)
	{
		ApplyGravity(elapsedSec);
		return;
	}

	m_ArbitraryTimer += elapsedSec;

	switch (m_ActionState)
	{
	case ActionState::moveToDestination:
		DoMoveToKirby();
		break;
	case ActionState::idle:
		DoIdle(elapsedSec);
		break;
	case ActionState::hopping:
		DoHops();
		break;
	case ActionState::ringing:
		DoRinging();
		break;
	case ActionState::ringingWithNotes:
		break;
	case ActionState::shuffleStep:
		break;
	}

	UpdateInvulnerability(elapsedSec);
	
	ApplyGravity(elapsedSec);
	SetIsOnGround();
}

void MrTickTock::Draw() const
{
	GameObject::Draw();
	if (m_ActionState == ActionState::ringing)
	{
		utils::SetColor(Color4f(1.f, 0.f, 0.f, 0.1f));
		utils::FillRect(m_RingingZone);
	}
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
	return m_ActionState == ActionState::dead;
}

void MrTickTock::InitializeSprites()
{
	m_pSprites.push_back(new Sprite{ 1, 0.f, "mrticktock_idle" });
	m_pSprites.push_back(new Sprite{ 2, 0.4f, "mrticktock_walking" });
	m_pSprites.push_back(new Sprite{ 4, 0.1f, "mrticktock_ringing" });
	m_pSprites.push_back(new Sprite{ 2, 0.1f, "mrticktock_dead" });
	m_pSprites.push_back(new Sprite{ 1, 0.f, "mrticktock_jumping" });
	CreateAltSprites();
}

void MrTickTock::Kill()
{
	m_pCurrentSprite = GetSpritePtr("mrticktock_dead");
	m_ActionState = ActionState::dead;
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

void MrTickTock::DoMoveToKirby()
{
	float xDifference{ m_Shape.left - m_XDestination };
	float yDifference{ m_Shape.bottom - m_pKirby->GetLocation().y};
	m_XDirection = (xDifference < 0.f) ? 1.f : -1.f;

	if (abs(xDifference) > 32.f && abs(yDifference) > 16.f && m_IsOnGround)
	{
		Hop();
	}
	else if (abs(xDifference) > 8.f)
	{
		m_Velocity.x = 100.f;
	}
	else
	{
		SetState(ActionState::idle);
	}
}

void MrTickTock::DoHops()
{
	if (m_ArbitraryTimer > 3.f)
	{
		SetState(ActionState::idle);		
	}
	else if (m_IsOnGround)
	{
		Hop();
		Camera::SetShake();
	}
}

void MrTickTock::Hop()
{
	m_Velocity.y = 200.f;
}

void MrTickTock::SelectNewAction()
{
	const int nrPossibleActions{ 3 };
	int randomNr{ rand() % nrPossibleActions };
	switch (randomNr)
	{
	case 0:
		m_XDestination = m_pKirby->GetLocation().x;
		SetState(ActionState::moveToDestination);
		break;
	case 1:
		SetState(ActionState::hopping);
		break;
	case 2:
		SetState(ActionState::ringing);
		break;
	case 3:
		SetState(ActionState::ringingWithNotes);
		break;
	case 4:
		SetState(ActionState::shuffleStep);
		break;
	}
}

void MrTickTock::DoIdle(float elapsedSec)
{
	SlowDown(elapsedSec); // Inside is functionality that sets arbitraryTimer to 0.f as long as tick tock is still moving

	const float timeBeforeNewAction{ 1.6f };
	if (m_ArbitraryTimer > timeBeforeNewAction)
	{
		SelectNewAction();
	}
}

void MrTickTock::SetState(ActionState state)
{
	m_ActionState = state;
	m_ArbitraryTimer = 0.f;
	switch (state)
	{
	case ActionState::idle:
		m_pCurrentSprite = GetSpritePtr("mrticktock_idle");
		break;
	case ActionState::hopping:
		m_pCurrentSprite = GetSpritePtr("mrticktock_jumping");
		break;
	case ActionState::moveToDestination:
	case ActionState::shuffleStep:
		m_pCurrentSprite = GetSpritePtr("mrticktock_walking");
		break;
	case ActionState::ringingWithNotes:
	case ActionState::ringing:
		m_pCurrentSprite = GetSpritePtr("mrticktock_ringing");
		break;
	}
}

void MrTickTock::DoRinging()
{
	m_RingingZone = Rectf{ 0.f, 0.f, 128.f, 64.f };
	m_RingingZone.left = m_Shape.left + m_Shape.width /2 - m_RingingZone.width / 2;
	m_RingingZone.bottom = m_Shape.bottom;

	if (m_ArbitraryTimer > 2.5f)
	{
		SetState(ActionState::idle);
	}
}

void MrTickTock::SlowDown(float elapsedSec)
{
	const float speedTreshold{ 8.f };
	const float deceleration{ 500.f };
	if (m_Velocity.x > speedTreshold)
	{
		m_Velocity.x -= deceleration * elapsedSec;
		m_ArbitraryTimer = 0.f; // Lock timer to 0.f as long as tick tock is still moving
	}
	else
	{
		m_Velocity.x = 0.f;
	}
}
