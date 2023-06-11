#pragma once

//Settings
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLFW_INCLUDE_VULKAN

//| >>= == = > <=== = <<|

//Standard
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <optional>
#include <set>
#include <memory>
#include <format>
#include <array>
#include <filesystem>
#include <ctime>
#include <ratio>
#include <chrono>

//Third-Party Misc
#include <sole.hpp>
#include <FreeImage.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//Third-Party Render
#include <GLFW/glfw3.h>

//| >>= == = > <=== = <<|

//Common Macros

//Debug
#ifdef NDEBUG
#define DEBUG false
#define NDEBUG true
#else
#define DEBUG true
#define NDEBUG false
#endif

//Packing 
#ifdef __GNUC__
  #define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
  #define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
#endif

#ifndef _WIN64
#define WIN64 0
#else
#define WIN64 1
#endif

#ifndef __APPLE__ || __MACH__
#define OSX 0
#else
#define OSX 1
#endif

#ifndef  __linux__
#define LINUX 0
#else
#define LINUX 1
#endif


//Fast Width Ints
#define FINT8 int_fast8_t
#define FINT16 int_fast16_t
#define FINT32 int_fast32_t
#define FINT64 int_fast64_t

#define UFINT8 uint_fast8_t
#define UFINT16 uint_fast16_t
#define UFINT32 uint_fast32_t
#define UFINT64 uint_fast64_t

#define SFINT8 int_fast8_t
#define SFINT16 int_fast16_t
#define SFINT32 int_fast32_t
#define SFINT64 int_fast64_t

//Width Type Int
#define INT8 int8_t
#define INT16 int16_t
#define INT32 int32_t
#define INT64 int64_t

#define UINT8 uint8_t
#define UINT16 uint16_t
#define UINT32 uint32_t
#define UINT64 uint64_t

#define SINT8 int8_t
#define SINT16 int16_t
#define SINT32 int32_t
#define SINT64 int64_t


//Current Tasks:
// Needs:
// DirectX:
// GetWindowName needs to be fixed for platform and framework use. It doesn't work appropriately here because DirectX expects widestrings
// Bring DirectX up to speed with Vulkan
// 
// Controls
// Add Ability for Motion to be either Atomic Dominant or Atomic Submissive
// Finish "ConfirmMotionReady" and associate aspects such as "SetInputVolume"
// Add to ControlConfigs Capitalization Options
// Add Alternative Names to KeyboardMacros
// SetSpecificGravity
// SetSpecificAcceleration
// -> Move to all Unit Types to config
// -> Remove SetOutputFeature
// 
// Features
// -> Finish Adding Transitivity Layer to Camera
// -> Move all Unit Types to config
// -> Move all internal calculations to shaders.
// 
// Utils
// -> Possible addition of index for "FromFile" functions
// -> SceneFromFile should limit vector by number of items in scene
// -> Finish SceneFromFile 
// -> Add Delimiter Option
// -> Fix changing clients the logfile is replaced
// -> Fix Const Char strings not updating length to 64
// 
// 
// Vulkan Renderer
// -> Update Logs to use new Segmented Log
// Finish GLFW rendering system
// Return pointer in all pointer functions
// -> Pointers in Vulkan Init 
// -> Pointers in Vulkan Pipe 
// -> Pointers in Vulkan Cmnd 
// -> Add Distance Option for Camera
// Pack your structs
// Add setting for detecting capslock
// Add setting for detecting shift
// Add setting for setting the lerp time
// Change Namespaces to Classes in Vulkan.cpp
// 
// Give Option for Window/Game Name - rename log file, update WIndow Names, etc.
// 
// Ideas:
// Update ErrbuffExtnd in Update Conf to include Client Number and Conf Appropriately
// Add Some Form of Obfuscation to PRVTPB using Macros
//Add image output operator in pbOutput
//Input Time Functions in WritetoTimedLog
//Add OPENGL functions
//Set default values of screenHeight, Width, etc. in GLFW window creation
//Finish Metrics
//Add location for Engine Version and Name
//Add function to interprit version for App/Engine
//Give Option for using FTXUI in console.
// 
//Release
//In production, set benchmarking depth to zero - add benchmarking depth
//Add system to add necessary files.
// Create temporary buffer t hold values while setting utilities config 