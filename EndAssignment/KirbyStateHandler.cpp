#include "pch.h"
#include "KirbyStateHandler.h"
#include "PowerUp.h"
#include "SoundEffect.h"

KirbyStateHandler::KirbyStateHandler(Kirby* pKirby)
	: m_pKirby(pKirby)
{
}

void KirbyStateHandler::HandleState(Kirby::ActionState actionState, Kirby::MacroState macroState)
{
	if (m_pKirby == nullptr) return;

	switch (actionState)
	{
	// BASIC SET
	case Kirby::ActionState::idle:
		HandleIdle(actionState, macroState);
		break;
	case Kirby::ActionState::walking:
		HandleWalking(actionState, macroState);
		break;
	case Kirby::ActionState::jumping:
		HandleJumping(macroState);
		break;
	case Kirby::ActionState::flipping:
		HandleFlipping(macroState);
		break;
	case Kirby::ActionState::falling:
		HandleFalling(macroState);
		break;
	case Kirby::ActionState::ducking:
		HandleDucking(macroState);
		break;
	case Kirby::ActionState::sliding:
		HandleSliding();
		break;
	// INFLATING SET
	case Kirby::ActionState::inflating:
		HandleInflating();
		break;
	case Kirby::ActionState::inflated:
		HandleInflated(macroState);
		break;
	case Kirby::ActionState::flapping:
		HandleFlapping(macroState);
		break;
	case Kirby::ActionState::deflating:
		HandleDeflating();
		break;
	// INHALING/BlOAT SET
	case Kirby::ActionState::start_inhaling:
		HandleStartInhaling();
		break;
	case Kirby::ActionState::inhaling:
		HandleInhaling(macroState);
		break;
	case Kirby::ActionState::bloat:
		HandleBloat();
		break;
	case Kirby::ActionState::exhaling:
		HandleExhaling();
		break;
	case Kirby::ActionState::bloated_idle:
		HandleBloatedIdle(actionState,macroState);
		break;
	case Kirby::ActionState::bloated_walking:
		HandleBloatedWalking(actionState, macroState);
		break;
	case Kirby::ActionState::swallowing:
		HandleSwallowing();
		break;
	case Kirby::ActionState::spitting:
		HandleSpitting();
		break;
		// POWER SET
	case Kirby::ActionState::power_start:
		HandlePowerStart();
		break;
	case Kirby::ActionState::power_continuous:
		HandlePowerContinuous();
		break;
	case Kirby::ActionState::power_end:
		HandlePowerEnd();
		break;
	case Kirby::ActionState::hurt:
		HandleHurt(macroState);
		break;
	}

	/* CONDITION LIST FOR LATER REFERENCE
	Vector2f velocity(m_pKirby->GetVelocity());
	const float yVelocityTreshold{ 100.f };
	const bool isOnGround{ m_pKirby->IsOnGround() };
	const bool isMoving{ abs(velocity.x) >= 1.f };
	const bool isIdle{ !isMoving };
	const bool isGoingUp{ velocity.y > 1.f };
	const bool isGoingDown{ velocity.y < -1.f };
	const bool isJumping{ isGoingUp && !(actionState == Kirby::ActionState::inflating || actionState == Kirby::ActionState::inflated) };
	const bool isMidAir{ abs(velocity.y) < yVelocityTreshold && !m_pKirby->IsOnGround() };
	const bool isFalling{ !m_pKirby->IsOnGround() && actionState != Kirby::ActionState::jumping };
	const bool isLoopDone{ m_pKirby->HasLooped() };
	const bool isUsingPower{ m_pKirby->HasPower() ? m_pKirby->GetPowerUp()->IsActive() : false };

	const bool isInflated{ macroState == Kirby::MacroState::inflated };
	const bool isDucking{ macroState == Kirby::MacroState::ducking };
	const bool isInhaling{ macroState == Kirby::MacroState::inhalation };
	const bool isBloated{ macroState == Kirby::MacroState::bloated };
	const bool isSliding{ macroState == Kirby::MacroState::sliding };
	*/
}

void KirbyStateHandler::HandleIdle(Kirby::ActionState actionState, Kirby::MacroState macroState)
{
	const Vector2f velocity(m_pKirby->GetVelocity());
	const float yVelocityTreshold{ 100.f };
	const bool isOnGround{ m_pKirby->IsOnGround() };
	const bool isMoving{ abs(velocity.x) >= 1.f };
	const bool isGoingUp{ velocity.y > 1.f };
	const bool isJumping{ isGoingUp && !(actionState == Kirby::ActionState::inflating || actionState == Kirby::ActionState::inflated) };
	const bool isFalling{ !m_pKirby->IsOnGround() && actionState != Kirby::ActionState::jumping };
	const bool isUsingPower{ m_pKirby->HasPowerUp() ? m_pKirby->GetPowerUp()->IsActive() : false };

	const bool isInflated{ macroState == Kirby::MacroState::inflated };
	const bool isDucking{ macroState == Kirby::MacroState::ducking };
	const bool isInhaling{ macroState == Kirby::MacroState::inhalation };

	if (isInflated)			m_pKirby->SetState(Kirby::ActionState::inflating);
	else if (isUsingPower)	m_pKirby->SetPowerState();
	else if (isInhaling)	m_pKirby->SetState(Kirby::ActionState::start_inhaling);
	else if (isJumping)		m_pKirby->SetState(Kirby::ActionState::jumping);
	else if (isFalling)		m_pKirby->SetState(Kirby::ActionState::falling);
	else if (isDucking)		m_pKirby->SetState(Kirby::ActionState::ducking);
	else if (isMoving)		m_pKirby->SetState(Kirby::ActionState::walking);
}

void KirbyStateHandler::HandleWalking(Kirby::ActionState actionState, Kirby::MacroState macroState)
{
	const Vector2f velocity(m_pKirby->GetVelocity());
	const bool isMoving{ abs(velocity.x) >= 1.f };
	const bool isIdle{ !isMoving };
	const bool isGoingUp{ velocity.y > 1.f };
	const bool isJumping{ isGoingUp && !(actionState == Kirby::ActionState::inflating || actionState == Kirby::ActionState::inflated) };
	const bool isFalling{ !m_pKirby->IsOnGround() && actionState != Kirby::ActionState::jumping };
	const bool isUsingPower{ m_pKirby->HasPowerUp() ? m_pKirby->GetPowerUp()->IsActive() : false };

	const bool isInflated{ macroState == Kirby::MacroState::inflated };
	const bool isDucking{ macroState == Kirby::MacroState::ducking };
	const bool isInhaling{ macroState == Kirby::MacroState::inhalation };

	if (isInflated)			m_pKirby->SetState(Kirby::ActionState::inflating);
	else if (isUsingPower)	m_pKirby->SetPowerState();
	else if (isInhaling)	m_pKirby->SetState(Kirby::ActionState::start_inhaling);
	else if (isJumping)		m_pKirby->SetState(Kirby::ActionState::jumping);
	else if (isFalling)		m_pKirby->SetState(Kirby::ActionState::falling);
	else if (isDucking)		m_pKirby->SetState(Kirby::ActionState::ducking);
	else if (isIdle)		m_pKirby->SetState(Kirby::ActionState::idle);
}

void KirbyStateHandler::HandleJumping(Kirby::MacroState macroState)
{
	const Vector2f velocity(m_pKirby->GetVelocity());
	const float yVelocityTreshold{ 100.f };
	const bool isInflated{ macroState == Kirby::MacroState::inflated };
	const bool isInhaling{ macroState == Kirby::MacroState::inhalation };
	const bool isUsingPower{ m_pKirby->HasPowerUp() ? m_pKirby->GetPowerUp()->IsActive() : false };
	const bool isOnGround{ m_pKirby->IsOnGround() };
	const bool isMidAir{ abs(velocity.y) < yVelocityTreshold && !m_pKirby->IsOnGround() };

	if (isInflated)			m_pKirby->SetState(Kirby::ActionState::inflating);
	else if (isUsingPower)	m_pKirby->SetPowerState();
	else if (isInhaling)	m_pKirby->SetState(Kirby::ActionState::inhaling);
	else if (isOnGround)	m_pKirby->SetState(Kirby::ActionState::idle);
	else if (isMidAir)		m_pKirby->SetState(Kirby::ActionState::flipping);
}

void KirbyStateHandler::HandleFlipping(Kirby::MacroState macroState)
{
	const bool isInflated{ macroState == Kirby::MacroState::inflated };
	const bool isUsingPower{ m_pKirby->HasPowerUp() ? m_pKirby->GetPowerUp()->IsActive() : false };
	const bool isInhaling{ macroState == Kirby::MacroState::inhalation };
	const bool isOnGround{ m_pKirby->IsOnGround() };
	const bool isLoopDone{ m_pKirby->HasLooped() };

	if (isInflated)
	{
		m_pKirby->SetState(Kirby::ActionState::inflating);
	}
	else if (isUsingPower)
	{
		m_pKirby->SetPowerState();
	}
	else if (isInhaling)
	{
		m_pKirby->SetState(Kirby::ActionState::inhaling);
	}
	else if (isOnGround)
	{
		m_pKirby->SetState(Kirby::ActionState::idle);
	}
	else if (isLoopDone)
	{
		m_pKirby->SetState(Kirby::ActionState::falling);
	}
}

void KirbyStateHandler::HandleFalling(Kirby::MacroState macroState)
{
	const bool isInflated{ macroState == Kirby::MacroState::inflated };
	const bool isUsingPower{ m_pKirby->HasPowerUp() ? m_pKirby->GetPowerUp()->IsActive() : false };
	const bool isInhaling{ macroState == Kirby::MacroState::inhalation };
	const bool isOnGround{ m_pKirby->IsOnGround() };

	if (isInflated)			m_pKirby->SetState(Kirby::ActionState::inflating);
	else if (isUsingPower)	m_pKirby->SetPowerState();
	else if (isInhaling)	m_pKirby->SetState(Kirby::ActionState::inhaling);
	else if (isOnGround)	m_pKirby->SetState(Kirby::ActionState::idle);
}

void KirbyStateHandler::HandleDucking(Kirby::MacroState macroState)
{
	const bool isSliding{ macroState == Kirby::MacroState::sliding };
	const bool isDucking{ macroState == Kirby::MacroState::ducking };

	if (isSliding)
	{
		m_pKirby->SetState(Kirby::ActionState::sliding);
	}
	else if (!isDucking)
	{
		m_pKirby->SetState(Kirby::ActionState::idle);
	}
}

void KirbyStateHandler::HandleSliding()
{
	Vector2f velocity(m_pKirby->GetVelocity());
	const bool isMoving{ abs(velocity.x) >= 1.f };

	if (!isMoving)
	{
		m_pKirby->SetState(Kirby::ActionState::ducking);
		m_pKirby->SetMacroState(Kirby::MacroState::ducking);
	}
}

void KirbyStateHandler::HandleInflating()
{
	const bool isLoopDone{ m_pKirby->HasLooped() };

	if (isLoopDone) 
	{
		m_pKirby->SetState(Kirby::ActionState::inflated);
	}
}

void KirbyStateHandler::HandleInflated(Kirby::MacroState macroState)
{
	const Vector2f velocity(m_pKirby->GetVelocity());
	const bool isInflated{ macroState == Kirby::MacroState::inflated };
	const bool isGoingUp{ velocity.y > 1.f };

	if (!isInflated)
	{
		m_pKirby->SetState(Kirby::ActionState::deflating);
	}
	else if (isGoingUp)
	{
		m_pKirby->SetState(Kirby::ActionState::flapping);
	}
}

void KirbyStateHandler::HandleFlapping(Kirby::MacroState macroState)
{
	const Vector2f velocity(m_pKirby->GetVelocity());
	const bool isInflated{ macroState == Kirby::MacroState::inflated };
	const bool isGoingDown{ velocity.y < -1.f };

	if (!isInflated)
	{
		m_pKirby->SetState(Kirby::ActionState::deflating);
	}
	else if (isGoingDown)
	{
		m_pKirby->SetState(Kirby::ActionState::inflated);
	}
}

void KirbyStateHandler::HandleDeflating()
{
	const bool isLoopDone{ m_pKirby->HasLooped() };
	if (isLoopDone)
	{
		m_pKirby->SetState(Kirby::ActionState::idle);
	}
}

void KirbyStateHandler::HandleStartInhaling()
{
	const bool isLoopDone{ m_pKirby->HasLooped() };
	if (isLoopDone)
	{
		m_pKirby->SetState(Kirby::ActionState::inhaling);
	}
}

void KirbyStateHandler::HandleInhaling(Kirby::MacroState macroState)
{
	const bool isInhaling{ macroState == Kirby::MacroState::inhalation };
	const bool isBloated{ macroState == Kirby::MacroState::bloated };

	if (isBloated)
	{
		m_pKirby->SetState(Kirby::ActionState::bloat);
	}
	else if (!isInhaling)
	{
		m_pKirby->SetState(Kirby::ActionState::exhaling);
	}
}

void KirbyStateHandler::HandleBloat()
{
	const bool isLoopDone{ m_pKirby->HasLooped() };

	if (isLoopDone)
	{
		m_pKirby->SetState(Kirby::ActionState::bloated_idle);
	}
}

void KirbyStateHandler::HandleExhaling()
{
	const bool isLoopDone{ m_pKirby->HasLooped() };

	if (isLoopDone)
	{
		m_pKirby->SetState(Kirby::ActionState::idle);
	}
}

void KirbyStateHandler::HandleBloatedIdle(Kirby::ActionState actionState, Kirby::MacroState macroState)
{
	const Vector2f velocity(m_pKirby->GetVelocity());
	const bool isMoving{ abs(velocity.x) >= 1.f };
	const bool isIdle{ !isMoving };
	const bool isGoingUp{ velocity.y > 1.f };
	const bool isJumping{ isGoingUp && !(actionState == Kirby::ActionState::inflating || actionState == Kirby::ActionState::inflated) };
	const bool isFalling{ !m_pKirby->IsOnGround() && actionState != Kirby::ActionState::jumping };
	const bool isBloated{ macroState == Kirby::MacroState::bloated };

	if (!isBloated)
	{
		m_pKirby->SetState(Kirby::ActionState::swallowing);
	}
	else if (isMoving || isJumping || isFalling)
	{
		m_pKirby->SetState(Kirby::ActionState::bloated_walking);
	}
}

void KirbyStateHandler::HandleBloatedWalking(Kirby::ActionState actionState, Kirby::MacroState macroState)
{
	const Vector2f velocity(m_pKirby->GetVelocity());
	const bool isMoving{ abs(velocity.x) >= 1.f };
	const bool isIdle{ !isMoving };
	const bool isGoingUp{ velocity.y > 1.f };
	const bool isJumping{ isGoingUp && !(actionState == Kirby::ActionState::inflating || actionState == Kirby::ActionState::inflated) };
	const bool isFalling{ !m_pKirby->IsOnGround() && actionState != Kirby::ActionState::jumping };
	const bool isBloated{ macroState == Kirby::MacroState::bloated };

	if (!isBloated)
	{
		m_pKirby->SetState(Kirby::ActionState::swallowing);
	}
	else if (isIdle && !isJumping && !isFalling)
	{
		m_pKirby->SetState(Kirby::ActionState::bloated_idle);
	}
}

void KirbyStateHandler::HandleSwallowing()
{
	const bool isLoopDone{ m_pKirby->HasLooped() };

	if (isLoopDone) m_pKirby->SetState(Kirby::ActionState::idle);
}

void KirbyStateHandler::HandleSpitting()
{
	const bool isLoopDone{ m_pKirby->HasLooped() };
	if (isLoopDone) m_pKirby->SetState(Kirby::ActionState::idle);
}

void KirbyStateHandler::HandlePowerStart()
{
	const bool isUsingPower{ m_pKirby->HasPowerUp() ? m_pKirby->GetPowerUp()->IsActive() : false };
	const bool isLoopDone{ m_pKirby->HasLooped() };
	if (!isUsingPower && m_pKirby->GetPowerUp()->HasEnd()) m_pKirby->SetState(Kirby::ActionState::power_end);
	else if (!isUsingPower) m_pKirby->SetState(Kirby::ActionState::idle);
	else if (isLoopDone && m_pKirby->GetPowerUp()->HasContinuous()) m_pKirby->SetState(Kirby::ActionState::power_continuous);
}

void KirbyStateHandler::HandlePowerContinuous()
{
	const bool isUsingPower{ m_pKirby->HasPowerUp() ? m_pKirby->GetPowerUp()->IsActive() : false };
	if (!isUsingPower && m_pKirby->GetPowerUp()->HasEnd()) m_pKirby->SetState(Kirby::ActionState::power_end);
	else if (!isUsingPower) m_pKirby->SetState(Kirby::ActionState::idle);
}

void KirbyStateHandler::HandlePowerEnd()
{
	const bool isLoopDone{ m_pKirby->HasLooped() };
	if (isLoopDone) m_pKirby->SetState(Kirby::ActionState::idle);
}

void KirbyStateHandler::HandleHurt(Kirby::MacroState macroState)
{
	const bool isLoopDone{ m_pKirby->HasLooped() };
	const bool isBloated{ macroState == Kirby::MacroState::bloated };
	const bool isInflated{ macroState == Kirby::MacroState::inflated };

	if (isLoopDone && isInflated)
	{
		m_pKirby->SetState(Kirby::ActionState::inflated);
	}
	else if (isLoopDone && isBloated)
	{
		m_pKirby->SetState(Kirby::ActionState::bloated_idle);
	}
	else if (isLoopDone) 
	{
		m_pKirby->SetState(Kirby::ActionState::idle);
	}
}

