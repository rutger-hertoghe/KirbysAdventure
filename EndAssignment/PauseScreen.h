#pragma once
#include "structs.h"
#include <unordered_map>
class Texture;
class Kirby;

class PauseScreen final
{
public:
	explicit PauseScreen(const Window& window, Kirby* pKirby);
	PauseScreen& operator=(const PauseScreen& other) = delete;
	PauseScreen(const PauseScreen& other) = delete;
	PauseScreen& operator=(PauseScreen&& other) = delete;
	PauseScreen(PauseScreen&& other) = delete;
	~PauseScreen();

	void Draw(); // Not const here, because DrawPanel function doesn't work as a const

	void CheckMouseInput(const SDL_MouseButtonEvent& e);

private:
	const int m_MaxSliderDots;
	const int m_MaxVolume;
	
	int m_MusicDots;
	int m_FXDots;

	float m_XScalar;
	float m_YScalar;

	bool m_IsMusicMuted;
	bool m_IsFXMuted;

	Rectf m_WindowRect;
	Rectf m_PanelRect;
	Rectf m_MusicSliderDotRect;
	Rectf m_MusicToggleRect;

	Rectf m_FXSliderDotRect;
	Rectf m_FXToggleRect;

	Texture* m_pMainTexture;
	Texture* m_pSliderTexture;
	Texture* m_pNoteTexture;
	Texture* m_pFXTexture;

	std::unordered_map<std::string, Texture*> m_pPowerPanels;
	Kirby* m_pKirby;

	void InitializeTextures();
	void AddPowerPanelTexture(const std::string& filePath);

	void CalculateScalingVars();
	void CalculateScalars();
	void CalculatePanelRect();

	void CalculateToggleRect(Rectf& toggleRect, Rectf& relatedSliderRect, Texture* texture);
	
	void CalculateSliderRect(Rectf& sliderRect);
	
	void DrawPanel();
	void DrawSlider(const Rectf& sliderRect, int dots) const;
	void DrawToggle(const Rectf& toggleRect, Texture* toggleTexture, bool muteBool);

	void ResumeMusic();
	void ResumeFX();

	void CheckSliderInput(const Point2f& mousePos);
	void CheckToggleInput(const Point2f& mousePos);
};

