#pragma once
#include "Actor.h"
class MrTickTock final : public Actor
{
public:
	explicit MrTickTock(const Point2f& location);
	MrTickTock(const MrTickTock& other) = delete;
	MrTickTock& operator=(const MrTickTock& other) = delete;
	MrTickTock(MrTickTock&& other) = delete;
	MrTickTock& operator=(MrTickTock&& other) = delete;
	~MrTickTock() override;

	virtual void Update(float elapsedSec) override;
	void DecrementHealth();
	bool IsDead() const;

private:
	const int m_MaxLives;
	int m_Lives;

	float m_InvulnerabilityTimer;
	
	int m_Sprite;
	Point2f m_CameraLockLocation;

	bool m_GotDamaged;
	bool m_IsDead;

	void InitializeSprites();
	void Kill();
	void UpdateInvulnerability(float elapsedSec);

	// TESTING FUNCTIONS, WILL NOT BE IN FINAL GAME
	void ChangeSpriteAfterTwoSeconds();

};

