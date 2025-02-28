//------------------------------------------------------------------------------
//
// File Name:	AsteroidsScene.c
// Author(s):	Sam Normile (sam.normile)
// Project:		
// 
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"

#include "Scene.h"
#include "SceneSystem.h"
#include "AsteroidsScene.h"
#include "DGL.h"
#include "EntityFactory.h"
#include "Entity.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct AsteroidsScene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.

} AsteroidsScene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void AsteroidsSceneLoad(void);
static void AsteroidsSceneInit(void);
static void AsteroidsSceneUpdate(float dt);
static void AsteroidsSceneExit(void);
static void AsteroidsSceneUnload(void);
static void AsteroidsSceneRender(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static AsteroidsScene instance =
{
	// Initialize the base structure:
	{ "Asteroids", AsteroidsSceneLoad, AsteroidsSceneInit, AsteroidsSceneUpdate, AsteroidsSceneRender, AsteroidsSceneExit, AsteroidsSceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Asteroids Scene.
// Returns:
//	 Pointer to the base Scene structure for this derived Scene.
const Scene* AsteroidsSceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

// Load any resources used by the scene.
static void AsteroidsSceneLoad(void)
{
}

// Initialize the entities and variables used by the scene.
static void AsteroidsSceneInit()
{
	DGL_Graphics_SetBackgroundColor(&(DGL_Color) { 0, 0, 0, 0 });
	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
	Entity* spaceship = EntityFactoryBuild("Spaceship");
	if (spaceship) {
		SceneAddEntity(spaceship);
	}
}

// Update the the variables used by the scene.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void AsteroidsSceneUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	UNREFERENCED_PARAMETER(dt);

	// NOTE: This call causes the engine to exit immediately.  Make sure to remove
	//   it when you are ready to test out a new scene.
	//SceneSystemSetNext(NULL);
}

// Render any objects associated with the scene.
void AsteroidsSceneRender(void)
{
}

// Free any objects associated with the scene.
static void AsteroidsSceneExit()
{
}

// Unload any resources used by the scene.
static void AsteroidsSceneUnload(void)
{
}

