#pragma once

#include "DrawableObject.h"

class Scene : public DrawableObject
{
	SET_CLASS_ID(0x1def9a43fb100804);

protected:
	List<SharedPtr<IAppObject>>	_objects;

public:
	Scene(const String& name = "");

public:
	virtual void AddObject(SharedPtr<IAppObject> object);
	virtual void RemoveObject(const String& name);
	virtual void DeleteObject(const String& name);
	virtual void ClearObjects();

public:
	virtual void Init(const AppState& appState) override;
	virtual void Tick(const AppState& appState) override;
};