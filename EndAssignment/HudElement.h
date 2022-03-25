#pragma once
#include "GameObject.h"
#include "Sprite.h"

class HudElement final : public GameObject
{
public:
	HudElement(Sprite* pSprite);
	~HudElement();

	void Update(float elapsedSec);
	void SetPosition(const Point2f& bottomLeft);
	float GetWidth();
	float GetHeight();
};

