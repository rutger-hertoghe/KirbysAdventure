#pragma once
#include "GameObject.h"
#include "Sprite.h"

class HudElement final : public GameObject
{
public:
	explicit HudElement(Sprite* pSprite);
	HudElement(const HudElement& other) = delete;
	HudElement& operator=(const HudElement& other) = delete;
	HudElement(HudElement&& other) = delete;
	HudElement& operator=(HudElement&& other) = delete;
	~HudElement();

	void Update(float elapsedSec);
	void SetPosition(const Point2f& bottomLeft);
	float GetWidth();
	float GetHeight();
};

