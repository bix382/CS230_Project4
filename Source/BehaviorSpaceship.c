//------------------------------------------------------------------------------
//
// File Name:	BehaviorSpaceship.c
// Author(s):	Sam Normile (sam.normile)
// Project:		Project 3
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorSpaceship.h"
#include "Behavior.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "DGL.h"
#include "Entity.h"
#include "Physics.h"
#include "Transform.h"
#include "Vector2D.h"
#include "EntityFactory.h"
#include "EntityContainer.h"
#include "Scene.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

static const float spaceshipAcceleration = 150.0f;
static const float spaceshipSpeedMax = 500.0f;
static const float spaceshipTurnRateMax = (float)M_PI / 1.5f;
static const float spaceshipWeaponCooldownTime = 0.034f;
static const float spaceshipWeaponBulletSpeed = 750.0f;


//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

enum SpaceshipStates {
	cSpaceshipInvalid = -1,
	cSpaceshipIdle,
	cSpaceshipThrust
};

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

static void BehaviorSpaceshipOnInit(Behavior* behavior);
static void BehaviorSpaceshipOnUpdate(Behavior* behavior, float dt);
static void BehaviorSpaceshipOnExit(Behavior* behavior);
static void BehaviorSpaceshipUpdateRotation(Behavior* behavior, float dt);
static void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt);
static void BehaviorSpaceshipUpdateWeapon(Behavior* behavior, float dt);
static void BehaviorSpaceshipSpawnBullet(Behavior* behavior);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Spaceship) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorSpaceshipCreate(void) {
	Behavior* spaceship = calloc(1, sizeof(Behavior));
	if (spaceship) {
		spaceship->stateCurr = cSpaceshipInvalid;
		spaceship->stateNext = cSpaceshipInvalid;
		spaceship->onInit = BehaviorSpaceshipOnInit;
		spaceship->onUpdate = BehaviorSpaceshipOnUpdate;
		spaceship->onExit = BehaviorSpaceshipOnExit;
	}
	return spaceship;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void BehaviorSpaceshipOnInit(Behavior* behavior) {
	UNREFERENCED_PARAMETER(behavior);
}

static void BehaviorSpaceshipOnUpdate(Behavior* behavior, float dt) {
	if (behavior) {
		switch (behavior->stateCurr) {
			case cSpaceshipIdle:
				BehaviorSpaceshipUpdateRotation(behavior, dt);
				BehaviorSpaceshipUpdateWeapon(behavior, dt);
				if (DGL_Input_KeyDown(VK_UP)) {
					behavior->stateNext = cSpaceshipThrust;
				}
				break;
			case cSpaceshipThrust:
	 			BehaviorSpaceshipUpdateRotation(behavior, dt);
				BehaviorSpaceshipUpdateVelocity(behavior, dt);
				BehaviorSpaceshipUpdateWeapon(behavior, dt);
				if (!DGL_Input_KeyDown(VK_UP)) {
					behavior->stateNext = cSpaceshipIdle;
				}
				break;
		}
	}
}

static void BehaviorSpaceshipOnExit(Behavior* behavior) {
	UNREFERENCED_PARAMETER(behavior);
}

static void BehaviorSpaceshipUpdateRotation(Behavior* behavior, float dt) {
	UNREFERENCED_PARAMETER(dt);

	if (behavior) {
		Physics* phys = EntityGetPhysics(behavior->parent);
		if (DGL_Input_KeyDown(VK_LEFT)) {
			PhysicsSetRotationalVelocity(phys, spaceshipTurnRateMax);
		}
		else if (DGL_Input_KeyDown(VK_RIGHT)) {
			PhysicsSetRotationalVelocity(phys, -spaceshipTurnRateMax);
		}
		else {
			PhysicsSetRotationalVelocity(phys, 0.f);
		}
	}
}

static void BehaviorSpaceshipUpdateVelocity(Behavior* behavior, float dt) {
	if (behavior) {
		Transform* trans = EntityGetTransform(behavior->parent);
		Physics* phys = EntityGetPhysics(behavior->parent);

		if (trans && phys) {
			float rot = TransformGetRotation(trans);
			Vector2D dirOfR = { 0 };
			Vector2DFromAngleRad(&dirOfR, rot);

			Vector2D velocity = *PhysicsGetVelocity(phys);
			Vector2DScaleAdd(&velocity, &dirOfR, spaceshipAcceleration * dt, &velocity);
			
			float speed = Vector2DLength(&velocity);
			if (speed > spaceshipSpeedMax) {
				Vector2DScale(&velocity, &velocity, spaceshipSpeedMax / speed);
			}

			PhysicsSetVelocity(phys, &velocity);
		}
	}
}

static void BehaviorSpaceshipUpdateWeapon(Behavior* behavior, float dt) {
	if (behavior) {
		if (behavior->timer > 0) {
			behavior->timer -= dt;
			if (behavior->timer < 0) {
				behavior->timer = 0;
			}
		}
		if (DGL_Input_KeyDown(VK_SPACE)) {
			if (behavior->timer <= 0) {
				BehaviorSpaceshipSpawnBullet(behavior);
				behavior->timer = spaceshipWeaponCooldownTime;
			}
		}
	}
}

static void BehaviorSpaceshipSpawnBullet(Behavior* behavior) {
	if (behavior) {
		Entity* bullet = EntityFactoryBuild("Bullet");
		if (bullet) {
			Transform* shipTrans = EntityGetTransform(behavior->parent);
			Transform* bulletTrans = EntityGetTransform(bullet);

			Vector2D shipPos = *TransformGetTranslation(shipTrans);
			float shipRot = TransformGetRotation(shipTrans);

			TransformSetTranslation(bulletTrans, &shipPos);
			TransformSetRotation(bulletTrans, shipRot);

			//Vector2D bulletPos = *TransformGetTranslation(bulletTrans);
			//float bulletRot = TransformGetRotation(bulletTrans);

			Vector2D shipDirOfR = { 0 };
			Vector2DFromAngleRad(&shipDirOfR, shipRot);

			Physics* bulletPhys = EntityGetPhysics(bullet);
			Vector2DScale(&shipDirOfR, &shipDirOfR, spaceshipWeaponBulletSpeed);
			PhysicsSetVelocity(bulletPhys, &shipDirOfR);

			SceneAddEntity(bullet);
		}
	}
}