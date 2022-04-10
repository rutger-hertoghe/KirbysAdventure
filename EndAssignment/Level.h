#pragma once
#include "Vector2f.h"
#include "Platform.h"
class Texture;
class Kirby;
class SoundStream;

class Level final
{
public:
	Level(std::string levelName, std::string musicPath);
	Level(const Level& other) = delete;
	Level& operator=(const Level& other) = delete;
	~Level();

	void Initialize();
	void DrawFull() const;
	void DrawForeGround() const;
	void DrawMidGround() const;
	void DrawBackGround() const;
	void DrawFarBackGround() const;

	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	void InitializeVertices();
	void InitializeTextures();

	bool IsOnGround(const Rectf& actorShape) const;
	std::string GetName() const;

	Rectf GetBoundaries() const;

	Point2f GetStartLocation() const;

private:

	std::string m_Name;
	Rectf m_Boundaries;
	Point2f m_StartLocation;

	std::vector<std::vector<Point2f>> m_Blockout;
	std::vector<std::vector<Point2f>> m_Platforms;

	Texture* m_pFullBackground;
	Texture* m_pForeground;
	Texture* m_pMidground;
	Texture* m_pBackground;
	Texture* m_pFarBackground;
	SoundStream* m_pLevelMusic;

	void DoVerticalCollisions(Rectf& actorShape, Vector2f& actorVelocity) const;
	void DoHorizontalCollisions(Rectf& actorShape, Vector2f& actorVelocity) const;
	void CreateTexture(const std::string& tag, Texture*& pTargetTexture);
};

