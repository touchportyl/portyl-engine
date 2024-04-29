#pragma once

// FlexEngine Main Include File
// This file includes all the necessary files to use the FlexEngine library.
// 
// Sections:
// - Core  . . . . . . The core of the engine. Required for all applications.
// - Tools . . . . . . Tools for logging, timing, and other utilities.
// - Wrappers  . . . . Wrappers for external libraries and tools.
// - Data Structures . Data structures used in the engine.


/* |----------------------------| */
/* |----------- Core -----------| */
/* |----------------------------| */

// Contains the application class.
// Inherit from FlexEngine::Application in the entry point to create a new application.
#include "FlexEngine/Core/application.h"

// Contains the layer class.
// Inherit from FlexEngine::Layer to create a new layer.
// Layers are used to separate different parts of the application.
// Layers are updated and rendered in the order they are pushed.
#include "FlexEngine/Core/layer.h"


/* |-----------------------------| */
/* |----------- Tools -----------| */
/* |-----------------------------| */

// Logger for logging messages to a file
// 
// Use Log::Debug for debugging information. (only logged in debug mode)
// Use Log::Info for general information.
// Use Log::Warning for warnings.
// Use Log::Error for errors.
// Use Log::Fatal for fatal errors. (this will terminate the application)
// 
// Flow logging is used to log the flow of the application and track
// the flow of systems within the application.
// 
// Use FLX_FLOW_FUNCTION() to log the current function in the flow.
// Use FLX_FLOW_BEGINSCOPE() to log the start of a system scope flow.
// Use FLX_FLOW_ENDSCOPE() to log the end of a system scope flow.
#include "FlexEngine/flexlogger.h"

// Assertion wrapper for handling fatal errors.
// These are used to check for destructive errors like memory leaks.
// The application will be terminated if an assertion fails.
// This hooks to the logger to log the error before terminating.
// 
// Use FLX_INTERNAL_ASSERT for assertions before the application is functional.
// Use FLX_CORE_ASSERT for assertions for the application core.
// Use FLX_ASSERT in place of regular assertions.
// Use FLX_NULLPTR_ASSERT for checking pointers.
#include "FlexEngine/Wrapper/flexassert.h"

// Formatter for storing metadata in files.
// Uses a simple key-value pair system to store metadata in files.
#include "FlexEngine/flexformatter.h"

// UUID class for generating unique identifiers.
#include "FlexEngine/uuid.h"

// Scoped timer to measure time taken by functions.
// Macros are provided to measure the time taken by a function.
#include "FlexEngine/timer.h"

// Math functions and constants.
// Contains constants like PI and EPSILON.
// Implements Vector2, Vector3, Vector4.
#include "FlexEngine/FlexMath/vector2.h"
#include "FlexEngine/FlexMath/vector3.h"
#include "FlexEngine/FlexMath/vector4.h"

// Used to manage states.
// Make your own states by inheriting from FlexEngine::State.
// Macros are provided to make the state management easier.
#include "FlexEngine/StateManager/statemanager.h"

// Implementation of Unity's PlayerPrefs.
// Used to store and retrieve data from the playerprefs.json config file.
// Set float, int, string, and bool values.
#include "FlexEngine/playerprefs.h"

// Input handling abstracted from GLFW.
// Use GLFW key codes.
// Currently does not support gamepads.
#include "FlexEngine/input.h"


/* |-----------------------------| */
/* |---------- Wrappers ---------| */
/* |-----------------------------| */

// Wraps the ImGui library for easier use.
// Includes the ImGui library.
#include "FlexEngine/Core/imguiwrapper.h"

// Handles the creation and destruction of vertex and index buffers.
// This is platform-independent and can be used with any graphics API.
// Although the current implementation is exclusively for OpenGL.
// Use the Create function to create a new buffer.
#include "FlexEngine/Renderer/buffer.h"
#include "FlexEngine/Renderer/OpenGL/openglbuffer.h"

// Contains the path, file, and filelist class.
// Has the Extension namespace with a list of safe and supported extensions.
// Handles the functionality to read and write files.
// The path class is used to ensure the validity of the path. Can throw exceptions.
#include "FlexEngine/Wrapper/path.h"
#include "FlexEngine/Wrapper/file.h"
#include "FlexEngine/Wrapper/filelist.h"

// Wrapper for a range of values.
// Use Get() to generate random values within the range.
// The number that is generated is inclusive of the min and max values.
// Can also be used to store a range of values by getting min and max.
#include "FlexEngine/Wrapper/range.h"

// OpenGL shaders handling.
// Load and compile shaders from files.
#include "FlexEngine/Wrapper/shaders.h"

// OpenGL texture handling.
// Load textures from files.
#include "FlexEngine/Wrapper/textures.h"


/* |-----------------------------| */
/* |------ Data Structures ------| */
/* |-----------------------------| */

// Contains the FlexECS system.
// This uses the Archetype-based Entity-Component-System architecture.
#include "FlexEngine/FlexECS/datastructures.h"

// Two way queue for storing and executing functions.
#include "FlexEngine/DataStructures/functionqueue.h"

// Specialized function queue for storing and executing functions for freeing
// heap objects after execution.
#include "FlexEngine/DataStructures/freequeue.h"
