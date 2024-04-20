#pragma once



/* |--------------------------| */
/* |---------- Core ----------| */
/* |--------------------------| */

// Contains the application class.
// Inherit from FlexEngine::Application in the entry point to create a new application.
#include "FlexEngine/Core/application.h"

// Contains the layer class.
// Inherit from FlexEngine::Layer to create a new layer.
// Layers are used to separate different parts of the application.
// Layers can be pushed and popped from the layer stack.
// Layers are updated and rendered in the order they are pushed.
#include "FlexEngine/Core/layer.h"

// Wraps the ImGui library for easier use.
// Includes the ImGui library.
#include "FlexEngine/Core/imguiwrapper.h"

// Contains the scene class.
// Scenes are used to manage entities and systems.
// Scenes are used to load and unload levels.
// Scenes are used to update and render entities.
//#include "FlexEngine/Core/scene.h"

// Contains the gameobject class.
// GameObjects are used to represent objects in the game world.
// Components can be attached to GameObjects.
// GameObjects are managed by the scene class.
//#include "FlexEngine/Core/gameobject.h"

// Contains all the components that can be attached to game objects.
//#include "FlexEngine/Core/components.h"


/* |---------------------------| */
/* |---------- Tools ----------| */
/* |---------------------------| */

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
#include "FlexEngine/flexassert.h"

// UUID class for generating unique identifiers.
// Get the UUID by casting it to a hash, string, or by
// using the << operator.
// Hash is an unsigned 64-bit integer. (unsigned long long)
#include "FlexEngine/uuid.h"

// Scoped timer to measure time taken by functions.
// Call the constructor at the start of the scope to start the timer.
// Call the destructor at the end to end the timer and print the
// time taken to the log.
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

// Two way queue for storing and executing functions.
#include "FlexEngine/functionqueue.h"

// Specialized function queue for storing and executing functions for freeing
// heap objects after execution.
#include "FlexEngine/freequeue.h"


/* |-----------------------------| */
/* |---------- Loaders ----------| */
/* |-----------------------------| */

// File handling.
// Read and write files.
#include "FlexEngine/Loader/files.h"

// OpenGL shaders handling.
// Load and compile shaders from files.
#include "FlexEngine/Loader/shaders.h"

// OpenGL texture handling.
// Load textures from files.
#include "FlexEngine/Loader/textures.h"
