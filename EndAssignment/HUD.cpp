#include "pch.h"
#include "HUD.h"
#include "Texture.h"
#include "Kirby.h"
#include "Sprite.h"
#include "PowerUp.h"

HUD::HUD()
	: m_PosDancingKirby{ 187.f, 32.f }
{
	m_pMainTexture = new Texture{ "resources/UI/HUD_Normal.png" };
	m_pPowerTextures = new Texture{ "resources/UI/HUD_Powers.png"};
	m_Dimensions.x = m_pMainTexture->GetWidth();
	m_Dimensions.y = m_pMainTexture->GetHeight();

	int nrFrames{ 1 };
	float animationSpeed{ 0.f };
	m_pHealthLost = new Sprite{ nrFrames, animationSpeed, "resources/ui/hud_health_lost.png" };

	nrFrames = 2;
	animationSpeed = 0.5f;
	m_pHealth = new Sprite{ nrFrames, animationSpeed, "resources/ui/hud_health.png" };

	nrFrames = 4;
	animationSpeed = 1.0f;
	m_pDancingKirby = new Sprite{ nrFrames, animationSpeed, "resources/UI/HUD_Dancing_Kirby.png" };
}

HUD::~HUD()
{
	delete m_pMainTexture;
	m_pMainTexture = nullptr;

	delete m_pPowerTextures;
	m_pPowerTextures = nullptr;

	delete m_pDancingKirby;
	m_pDancingKirby = nullptr;

	delete m_pHealthLost;
	m_pHealthLost = nullptr;

	delete m_pHealth;
	m_pHealth = nullptr;
}

void HUD::Update(float elapsedSec)
{
	m_pDancingKirby->Update(elapsedSec);
	m_pHealth->Update(elapsedSec);
}

void HUD::Draw(Kirby* kirbyPtr) const
{
	m_pMainTexture->Draw();
	m_pDancingKirby->Draw(m_PosDancingKirby);
	DrawLives(kirbyPtr);
	DrawPowerPanel(kirbyPtr);
}

void HUD::DrawLives(Kirby* kirbyPtr) const
{
	const int maxHealth{ 6 };
	const int health{ kirbyPtr->GetHealth()};
	Point2f position{ 72.f, 37.f };
	for (int idx{}; idx < maxHealth; ++idx)
	{
		if (idx < health)
		{
			m_pHealth->Draw(position);
		}
		else m_pHealthLost->Draw(position);
		position.x += m_pHealth->GetFrameDimensions().x;
	}
}

void HUD::DrawPowerPanel(Kirby* kirbyPtr) const
{
	const float panelWidth{ 32.f };
	const float panelHeight{ 40.f };
	const int panelsPerRow{ int(m_pPowerTextures->GetWidth() / panelWidth) };

	const Point2f hudLocation{ 144.f, 16.f };
	const Rectf powerUpPanel{ hudLocation.x, hudLocation.y, panelWidth, panelHeight };

	PowerUp* power{ kirbyPtr->GetPowerUp() };

	if (power)
	{
		const int panel{ power->GetPowerPanelSlot() };
		const int panelCol{ panel % panelsPerRow};
		const int panelRow{ panel / panelsPerRow + 1};
		Rectf srcRect{ 
			panel * panelWidth, 
			m_pPowerTextures->GetHeight() - panelRow * panelHeight, 
			panelWidth, 
			panelHeight 
		};
		m_pPowerTextures->Draw(powerUpPanel, srcRect);
	}
	else
	{
		Rectf srcRect{ 0.f, 40.f, 32.f, 40.f };
		m_pPowerTextures->Draw(powerUpPanel, srcRect);
	}
	
}

float HUD::GetHeight() const
{
	return m_Dimensions.y;
}

float HUD::GetWidth() const
{
	return m_Dimensions.x;
}


