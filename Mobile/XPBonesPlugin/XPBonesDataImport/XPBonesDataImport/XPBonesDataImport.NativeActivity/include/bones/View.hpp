#pragma once

#include "IViewComponent.hpp"

namespace bones
{
	class View
	{
	protected:
		std::vector<IViewComponent*> _components;

	public:
		virtual void Init()
		{
			for (auto it = _components.begin(); it != _components.end(); it++)
				(*it)->Init();
		}

		virtual void Refresh() 
		{
			for (auto it = _components.begin(); it != _components.end(); it++)
				(*it)->Refresh();
		}

		virtual void AddComponent(IViewComponent* component)
		{
			_components.push_back(component);
		}
	};
}
