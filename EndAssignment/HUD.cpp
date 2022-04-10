#include "pch.h"
#include "HUD.h"
#include "Texture.h"
#include "Kirby.h"
#include "Sprite.h"
#include "PowerUp.h"
#include "HudElement.h"

HUD::HUD()
	: m_PosDancingKirby{ 187.f, 32.f }
	, m_pDancingKirby{nullptr}
{
	m_pMainTexture = new Texture{ "resources/UI/HUD_Normal.png" };
	m_pPowerTextures = new Texture{ "resources/UI/HUD_Powers.png"};
	m_Dimensions.x = m_pMainTexture->GetWidth();
	m_Dimensions.y = m_pMainTexture->GetHeight();

	int nrFrames{ 1 };
	float animationSpeed{ 0.f };
	Sprite* spritePtr{ new Sprite{ nrFrames, animationSpeed, "hud_health_lost" } };
	m_pHealthLost = new HudElement{ spritePtr };

	nrFrames = 2;
	animationSpeed = 0.5f;
	spritePtr = new Sprite{ nrFrames, animationSpeed, "hud_health" };
	m_pHealth = new HudElement{ spritePtr };

	nrFrames = 4;
	animationSpeed = 1.0f;
}

HUD::~HUD()
{
	delete m_pMainTexture;
	m_pMainTexture = nullptr;

	delete m_pPowerTextures;
	m_pPowerTextures = nullptr;

	// delete m_pDancingKirby;
	// m_pDancingKirby = nullptr;

	delete m_pHealthLost;
	m_pHealthLost = nullptr;

	delete m_pHealth;
	m_pHealth = nullptr;
}

void HUD::Update(float elapsedSec)
{
	// m_pDancingKirby->Update(elapsedSec);
	m_pHealth->Update(elapsedSec);

}

void HUD::Draw(Kirby* kirbyPtr) const
{
	m_pMainTexture->Draw();
	// m_pDancingKirby->Draw();
	DrawPowerPanel(kirbyPtr);
	DrawHealth(kirbyPtr);
}

void HUD::DrawPowerPanel(Kirby* pKirby) const
{
	const float panelWidth{ 32.f };
	const float panelHeight{ 40.f };
	const int panelsPerRow{ int(m_pPowerTextures->GetWidth() / panelWidth) };

	const Point2f hudLocation{ 144.f, 16.f };
	const Rectf panelRect{ hudLocation.x, hudLocation.y, panelWidth, panelHeight };

	PowerUp* power{ pKirby->GetPowerUp() };
	int panel{ 0 };

	if (power && pKirby->IsBloated() == false)
	{
		panel = power->GetPowerPanelSlot();
	}
	else if (pKirby->IsInvulnerable())
	{
		panel = 10;
	}
	const int panelCol{ panel % panelsPerRow };
	const int panelRow{ panel / panelsPerRow + 1 };
	Rectf srcRect{
			panel * panelWidth,
			m_pPowerTextures->GetHeight() - panelRow * panelHeight,
			panelWidth,
			panelHeight
	};
	m_pPowerTextures->Draw(panelRect, srcRect);
}

void HUD::DrawHealth(Kirby* kirbyPtr) const
{
	const int health{ kirbyPtr->GetHealth() };
	const int maxHealth{ 6 };
	Point2f location{76.f, 37.f};
	for (int currentHealthSprite{}; currentHealthSprite < maxHealth; ++currentHealthSprite)
	{
		if (currentHealthSprite < health)
		{
			m_pHealth->SetPosition(location);
			m_pHealth->Draw();
		}
		else
		{
			m_pHealthLost->SetPosition(location);
			m_pHealthLost->Draw();
		}
		location.x += m_pHealth->GetWidth();
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


