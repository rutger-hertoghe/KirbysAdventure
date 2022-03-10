#pragma once
#include <vector>
#include "Vector2f.h"
#include "Platform.h"
class Texture;
class Kirby;
class PowerStar;

class Level
{
private:
	Rectf m_Boundaries;

	std::vector<Point2f> m_Vertices;
	std::vector<Platform> m_Platforms;

	Texture* m_pBackground;
	Kirby* m_pKirby;
	PowerStar* m_pTestStar;
	
public:
	Level(std::string texturePath);
	~Level();

	void Draw() const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	void InitializeVertices();
	void InitializePlatforms();

	bool IsOnGround(Rectf& actorShape) const;

	Rectf GetBoundaries() const;
};

