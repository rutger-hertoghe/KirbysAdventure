#include "pch.h"
#include "SparkPower.h"
#include <iostream>

SparkPower::SparkPower()
	: PowerUp{PowerUpType::spark, true, true, false}
{
}

SparkPower::~SparkPower()
{
}

void SparkPower::OnKeyDownEvent(const Rectf& shape, float xDirection)
{
	m_IsActive = true;
	std::cout << "Spark power is active\n";
}

void SparkPower::ContinuousKeyEvent(const Rectf& shape, float xDirection)
{
}

void SparkPower::OnKeyUpEvent(const Rectf& shape, float xDirection)
{
	m_IsActive = false;
	std::cout << "Spark power has been deactivated\n";
}

bool SparkPower::IsActive() const
{
	return m_IsActive;
}

std::string SparkPower::GetPowerSuffix() const
{
	return "spark_";
}

void SparkPower::Update(float elapsedSec)
{
}
