#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include "utils.h"
#include "Kirby.h"
#include "SoundStream.h"

Level::Level(std::string texturePath, std::string musicPath)
	: m_pBackground{ new Texture{ texturePath } }
	, m_pLevelMusic{ new SoundStream{ musicPath } }
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
	Point2f verticalRayStart{actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height};
	Point2f verticalRayEnd{ actorShape.left + actorShape.width / 2, actorShape.bottom};

	Point2f horizontalRayStart{ actorShape.left, actorShape.bottom + actorShape.height / 2 };
	Point2f horizontalRayEnd{ actorShape.left + actorShape.width, actorShape.bottom + actorShape.height / 2 };

	utils::HitInfo hitInfo{};
	for (std::vector<Point2f> platform : m_WalkablePlatforms) {
		// Vertical collision
		if (utils::Raycast(platform, verticalRayStart, verticalRayEnd, hitInfo) && actorVelocity.y < 0.f)
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
	std::vector<Point2f> mainPlatform;
	mainPlatform.push_back(Point2f{ m_Boundaries.left, m_Boundaries.bottom });
	mainPlatform.push_back(Point2f{ m_Boundaries.left, m_Boundaries.bottom + 10.f });
	mainPlatform.push_back(Point2f{ 100.f, m_Boundaries.bottom + 10.f });
	mainPlatform.push_back(Point2f{ 100.f, 200.f });
	mainPlatform.push_back(Point2f{ 100.f, m_Boundaries.bottom + 10.f });
	mainPlatform.push_back(Point2f{ m_Boundaries.left + m_Boundaries.width, m_Boundaries.bottom + 10.f });
	mainPlatform.push_back(Point2f{ m_Boundaries.left + m_Boundaries.width, m_Boundaries.bottom });

	m_WalkablePlatforms.push_back(mainPlatform);

	std::vector<Point2f>platform1;
	platform1.push_back(Point2f{ 272.f, 88.f });
	platform1.push_back(Point2f{ 304.f, 88.f });
	m_WalkablePlatforms.push_back(platform1);

}

bool Level::IsOnGround(Rectf& actorShape) const
{
	Point2f rayStart{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height};
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

