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
#include <filesystem>

//Third-Party Misc
#include <sole.hpp>
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

//|>>====><====<<|

// Render Cases
#define VULKAN13 0x0A
#define OPENGL46 0x09
#define OPENGL45 0x08
#define OPENGL44 0x07
#define OPENGL43 0x06
#define OPENGL42 0x05
#define OPENGL41 0x04
#define OPENGL40 0x03
#define OPENGL30 0x02
#define OPENGL20 0x01

 //| >>= == = > <=== = <<|

//OUTPUTS
#define COUT 0x01
#define CERR 0x02
#define FTX 0x03

 //| >>= == = > <=== = <<|

//Solids

//Prisms
#define DIGONAL_PRISM 0x00
#define TRIANGULAR_PRISM 0x01
#define SQUARE_PRISM 0x02
#define PENTAGONAL_PRISM 0x03
#define HEXAGONAL_PRISM 0x04
#define HEPTAGONAL_PRISM 0x05
#define OCTAGONAL_PRISM 0x06
#define ENNEAGONAL_PRISM 0x07
#define DECAGONAL_PRISM 0x08

//AntiPrisms
#define DIGONAL_ANTIPRISM 0x09
#define TRIANGULAR_ANTIPRISM 0x0A
#define SQUARE_ANTIPRISM 0x0B
#define PENTAGONAL_ANTIPRISM 0x0C
#define HEXAGONAL_ANTIPRISM 0x0D
#define HEPTAGONAL_ANTIPRISM 0x0E
#define OCTAGONAL_ANTIPRISM 0x0F
#define ENNEAGONAL_ANTIPRISM 0x10
#define DECAGONAL_ANTIPRISM 0x11

//PlatonicSolids
#define TETRAHEDRON 0x12
#define CUBE 0x02
#define OCTAHEDRON 0x13
#define ICOSAHEDRON 0x14
#define DODECAHEDRON 0x15

 //| >>= == = > <=== = <<|

//Current Tasks:
// Needs:
// Vulkan Renderer
// Finish GLFW rendering system
// Move Utils to Client
// Return pointer in all pointer functions
// 
// Ideas:
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