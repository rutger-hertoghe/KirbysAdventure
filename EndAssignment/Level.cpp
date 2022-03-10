#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include "utils.h"

Level::Level(std::string texturePath)
{
	m_pBackground = new Texture{ texturePath };
	m_Boundaries = Rectf{ 0.f, 0.f, m_pBackground->GetWidth(), m_pBackground->GetHeight() };
	InitializeVertices();
	InitializePlatforms();
}

Level::~Level()
{
	delete m_pBackground;
	m_pBackground = nullptr;
}

void Level::Draw() const
{
	Rectf dstRect{ 0.f, 0.f, m_pBackground->GetWidth(), m_pBackground->GetHeight() };
	m_pBackground->Draw(dstRect);
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	Point2f rayStart{actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height};
	Point2f rayEnd{ actorShape.left + actorShape.width / 2, actorShape.bottom};

	utils::HitInfo hitInfo{};
	if (utils::Raycast(m_Vertices, rayStart, rayEnd, hitInfo))
	{
		actorShape.bottom = hitInfo.intersectPoint.y;
		actorVelocity.y = 0;
	}
	
	for (const Platform& platform : m_Platforms)
	{
		platform.HandleCollision(actorShape, actorVelocity);
	}
}

void Level::InitializeVertices()
{
	m_Vertices.push_back(Point2f{ m_Boundaries.left, m_Boundaries.bottom });
	m_Vertices.push_back(Point2f{ m_Boundaries.left, m_Boundaries.bottom + 10.f });
	m_Vertices.push_back(Point2f{ m_Boundaries.left + m_Boundaries.width, m_Boundaries.bottom + 10.f });
	m_Vertices.push_back(Point2f{ m_Boundaries.left + m_Boundaries.width, m_Boundaries.bottom });
}

void Level::InitializePlatforms()
{
	m_Platforms.push_back(Platform(Rectf{ 272.f, 88.f, 32.f, 0.f }));
	m_Platforms.push_back(Platform(Rectf{ 288.f, 24.f, 48.f, 0.f }));
	m_Platforms.push_back(Platform(Rectf{ 16.f, 24.f, 224.f, 0.f }));
	m_Platforms.push_back(Platform(Rectf{ 384.f, 40.f, 64.f, 0.f }));
	m_Platforms.push_back(Platform(Rectf{ 480.f, 24.f, 32.f, 0.f }));
	m_Platforms.push_back(Platform(Rectf{ 544.f, 24.f, 64.f, 0.f }));
}

bool Level::IsOnGround(Rectf& actorShape) const
{
	Point2f rayStart{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height};
	Point2f rayEnd{ actorShape.left + actorShape.width / 2, actorShape.bottom - 1.f};

	utils::HitInfo hitInfo{};
	if (utils::Raycast(m_Vertices, rayStart, rayEnd, hitInfo))
	{
		return true;
	}

	for (const Platform& platform : m_Platforms)
	{
		if (platform.IsOnGround(actorShape)) return true;
	}
	return false;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}
