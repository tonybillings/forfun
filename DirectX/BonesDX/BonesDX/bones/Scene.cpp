#include "Scene.h"

Scene::Scene(const String& name)
	: DrawableObject(name)
{
	EnableDefaultLighting();
}

void Scene::AddObject(SharedPtr<IAppObject> object)
{
	_objects.push_back(std::move(object));
}

void Scene::RemoveObject(const String& name)
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

void Scene::DeleteObject(const String& name)
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

void Scene::ClearObjects()
{
	_objects.clear();
}

void Scene::Init(const AppState& appState)
{
	for (auto& object : _objects)
		object->Init(appState);
}

void Scene::Tick(const AppState& appState)
{
	for (auto& object : _objects)
	{
		if (object->IsEnabled())
			object->Tick(appState);
	}
}