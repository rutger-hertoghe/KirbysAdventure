#include "pch.h"
#include "MrTickTock.h"
#include "Sprite.h"
#include "Camera.h"
#include "ObjectManager.h"
#include "Note.h"
#include "SoundWave.h"

MrTickTock::MrTickTock(const Point2f& location, Kirby* pKirby, LevelManager* pLevelManager, ProjectileManager* pProjectileManager)
	: Actor{pLevelManager, pProjectileManager}
	, m_pKirby{pKirby}
	, m_CameraLockLocation{location}
	, m_Sprite{0}
	, m_MaxLives{ 3 }
	, m_InvulnerabilityTimer{0.f}
	, m_GotDamaged{false}
	, m_ActionState{ActionState::idle}
	, m_NotesShot{0}
	, m_TimesShuffleStepped{0}
	, m_RingingZone{0.f, 0.f, 64.f, 40.f }
{
	m_Lives = m_MaxLives; // Initialized here because it requires m_MaxLives to be initialized
	m_IsInhalable = false; // Initialized here because is member of Actor class

	InitializeSprites();
	InitializeSoundWaves();
	InitializeNotes();

	SetInitialSprite();
	SetDimsFromSprite();
	SetLocation(location);

	SetBaseVelocity(100.f, 200.f);
	SetVelocity(0.f, 0.f);
}

MrTickTock::~MrTickTock()
{
	for (Sprite*& pSprite : m_pSprites)
	{
		delete pSprite;
		pSprite = nullptr;
	}
	m_pCurrentSprite = nullptr;

	for (SoundWave* pWave : m_pSoundWaves)
	{
		delete pWave;
		pWave = nullptr;
	}
}

void MrTickTock::Update(float elapsedSec)
{
	UpdateSprite(elapsedSec);
	UpdateSoundWaves(elapsedSec);

	ApplyVelocities(elapsedSec, m_XDirection * m_Velocity.x, m_Velocity.y);
	HandleLevelCollisions();

	if (m_ActionState  == ActionState::dead)
	{
		ApplyGravity(elapsedSec);
		return;
	}

	m_ArbitraryTimer += elapsedSec;

	DoStateAction(elapsedSec);

	UpdateInvulnerability(elapsedSec);
	
	ApplyGravity(elapsedSec);
	SetIsOnGround();
}

void MrTickTock::Draw() const
{
	GameObject::Draw();
	DrawSoundWaves();
}

void MrTickTock::DecrementHealth()
{
	if (m_InvulnerabilityTimer > 0.f)
	{
		return;
	}
	--m_Lives;
	m_GotDamaged = true;
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

void MrTickTock::InitializeNotes()
{
	const int notesShotPerRing{ 4 };
	for (int i{}; i < notesShotPerRing; ++i)
	{
		m_pNotes.push_back(new Note{ GetCenter(), m_pKirby, m_pLevelManager, m_pProjectileManager });
		ObjectManager::GetObjectMngr()->AddItem(m_pNotes[i]);
	}
}

void MrTickTock::Kill()
{
	m_pCurrentSprite = GetSpritePtr("mrticktock_dead");
	m_ActionState = ActionState::dead;
	m_IsInhalable = true;
	m_pSoundWaves[0]->SetActive(false);
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

void MrTickTock::DoStateAction(float elapsedSec)
{
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
	/*case ActionState::ringingWithNotes:
		DoRingingWithNotes();
		break;*/
	case ActionState::shuffleStep:
		DoShuffleStep();
		break;
	}
}

void MrTickTock::DoMoveToKirby()
{
	const float distanceTreshold{ 8.f };
	const float xDifference{ m_Shape.left - m_XDestination };
	const float yDifference{ m_Shape.bottom - m_pKirby->GetLocation().y};

	m_XDirection = (xDifference < 0.f) ? 1.f : -1.f;

	const float minDistanceForJump{ 64.f };
	const float minHeightDifferenceForJump{ 16.f };
	const float maxMovementTime{ 3.f };

	if (abs(xDifference) > minDistanceForJump && abs(yDifference) > minHeightDifferenceForJump && m_IsOnGround)
	{
		Hop();
	}
	else if (abs(xDifference) > distanceTreshold && m_ArbitraryTimer < maxMovementTime)
	{
		m_Velocity.x = m_BaseVelocity.x;
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
	m_Velocity.y = m_BaseVelocity.y;
}

void MrTickTock::SelectNewAction()
{
	const int nrPossibleActions{ int(ActionState::END_OF_SELECTABLE_ACTIONS) };
	ActionState nextActionState;
	do {
		nextActionState = ActionState(rand() % nrPossibleActions);
	} while (nextActionState == m_PreviousSelectedState); // Repeat while true so same action is never chosen twice consecutively

	if (nextActionState == ActionState::moveToDestination)
	{
		m_XDestination = m_pKirby->GetLocation().x;
	}

	SetState(nextActionState);
	m_PreviousSelectedState = nextActionState;
}

void MrTickTock::DoIdle(float elapsedSec)
{
	SlowDown(elapsedSec); // Inside is functionality that sets arbitraryTimer to 0.f as long as tick tock is still moving

	m_XDirection = -GetRelativeDirection(m_pKirby); // Make Ticktock always face kirby while idle

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
	// case ActionState::ringingWithNotes:
	case ActionState::ringing:
		m_pCurrentSprite = GetSpritePtr("mrticktock_ringing");
		break;
	}
}

void MrTickTock::DoRinging()
{
	m_RingingZone.left = GetCenter().x - m_RingingZone.width / 2;
	m_RingingZone.bottom = m_Shape.bottom;

	const Point2f centerOfTicktock{ GetCenter() };
	if (m_pSoundWaves[0]->IsActive() == false) // All should be activated at once so it suffices using activation state of 0th element
	{
		ActivateSoundWaves();
	}

	if (m_ArbitraryTimer > 2.5f)
	{
		SetState(ActionState::idle);
		m_pSoundWaves[0]->SetActive(false);
	}

	if (utils::IsPointInRect(m_pKirby->GetCenter(), m_RingingZone))
	{
		m_pKirby->DecrementHealth(-GetRelativeDirection(m_pKirby)); // minus because tick tock is this, and Kirby is other, so result is reversed
	}
	SoundFXManager::Play("noteshot");
}

void MrTickTock::DoRingingWithNotes()
{
	const int maxNotes{ int(m_pNotes.size())};
	const float timeBetweenShots{0.8f};
	const float distIncreasePerShot{ 32.f };
	if (m_NotesShot < maxNotes && m_ArbitraryTimer > timeBetweenShots)
	{
		const int notesShotPlusOne{ m_NotesShot + 1 };
		float targetXLocation{ GetLocation().x + powf(-1.f, float(notesShotPlusOne)) * notesShotPlusOne * distIncreasePerShot};
		if (m_NotesShot < maxNotes) // Additional safety to prevent going out of bounds
		{
			m_pNotes[m_NotesShot]->Activate(GetLocation(), targetXLocation);
		}
		SoundFXManager::Play("noteshot");
		m_ArbitraryTimer = 0.f;
		++m_NotesShot;
	}
	else if (m_NotesShot >= maxNotes)
	{
		m_NotesShot = 0;
		SetState(ActionState::idle);
	}
	
}

void MrTickTock::DoShuffleStep()
{
	const int maxShuffles{ 4 };
	const float shuffleDuration{ 0.3f };
	if (m_ArbitraryTimer < shuffleDuration)
	{
		m_Velocity.x = powf(-1.f, float(m_TimesShuffleStepped)) * m_BaseVelocity.x;
		SoundFXManager::Play("kick");
	}
	else if (m_TimesShuffleStepped >= maxShuffles)
	{
		m_TimesShuffleStepped = 0;
		SetState(ActionState::idle);
	}
	else
	{
		m_ArbitraryTimer = 0.f;
		++m_TimesShuffleStepped;
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

void MrTickTock::InitializeSoundWaves()
{
	m_pSoundWaves.push_back(new SoundWave{ SoundWave::Orientation::topLeft });
	m_pSoundWaves.push_back(new SoundWave{ SoundWave::Orientation::topRight });
	m_pSoundWaves.push_back(new SoundWave{ SoundWave::Orientation::bottomLeft });
	m_pSoundWaves.push_back(new SoundWave{ SoundWave::Orientation::bottomRight });
}

void MrTickTock::UpdateSoundWaves(float elapsedSec)
{
	if (m_pSoundWaves[0]->IsActive())
	{
		for (SoundWave* pWave : m_pSoundWaves)
		{
			pWave->Update(elapsedSec);
		}
	}
}

void MrTickTock::DrawSoundWaves() const
{
	if (m_pSoundWaves[0]->IsActive()) // All should be activated at once so it suffices using activation state of 0th element
	{
		for (SoundWave* pWave : m_pSoundWaves)
		{
			{
				pWave->Draw();
			}
		}
	}
}

void MrTickTock::ClearSoundWaves()
{
	for (SoundWave* pWave : m_pSoundWaves)
	{
		delete pWave;
	}
	m_pSoundWaves.clear();
}

void MrTickTock::ActivateSoundWaves()
{
	const float soundWaveWidth{ m_pSoundWaves[0]->GetShape().width };
	const Point2f topLeftLocation
	{
		m_RingingZone.left,
		m_RingingZone.bottom + m_RingingZone.height - soundWaveWidth
	};
	const Point2f topRightLocation
	{
		m_RingingZone.left + m_RingingZone.width - soundWaveWidth,
		m_RingingZone.bottom + m_RingingZone.height - soundWaveWidth
	};
	const Point2f bottomLeftLocation
	{
		m_RingingZone.left,
		m_RingingZone.bottom
	};
	const Point2f bottomRightLocation
	{
		m_RingingZone.left + m_RingingZone.width - soundWaveWidth,
		m_RingingZone.bottom
	};

	m_pSoundWaves[0]->SetActive(true); // All should be activated at once so it suffices using activation state of 0th element

	for (SoundWave* pWave : m_pSoundWaves)
	{
		switch (pWave->GetOrientation())
		{
		case SoundWave::Orientation::bottomLeft:
			pWave->SetTargetLocation(bottomLeftLocation);
			break;
		case SoundWave::Orientation::bottomRight:
			pWave->SetTargetLocation(bottomRightLocation);
			break;
		case SoundWave::Orientation::topLeft:
			pWave->SetTargetLocation(topLeftLocation);
			break;
		case SoundWave::Orientation::topRight:
			pWave->SetTargetLocation(topRightLocation);
			break;
		}
	}
}
