#pragma once
#include "Kirby.h"

class KirbyStateHandler final
{
public:
	explicit KirbyStateHandler(Kirby* pKirby);
	void HandleState(Kirby::ActionState actionState, Kirby::MacroState macroState);

private:
	Kirby* m_pKirby;

	void HandleIdle(Kirby::ActionState actionState, Kirby::MacroState macroState);
	void HandleWalking(Kirby::ActionState actionState, Kirby::MacroState macroState);
	void HandleJumping(Kirby::MacroState macroState);
	void HandleFlipping(Kirby::MacroState macroState);
	void HandleFalling(Kirby::MacroState macroState);
	void HandleDucking(Kirby::MacroState macroState);
	void HandleSliding();
	void HandleInflating();
	void HandleInflated(Kirby::MacroState macroState);
	void HandleFlapping(Kirby::MacroState macroState);
	void HandleDeflating();
	void HandleStartInhaling();
	void HandleInhaling(Kirby::MacroState macroState);
	void HandleBloat();
	void HandleExhaling();
	void HandleBloatedIdle(Kirby::ActionState actionState, Kirby::MacroState macroState);
	void HandleBloatedWalking(Kirby::ActionState actionState, Kirby::MacroState macroState);
	void HandleSwallowing();
	void HandleSpitting();
	void HandlePowerStart();
	void HandlePowerContinuous();
	void HandlePowerEnd();
	void HandleHurt(Kirby::MacroState macroState);
};

