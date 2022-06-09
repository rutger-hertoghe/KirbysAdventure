#pragma once
#include "TextureManager.h" // Included here so it doesn't have to be added to every class using Sprite

class Texture;

class Sprite
{
public:
	Sprite(int nrFrames, float loopTime, const std::string& textureName, int rows = 1, bool invulnerabilitySprite = false);
	Sprite(int nrFrames, float loopTime, const std::string& textureName, Texture* pTexture, int rows = 1);
	Sprite(const Sprite& other) = delete;
	Sprite& operator=(const Sprite& other) = delete;
	Sprite(Sprite&& other) noexcept;
	Sprite& operator=(Sprite&& other) noexcept;
	~Sprite();
	

	void Draw(const Point2f& location, int frame) const;
	void Draw(const Rectf& dstRect, int frame) const;

	std::string GetName() const;
	Point2f GetFrameDimensions() const;
	float GetLoopTime() const;
	int GetNrOfFrames() const;
	int GetTextureRows() const;

private:
	const int m_NrFrames;
	const float m_LoopTime;
	const int m_Rows;

	// TODO: remove related functionality of bool when all sprites have been adapted to work with the new texturemanager system
	bool m_LinkedToTextureManager;
	int m_FramesPerRow;
	float m_FrameTime;

	std::string m_Name;

	Point2f m_FrameDimensions;
	Texture* m_pTexture;

	void SetDimensions();

	std::string GenerateSpritePath();
};

