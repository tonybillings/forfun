#pragma once

#include "AppState.h"
#include "TypeDefs.h"
#include "Window.h"
#include "Timer.h"
#include "File.h"
#include "IAppObject.h"
#include "GfxTypes.h"

class App
{
private:
	bool _stateFlag;

protected:
	Window								_window;
	Timer								_timer;
	List<SharedPtr<IAppObject>>			_objects;
	Float3								_backgroundColor;

public:
	App(int width, int height, const String& name);

public:
	virtual void AddObject(SharedPtr<IAppObject> object);
	virtual void RemoveObject(const String& name);
	virtual void DeleteObject(const String& name);
	virtual void ClearObjects();

public:
	virtual int Run();
	virtual void Tick();
	virtual const AppState& GetAppState();
	
public:
	inline void SetBackgroundColor(Float3 value) noexcept
	{
		_backgroundColor = value;
	}

	inline const Float3& GetBackgroundColor(Float3 value) const noexcept
	{
		return _backgroundColor;
	}
};