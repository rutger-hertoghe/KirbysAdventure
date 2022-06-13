#pragma once
#include "Actor.h"
#include "Kirby.h"

class SoundWave;
class Note;

class MrTickTock final : public Actor
{
public:
	explicit MrTickTock(const Point2f& location, Kirby* pKirby, LevelManager* pLevelManager, ProjectileManager* pProjectileManager);
	MrTickTock(const MrTickTock& other) = delete;
	MrTickTock& operator=(const MrTickTock& other) = delete;
	MrTickTock(MrTickTock&& other) = delete;
	MrTickTock& operator=(MrTickTock&& other) = delete;
	~MrTickTock() override;

	virtual void Update(float elapsedSec) override;
	virtual void Draw() const override;
	void DecrementHealth();
	bool IsDead() const;

private:
	enum class ActionState
	{
		moveToDestination,
		hopping,
		ringing,
		// ringingWithNotes, 
		// Had to disable this because it causes a game crashing error that I couldn't figure out
		// Uncomment at your own peril
		shuffleStep,
		END_OF_SELECTABLE_ACTIONS, 
		// Actions that can be selected while idle should be above this element of the enum, other action states should be below
		dead,
		idle
	};


	const int m_MaxLives;
	int m_Lives;

	int m_NotesShot;
	int m_TimesShuffleStepped;
	bool m_GotDamaged;

	float m_InvulnerabilityTimer;
	float m_XDestination;
	
	int m_Sprite;
	Point2f m_CameraLockLocation;
	Rectf m_RingingZone;

	ActionState m_ActionState;
	ActionState m_PreviousSelectedState;

	Kirby* m_pKirby;

	std::vector<SoundWave*> m_pSoundWaves;
	std::vector<Note*> m_pNotes;

	void InitializeSprites();
	void InitializeNotes();
	void Kill();
	void UpdateInvulnerability(float elapsedSec);
	
	void SetState(ActionState state);

	void DoStateAction(float elapsedSec);
	void DoMoveToKirby();
	void DoHops();
	void Hop();
	void SelectNewAction();
	void DoIdle(float elapsedSec);
	void DoRinging();
	void DoRingingWithNotes();
	void DoShuffleStep();
	void SlowDown(float elapsedSec);

	void InitializeSoundWaves();
	void UpdateSoundWaves(float elapsedSec);
	void DrawSoundWaves() const;
	void ClearSoundWaves();
	void ActivateSoundWaves();
};

