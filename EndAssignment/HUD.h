#pragma once
class Texture;
class Kirby;
class Sprite;
class HudElement;

class HUD final
{
public:
	explicit HUD(Kirby* pKirby);
	HUD(const HUD& other) = delete;
	HUD& operator=(const HUD& rhs) = delete;
	HUD(HUD&& other) = delete;
	HUD& operator=(HUD&& other) = delete;
	~HUD();

	void Draw() const;
	float GetHeight() const;
	float GetWidth() const;

	void Update(float elapsedSec);

private:
	Point2f m_PosDancingKirby;
	Point2f m_PosScore;

	Kirby* m_pKirby;

	Texture* m_pNumbers;
	Texture* m_pMainTexture;
	Texture* m_pPowerTextures;
	Texture* m_pScore;
	
	HudElement* m_pHealth;
	HudElement* m_pHealthLost;
	HudElement* m_pDancingKirby;
	Point2f m_Dimensions;

	void DrawPowerPanel() const;
	void DrawHealth() const;
	void DrawLives() const;
	void DrawScore() const;
	void DrawNumber(Point2f& location, int number, int digits) const;
};

