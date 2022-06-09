#pragma once
class Sprite;

class GameObject
{
public:
	explicit GameObject();
	GameObject(const GameObject& other) = delete;
	GameObject& operator=(const GameObject&) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;
	virtual ~GameObject() = default;

	virtual void Draw() const;
	virtual void Update(float elapsedSec) = 0;

	Rectf GetShape() const;
	float GetDirection() const;
	bool IsOnScreen() const;

protected:
	Rectf m_Shape;
	int m_CurrentFrame;
	float m_AccumulatedTime;
	float m_LoopProgressTime;
	bool m_HasLooped;
	
	float m_XDirection;

	// TODO: sprite info terug in sprites steken en projectile management fixen OF ervoor zorgen dat sprite ALTIJD gereset wordt
	Sprite* m_pCurrentSprite;


	std::vector<Sprite*> m_pSprites;

	void UpdateSprite(float elapsedSec);
	void ChangeDirection();

	void SetInitialSprite(const std::string& spriteName = "");
	void SetDimsFromSprite();
	Sprite* GetSpritePtr(const std::string& spriteName) const;

private:
	
};

