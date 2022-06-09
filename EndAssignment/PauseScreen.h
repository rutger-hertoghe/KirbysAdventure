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

	void Draw();

	void CheckMouseInput(const SDL_MouseButtonEvent& e);
private:
	const int m_MaxSliderDots;
	const int m_MaxVolume;
	
	int m_MusicDots;

	float m_XScalar;
	float m_YScalar;

	bool m_IsMusicMuted;

	Rectf m_WindowRect;
	Rectf m_PanelRect;
	Rectf m_MusicSliderDotRect;
	Rectf m_MusicToggleRect;

	Texture* m_pMainTexture;
	Texture* m_pSliderTexture;
	Texture* m_pNoteTexture;
	std::unordered_map<std::string, Texture*> m_pPowerPanels;
	Kirby* m_pKirby;

	void InitializeTextures();
	void AddPowerPanelTexture(const std::string& filePath);

	void CalculateScalingVars();
	void CalculateScalars();
	void CalculatePanelRect();
	void CalculateMusicSliderRect();
	void CalculateMusicToggleRect();
	
	void DrawPanel();
	void DrawMusicSlider();
	void DrawMusicToggle();

	void ResumeMusic();

	void CheckSliderInput(const Point2f& mousePos);
	void CheckToggleInput(const Point2f& mousePos);
};

