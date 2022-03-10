#pragma once
#include "Gameobject.h"

class PowerStar : public GameObject
{
public:
	PowerStar(Rectf shape);
	~PowerStar();

	void Draw() const;
};

