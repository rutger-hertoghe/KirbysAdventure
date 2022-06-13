#include "pch.h"
#include "KirbyStateHandler.h"
#include "PowerUp.h"
#include "SoundEffect.h"

KirbyStateHandler::KirbyStateHandler(Kirby* pKirby)
	: m_pKirby(pKirby)
	, m_Conditions{}
{
}

void KirbyStateHandler::HandleState(Action actionState, Macro macroState)
{
	if (m_pKirby == nullptr) return;

	CheckConditions(actionState, macroState);

	switch (actionState)
	{
	// BASIC SET
	case Action::idle:
		HandleIdle(actionState, macroState);
		break;
	case Action::walking:
		HandleWalking(actionState, macroState);
		break;
	case Action::jumping:
		HandleJumping(macroState);
		break;
	case Action::flipping:
		HandleFlipping(macroState);
		break;
	case Action::falling:
		HandleFalling(macroState);
		break;
	case Action::ducking:
		HandleDucking(macroState);
		break;
	case Action::sliding:
		HandleSliding();
		break;
	// INFLATING SET
	case Action::inflating:
		HandleInflating();
		break;
	case Action::inflated:
		HandleInflated(macroState);
		break;
	case Action::flapping:
		HandleFlapping(macroState);
		break;
	case Action::deflating:
		HandleDeflating();
		break;
	// INHALING/BlOAT SET
	case Action::start_inhaling:
		HandleStartInhaling();
		break;
	case Action::inhaling:
		HandleInhaling(macroState);
		break;
	case Action::bloat:
		HandleBloat();
		break;
	case Action::exhaling:
		HandleExhaling();
		break;
	case Action::bloated_idle:
		HandleBloatedIdle(actionState,macroState);
		break;
	case Action::bloated_walking:
		HandleBloatedWalking(actionState, macroState);
		break;
	case Action::swallowing:
		HandleSwallowing();
		break;
	case Action::spitting:
		HandleSpitting();
		break;
		// POWER SET
	case Action::power_start:
		HandlePowerStart();
		break;
	case Action::power_continuous:
		HandlePowerContinuous();
		break;
	case Action::power_end:
		HandlePowerEnd();
		break;
	case Action::hurt:
		HandleHurt(macroState);
		break;
	}
}

void KirbyStateHandler::CheckConditions(Action actionState, Macro macroState)
{
	Vector2f velocity(m_pKirby->GetVelocity());
	const float yVelocityTreshold{ 100.f };

	m_Conditions.isOnGround = m_pKirby->IsOnGround();
	m_Conditions.isMoving = abs(velocity.x) >= 1.f;
	m_Conditions.isIdle = !m_Conditions.isMoving;
	m_Conditions.isGoingUp = velocity.y > 1.f;
	m_Conditions.isGoingDown = velocity.y < -1.f;
	m_Conditions.isJumping = m_Conditions.isGoingUp && !(actionState == Kirby::ActionState::inflating || actionState == Kirby::ActionState::inflated);
	m_Conditions.isMidAir = abs(velocity.y) < yVelocityTreshold && !m_pKirby->IsOnGround();
	m_Conditions.isFalling = !m_pKirby->IsOnGround() && actionState != Kirby::ActionState::jumping;
	m_Conditions.isLoopDone = m_pKirby->HasLooped();
	m_Conditions.isUsingPower = m_pKirby->HasPowerUp() ? m_pKirby->GetPowerUp()->IsActive() : false;

	// State bools (for clarity & consistency further on, not the most optimized way of working here)
	m_Conditions.isInflated = (macroState == Kirby::MacroState::inflated);
	m_Conditions.isDucking = (macroState == Kirby::MacroState::ducking);
	m_Conditions.isInhaling = (macroState == Kirby::MacroState::inhalation);
	m_Conditions.isBloated = (macroState == Kirby::MacroState::bloated);
	m_Conditions.isSliding = (macroState == Kirby::MacroState::sliding);
}

void KirbyStateHandler::HandleIdle(Action actionState, Macro macroState)
{
	if		(m_Conditions.isInflated)	m_pKirby->SetState(Action::inflating);
	else if (m_Conditions.isUsingPower)	m_pKirby->SetPowerState();
	else if (m_Conditions.isInhaling)	m_pKirby->SetState(Action::start_inhaling);
	else if (m_Conditions.isJumping)	m_pKirby->SetState(Action::jumping);
	else if (m_Conditions.isFalling)	m_pKirby->SetState(Action::falling);
	else if (m_Conditions.isDucking)	m_pKirby->SetState(Action::ducking);
	else if (m_Conditions.isMoving)		m_pKirby->SetState(Action::walking);
}

void KirbyStateHandler::HandleWalking(Action actionState, Macro macroState)
{
	if		(m_Conditions.isInflated)	m_pKirby->SetState(Action::inflating);
	else if (m_Conditions.isUsingPower)	m_pKirby->SetPowerState();
	else if (m_Conditions.isInhaling)	m_pKirby->SetState(Action::start_inhaling);
	else if (m_Conditions.isJumping)	m_pKirby->SetState(Action::jumping);
	else if (m_Conditions.isFalling)	m_pKirby->SetState(Action::falling);
	else if (m_Conditions.isDucking)	m_pKirby->SetState(Action::ducking);
	else if (m_Conditions.isIdle)		m_pKirby->SetState(Action::idle);
}

void KirbyStateHandler::HandleJumping(Macro macroState)
{
	if		(m_Conditions.isInflated)	m_pKirby->SetState(Action::inflating);
	else if (m_Conditions.isUsingPower)	m_pKirby->SetPowerState();
	else if (m_Conditions.isInhaling)	m_pKirby->SetState(Action::inhaling);
	else if (m_Conditions.isOnGround)	m_pKirby->SetState(Action::idle);
	else if (m_Conditions.isMidAir)		m_pKirby->SetState(Action::flipping);
}

void KirbyStateHandler::HandleFlipping(Macro macroState)
{
	if		(m_Conditions.isInflated)	m_pKirby->SetState(Action::inflating);
	else if (m_Conditions.isUsingPower)	m_pKirby->SetPowerState();
	else if (m_Conditions.isInhaling)	m_pKirby->SetState(Action::inhaling);
	else if (m_Conditions.isOnGround)	m_pKirby->SetState(Action::idle);
	else if (m_Conditions.isLoopDone)	m_pKirby->SetState(Action::falling);
}

void KirbyStateHandler::HandleFalling(Macro macroState)
{
	if		(m_Conditions.isInflated)	m_pKirby->SetState(Action::inflating);
	else if (m_Conditions.isUsingPower)	m_pKirby->SetPowerState();
	else if (m_Conditions.isInhaling)	m_pKirby->SetState(Action::inhaling);
	else if (m_Conditions.isOnGround)	m_pKirby->SetState(Action::idle);
}

void KirbyStateHandler::HandleDucking(Macro macroState)
{
	if		(m_Conditions.isSliding)	m_pKirby->SetState(Action::sliding);
	else if (!m_Conditions.isDucking)	m_pKirby->SetState(Action::idle);
}

void KirbyStateHandler::HandleSliding()
{
	if (!m_Conditions.isMoving)
	{
		m_pKirby->SetState(Action::ducking);
		m_pKirby->SetMacroState(Macro::ducking);
	}
}

void KirbyStateHandler::HandleInflating()
{
	if (m_Conditions.isLoopDone) m_pKirby->SetState(Action::inflated);
}

void KirbyStateHandler::HandleInflated(Macro macroState)
{
	if		(!m_Conditions.isInflated)	m_pKirby->SetState(Action::deflating);
	else if (m_Conditions.isGoingUp)	m_pKirby->SetState(Action::flapping);
}

void KirbyStateHandler::HandleFlapping(Macro macroState)
{
	if		(!m_Conditions.isInflated)	m_pKirby->SetState(Action::deflating);
	else if (m_Conditions.isGoingDown)	m_pKirby->SetState(Action::inflated);
}

void KirbyStateHandler::HandleDeflating()
{
	if (m_Conditions.isLoopDone)	m_pKirby->SetState(Action::idle);
}

void KirbyStateHandler::HandleStartInhaling()
{
	if (m_Conditions.isLoopDone)	m_pKirby->SetState(Action::inhaling);
}

void KirbyStateHandler::HandleInhaling(Macro macroState)
{
	if		(m_Conditions.isBloated)	m_pKirby->SetState(Action::bloat);
	else if (!m_Conditions.isInhaling)	m_pKirby->SetState(Action::exhaling);
}

void KirbyStateHandler::HandleBloat()
{
	if (m_Conditions.isLoopDone)	m_pKirby->SetState(Action::bloated_idle);
}

void KirbyStateHandler::HandleExhaling()
{
	if (m_Conditions.isLoopDone)	m_pKirby->SetState(Action::idle);
}

void KirbyStateHandler::HandleBloatedIdle(Action actionState, Macro macroState)
{
	if (!m_Conditions.isBloated)
	{
		m_pKirby->SetState(Action::swallowing);
	}
	else if (m_Conditions.isMoving || m_Conditions.isJumping || m_Conditions.isFalling)
	{
		m_pKirby->SetState(Action::bloated_walking);
	}
}

void KirbyStateHandler::HandleBloatedWalking(Action actionState, Macro macroState)
{
	if (!m_Conditions.isBloated)
	{
		m_pKirby->SetState(Action::swallowing);
	}
	else if (m_Conditions.isIdle && !m_Conditions.isJumping && !m_Conditions.isFalling)
	{
		m_pKirby->SetState(Action::bloated_idle);
	}
}

void KirbyStateHandler::HandleSwallowing()
{
	if (m_Conditions.isLoopDone) m_pKirby->SetState(Action::idle);
}

void KirbyStateHandler::HandleSpitting()
{
	if (m_Conditions.isLoopDone) m_pKirby->SetState(Action::idle);
}

void KirbyStateHandler::HandlePowerStart()
{
	if (!m_Conditions.isUsingPower && m_pKirby->GetPowerUp()->HasEnd())
	{
		m_pKirby->SetState(Action::power_end);
	}
	else if (!m_Conditions.isUsingPower)
	{
		m_pKirby->SetState(Action::idle);
	}
	else if (m_Conditions.isLoopDone && m_pKirby->GetPowerUp()->HasContinuous())
	{
		m_pKirby->SetState(Action::power_continuous);
	}
}
void KirbyStateHandler::HandlePowerContinuous()
{
	if (!m_Conditions.isUsingPower && m_pKirby->GetPowerUp()->HasEnd())
	{
		m_pKirby->SetState(Action::power_end);
	}
	else if (!m_Conditions.isUsingPower)
	{
		m_pKirby->SetState(Action::idle);
	}
}

void KirbyStateHandler::HandlePowerEnd()
{
	if (m_Conditions.isLoopDone) m_pKirby->SetState(Action::idle);
}

void KirbyStateHandler::HandleHurt(Macro macroState)
{
	if (m_Conditions.isLoopDone && m_Conditions.isInflated)
	{
		m_pKirby->SetState(Action::inflated);
	}
	else if (m_Conditions.isLoopDone && m_Conditions.isBloated)
	{
		m_pKirby->SetState(Action::bloated_idle);
	}
	else if (m_Conditions.isLoopDone)
	{
		m_pKirby->SetState(Action::idle);
	}
}

