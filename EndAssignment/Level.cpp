#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include "utils.h"
#include "Kirby.h"
#include "SoundStream.h"
#include "SVGParser.h"
#include <fstream>

Level::Level(std::string levelName, std::string musicPath)
	: m_Name{levelName}
	, m_pLevelMusic{ new SoundStream{ musicPath } }
	, m_pForeground{ nullptr }
	, m_StartLocation{24.f, 72.f}
{
	Initialize();
}

Level::~Level()
{
	delete m_pFullBackground;
	m_pFullBackground = nullptr;

	delete m_pForeground;
	m_pForeground = nullptr;

	delete m_pMidground;
	m_pMidground = nullptr;
	
	delete m_pBackground;
	m_pBackground = nullptr;
	
	delete m_pFarBackground;
	m_pFarBackground = nullptr;

	delete m_pLevelMusic;
	m_pLevelMusic = nullptr;
}

void Level::Initialize()
{
	// m_pLevelMusic->Play(true);
	InitializeVertices();
	InitializeTextures();
	LoadDoorsFromFile();
}

void Level::InitializeVertices()
{
	std::string svgPath{ "resources/SVG/" };
	std::string blockOutPath{ svgPath + m_Name + "_blockout.svg"};
	std::string platformPath{ svgPath + m_Name + "_platforms.svg" };
	SVGParser::GetVerticesFromSvgFile(platformPath, m_Platforms);
	SVGParser::GetVerticesFromSvgFile(blockOutPath, m_Blockout);
}

void Level::InitializeTextures()
{
	std::string texturePath{ "resources/backgrounds/" };
	texturePath.append(m_Name);

	std::string fullBackgroundPath{ texturePath };
	fullBackgroundPath.append("_full.png");
	m_pFullBackground = new Texture{ fullBackgroundPath };

	std::string foregroundPath{ texturePath};
	foregroundPath.append("_foreground.png");
	m_pForeground = new Texture{ foregroundPath };
	
	std::string midGroundPath{ texturePath};
	midGroundPath.append("_midground.png");
	m_pMidground = new Texture{ midGroundPath };

	std::string backGroundPath{ texturePath };
	backGroundPath.append("_background.png");
	m_pBackground = new Texture{ backGroundPath };
	
	std::string farBackGroundPath{ texturePath };
	farBackGroundPath.append("_farbackground.png");
	m_pFarBackground = new Texture{ farBackGroundPath };

	m_Boundaries = Rectf{ 0.f, 0.f, m_pFullBackground->GetWidth(), m_pFullBackground->GetHeight() };
}

void Level::DrawFull() const
{
	Rectf dstRect{ 0.f, 0.f, m_pFullBackground->GetWidth(), m_pFullBackground->GetHeight() };
	m_pFullBackground->Draw(dstRect);
}

void Level::DrawForeGround() const
{
	Rectf dstRect{ 0.f, 0.f, m_pForeground->GetWidth(), m_pForeground->GetHeight() };
	m_pForeground->Draw(dstRect);
}

void Level::DrawMidGround() const
{
	Rectf dstRect{ 0.f, 0.f, m_pMidground->GetWidth(), m_pForeground->GetHeight() };
	m_pMidground->Draw(dstRect);
}

void Level::DrawBackGround() const
{
	Rectf dstRect{ 0.f, 0.f, m_pBackground->GetWidth(), m_pBackground->GetHeight() };
	m_pBackground->Draw(dstRect);
}

void Level::DrawFarBackGround() const
{
	Rectf dstRect{ 0.f, 0.f, m_pFarBackground->GetWidth(), m_pFarBackground->GetHeight() };
	m_pFarBackground->Draw(dstRect);
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	DoVerticalCollisions(actorShape, actorVelocity);
	DoHorizontalCollisions(actorShape, actorVelocity);
}

bool Level::IsOnGround(const Rectf& actorShape) const
{
	float pixelReduction{ 1.f };
	Point2f leftRayStart{ actorShape.left + pixelReduction, actorShape.bottom + actorShape.height / 4 };
	Point2f leftRayEnd{ actorShape.left + pixelReduction, actorShape.bottom};

	Point2f rightRayStart{ actorShape.left + actorShape.width - pixelReduction, actorShape.bottom + actorShape.height / 4 };
	Point2f rightRayEnd{ actorShape.left + actorShape.width - pixelReduction, actorShape.bottom - 1.f}; // -1.f extends ray by one pixel to set on ground because collision sets it one pixel above

	utils::HitInfo hitInfo{};
	for (const std::vector<Point2f>& platform : m_Blockout)
	{
		if (utils::Raycast(platform, leftRayStart, leftRayEnd, hitInfo))
		{
			return true;
		}
		else if (utils::Raycast(platform, rightRayStart, rightRayEnd, hitInfo))
		{
			return true;
		}
	}

	for (const std::vector<Point2f>& platform : m_Platforms)
	{
		if (utils::Raycast(platform, leftRayStart, leftRayEnd, hitInfo))
		{
			return true;
		}
		else if (utils::Raycast(platform, rightRayStart, rightRayEnd, hitInfo))
		{
			return true;
		}
	}
	return false;
}

std::string Level::GetName() const
{
	return m_Name;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

Point2f Level::GetStartLocation() const
{
	return m_StartLocation;
}

Door Level::GetDoorInfo(const Rectf& actorShape)
{
	for (Door door : m_Doors)
	{
		if (door.IsOnDoor(actorShape))
		{
			return door;
		}
	}
	return Door{ "", Point2f{0, 0}, Point2f{0,0} };
}

void Level::DoVerticalCollisions(Rectf& actorShape, Vector2f& actorVelocity) const
{
	float pixelReduction{ 1.f };
	Point2f leftRayStart{ actorShape.left + pixelReduction, actorShape.bottom + actorShape.height / 4 };
	Point2f leftRayEnd{ actorShape.left + pixelReduction, actorShape.bottom };

	Point2f rightRayStart{ actorShape.left + actorShape.width - pixelReduction, actorShape.bottom + actorShape.height / 4 };
	Point2f rightRayEnd{ actorShape.left + actorShape.width - pixelReduction, actorShape.bottom}; // -1.f extends ray by one pixel to set on ground because collision sets it one pixel above

	utils::HitInfo hitInfo{};
	if (actorVelocity.y < 0.f) // Skip over this code if actor is not going downwards
	{
		for (std::vector<Point2f> platform : m_Platforms) {
			if (utils::Raycast(platform, leftRayStart, leftRayEnd, hitInfo))
			{
				actorShape.bottom = hitInfo.intersectPoint.y;
				actorVelocity.y = 0;
				return;
			}
			else if (actorVelocity.y < 0.f
				&& utils::Raycast(platform, rightRayStart, rightRayEnd, hitInfo))
			{
				actorShape.bottom = hitInfo.intersectPoint.y;
				actorVelocity.y = 0;
				return;
			}
		}
	}

	// Use full raycast line to also check ceiling collisions
	leftRayStart.y = actorShape.bottom + actorShape.height - pixelReduction;
	rightRayStart.y = actorShape.bottom + actorShape.height - pixelReduction;

	for (std::vector<Point2f> platform : m_Blockout) 
	{
		if (utils::Raycast(platform, leftRayStart, leftRayEnd, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y + (hitInfo.lambda < 0.5f ? -actorShape.height  - 1.f: 0.f);
			actorVelocity.y = 0;
			return;
		}
		else if (utils::Raycast(platform, rightRayStart, rightRayEnd, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y + (hitInfo.lambda < 0.5f ? -actorShape.height - 1.f : 0.f);
			actorVelocity.y = 0;
			return;
		}
	}
}

void Level::DoHorizontalCollisions(Rectf& actorShape, Vector2f& actorVelocity) const
{
	Point2f horizontalRayStart{ actorShape.left, actorShape.bottom + actorShape.height / 2 };
	Point2f horizontalRayEnd{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2 };

	utils::HitInfo hitInfo{};

	for (std::vector<Point2f> platform : m_Blockout)
	{
		if (utils::Raycast(platform, horizontalRayStart, horizontalRayEnd, hitInfo))
		{

			actorShape.left = hitInfo.intersectPoint.x + (hitInfo.lambda > 0.5f ? -actorShape.width : 0.f);
			actorVelocity.x = 0;
			return;
		}
	}
}

void Level::LoadDoorsFromFile()
{
	std::string path{ "resources/doordata/" + m_Name + ".txt" };
	std::fstream file;
	file.open(path, std::ios::in);

	if (!file)
	{
		std::cout << "DOOR FILE NOT FOUND\n";
	}

	while (file.good())
	{
		std::string bufferString;
		std::getline(file, bufferString, '\n');

		int firstComma{ int(bufferString.find(',')) };
		std::string levelName{ bufferString.substr(0, firstComma) };

		int indexAfter{ firstComma + 1 };
		int secondComma{ int(bufferString.find(',', indexAfter)) };
		float xLocation{ std::stof(bufferString.substr(indexAfter, secondComma)) };

		indexAfter = secondComma + 1;
		int thirdComma{ int(bufferString.find(',', indexAfter)) };
		float yLocation{ std::stof(bufferString.substr(indexAfter, thirdComma)) };

		indexAfter = thirdComma + 1;
		int fourthComma{ int(bufferString.find(',', indexAfter)) };
		float xExit{ std::stof(bufferString.substr(indexAfter, fourthComma)) };

		indexAfter = fourthComma + 1;
		int endPos{ int(bufferString.size()) };
		float yExit{ std::stof(bufferString.substr(indexAfter, endPos)) };
		m_Doors.push_back(Door{ levelName, Point2f{ xLocation, yLocation }, Point2f{ xExit, yExit } });
	}
}

