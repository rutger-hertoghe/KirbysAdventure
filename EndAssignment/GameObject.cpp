#include "pch.h"
#include "Sprite.h"
#include "GameObject.h"
#include "ObjectManager.h"

GameObject::GameObject()
	: m_AccumulatedTime{ 0.f }
	, m_LoopProgressTime{ 0.f }
	, m_CurrentFrame{0}
	, m_HasLooped{false}
	, m_pCurrentSprite{nullptr}
	, m_XDirection{1.f}
{
}

void GameObject::Draw() const
{
	glPushMatrix();

	Point2f originTranslation(-m_Shape.left, -m_Shape.bottom);

	glTranslatef(-originTranslation.x, -originTranslation.y, 0.f);
	if (m_XDirection < 0.f)
	{
		glTranslatef(m_Shape.width, 0.f, 0.f);
	}
	glScalef(m_XDirection, 1, 0);
	glTranslatef(originTranslation.x, originTranslation.y, 0.f);

	if (m_pCurrentSprite) m_pCurrentSprite->Draw(m_Shape, m_CurrentFrame);

	glPopMatrix();
}

void GameObject::UpdateSprite(float elapsedSec)
{
	const float loopTime{ m_pCurrentSprite->GetLoopTime() };
	const int nrOfFrames{ m_pCurrentSprite->GetNrOfFrames() };
	const float frameTime{ loopTime / nrOfFrames };

	if (loopTime < 0.001f) return;
	m_AccumulatedTime += elapsedSec;
	m_LoopProgressTime += elapsedSec;
	if (m_AccumulatedTime >= frameTime)
	{
		++m_CurrentFrame;
		m_CurrentFrame %= nrOfFrames;
		m_AccumulatedTime = 0;
	}

	if (m_LoopProgressTime > loopTime)
	{
		m_HasLooped = true;
		m_LoopProgressTime = 0.f;
	}
	else m_HasLooped = false;
}

void GameObject::ChangeDirection()
{
	m_XDirection *= -1.f;
}

Rectf GameObject::GetShape() const
{
	return m_Shape;
}

Point2f GameObject::GetCenter() const
{
	return Point2f{ m_Shape.left + m_Shape.width / 2, m_Shape.bottom + m_Shape.height / 2};
}

float GameObject::GetDirection() const
{
	return m_XDirection;
}

Point2f GameObject::GetLocation() const
{
	return Point2f(m_Shape.left, m_Shape.bottom);
}

void GameObject::SetLocation(const Point2f& location)
{
	m_Shape.left = location.x;
	m_Shape.bottom = location.y;
}

void GameObject::SetLocation(float x, float y)
{
	m_Shape.left = x;
	m_Shape.bottom = y;
}

void GameObject::SetInitialSprite(const std::string& spriteName)
{
	if (spriteName == "")
	{
		m_pCurrentSprite = m_pSprites[0];
	}
	else
	{
		m_pCurrentSprite = GetSpritePtr(spriteName);
		if (m_pCurrentSprite == nullptr)
		{
			m_pCurrentSprite = m_pSprites[0];
		}
	}
}

void GameObject::SetDimsFromSprite()
{
	m_Shape.width = m_pCurrentSprite->GetFrameDimensions().x;
	m_Shape.height = m_pCurrentSprite->GetFrameDimensions().y;
}

Sprite* GameObject::GetSpritePtr(const std::string& spriteName) const
{
	for (Sprite* pSprite : m_pSprites)
	{
		if (pSprite->GetName() == spriteName)
		{
			return pSprite;
		}
	}
	std::cout << "SPRITE POINTER '" << spriteName << "' NOT FOUND\n";
	return nullptr;
}

bool GameObject::IsOnScreen() const
{
	const float viewExtension{ 0.f };
	Rectf visibleArea{ ObjectManager::GetObjectMngr()->GetVisibleArea()};
	const bool isInsideXScreenBounds
	(
		visibleArea.left < m_Shape.left + m_Shape.width + viewExtension
		&& m_Shape.left < visibleArea.left + visibleArea.width + viewExtension
	);
	const bool  isInsideYScreenBounds
	(
		visibleArea.bottom < m_Shape.bottom + m_Shape.height + viewExtension
		&& m_Shape.bottom < visibleArea.bottom + visibleArea.height + viewExtension
	);

	return (isInsideXScreenBounds && isInsideYScreenBounds);
}

float GameObject::GetRelativeDirection(GameObject* other) const
{
	return (m_Shape.left - other->m_Shape.left) > 0.f ? 1.f : -1.f;
}
