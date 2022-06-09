#pragma once
#include "Vector2f.h"
#include "Door.h"

class Texture;
class Kirby;
class SoundStream;
class ObjectManager;
class Camera;

class Level final
{
public:
	Level(std::string levelName, std::string musicPath);
	Level(const Level& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level&& other) = delete;
	~Level(); // Level has no ownership of dynamically allocated objects

	void DrawFull() const;
	void DrawForeGround() const;
	void DrawMidGround() const;
	void DrawBackGround() const;
	void DrawFarBackGround() const;

	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;

	bool IsOnGround(const Rectf& actorShape) const;
	bool IsAgainstWall(const Rectf& actorShape, float actorDirection) const;

	std::string GetName() const;
	Rectf GetBoundaries() const;
	Point2f GetStartLocation() const;
	Door GetDoorInfo(const Rectf& actorShape);

private:
	std::string m_Name;
	Rectf m_Boundaries;
	Point2f m_StartLocation;

	std::vector<Door> m_Doors;

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

	void LoadDoorsFromFile();

	void Initialize();
	void InitializeVertices();
	void InitializeTextures();
};

