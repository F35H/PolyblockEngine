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


//|>>====><====<<|

// Render Cases
#define DIRECTX12                                                                 0x0C
#define DIRECTX11                                                                 0x0B

#define VULKAN13                                                                 0x0A

#define OPENGL46                                                                 0x09
#define OPENGL45                                                                 0x08 
#define OPENGL44                                                                 0x07
#define OPENGL43                                                                 0x06
#define OPENGL42                                                                 0x05
#define OPENGL41                                                                 0x04
#define OPENGL40                                                                 0x03
#define OPENGL30                                                                 0x02
#define OPENGL20                                                                 0x01

 //| >>= == = > <=== = <<|

//OUTPUTS
#define COUT                                                                         0x01
#define CERR                                                                          0x02
#define FTX                                                                             0x03

 //| >>= == = > <=== = <<|

//Solids

//Prisms
#define DIGONAL_PRISM                                                       0x00
#define TRIANGULAR_PRISM                                                 0x01
#define SQUARE_PRISM                                                         0x02
#define PENTAGONAL_PRISM                                                0x03
#define HEXAGONAL_PRISM                                                  0x04
#define HEPTAGONAL_PRISM                                                0x05
#define OCTAGONAL_PRISM                                                 0x06
#define ENNEAGONAL_PRISM                                               0x07
#define DECAGONAL_PRISM                                                 0x08

//AntiPrisms
#define DIGONAL_ANTIPRISM                                              0x09
#define TRIANGULAR_ANTIPRISM                                        0x0A
#define SQUARE_ANTIPRISM                                                0x0B
#define PENTAGONAL_ANTIPRISM                                      0x0C
#define HEXAGONAL_ANTIPRISM                                        0x0D
#define HEPTAGONAL_ANTIPRISM                                      0x0E
#define OCTAGONAL_ANTIPRISM                                       0x0F
#define ENNEAGONAL_ANTIPRISM                                     0x10
#define DECAGONAL_ANTIPRISM                                        0x11

//PlatonicSolids
#define TETRAHEDRON                                                         0x12
#define CUBE                                                                         0x02
#define OCTAHEDRON                                                          0x13
#define ICOSAHEDRON                                                         0x14
#define DODECAHEDRON                                                     0x15

 //| >>= == = > <=== = <<|

//INPUTS

//Miscellanious Lower
#define KEYBOARD_APOSTROPHE                                       0x01
#define KEYBOARD_COMMA                                                0x02
#define KEYBOARD_MINUS                                                  0x03
#define KEYBOARD_PERIOD                                                 0x04
#define KEYBOARD_SLASH                                                   0x05
#define KEYBOARD_SEMICOLON                                         0x06
#define KEYBOARD_EQUAL                                                  0x07
#define KEYBOARD_BRACKET_LEFT_SQUARE                      0x08
#define KEYBOARD_BRACKET_RIGHT_SQUARE                   0x09
#define KEYBOARD_BACKSLASH                                          0x0A
#define KEYBOARD_ACCENT_GRAVE                                   0x0B

//Miscellanious Upper
#define KEYBOARD_QUOTATION                                        0x0C
#define KEYBOARD_LESS_THAN                                          0x0D
#define KEYBOARD_GREATER_THAN                                   0x0E
#define KEYBOARD_UNDERSCORE                                      0x0F
#define KEYBOARD_QUESTION_MARK                                0x10
#define KEYBOARD_COLON                                                 0x11
#define KEYBOARD_PLUS                                                     0x12
#define KEYBOARD_PIPE                                                      0x13
#define KEYBOARD_BRACKET_LEFT_CURLED                       0x14
#define KEYBOARD_BRACKET_RIGHT_CURLED                    0x15
#define KEYBOARD_ACCENT_TILDE                                     0x16

//Miscellanious Other
#define KEYBOARD_TAB                                                       0x17
#define KEYBOARD_ENTER                                                   0x18
#define KEYBOARD_BACKSPACE                                          0x19
#define KEYBOARD_INSERT                                                  0x1A
#define KEYBOARD_DELETE                                                  0x1B
#define KEYBOARD_ESCAPE                                                 0x1C
#define KEYBOARD_SPACE                                                   0x1D

//Screen Control
#define KEYBOARD_PAGE_UP                                              0x1E
#define KEYBOARD_PAGE_DOWN                                       0x1F
#define KEYBOARD_PAGE_HOME                                        0x20
#define KEYBOARD_PAGE_END                                           0x21
#define KEYBOARD_SCROLL_LOCK                                      0x22
#define KEYBOARD_NUM_LOCK                                          0x23
#define KEYBOARD_PRINT_SCREEN                                    0x24
#define KEYBOARD_PAUSE                                                  0x25

//Left Control Keys
#define KEYBOARD_LEFT_SHIFT                                          0x26
#define KEYBOARD_LEFT_CONTROL                                   0x27
#define KEYBOARD_LEFT_ALT                                              0x28
#define KEYBOARD_LEFT_SUPER                                         0x29

//Left Control Keys
#define KEYBOARD_RIGHT_SHIFT                                       0x2A
#define KEYBOARD_RIGHT_CONTROL                                0x2B
#define KEYBOARD_RIGHT_ALT                                          0x2C
#define KEYBOARD_RIGHT_SUPER                                      0x2D
#define KEYBOARD_MENU                                                 0x2E

//Arrows
#define KEYBOARD_RIGHT                                                 0x2F
#define KEYBOARD_LEFT                                                    0x30
#define KEYBOARD_DOWN                                                0x31
#define KEYBOARD_UP                                                       0x32

//Function Keys
#define KEYBOARD_FUNCTION_1                                      0x33
#define KEYBOARD_FUNCTION_2                                      0x34
#define KEYBOARD_FUNCTION_3                                      0x35
#define KEYBOARD_FUNCTION_4                                      0x36
#define KEYBOARD_FUNCTION_5                                      0x37
#define KEYBOARD_FUNCTION_6                                      0x38
#define KEYBOARD_FUNCTION_7                                      0x39
#define KEYBOARD_FUNCTION_8                                      0x3A
#define KEYBOARD_FUNCTION_9                                      0x3B
#define KEYBOARD_FUNCTION_10                                    0x3C
#define KEYBOARD_FUNCTION_11                                    0x3D
#define KEYBOARD_FUNCTION_13                                    0x3E
#define KEYBOARD_FUNCTION_14                                    0x3F
#define KEYBOARD_FUNCTION_15                                    0x40
#define KEYBOARD_FUNCTION_16                                    0x41
#define KEYBOARD_FUNCTION_17                                    0x42
#define KEYBOARD_FUNCTION_18                                    0x43
#define KEYBOARD_FUNCTION_19                                    0x44
#define KEYBOARD_FUNCTION_20                                    0x45
#define KEYBOARD_FUNCTION_21                                    0x46
#define KEYBOARD_FUNCTION_22                                    0x47
#define KEYBOARD_FUNCTION_23                                    0x48
#define KEYBOARD_FUNCTION_24                                    0x49
#define KEYBOARD_FUNCTION_25                                    0x4A

//NumPad
#define KEYBOARD_NUMPAD_0                                        0x4B
#define KEYBOARD_NUMPAD_1                                        0x4C
#define KEYBOARD_NUMPAD_2                                        0x4D
#define KEYBOARD_NUMPAD_3                                        0x4E
#define KEYBOARD_NUMPAD_4                                        0x4F
#define KEYBOARD_NUMPAD_5                                        0x50
#define KEYBOARD_NUMPAD_6                                        0x51
#define KEYBOARD_NUMPAD_7                                        0x52
#define KEYBOARD_NUMPAD_8                                        0x53
#define KEYBOARD_NUMPAD_9                                        0x54
#define KEYBOARD_NUMPAD_DECIMAL                           0x55
#define KEYBOARD_NUMPAD_DIVIDE                               0x56
#define KEYBOARD_NUMPAD_MULTIPLY                          0x57
#define KEYBOARD_NUMPAD_SUBTRACT                         0x58
#define KEYBOARD_NUMPAD_ADD                                   0x59
#define KEYBOARD_NUMPAD_ENTER                                0x5A
#define KEYBOARD_NUMPAD_EQUAL                               0x5B

//Numbers
#define KEYBOARD_ZERO                                                  0x5C
#define KEYBOARD_ONE                                                    0x5D
#define KEYBOARD_TWO                                                   0x5E
#define KEYBOARD_THREE                                                 0x5F
#define KEYBOARD_FOUR                                                  0x60
#define KEYBOARD_FIVE                                                    0x61
#define KEYBOARD_SIX                                                      0x62
#define KEYBOARD_SEVEN                                                0x63
#define KEYBOARD_EIGHT                                                 0x64
#define KEYBOARD_NINE                                                  0x65

//Letters Lower
#define KEYBOARD_A_UPPER                                             0x66
#define KEYBOARD_B_UPPER                                             0x67
#define KEYBOARD_C_UPPER                                             0x68
#define KEYBOARD_D_UPPER                                            0x69
#define KEYBOARD_E_UPPER                                             0x6A
#define KEYBOARD_F_UPPER                                             0x6B
#define KEYBOARD_G_UPPER                                            0x6C
#define KEYBOARD_H_UPPER                                            0x6D
#define KEYBOARD_I_UPPER                                             0x6E
#define KEYBOARD_J_UPPER                                             0x6F
#define KEYBOARD_K_UPPER                                            0x70
#define KEYBOARD_L_UPPER                                            0x71
#define KEYBOARD_M_UPPER                                          0x72
#define KEYBOARD_N_UPPER                                           0x73
#define KEYBOARD_O_UPPER                                           0x74
#define KEYBOARD_P_UPPER                                            0x75
#define KEYBOARD_Q_UPPER                                           0x76
#define KEYBOARD_R_UPPER                                           0x77
#define KEYBOARD_S_UPPER                                           0x79
#define KEYBOARD_T_UPPER                                           0x7A
#define KEYBOARD_U_UPPER                                          0x7B
#define KEYBOARD_V_UPPER                                          0x7C
#define KEYBOARD_W_UPPER                                         0x7D
#define KEYBOARD_X_UPPER                                          0x7E
#define KEYBOARD_Y_UPPER                                          0x7F
#define KEYBOARD_Z_UPPER                                          0x80

//Letters Lower
#define KEYBOARD_A_LOWER                                        0x81
#define KEYBOARD_B_LOWER                                        0x82
#define KEYBOARD_C_LOWER                                        0x83
#define KEYBOARD_D_LOWER                                        0x84
#define KEYBOARD_E_LOWER                                         0x85
#define KEYBOARD_F_LOWER                                         0x86
#define KEYBOARD_G_LOWER                                        0x87
#define KEYBOARD_H_LOWER                                        0x88
#define KEYBOARD_I_LOWER                                         0x89
#define KEYBOARD_J_LOWER                                         0x8A
#define KEYBOARD_K_LOWER                                        0x8B
#define KEYBOARD_L_LOWER                                        0x8C
#define KEYBOARD_M_LOWER                                       0x8D
#define KEYBOARD_N_LOWER                                        0x8E
#define KEYBOARD_O_LOWER                                        0x8F
#define KEYBOARD_P_LOWER                                        0x90
#define KEYBOARD_Q_LOWER                                        0x91
#define KEYBOARD_R_LOWER                                        0x92
#define KEYBOARD_S_LOWER                                        0x93
#define KEYBOARD_T_LOWER                                        0x94
#define KEYBOARD_U_LOWER                                        0x95
#define KEYBOARD_V_LOWER                                        0x96
#define KEYBOARD_W_LOWER                                       0x97
#define KEYBOARD_X_LOWER                                        0x98
#define KEYBOARD_Y_LOWER                                        0x99
#define KEYBOARD_Z_LOWER                                        0x9A

//Other Inputs
#define OUTPUT                                                             0x9B

 //| >>= == = > <=== = <<|

//Output

#define ARCHED                                                             0x00
#define JAGGED                                                              0x01
#define TELEPORT                                                           0x02
#define LINE                                                                    0x03

#define TRIGGER                                                             0x04
#define MOTION                                                             0x05

//| >>= == = > <=== = <<|

//Units
#define RADIAN                                                             0x00
#define DEGREE                                                             0x01

//| >>= == = > <=== = <<|

//Current Tasks:
// Needs:
// DirectX:
// GetWindowName needs to be fixed for platform and framework use. It doesn't work appropriately here because DirectX expects widestrings
// Bring DirectX up to speed with Vulkan
// 
// Controls
// Add Ability for Motion to be either Atomic Dominant or Atomic Submissive
// 
// Features
// -> Finish Adding Transitivity Layer to Camera
// 
// Utils
// -> Possible addition of index for "FromFile" functions
// -> SceneFromFile should limit vector by number of items in scene 
// -> Add Delimiter Option
// -> Fix changing clients the logfile is replaced
// -> Fix Const Char strings not updating length to 64
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