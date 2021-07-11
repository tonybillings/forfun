#ifndef __bones_event_H__
#define __bones_event_H__

namespace Bones
{
#pragma region Definitions
	class NullClass {};
	class StaticFunction {};
	#define EVT_HNDL_CAP 10
	#define EVT_HNDL_CAP_INC 10
	#define STATICFUNC StaticFunction
	#define NULLCLASS NullClass
#pragma endregion

#pragma region Argument
	class IEventArg {};

	template <typename T>
	class EventArg : public IEventArg
	{
	public:
		T Value;
	public:
		EventArg(T value) : Value(value) {};
	};
#pragma endregion

#pragma region Handler
	class IEventHandler
	{
	public:
		virtual void SetId(int id) {};
		virtual int GetId() const { return -1; }
	public:
		virtual void operator() () {};
		virtual void operator() (const IEventArg*) {};
		virtual void operator() (const IEventArg*, const IEventArg*) {};
	};

	template <class T, typename U = NULLCLASS, typename V = NULLCLASS>
	class EventHandler : public IEventHandler
	{
		typedef void(*StaticFuncNoParam)();
		typedef void(*StaticFuncOneParam)(U);
		typedef void(*StaticFuncTwoParam)(U, V);
		typedef void(T::*MemberFuncNoParam)();
		typedef void(T::*MemberFuncOneParam)(U);
		typedef void(T::*MemberFuncTwoParam)(U, V);

	private:
		int handlerId;
		T* obj;
		bool isStatic;
		union FUNCPTR
		{
			StaticFuncNoParam sf0;
			StaticFuncOneParam sf1;
			StaticFuncTwoParam sf2;
			MemberFuncNoParam mf0;
			MemberFuncOneParam mf1;
			MemberFuncTwoParam mf2;
		} funcptr;
	public:
		EventHandler(StaticFuncNoParam func)
		{
			funcptr.sf0 = func;
			isStatic = true;
		}

		EventHandler(StaticFuncOneParam func)
		{
			funcptr.sf1 = func;
			isStatic = true;
		}

		EventHandler(StaticFuncTwoParam func)
		{
			funcptr.sf2 = func;
			isStatic = true;
		}

		EventHandler(T* object, MemberFuncNoParam func)
		{
			obj = object;
			funcptr.mf0 = func;
			isStatic = false;
		}

		EventHandler(T* object, MemberFuncOneParam func)
		{
			obj = object;
			funcptr.mf1 = func;
			isStatic = false;
		}

		EventHandler(T* object, MemberFuncTwoParam func)
		{
			obj = object;
			funcptr.mf2 = func;
			isStatic = false;
		}
	public:
		void SetId(int id)
		{
			handlerId = id;
		}

		int GetId() const
		{
			return handlerId;
		}
	public:
		void operator() ()
		{
			if (isStatic)
				funcptr.sf0();
			else
				(obj->*funcptr.mf0)();
		}

		void operator() (const IEventArg* arg1)
		{
			U val = (static_cast<const EventArg<U>*>(arg1))->Value;

			if (isStatic)
				funcptr.sf1(val);
			else
				(obj->*funcptr.mf1)(val);
		}

		void operator() (const IEventArg* arg1, const IEventArg* arg2)
		{
			U val1 = (static_cast<const EventArg<U>*>(arg1))->Value;
			V val2 = (static_cast<const EventArg<V>*>(arg2))->Value;

			if (isStatic)
				funcptr.sf2(val1, val2);
			else
				(obj->*funcptr.mf2)(val1, val2);
		}
	};
#pragma endregion

#pragma region Event
	template <typename T = NULLCLASS, typename U = NULLCLASS>
	class Event
	{
	private:
		int identity;
		int count;
		int capacity;
		IEventHandler** handlers;
	public:
		Event()
		{
			identity = 0;
			count = 0;
			capacity = EVT_HNDL_CAP;
			handlers = new IEventHandler*[capacity];
		}

		~Event()
		{
			for (int i = 0; i < count; i++)
				delete handlers[i];

			delete[] handlers;
		}
	public:
		const IEventHandler* operator+= (IEventHandler* handler)
		{
			if (count < capacity)
			{
				handlers[count++] = handler;
			}
			else
			{
				IEventHandler** temp = new IEventHandler*[capacity + EVT_HNDL_CAP_INC];

				for (int i = 0; i < capacity; i++)
					temp[i] = handlers[i];

				delete[] handlers;
				handlers = temp;
				capacity += EVT_HNDL_CAP_INC;
				handlers[count++] = handler;
			}
			handler->SetId(identity++);
			return handler;
		}

		void operator-= (const IEventHandler* handler)
		{
			IEventHandler** temp = new IEventHandler*[capacity];
			int index = 0;
			int currentCount = count;

			for (int i = 0; i < currentCount; i++)
			{
				if (handlers[i]->GetId() != handler->GetId())
					temp[index++] = handlers[i];
				else
					count--;
			}

			delete[] handlers;
			handlers = temp;
			delete handler;
		}

		void operator() ()
		{
			for (int i = 0; i < count; i++)
				(*(handlers[i]))();
		}

		void operator()(T arg)
		{
			EventArg<T> a(arg);

			for (int i = 0; i < count; i++)
				(*(handlers[i]))(&a);
		}

		void operator()(T arg1, U arg2)
		{
			EventArg<T> a(arg1);
			EventArg<U> b(arg2);

			for (int i = 0; i < count; i++)
				(*(handlers[i]))(&a, &b);
		}
	};
#pragma endregion
}
 
#endif
