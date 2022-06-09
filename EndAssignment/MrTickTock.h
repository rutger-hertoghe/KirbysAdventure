#pragma once
#include "Actor.h"
#include "Kirby.h"

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
		ringingWithNotes,
		dead,
		idle,
		shuffleStep
	};


	const int m_MaxLives;
	int m_Lives;
	bool m_GotDamaged;

	float m_InvulnerabilityTimer;
	float m_XDestination;
	
	int m_Sprite;
	Point2f m_CameraLockLocation;
	Rectf m_RingingZone;

	ActionState m_ActionState;

	Kirby* m_pKirby;

	void InitializeSprites();
	void Kill();
	void UpdateInvulnerability(float elapsedSec);
	void DoMoveToKirby();
	void DoHops();
	void Hop();
	void SelectNewAction();
	void DoIdle(float elapsedSec);
	void SetState(ActionState state);
	void DoRinging();
	void SlowDown(float elapsedSec);
};

