#include "pch.h"
#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite(int nrFrames, float loopTime, const std::string& texturePath, int rows)
	: m_NrFrames{ nrFrames }
	, m_LoopTime{ loopTime }
	, m_Rows{ rows }
	, m_FrameDimensions{ Point2f{} }
{
	m_pTexture = new Texture(texturePath);

	m_FrameTime = m_LoopTime / m_NrFrames;
	m_FramesPerRow = m_NrFrames / m_Rows;

	m_Name = GenerateSpriteName(texturePath);

	SetDimensions();
}

Sprite::~Sprite()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void Sprite::SetDimensions()
{
	m_FrameDimensions.x = m_pTexture->GetWidth() / m_FramesPerRow;
	m_FrameDimensions.y = m_pTexture->GetHeight() / m_Rows;
}

std::string Sprite::GenerateSpriteName(const std::string& texturePath)
{
	std::string path{ texturePath };
	int slashIndex{ int(path.find_last_of("/")) + 1 };
	int pointIndex{ int(path.find_last_of(".")) };
	int range{ pointIndex - slashIndex };
	std::string name{ path.substr(slashIndex, range) };
	return name;
}

void Sprite::Draw(const Point2f& location, int frame) const
{
	Rectf dstRect{ location.x, location.y, m_FrameDimensions.x, m_FrameDimensions.y };
	Draw(dstRect, frame);
}

void Sprite::Draw(const Rectf& dstRect, int frame) const
{
	int row{ frame / m_FramesPerRow };
	int col{ frame % m_FramesPerRow };

	Rectf srcRect{
		col * m_FrameDimensions.x,
		m_pTexture->GetHeight() - row * m_FrameDimensions.y,
		m_FrameDimensions.x,
		m_FrameDimensions.y
	};

	const bool isDstRectEqual{
		abs(dstRect.width - m_FrameDimensions.x) > 0.001f
		&& abs(dstRect.height - m_FrameDimensions.y) > 0.001f
	};

	Rectf newRect{};
	const Point2f difference{ m_FrameDimensions.x - dstRect.width, m_FrameDimensions.y - dstRect.height };
	const float xOffset{ -difference.x / 2 };
	newRect = Rectf{ dstRect.left + xOffset, dstRect.bottom, m_FrameDimensions.x, m_FrameDimensions.y };
	
	m_pTexture->Draw(newRect, srcRect);
}

std::string Sprite::GetName() const
{
	return m_Name;
}

Point2f Sprite::GetFrameDimensions() const
{
	return m_FrameDimensions;
}

float Sprite::GetLoopTime() const
{
	return m_LoopTime;
}

int Sprite::GetNrOfFrames() const
{
	return m_NrFrames;
}
