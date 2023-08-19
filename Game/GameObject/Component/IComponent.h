#pragma once
#include"StepTimer.h"

class Actor;

class IComponent
{
public:
	virtual ~IComponent() = 0;

	virtual void Update(const DX::StepTimer& timer) = 0;

};