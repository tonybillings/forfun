#pragma once

#include "AppState.h"
#include "TypeDefs.h"

class IAppObject
{
public:
	virtual void Init(const AppState& appState) = 0;
	virtual void Tick(const AppState& appState) = 0;
	virtual const String& GetName() const noexcept = 0;
	virtual bool IsEnabled() const noexcept = 0;
};