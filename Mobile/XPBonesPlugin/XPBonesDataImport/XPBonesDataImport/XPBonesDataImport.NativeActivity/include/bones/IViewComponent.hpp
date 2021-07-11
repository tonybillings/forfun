#pragma once

namespace bones
{
	class IViewComponent
	{
	public:
		virtual ~IViewComponent() { }
		virtual void Init() = 0;
		virtual void Refresh() = 0;
	};
}