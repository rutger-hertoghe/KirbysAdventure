#include "pch.h"
#include "HUD.h"
#include "Texture.h"
#include "Kirby.h"
#include "Sprite.h"
#include "PowerUp.h"
#include "HudElement.h"

HUD::HUD(Kirby* pKirby)
	: m_PosDancingKirby{ 193.5f, 32.f }
	, m_PosScore{ 15.f, 23.f }
	, m_pKirby{ pKirby }
{
	m_pMainTexture = new Texture{ "resources/UI/HUD_Normal.png" };
	m_pPowerTextures = new Texture{ "resources/UI/HUD_Powers.png"};
	m_pNumbers = new Texture{ "resources/UI/HUD_Numbers.png" };
	m_pScore = new Texture{ "resources/UI/HUD_Score.png" };

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
	spritePtr = new Sprite{ nrFrames, animationSpeed, "hud_dancing_kirby" };
	m_pDancingKirby = new HudElement{ spritePtr };

	nrFrames = 4;
	animationSpeed = 1.0f;
}

HUD::~HUD()
{
	delete m_pMainTexture;
	m_pMainTexture = nullptr;

	delete m_pPowerTextures;
	m_pPowerTextures = nullptr;

	delete m_pNumbers;
	m_pNumbers = nullptr;

	delete m_pScore;
	m_pScore = nullptr;

	delete m_pDancingKirby;
	m_pDancingKirby = nullptr;

	delete m_pHealthLost;
	m_pHealthLost = nullptr;

	delete m_pHealth;
	m_pHealth = nullptr;
}

void HUD::Update(float elapsedSec)
{
	const float danceSpeedScalar{float(m_pKirby->GetMaxHealth()) / (m_pKirby->GetHealth() + 1)}; // +1 to make the dancing kirby less hyper active at 1 health
	m_pDancingKirby->Update(elapsedSec * danceSpeedScalar);
	m_pHealth->Update(elapsedSec);

}

void HUD::Draw() const
{
	m_pMainTexture->Draw();
	m_pScore->Draw(m_PosScore);
	m_pDancingKirby->SetPosition(m_PosDancingKirby);
	m_pDancingKirby->Draw();
	DrawPowerPanel();
	DrawHealth();
	DrawLives();
	DrawScore();
}

void HUD::DrawPowerPanel() const
{
	const float panelWidth{ 32.f };
	const float panelHeight{ 40.f };
	const int panelsPerRow{ int(m_pPowerTextures->GetWidth() / panelWidth) };

	const Point2f hudLocation{ 144.f, 16.f };
	const Rectf panelRect{ hudLocation.x, hudLocation.y, panelWidth, panelHeight };

	PowerUp* power{ m_pKirby->GetPowerUp() };
	int panel{ 0 };

	if (m_pKirby->IsDead())
	{
		panel = 11;
	}
	else if (power && m_pKirby->IsBloated() == false)
	{
		panel = power->GetPowerPanelSlot();
	}
	else if (m_pKirby->IsInvulnerable())
	{
		panel = 10;
	}

	const int panelCol{ panel % panelsPerRow };
	const int panelRow{ panel / panelsPerRow + 1 };
	Rectf srcRect
	{
			panel * panelWidth,
			m_pPowerTextures->GetHeight() - panelRow * panelHeight,
			panelWidth,
			panelHeight
	};
	m_pPowerTextures->Draw(panelRect, srcRect);
}

void HUD::DrawHealth() const
{
	const int health{ m_pKirby->GetHealth() };
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

void HUD::DrawLives() const
{
	Point2f location{ 208.f, 32.f };
	DrawNumber(location, m_pKirby->GetLives(), 2);
}

void HUD::DrawScore() const
{
	Point2f location{ 72.f, 24.f };
	DrawNumber(location, m_pKirby->GetScore(), 7);
	
}

void HUD::DrawNumber(Point2f& location, int number, int digits) const
{
	int divisionResult{};
	int score{  };
	Rectf srcRect{};
	srcRect.width = m_pNumbers->GetWidth() / 10;
	srcRect.height = m_pNumbers->GetHeight();
	srcRect.bottom = srcRect.height;

	for (int i{}; i < digits; ++i) 
	{
		int currentExponent{ digits - i - 1}; // -1 because 10 to the amount of digits will result in a number that's on digit longer
		int currentPower{ int(pow(10, currentExponent)) };
		divisionResult = number / currentPower;
		score %= currentPower;

		srcRect.left = divisionResult * srcRect.width;

		m_pNumbers->Draw(location, srcRect);

		location.x += srcRect.width;
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


