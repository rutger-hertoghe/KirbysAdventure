#pragma once
class PowerUp;

class GameObject
{
protected:
	bool m_IsInhalable;
	
	float m_XDirection;

	Rectf m_Shape;

	PowerUp* m_pPowerUp;

public:
	GameObject();
	~GameObject();
	void TransferPowerUp(GameObject* newOwner);
	bool HasPower();
	virtual void SetPowerUp(PowerUp* powerUp);

};

