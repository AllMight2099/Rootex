#pragma once

// Smart pointers
#include <memory>
/// std::unique_ptr
template <class T>
using Ptr = std::unique_ptr<T>;
/// std::shared_ptr
template <class T>
using Ref = std::shared_ptr<T>;
/// std::weak_ptr
template <class T>
using Weak = std::weak_ptr<T>;
#include <wrl.h> // For using Microsoft::WRL::ComPtr<T>

// Serialization streams
#include <fstream>
/// std::fstream
typedef std::fstream InputOutputFileStream;
/// std::ofstream
typedef std::ofstream OutputFileStream;
/// std::ifstream
typedef std::ifstream InputFileStream;
#include <sstream>
/// std::stringstream
typedef std::stringstream StringStream;

// Containers
#include <string>
/// std::string
typedef std::string String;

#include <map>
/// std::map
template <class P, class Q>
using Map = std::map<P, Q>;

#include <unordered_map>
/// std::unordered_map
template <class P, class Q>
using HashMap = std::unordered_map<P, Q>;

#include <utility>
/// std::tuple
template <typename...P>
using Tuple = std::tuple<P...>;
/// std::pair
template <class P, class Q>
using Pair = std::pair<P, Q>;

#include <vector>
/// std::vector
template <class T>
using Vector = std::vector<T>;

#include <filesystem>
/// std::filesystem::path
using FilePath = std::filesystem::path;

// Math Containers
#include <d3d11.h>
#include "vendor/DirectXTK/Inc/SimpleMath.h"
/// DirectX::SimpleMath::Matrix
typedef DirectX::SimpleMath::Matrix Matrix;
/// DirectX::SimpleMath::Vector2
typedef DirectX::SimpleMath::Vector2 Vector2;
/// DirectX::SimpleMath::Vector3
typedef DirectX::SimpleMath::Vector3 Vector3;
/// DirectX::SimpleMath::Vector4
typedef DirectX::SimpleMath::Vector4 Vector4;
/// DirectX::SimpleMath::Quaternion
typedef DirectX::SimpleMath::Quaternion Quaternion;
/// DirectX::SimpleMath::Color
typedef DirectX::SimpleMath::Color Color;

#include <DirectXColors.h>
/// DirectX::Colors
namespace ColorPresets = DirectX::Colors;

#include <variant>
/// Vector of std::variant of bool, int, char, float, String, Vector2, Vector3, Vector4, Matrix
typedef Vector<std::variant<bool, int, char, float, String, Vector2, Vector3, Vector4, Matrix>> VariantVector;
class Entity;
/// std::variant of bool, int, char, float, String, Vector2, Vector3, Vector4, Matrix VariantVector, Ref<Entity>, Vector<String>
using Variant = std::variant<bool, int, char, float, String, Vector2, Vector3, Vector4, Matrix, VariantVector, Ref<Entity>, Vector<String>>;
/// Extract the value of type TypeName from a Variant
#define Extract(TypeName, variant) std::get<TypeName>((variant))

#include <functional>
/// std::function
template <class T>
using Function = std::function<T>;

#include "JSON/json.hpp"
/// Namespace for the JSON library
namespace JSON = nlohmann;

#include "imgui.h"

// target Windows 7 or later
#define _WIN32_WINNT 0x0601
#include <sdkddkver.h>

#ifndef WINDOWS_NO_DUMP
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#endif

#define NOMINMAX

#define STRICT

#include <Windows.h>
