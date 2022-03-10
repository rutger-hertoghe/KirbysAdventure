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

void SparkPower::OnKeyDownEvent(Rectf shape, float xDirection)
{
	m_IsActive = true;
	std::cout << "Spark power is active\n";
}

void SparkPower::ContinuousKeyEvent(Rectf shape, float xDirection)
{
}

void SparkPower::OnKeyUpEvent(Rectf shape, float xDirection)
{
	m_IsActive = false;
	std::cout << "Spark power has been deactivated\n";
}

void SparkPower::Draw() const
{
}

bool SparkPower::IsActive() const
{
	return m_IsActive;
}

bool SparkPower::IsOneShot() const
{
	return false;
}

std::string SparkPower::GetPowerSuffix() const
{
	return "spark_";
}
