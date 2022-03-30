#pragma once
class Texture;
class Kirby;
class Sprite;
class HudElement;

class HUD
{
public:
	HUD();
	HUD(const HUD& other) = delete;
	HUD& operator=(const HUD& rhs) = delete;
	~HUD();

	void Draw(Kirby* kirbyPtr) const;
	float GetHeight() const;
	float GetWidth() const;

	void Update(float elapsedSec);

private:
	Point2f m_PosDancingKirby;

	Texture* m_pMainTexture;
	Texture* m_pPowerTextures;
	
	HudElement* m_pHealth;
	HudElement* m_pHealthLost;
	HudElement* m_pDancingKirby;
	Point2f m_Dimensions;

	void DrawPowerPanel(Kirby* kirbyPtr) const;
	void DrawHealth(Kirby* kirbyPtr) const;
};

