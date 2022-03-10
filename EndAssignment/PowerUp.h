#pragma once
class PowerUp
{
public:
	enum class PowerUpType
	{
		fire,
		beam,
		laser,
		mike,
		parasol,
		spark,
		stone,
		sword
	};

	PowerUp(PowerUpType type, bool hasStart, bool hasContinuous, bool hasEnd, bool hasCompleteSpriteSet = false);
	virtual ~PowerUp();
	virtual void OnKeyDownEvent(Rectf shape, float xDirection)		= 0;
	virtual void ContinuousKeyEvent(Rectf shape, float xDirection)	= 0;
	virtual void OnKeyUpEvent(Rectf shape, float xDirection)		= 0;

	virtual void Draw() const = 0;

	virtual bool IsActive() const = 0;
	virtual bool IsOneShot() const = 0;
	virtual int GetPowerPanelSlot() const;
	virtual std::string GetPowerSuffix() const = 0;

	bool HasStart() const;
	bool HasContinuous() const;
	bool HasEnd() const;
	bool HasCompleteSpriteSet() const;

protected:
	const bool m_HasStart;
	const bool m_HasContinuous;
	const bool m_HasEnd;
	const bool m_HasCompleteSpriteSet;

	bool m_IsActive;
	PowerUpType m_Type;
};

