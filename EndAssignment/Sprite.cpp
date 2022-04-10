#include "pch.h"
#include "Sprite.h"
#include "Texture.h"

Sprite::Sprite(int nrFrames, float loopTime, const std::string& textureName, int rows, bool invulnerabilitySprite)
	: m_NrFrames{ nrFrames }
	, m_LoopTime{ loopTime }
	, m_Rows{ rows }
	, m_FrameDimensions{ Point2f{} }
	, m_pTexture{ nullptr }
	, m_Name{textureName}
{
	if (invulnerabilitySprite)
	{
		Color4f orange{ 1.f, 1.f, 0.f, 1.f };
		m_pTexture = new Texture(GenerateSpritePath(), orange);
		m_Name.append("_alt");
	}
	else
	{
		m_pTexture = new Texture(GenerateSpritePath());
	}

	m_FrameTime = m_LoopTime / m_NrFrames;
	m_FramesPerRow = m_NrFrames / m_Rows;

	SetDimensions();
}

Sprite::Sprite(Sprite&& other) noexcept
	: m_NrFrames{ other.m_NrFrames }
	, m_LoopTime{ other.m_LoopTime }
	, m_Rows{ other.m_Rows }
	, m_FramesPerRow{ other.m_FramesPerRow }
	, m_FrameTime{ other.m_FrameTime }
	, m_Name{ other.m_Name }
	, m_FrameDimensions{other.m_FrameDimensions}
	, m_pTexture{other.m_pTexture}
{
	other.m_pTexture = nullptr;
}

Sprite& Sprite::operator=(Sprite&& other) noexcept
{
	if (this != &other)
	{
		if (m_pTexture)
		{
			delete m_pTexture;
		}
		m_pTexture = other.m_pTexture;
		other.m_pTexture = nullptr;
	}
	return *this;
}

Sprite::~Sprite()
{
	delete m_pTexture;
}

void Sprite::SetDimensions()
{
	m_FrameDimensions.x = m_pTexture->GetWidth() / m_FramesPerRow;
	m_FrameDimensions.y = m_pTexture->GetHeight() / m_Rows;
}

std::string Sprite::GenerateSpritePath()
{
	std::string path{ "resources/sprites/" };
	path.append(m_Name);
	path.append(".png");
	return path;
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

int Sprite::GetTextureRows() const
{
	return m_Rows;
}
