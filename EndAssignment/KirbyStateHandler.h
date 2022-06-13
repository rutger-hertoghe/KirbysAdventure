#pragma once
#include "Kirby.h"

using Action = Kirby::ActionState;
using Macro = Kirby::MacroState;

struct StateConditions
{
	bool isOnGround{false};
	bool isMoving{false};
	bool isIdle{true};
	bool isGoingUp{false};
	bool isGoingDown{false};
	bool isJumping{false};
	bool isMidAir{false};
	bool isFalling{ false };
	bool isLoopDone{ false };
	bool isUsingPower{ false };

	bool isInflated{ false };
	bool isDucking{ false };
	bool isInhaling{ false };
	bool isBloated{ false };
	bool isSliding{ false };
};

class KirbyStateHandler final
{
public:
	explicit KirbyStateHandler(Kirby* pKirby);
	// No dynamically allocated memory so rule of five implementation is not needed, default copy/move constructors and destructor are fine 
	void HandleState(Action actionState, Macro macroState);

private:
	Kirby* m_pKirby;
	StateConditions m_Conditions;

	void CheckConditions(Action actionState, Macro macroState);

	void HandleIdle(Action actionState, Macro macroState);
	void HandleWalking(Action actionState, Macro macroState);
	void HandleJumping(Macro macroState);
	void HandleFlipping(Macro macroState);
	void HandleFalling(Macro macroState);
	void HandleDucking(Macro macroState);
	void HandleSliding();
	void HandleInflating();
	void HandleInflated(Macro macroState);
	void HandleFlapping(Macro macroState);
	void HandleDeflating();
	void HandleStartInhaling();
	void HandleInhaling(Macro macroState);
	void HandleBloat();
	void HandleExhaling();
	void HandleBloatedIdle(Action actionState, Macro macroState);
	void HandleBloatedWalking(Action actionState, Macro macroState);
	void HandleSwallowing();
	void HandleSpitting();
	void HandlePowerStart();
	void HandlePowerContinuous();
	void HandlePowerEnd();
	void HandleHurt(Macro macroState);
};

