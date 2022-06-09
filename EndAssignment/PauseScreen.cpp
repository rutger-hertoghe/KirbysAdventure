#include "pch.h"
#include "PauseScreen.h"
#include "Texture.h"
#include "Kirby.h"
#include "PowerUp.h"
#include "SoundStream.h"
#include "SoundEffect.h"

using Type = PowerUp::PowerUpType;

PauseScreen::PauseScreen(const Window& window, Kirby* pKirby)
	: m_pKirby(pKirby)
	, m_WindowRect(0.f, 0.f, window.width, window.height)
	, m_PanelRect(24.f, 88.f, 0.f, 0.f)
	, m_MusicSliderDotRect(0.f, 64.f, 0.f, 0.f)
	, m_MaxSliderDots{8}
	, m_MaxVolume{128}
	, m_IsMusicMuted{false}
{
	m_MusicDots = SoundStream::GetVolume() / (m_MaxVolume / m_MaxSliderDots);

	InitializeTextures();
	CalculateScalingVars();
}

PauseScreen::~PauseScreen()
{
	delete m_pMainTexture;
	delete m_pSliderTexture;
	delete m_pNoteTexture;

	for (std::pair<std::string, Texture*> texturePair : m_pPowerPanels)
	{
		delete texturePair.second;
	}
}

void PauseScreen::Draw()
{
	// Select panel to draw
	m_pMainTexture->Draw(m_WindowRect);
	DrawPanel();
	DrawMusicSlider();
	DrawMusicToggle();
}

void PauseScreen::CheckMouseInput(const SDL_MouseButtonEvent& e)
{
	Point2f mousePos{ float(e.x), float(e.y) };

	CheckSliderInput(mousePos);
	CheckToggleInput(mousePos);
}

void PauseScreen::CalculatePanelRect()
{
	if (m_pPowerPanels.size() == 0) return; // Check if there is a valid power panel

	m_PanelRect.left *= m_XScalar;
	m_PanelRect.bottom *= m_YScalar;

	m_PanelRect.width = m_pPowerPanels.begin()->second->GetWidth() * m_XScalar;
	m_PanelRect.height = m_pPowerPanels.begin()->second->GetHeight() * m_YScalar;
}

void PauseScreen::CalculateMusicSliderRect()
{
	m_MusicSliderDotRect.bottom *= m_YScalar;
	
	m_MusicSliderDotRect.width = m_pSliderTexture->GetWidth() / 2 * m_XScalar;
	m_MusicSliderDotRect.height = m_pSliderTexture->GetHeight() * m_YScalar;

	m_MusicSliderDotRect.left = (m_WindowRect.width  - m_MusicSliderDotRect.width * m_MaxSliderDots) / 2;
}

void PauseScreen::AddPowerPanelTexture(const std::string& filePath)
{
	const size_t startPosName{ filePath.find('_') + 1};
	const size_t lengthName{ filePath.find('.') - startPosName};
	const std::string name{ filePath.substr(startPosName, lengthName) };
	
	m_pPowerPanels.insert(std::make_pair<const std::string&, Texture*>(name, new Texture(filePath)));
}

void PauseScreen::DrawPanel()
{
	if (m_pKirby->HasPowerUp())
	{
		switch (m_pKirby->GetPowerUp()->GetType())
		{
		case Type::fire:
			m_pPowerPanels["fire"]->Draw(m_PanelRect);
			break;
		case Type::spark:
			m_pPowerPanels["spark"]->Draw(m_PanelRect);
			break;
		case Type::stone:
			m_pPowerPanels["stone"]->Draw(m_PanelRect);
			break;
		}
	}
	else if (m_pPowerPanels.size() != 0)
	{
		m_pPowerPanels["normal"]->Draw(m_PanelRect);
	}
}

void PauseScreen::DrawMusicSlider()
{
	Rectf srcRect{ 0.f, m_pSliderTexture->GetHeight(), m_pSliderTexture->GetWidth() / 2, m_pSliderTexture->GetHeight() };
	Rectf dstRect{ m_MusicSliderDotRect.left, m_MusicSliderDotRect.bottom, m_MusicSliderDotRect.width, m_MusicSliderDotRect.height };
	for (int i{}; i < m_MaxSliderDots; ++i)
	{
		if (i < m_MusicDots)
		{
			srcRect.left = m_pSliderTexture->GetWidth() / 2;
		}
		else
		{
			srcRect.left = 0.f;
		}
		m_pSliderTexture->Draw(dstRect, srcRect);
		dstRect.left += m_MusicSliderDotRect.width;
	}
}

void PauseScreen::CalculateScalars()
{
	m_XScalar = m_WindowRect.width / m_pMainTexture->GetWidth();
	m_YScalar = m_WindowRect.height / m_pMainTexture->GetHeight();
}

void PauseScreen::CalculateScalingVars()
{
	CalculateScalars();
	CalculatePanelRect();
	CalculateMusicSliderRect();
	CalculateMusicToggleRect(); // Always after music slider because it requires data of the music slider rect
}

void PauseScreen::InitializeTextures()
{
	m_pMainTexture = new Texture("resources/ui/pausescreen.png");
	m_pSliderTexture = new Texture("resources/sprites/hud_health.png");
	m_pNoteTexture = new Texture("resources/ui/note_toggle.png");

	AddPowerPanelTexture("resources/ui/pause_normal.png");
	AddPowerPanelTexture("resources/ui/pause_fire.png");
	AddPowerPanelTexture("resources/ui/pause_spark.png");
	AddPowerPanelTexture("resources/ui/pause_stone.png");
}

void PauseScreen::DrawMusicToggle()
{
	Rectf srcRect{ 0.f, m_pNoteTexture->GetHeight(), m_pNoteTexture->GetWidth() / 2, m_pNoteTexture->GetHeight()};
	if (m_IsMusicMuted)
	{
		srcRect.left += srcRect.width;
	}
	m_pNoteTexture->Draw(m_MusicToggleRect, srcRect);
}

void PauseScreen::CalculateMusicToggleRect()
{
	m_MusicToggleRect.width = m_pNoteTexture->GetWidth() / 2 * m_XScalar;
	m_MusicToggleRect.height = m_pNoteTexture->GetHeight() * m_YScalar;
	m_MusicToggleRect.left = m_MusicSliderDotRect.left - m_MusicToggleRect.width;
	m_MusicToggleRect.bottom = m_MusicSliderDotRect.bottom;
}

void PauseScreen::ResumeMusic()
{
	SoundStream::SetVolume(m_MusicDots * (m_MaxVolume / m_MaxSliderDots));
	SoundStream::Resume();
	m_IsMusicMuted = false;
}

void PauseScreen::CheckSliderInput(const Point2f& mousePos)
{
	for (int i{}; i < m_MaxSliderDots; ++i)
	{
		Rectf sliderRect{ m_MusicSliderDotRect };
		sliderRect.left += sliderRect.width * i;
		if (utils::IsPointInRect(mousePos, sliderRect))
		{
			const int sliderValue{ i + 1 }; // i goes from 0-7 instead of 1-8, so correct by 1 for true slider value
			m_MusicDots = sliderValue;
			ResumeMusic();
			return;
		}
	}
}

void PauseScreen::CheckToggleInput(const Point2f& mousePos)
{
	if (utils::IsPointInRect(mousePos, m_MusicToggleRect))
	{
		if (m_IsMusicMuted)
		{
			ResumeMusic();
		}
		else
		{
			SoundStream::Pause();
			m_IsMusicMuted = true;
		}
	}
}
