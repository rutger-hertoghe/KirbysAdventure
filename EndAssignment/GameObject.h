#pragma once
class PowerUp;
class Sprite;

class GameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Draw() const;
	virtual void Update(float elapsedSec) = 0;

	Rectf GetRect() const;

protected:
	int m_CurrentFrame;
	float m_AccumulatedTime;
	float m_LoopProgressTime;
	bool m_HasLooped;
	
	float m_XDirection;

	Rectf m_Shape;

	Sprite* m_pCurrentSprite;

	std::vector<Sprite*> m_pSprites;

	void UpdateSprite(float elapsedSec);
};

