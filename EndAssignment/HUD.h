#pragma once
class Texture;
class Kirby;
class Sprite;

class HUD
{
public:
	HUD();
	~HUD();

	void Draw(Kirby* kirbyPtr) const;
	float GetHeight() const;
	float GetWidth() const;

	void Update(float elapsedSec);

private:
	Point2f m_PosDancingKirby;

	Texture* m_pMainTexture;
	Texture* m_pPowerTextures;
	Sprite* m_pHealth;
	Sprite* m_pHealthLost;
	Sprite* m_pDancingKirby;
	Point2f m_Dimensions;

	void DrawLives(Kirby* kirbyPtr) const;
	void DrawPowerPanel(Kirby* kirbyPtr) const;
};

