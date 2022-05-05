#pragma once
#include "Actor.h"
#include "Kirby.h"

class MrTickTock final : public Actor
{
public:
	explicit MrTickTock(const Point2f& location, Kirby* pKirby);
	MrTickTock(const MrTickTock& other) = delete;
	MrTickTock& operator=(const MrTickTock& other) = delete;
	MrTickTock(MrTickTock&& other) = delete;
	MrTickTock& operator=(MrTickTock&& other) = delete;
	~MrTickTock() override;

	virtual void Update(float elapsedSec) override;
	void DecrementHealth();
	bool IsDead() const;

private:
	enum class ActionState
	{
		movingToKirby,
		hopping,
		ringing,
		ringingWithNotes,
		dead,
		idle
	};


	const int m_MaxLives;
	int m_Lives;
	bool m_GotDamaged;

	float m_InvulnerabilityTimer;
	
	int m_Sprite;
	Point2f m_CameraLockLocation;

	ActionState m_ActionState;

	Kirby* m_pKirby;

	void InitializeSprites();
	void Kill();
	void UpdateInvulnerability(float elapsedSec);

	void SelectRandomState();
};

