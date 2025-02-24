//------------------------------------------------------------------------------
//
// File Name:	Level2Scene.c
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
#include "Level2Scene.h"
#include "Stream.h"
#include "Trace.h"
#include "SandboxScene.h"
#include "Mesh.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "Sprite.h"
#include "DGL.h"
#include "Vector2D.h"
#include "DemoScene.h"
#include "Level1Scene.h"
#include "Engine.h"
#include "Transform.h"
#include "Physics.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const float spaceshipSpeed = 500.f;

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Level2Scene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.
	Mesh* meshPtr;
	Entity* entityPtr;

} Level2Scene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void Level2SceneLoad(void);
static void Level2SceneInit(void);
static void Level2SceneUpdate(float dt);
static void Level2SceneExit(void);
static void Level2SceneUnload(void);
static void Level2SceneRender(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static Level2Scene instance =
{
	// Initialize the base structure:
	{ "Level2", Level2SceneLoad, Level2SceneInit, Level2SceneUpdate, Level2SceneRender, Level2SceneExit, Level2SceneUnload },

	// Initialize any scene-specific variables:
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Level2 Scene.
// Returns:
//	 Pointer to the base Scene structure for this derived Scene.
const Scene* Level2SceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

//Handle entity movement
static void Level2SceneMovementController(Entity* entity) {
	Physics* physics = EntityGetPhysics(entity);
	Transform* transform = EntityGetTransform(entity);
	if (physics && transform) {
		Vector2D mouse = DGL_Input_GetMousePosition();
		mouse = DGL_Camera_ScreenCoordToWorld(&mouse);
		Vector2D direction;
		Vector2DSub(&direction, &mouse, TransformGetTranslation(transform));
		Vector2DNormalize(&direction, &direction);
		TransformSetRotation(transform, Vector2DToAngleRad(&direction));

		Vector2D vect;
		Vector2DScale(&vect, &direction, spaceshipSpeed);

		PhysicsSetVelocity(physics, &vect);
	}
}

// Load any resources used by the scene.
static void Level2SceneLoad(void)
{
	instance.meshPtr = MeshCreate();
	MeshBuildSpaceship(instance.meshPtr);
}

// Initialize the entities and variables used by the scene.
static void Level2SceneInit()
{
	instance.entityPtr = EntityFactoryBuild("./Data/SpaceshipHoming.txt");
	if (instance.entityPtr) {
		Sprite* sprite = EntityGetSprite(instance.entityPtr);
		SpriteSetMesh(sprite, instance.meshPtr);
	}
	DGL_Graphics_SetBackgroundColor(&(DGL_Color) { 0, 0, 0, 1 });
	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
}

// Update the the variables used by the scene.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void Level2SceneUpdate(float dt)
{
	Level2SceneMovementController(instance.entityPtr);
	EntityUpdate(instance.entityPtr, dt);

	if (DGL_Input_KeyTriggered('Z')) {
		SpriteSetAlpha(EntityGetSprite(instance.entityPtr), 0.5f);
	}
	else if (DGL_Input_KeyTriggered('X')) {
		SpriteSetAlpha(EntityGetSprite(instance.entityPtr), 1.f);
	}
	else if (DGL_Input_KeyTriggered(VK_ESCAPE)) {
		EngineExit();
	}
}

// Render any objects associated with the scene.
void Level2SceneRender(void)
{
	EntityRender(instance.entityPtr);
}

// Free any objects associated with the scene.
static void Level2SceneExit()
{
	EntityFree(&instance.entityPtr);
}

// Unload any resources used by the scene.
static void Level2SceneUnload(void)
{
	MeshFree(&instance.meshPtr);
}