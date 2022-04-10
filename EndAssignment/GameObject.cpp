#include "pch.h"
#include "Sprite.h"
#include "PowerUp.h"
#include "FirePower.h"
#include "GameObject.h"

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

Rectf GameObject::GetShape() const
{
	return m_Shape;
}

float GameObject::GetDirection() const
{
	return m_XDirection;
}

