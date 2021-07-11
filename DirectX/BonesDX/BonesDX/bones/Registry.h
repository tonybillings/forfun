#pragma once

#include <map>
#include <memory>
#include <string>
#include <mutex>
#include <vector>
#include <locale>
#include <codecvt>

#ifdef GetClassName
#define GetClassName_was_defined
#undef GetClassName
#endif

typedef long long ClassId;

#define REGISTERED_CLASS(CLASS_NAME, UINT64_ID) protected: \
String	_name; \
public: \
inline const String& GetName() noexcept \
{ return _name; } \
static inline ClassId ClassID() noexcept \
{ return UINT64_ID; } \
virtual inline ClassId GetClassID() const noexcept \
{ return CLASS_NAME::ClassID(); } \
inline const char* GetClassName() const noexcept \
{ return typeid(this).name(); } \
private: \
	static std::map<String, CLASS_NAME*>* GetRegistry() \
	{ \
		static std::map<String, CLASS_NAME*> registry; \
		return &registry; \
	} \
	static ClassId* RegistryIdentity() \
	{ \
		static ClassId identity; \
		return &identity; \
	} \
	static std::mutex* GetRegistryMutex() \
	{ \
		static std::mutex mutex; \
		return &mutex; \
	} \
public: \
	static void Register(CLASS_NAME* obj) \
	{ \
		std::lock_guard<std::mutex> guard(*GetRegistryMutex()); \
		auto reg = GetRegistry(); \
		if (obj->GetName().empty()) \
		{ \
			auto identity = *RegistryIdentity(); \
			String name = String("UNNAMED_") + std::to_string(identity); \
			*RegistryIdentity() = identity + 1; \
			obj->_name = name; \
			reg->insert(std::pair(obj->GetName(), obj)); \
			return; \
		} \
		size_t count = reg->count(obj->GetName()); \
		if (count == 0) \
		{ \
			reg->insert(std::pair(obj->GetName(), obj)); \
		} \
		else \
		{	\
			auto identity = *RegistryIdentity(); \
			String name = obj->GetName() + String("_") + std::to_string(identity); \
			*RegistryIdentity() = identity + 1; \
			obj->_name = name; \
			reg->insert(std::pair(obj->GetName(), obj)); \
		} \
	} \
	static void Unregister(CLASS_NAME* obj) \
	{ \
		std::lock_guard<std::mutex> guard(*GetRegistryMutex()); \
		auto reg = GetRegistry(); \
		if (reg->count(obj->GetName())) \
			reg->erase(obj->GetName()); \
	} \
	static CLASS_NAME* Find(const String& name) \
	{ \
		std::lock_guard<std::mutex> guard(*GetRegistryMutex()); \
		auto reg = GetRegistry(); \
		if (reg->count(name)) \
			return reg->at(name); \
		else \
			return nullptr; \
	} \
	static CLASS_NAME* Find(const WString& name) \
	{ \
		std::lock_guard<std::mutex> guard(*GetRegistryMutex()); \
		String strName = WStringConverter().to_bytes(name); \
		auto reg = GetRegistry(); \
		if (reg->count(strName)) \
			return reg->at(strName); \
		else \
			return nullptr; \
	} \
	template <class T> \
	static List<T*> Find() \
	{ \
		std::lock_guard<std::mutex> guard(*GetRegistryMutex()); \
		List<T*> result; \
		auto reg = *GetRegistry(); \
		for (std::pair<String, CLASS_NAME*> kv : reg) \
		{ \
			if (kv.second->GetClassID() == T::ClassID()) \
				result.push_back(dynamic_cast<T*>(kv.second)); \
		} \
		return result; \
	} \
	static List<CLASS_NAME*> FindAll() \
	{ \
		std::lock_guard<std::mutex> guard(*GetRegistryMutex()); \
		List<CLASS_NAME*> result; \
		auto reg = *GetRegistry(); \
		for (std::pair<String, CLASS_NAME*> kv : reg) \
			result.push_back(kv.second); \
		return result; \
	}

#define SET_CLASS_ID(UINT64_ID) public: \
	static inline ClassId ClassID() noexcept { return UINT64_ID; } \
	virtual inline ClassId GetClassID() const noexcept override { return UINT64_ID; }

#ifdef GetClassName_was_defined
#undef GetClassName_was_defined
#ifdef UNICODE
#define GetClassName  GetClassNameW
#else
#define GetClassName  GetClassNameA
#endif 
#endif