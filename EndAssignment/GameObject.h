#pragma once
#include "SoundFXManager.h" // Include here to have the manager accessible in any game object without having to add the header to every derived class

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
	Point2f GetCenter() const;
	float GetDirection() const;
	Point2f GetLocation() const;

	void SetLocation(const Point2f& location);
	void SetLocation(float x, float y);

	bool IsOnScreen() const;
	float GetRelativeDirection(GameObject* other) const;

protected:
	Rectf m_Shape;
	int m_CurrentFrame;
	float m_AccumulatedTime;
	float m_LoopProgressTime;
	bool m_HasLooped;
	
	float m_XDirection;

	Sprite* m_pCurrentSprite;

	std::vector<Sprite*> m_pSprites;

	void UpdateSprite(float elapsedSec);
	void ChangeDirection();

	void SetInitialSprite(const std::string& spriteName = "");
	void SetDimsFromSprite();
	Sprite* GetSpritePtr(const std::string& spriteName) const;

private:
	
};

