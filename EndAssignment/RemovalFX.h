#pragma once
#include "GameObject.h"
class RemovalFX final : public GameObject
{
public:
	enum class FXType {
		enemy,
		item
};
	explicit RemovalFX(const Point2f& location, RemovalFX::FXType type);
	RemovalFX(const RemovalFX& other) = delete;
	RemovalFX& operator=(const RemovalFX&) = delete;
	RemovalFX(RemovalFX&& other) = delete;
	RemovalFX& operator=(RemovalFX&& other) = delete;
	~RemovalFX() override;

	virtual void Update(float elapsedSec) override;
	bool IsReadyToDestroy() const;

private:
	bool m_IsReadyToDestroy;

	FXType m_Type;
	
	void InitializeSprites();
};

