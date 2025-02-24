//------------------------------------------------------------------------------
//
// File Name:	Level1Scene.c
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
#include "Level1Scene.h"
#include "Stream.h"
#include "Level2Scene.h"
#include "Trace.h"
#include "Mesh.h"
#include "SpriteSource.h"
#include "Entity.h"
#include "Vector2D.h"
#include "EntityFactory.h"
#include "Sprite.h"
#include "Physics.h"
#include "Transform.h"
#include "SandboxScene.h"
#include "DemoScene.h"
#include "Engine.h"
#include "Animation.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const float groundHeight = -150.0f;
static const float moveVelocity = 500.0f;
static const float jumpVelocity = 1000.0f;
static const Vector2D gravityNormal = { 0.0f, -1500.0f };
static const Vector2D gravityNone = { 0.0f, 0.0f };
static const enum MonkeyStates { MonkeyInvalid = -1, MonkeyIdle = 0, MonkeyWalk = 1, MonkeyJump = 2 } MonkeyStates;
static const float wallDistance = 462.0f;
static const float CheckSquareDistance = (75.0f * 75.0f);

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

typedef struct Level1Scene
{
	// WARNING: The base class must always be included first.
	Scene	base;

	// Add any scene-specific variables second.
	int numLives;
	Mesh* meshPtr; //1x1
	SpriteSource* spriteSourcePtr;
	Entity* entityPtr;
	enum MonkeyStates monkeyState;
	char livesBuffer[16];

	Mesh* monkeyMesh; //3x3
	Entity* monkeyEntity;
	SpriteSource* monkeyIdle;
	SpriteSource* monkeyJump;
	SpriteSource* monkeyWalk;

	Mesh* livesMesh; //16x8
	SpriteSource* livesSpriteSource;
	Entity* livesEntity;

} Level1Scene;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void Level1SceneLoad(void);
static void Level1SceneInit(void);
static void Level1SceneUpdate(float dt);
static void Level1SceneExit(void);
static void Level1SceneUnload(void);
static void Level1SceneRender(void);

//------------------------------------------------------------------------------
// Instance Variable:
//------------------------------------------------------------------------------

static Level1Scene instance =
{
	// Initialize the base structure:
	{ "Level1", Level1SceneLoad, Level1SceneInit, Level1SceneUpdate, Level1SceneRender, Level1SceneExit, Level1SceneUnload },

	// Initialize any scene-specific variables:
	0
};

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Get the instance of the Level1 Scene.
// Returns:
//	 Pointer to the base Scene structure for this derived Scene.
const Scene* Level1SceneGetInstance(void)
{
	return &(instance.base);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


static void Level1SceneSetMonkeyState(Entity* entity, enum MonkeyStates newState) {
	if (entity && instance.monkeyState != newState) {
		instance.monkeyState = newState;
		//instance.monkeyState = MonkeyIdle;
		Sprite* sprite = EntityGetSprite(entity);
		Animation* ani = EntityGetAnimation(entity);
		if (sprite && ani) {
			switch (newState) {
				case MonkeyIdle:
					SpriteSetMesh(sprite, instance.meshPtr);
					SpriteSetSpriteSource(sprite, instance.monkeyIdle);
					AnimationPlay(ani, 1, 0.f, false);
					break;
				case MonkeyWalk:
					SpriteSetMesh(sprite, instance.monkeyMesh);
					SpriteSetSpriteSource(sprite, instance.monkeyWalk);
					AnimationPlay(ani, 8, .05f, true);
					break;
				case MonkeyJump:
					SpriteSetMesh(sprite, instance.meshPtr);
					SpriteSetSpriteSource(sprite, instance.monkeyJump);
					AnimationPlay(ani, 1, 0.f, false);
					break;
			}
		}
	}
}

// Handle entity movement
static void Level1SceneMovementController(Entity* entity) {
	
	Physics* entityPhys = EntityGetPhysics(entity);
	Transform* entityTrans = EntityGetTransform(entity);
	if (entityPhys && entityTrans) {
		Vector2D velocity = *PhysicsGetVelocity(entityPhys);
		if (DGL_Input_KeyDown(VK_LEFT)) {
			velocity.x = -moveVelocity;
			if (instance.monkeyState != MonkeyJump) {
				Level1SceneSetMonkeyState(entity, MonkeyWalk);
			}
		}
		else if (DGL_Input_KeyDown(VK_RIGHT)) {
			velocity.x = moveVelocity;
			if (instance.monkeyState != MonkeyJump) {
				Level1SceneSetMonkeyState(entity, MonkeyWalk);
			}
		}
		else {
			velocity.x = 0;
			if (instance.monkeyState != MonkeyJump) {
				Level1SceneSetMonkeyState(entity, MonkeyIdle);
			}
		}
		if (DGL_Input_KeyTriggered(VK_UP)) {
			velocity.y = jumpVelocity;
			PhysicsSetAcceleration(entityPhys, &gravityNormal);
			Level1SceneSetMonkeyState(entity, MonkeyJump);
		}
		Vector2D translation = *TransformGetTranslation(entityTrans);
		if (translation.y < groundHeight) {
			TransformSetTranslation(entityTrans, &(Vector2D) {translation.x, groundHeight});
			velocity.y = 0;
			PhysicsSetAcceleration(entityPhys, &gravityNone);
			Level1SceneSetMonkeyState(entity, MonkeyIdle);
		}
		PhysicsSetVelocity(entityPhys, &velocity); 
	}
}


static void Level1SceneBounceController(Entity* entity) {
	Physics* phys = EntityGetPhysics(entity);
	Transform* trans = EntityGetTransform(entity);
	if (phys && trans) {
		Vector2D pos = *TransformGetTranslation(trans);
		Vector2D velocity = *PhysicsGetVelocity(phys);
		if (pos.x <= -wallDistance) {
			pos.x = -wallDistance;
			velocity.x = -velocity.x;
		}
		if (pos.x >= wallDistance) {
			pos.x = wallDistance;
			velocity.x = -velocity.x;
		}
		if (pos.y <= groundHeight) {
			pos.y = groundHeight + (groundHeight - pos.y);
			velocity.y = -velocity.y;
		}
		PhysicsSetVelocity(phys, &velocity);
		TransformSetTranslation(trans, &pos);
	}
}

static bool Level1SceneIsColliding(const Entity* entityA, const Entity* entityB) {
	Transform* entityATrans = EntityGetTransform(entityA);
	const Vector2D* entityAPos = TransformGetTranslation(entityATrans);
	Transform* entityBTrans = EntityGetTransform(entityB);
	const Vector2D* entityBPos = TransformGetTranslation(entityBTrans);
	
	float dist = Vector2DSquareDistance(entityAPos, entityBPos);
	if (dist <= CheckSquareDistance) {
		return true;
	}
	return false;
}

// Load any resources used by the scene.
static void Level1SceneLoad(void)
{
	Stream lvl1LivesFile = StreamOpen("./Data/Level1_Lives.txt");
	if (lvl1LivesFile) {
		instance.numLives = StreamReadInt(lvl1LivesFile);
		StreamClose(&lvl1LivesFile);
	}
	else {
		TraceMessage("Error: Level1SceneLoad: could not open file");
	}

	instance.meshPtr = MeshCreate();
	MeshBuildQuad(instance.meshPtr, 0.5f, 0.5f, 1.f, 1.f, "Mesh1x1");
	instance.spriteSourcePtr = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.spriteSourcePtr, 1, 1, "PlanetTexture.png");

	instance.monkeyMesh = MeshCreate();
	MeshBuildQuad(instance.monkeyMesh, 0.5f, 0.5f, 1.f / 3, 1.f / 3, "Mesh3x3");

	instance.livesMesh = MeshCreate();
	MeshBuildQuad(instance.livesMesh, 0.5f, 0.5f, 1.f / 16, 1.f / 8, "Mesh16x8");

	instance.monkeyIdle = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.monkeyIdle, 1, 1, "MonkeyIdle.png");

	instance.monkeyJump = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.monkeyJump, 1, 1, "MonkeyJump.png");

	instance.monkeyWalk = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.monkeyWalk, 3, 3, "MonkeyWalk.png");

	instance.livesSpriteSource = SpriteSourceCreate();
	SpriteSourceLoadTexture(instance.livesSpriteSource, 16, 8, "Roboto_Mono_black.png");
}

// Initialize the entities and variables used by the scene.
static void Level1SceneInit()
{
	instance.entityPtr = EntityFactoryBuild("./Data/PlanetBounce.txt");
	if (instance.entityPtr) {
		Sprite* planetSprite = EntityGetSprite(instance.entityPtr);
		SpriteSetMesh(planetSprite, instance.meshPtr);
		SpriteSetSpriteSource(planetSprite, instance.spriteSourcePtr);
		SpriteSetFrame(planetSprite, 0);
	}

	instance.monkeyEntity = EntityFactoryBuild("./Data/Monkey.txt");
	if (instance.monkeyEntity) {
		instance.monkeyState = MonkeyInvalid;
		Level1SceneSetMonkeyState(instance.monkeyEntity, MonkeyIdle);
	}

	instance.livesEntity = EntityFactoryBuild("./Data/MonkeyLivesText.txt");
	if (instance.livesEntity) {
		Sprite* sprite = EntityGetSprite(instance.livesEntity);
		SpriteSetMesh(sprite, instance.livesMesh);
		SpriteSetSpriteSource(sprite, instance.livesSpriteSource);
		sprintf_s(instance.livesBuffer, sizeof(instance.livesBuffer), "Lives: %d", instance.numLives);
		SpriteSetText(sprite, instance.livesBuffer);
	}

	DGL_Graphics_SetBackgroundColor(&(DGL_Color) { 1, 1, 1, 1 });
	DGL_Graphics_SetBlendMode(DGL_BM_BLEND);
}

// Update the the variables used by the scene.
// Params:
//	 dt = Change in time (in seconds) since the last game loop.
static void Level1SceneUpdate(float dt)
{
	// Tell the compiler that the 'dt' variable is unused.
	//UNREFERENCED_PARAMETER(dt);
	
	Level1SceneMovementController(instance.monkeyEntity);
	Level1SceneBounceController(instance.entityPtr);
	EntityUpdate(instance.entityPtr, dt);
	EntityUpdate(instance.monkeyEntity, dt);
	EntityUpdate(instance.livesEntity, dt);
	if (Level1SceneIsColliding(instance.monkeyEntity, instance.entityPtr)) {
		instance.numLives--;
		if (instance.numLives <= 0) {
			SceneSystemSetNext(Level2SceneGetInstance());
		}
		else {
			SceneSystemRestart();
		}
	}

	if (DGL_Input_KeyTriggered(VK_ESCAPE)) {
		EngineExit();
	}
}

// Render any objects associated with the scene.
void Level1SceneRender(void)
{
	EntityRender(instance.entityPtr);
	EntityRender(instance.livesEntity);
	EntityRender(instance.monkeyEntity);
}

// Free any objects associated with the scene.
static void Level1SceneExit()
{
	EntityFree(&instance.entityPtr);
	EntityFree(&instance.monkeyEntity);
	EntityFree(&instance.livesEntity);
}

// Unload any resources used by the scene.
static void Level1SceneUnload(void)
{
	SpriteSourceFree(&instance.spriteSourcePtr);
	SpriteSourceFree(&instance.monkeyIdle);
	SpriteSourceFree(&instance.monkeyWalk);
	SpriteSourceFree(&instance.monkeyJump);
	SpriteSourceFree(&instance.livesSpriteSource);

	MeshFree(&instance.meshPtr);
	MeshFree(&instance.monkeyMesh);
	MeshFree(&instance.livesMesh);
}
