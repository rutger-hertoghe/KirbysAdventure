#pragma once
class Texture;

class Sprite
{
public:
	Sprite(int nrFrames, float loopTime, const std::string& texturePath, int rows = 1);
	~Sprite();

	void Draw(const Point2f& location) const;
	void Draw(const Rectf& dstRect) const;

	void Update(float elapsedSec);
	void ResetLoop();

	bool HasLooped() const;

	std::string GetName() const;
	Point2f GetFrameDimensions() const;

private:
	const int m_NrFrames;
	const float m_LoopTime;
	const int m_Rows;

	int m_FramesPerRow;
	int m_CurrentFrame;
	float m_FrameTime;
	float m_AccumulatedTime;
	float m_LoopProgressTime;

	bool m_HasLooped;

	std::string m_Name;

	Point2f m_FrameDimensions;
	Texture* m_pTexture;

	void SetDimensions();

	std::string GenerateSpriteName(const std::string& texturePath);

};

