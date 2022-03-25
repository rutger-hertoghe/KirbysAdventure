#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include "utils.h"
#include "Kirby.h"
#include "SoundStream.h"
#include "SVGParser.h"

Level::Level(std::string texturePath, std::string musicPath)
	: m_pBackground{ new Texture{ texturePath } }
	, m_pLevelMusic{ new SoundStream{ musicPath } }
	, m_StartLocation{100.f, 100.f}
{
	m_Boundaries = Rectf{ 0.f, 0.f, m_pBackground->GetWidth(), m_pBackground->GetHeight() };
	InitializeVertices();
	Initialize();
}

Level::~Level()
{
	delete m_pBackground;
	m_pBackground = nullptr;

	delete m_pLevelMusic;
	m_pLevelMusic = nullptr;
}

void Level::Initialize()
{
	m_pLevelMusic->Play(true);
}

void Level::Draw() const
{
	Rectf dstRect{ 0.f, 0.f, m_pBackground->GetWidth(), m_pBackground->GetHeight() };
	m_pBackground->Draw(dstRect);
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	Point2f rayStart{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 4 };
	Point2f rayEnd{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1.f };

	Point2f horizontalRayStart{ actorShape.left, actorShape.bottom + actorShape.height / 2 };
	Point2f horizontalRayEnd{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2 };

	utils::HitInfo hitInfo{};
	for (std::vector<Point2f> platform : m_WalkablePlatforms) {
		// Vertical collision
		if (actorVelocity.y < 0.f
			&& utils::Raycast(platform, rayStart, rayEnd, hitInfo))
		{
			actorShape.bottom = hitInfo.intersectPoint.y;
			actorVelocity.y = 0;
		}

		// Horizontal collision
		if (utils::Raycast(platform, horizontalRayStart, horizontalRayEnd, hitInfo))
		{
			
			actorShape.left = hitInfo.intersectPoint.x + (hitInfo.lambda > 0.5f ? - actorShape.width : 0.f);
			actorVelocity.x = 0;
		}
	}
}

void Level::InitializeVertices()
{
	SVGParser::GetVerticesFromSvgFile("resources/SVG/part1.svg", m_WalkablePlatforms);
}

bool Level::IsOnGround(const Rectf& actorShape) const
{
	Point2f rayStart{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 4 };
	Point2f rayEnd{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1.f};

	utils::HitInfo hitInfo{};
	for (const std::vector<Point2f>& platform : m_WalkablePlatforms)
	{
		if (utils::Raycast(platform, rayStart, rayEnd, hitInfo))
		{
			return true;
		}
	}
	return false;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

Point2f Level::GetStartLocation() const
{
	return m_StartLocation;
}

