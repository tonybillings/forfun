#include "App.h"

App::App(int width, int height, const String& name)
	: _stateFlag(false), _window(width, height, name), _backgroundColor(0, 0, 0)
{}

void App::AddObject(SharedPtr<IAppObject> object)
{
	_objects.push_back(std::move(object));
}

void App::RemoveObject(const String& name)
{
	for (size_t i = 0; i < _objects.size(); i++)
	{
		if (_objects[i]->GetName() == name)
		{
			_objects.erase(_objects.begin() + i);
			return;
		}
	}
}

void App::DeleteObject(const String& name)
{
	for (size_t i = 0; i < _objects.size(); i++)
	{
		auto obj = _objects[i];

		if (obj->GetName() == name)
		{
			_objects.erase(_objects.begin() + i);
			return;
		}
	}
}

void App::ClearObjects()
{
	_objects.clear();
}

int App::Run()
{
	AppState state =
	{
		0,
		0,
		&_window.Keyboard,
		&_window.Mouse,
		_window.GetWidth(),
		_window.GetHeight(),
		&_window.Graphics
	};

	for (auto& object : _objects)
		object->Init(state);

	while (true)
	{
		if (const auto code = Window::ProcessMessages())
		{
			_objects.clear();
			File::UnloadAll();
			return *code;
		}

		Tick();
	}
}

void App::Tick()
{
	_window.Graphics.ClearBuffer(_backgroundColor.x, _backgroundColor.y, _backgroundColor.z);

	auto state = GetAppState();

	for (auto& object : _objects)
	{
		if (object->IsEnabled())
			object->Tick(state);
	}

	_window.Graphics.Present();
	_stateFlag = true;
}

const AppState& App::GetAppState()
{
	static AppState state = 
	{
		0,
		0,
		&_window.Keyboard,
		&_window.Mouse,
		_window.GetWidth(),
		_window.GetHeight(),
		&_window.Graphics
	};

	static double totalTime = 0;

	if (_stateFlag)
	{
		_stateFlag = false;

		float deltaTime = _timer.Mark();
		totalTime += deltaTime;

		state.TotalTime = totalTime;
		state.DeltaTime = deltaTime;
		state.WindowWidth = _window.GetWidth();
		state.WindowHeight = _window.GetHeight();
	}

	return state;
}