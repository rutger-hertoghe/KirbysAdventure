#pragma once
#include "Vector2f.h"
#include "Platform.h"
class Texture;
class Kirby;
class SoundStream;

class Level final
{
private:
	Rectf m_Boundaries;
	Point2f m_StartLocation;

	std::vector<std::vector<Point2f>> m_WalkablePlatforms;

	Texture* m_pBackground;
	SoundStream* m_pLevelMusic;
	
public:
	Level(std::string texturePath, std::string musicPath);
	~Level();

	void Initialize();
	void Draw() const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	void InitializeVertices();

	bool IsOnGround(const Rectf& actorShape) const;

	Rectf GetBoundaries() const;

	Point2f GetStartLocation() const;
};

