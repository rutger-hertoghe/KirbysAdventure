#pragma once

class Texture;

class Sprite
{
public:
	Sprite(int nrFrames, float loopTime, const std::string& texturePath, int rows = 1);
	~Sprite();

	void Draw(const Point2f& location, int frame) const;
	void Draw(const Rectf& dstRect, int frame) const;

	std::string GetName() const;
	Point2f GetFrameDimensions() const;
	float GetLoopTime() const;
	int GetNrOfFrames() const;

private:
	const int m_NrFrames;
	const float m_LoopTime;
	const int m_Rows;

	int m_FramesPerRow;
	float m_FrameTime;

	std::string m_Name;

	Point2f m_FrameDimensions;
	Texture* m_pTexture;

	void SetDimensions();

	std::string GenerateSpriteName(const std::string& texturePath);

};

