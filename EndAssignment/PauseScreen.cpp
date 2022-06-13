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
	, m_FXSliderDotRect(0.f, 44.f, 0.f, 0.f)
	, m_MaxSliderDots{8}
	, m_MaxVolume{128}
	, m_IsMusicMuted{false}
	, m_IsFXMuted{false}
{
	const int startVolume{64};
	SoundStream::SetVolume(startVolume);
	m_MusicDots = SoundStream::GetVolume() / (m_MaxVolume / m_MaxSliderDots);
	m_FXDots = m_MaxVolume;

	InitializeTextures();
	CalculateScalingVars();
}

PauseScreen::~PauseScreen()
{
	delete m_pMainTexture;
	delete m_pSliderTexture;
	delete m_pNoteTexture;
	delete m_pFXTexture;

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
	DrawSlider(m_MusicSliderDotRect, m_MusicDots);
	DrawSlider(m_FXSliderDotRect, m_FXDots);
	DrawToggle(m_MusicToggleRect, m_pNoteTexture, m_IsMusicMuted);
	DrawToggle(m_FXToggleRect, m_pFXTexture, m_IsFXMuted);
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


void PauseScreen::CalculateSliderRect(Rectf& sliderRect)
{
	sliderRect.bottom *= m_YScalar;

	sliderRect.width = m_pSliderTexture->GetWidth() / 2 * m_XScalar;
	sliderRect.height = m_pSliderTexture->GetHeight() * m_YScalar;

	sliderRect.left = (m_WindowRect.width - sliderRect.width * m_MaxSliderDots) / 2;
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

void PauseScreen::DrawSlider(const Rectf& sliderRect, int dots) const
{
	Rectf srcRect{ 0.f, m_pSliderTexture->GetHeight(), m_pSliderTexture->GetWidth() / 2, m_pSliderTexture->GetHeight() };
	Rectf dstRect{ sliderRect.left, sliderRect.bottom, sliderRect.width, sliderRect.height };
	for (int i{}; i < m_MaxSliderDots; ++i)
	{
		if (i < dots)
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

	CalculateSliderRect(m_MusicSliderDotRect);
	CalculateSliderRect(m_FXSliderDotRect);

	// Toggles always after sliders because it requires data of the music slider rect
	CalculateToggleRect(m_FXToggleRect, m_FXSliderDotRect, m_pFXTexture);
	CalculateToggleRect(m_MusicToggleRect, m_MusicSliderDotRect, m_pNoteTexture);
}

void PauseScreen::InitializeTextures()
{
	m_pMainTexture = new Texture("resources/ui/pausescreen.png");
	m_pSliderTexture = new Texture("resources/sprites/hud_health.png");
	m_pNoteTexture = new Texture("resources/ui/note_toggle.png");
	m_pFXTexture = new Texture("resources/ui/fx_toggle.png");

	AddPowerPanelTexture("resources/ui/pause_normal.png");
	AddPowerPanelTexture("resources/ui/pause_fire.png");
	AddPowerPanelTexture("resources/ui/pause_spark.png");
	AddPowerPanelTexture("resources/ui/pause_stone.png");
}

void PauseScreen::DrawToggle(const Rectf& toggleRect, Texture* toggleTexture, bool muteBool)
{
	Rectf srcRect{ 0.f, toggleTexture->GetHeight(), toggleTexture->GetWidth() / 2, toggleTexture->GetHeight() };
	if (muteBool)
	{
		srcRect.left += srcRect.width;
	}
	toggleTexture->Draw(toggleRect, srcRect);
}

void PauseScreen::ResumeFX()
{
	SoundFXManager::SetVolume(m_FXDots * (m_MaxVolume / m_MaxSliderDots));
	SoundFXManager::Play("jump");
	m_IsFXMuted = false;
}

void PauseScreen::CalculateToggleRect(Rectf& toggleRect, Rectf& relatedSliderRect, Texture* texture)
{
	toggleRect.width = texture->GetWidth() / 2 * m_XScalar;
	toggleRect.height = texture->GetHeight() * m_YScalar;
	toggleRect.left = relatedSliderRect.left - toggleRect.width;
	toggleRect.bottom = relatedSliderRect.bottom;
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
		// MUSIC SLIDER
		Rectf sliderRect{ m_MusicSliderDotRect };
		sliderRect.left += sliderRect.width * i;
		if (utils::IsPointInRect(mousePos, sliderRect))
		{
			const int sliderValue{ i + 1 }; // i goes from 0-7 instead of 1-8, so correct by 1 for true slider value
			m_MusicDots = sliderValue;
			ResumeMusic();
			return;
		}
		// FX SLIDER
		sliderRect = m_FXSliderDotRect;
		sliderRect.left += sliderRect.width * i;
		if (utils::IsPointInRect(mousePos, sliderRect))
		{
			const int sliderValue{ i + 1 }; // i goes from 0-7 instead of 1-8, so correct by 1 for true slider value
			m_FXDots = sliderValue;
			ResumeFX();
			return;
		}
	}
}

void PauseScreen::CheckToggleInput(const Point2f& mousePos)
{
	// MUSIC TOGGLE
	if (utils::IsPointInRect(mousePos, m_MusicToggleRect))
	{
		if (m_IsMusicMuted)
		{
			ResumeMusic();
		}
		else
		{
			SoundStream::Pause();
			SoundStream::SetVolume(0);
			m_IsMusicMuted = true;
		}
	}
	// FX TOGGLE
	if (utils::IsPointInRect(mousePos, m_FXToggleRect))
	{
		if (m_IsFXMuted)
		{
			ResumeFX();
		}
		else
		{
			SoundFXManager::SetVolume(0);
			m_IsFXMuted = true;
		}
	}
}
