#pragma once

#include <wrl.h>
#include <memory>
#include <map>
#include <vector>
#include <locale>
#include <codecvt>

typedef std::string String;
typedef std::wstring WString;

template <class T, class U> using Map = std::map<T, U>;
template <class T> using List = std::vector<T>;

template <class T> using UniquePtr = std::unique_ptr<T>;
template <class T> using SharedPtr = std::shared_ptr<T>;

template <class T> using UniquePtrList = std::vector<std::unique_ptr<T>>;
template <class T> using SharedPtrList = std::vector<std::shared_ptr<T>>;

template <class T> using UniqueListPtr = std::unique_ptr<std::vector<T>>;
template <class T> using SharedListPtr = std::shared_ptr<std::vector<T>>;

typedef std::wstring_convert<std::codecvt_utf8<wchar_t>> WStringConverter;

template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;