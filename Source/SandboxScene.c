//------------------------------------------------------------------------------
//
// File Name:	SandboxScene.c
// Author(s):	Sam Normile (sam.normile)
// Project:		Project 3
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Scene.h"
#include "SceneSystem.h"
#include "SandboxScene.h"
#include "Trace.h"
#include "Vector2D.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct SandboxScene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.

} SandboxScene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void SandboxSceneLoad(void);
static void SandboxSceneInit(void);
static void SandboxSceneUpdate(float dt);
static void SandboxSceneExit(void);
static void SandboxSceneUnload(void);
static void SandboxSceneRender(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static SandboxScene instance =
{
	// Initialize the base structure:
	{ "Sandbox", SandboxSceneLoad, SandboxSceneInit, SandboxSceneUpdate, SandboxSceneRender, SandboxSceneExit, SandboxSceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Sandbox Scene.
// Returns:
//	 Pointer to the base Scene structure for this derived Scene.
const Scene* SandboxSceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

//Write float values to trace file
//static void SandboxSceneTraceFloat(const char* text, float f) {
//	TraceMessage("Vector Test: %s = %f", text, f);
//}
//
////Write Vector2D values to trace file
//static void SandboxSceneTraceVector(const char* text, const Vector2D* v) {
//	TraceMessage("Vector Test: %s = [%f, %f]", text, v->x, v->y);
//}

// Load any resources used by the scene.
static void SandboxSceneLoad(void)
{
}

// Initialize the entities and variables used by the scene.
static void SandboxSceneInit()
{
}

// Update the the variables used by the scene.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void SandboxSceneUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);

	

	// NOTE: This call causes the engine to exit immediately.  Make sure to remove
	//   it when you are ready to test out a new scene.
	SceneSystemSetNext(NULL);
}

// Render any objects associated with the scene.
void SandboxSceneRender(void)
{
}

// Free any objects associated with the scene.
static void SandboxSceneExit()
{
}

// Unload any resources used by the scene.
static void SandboxSceneUnload(void)
{
}