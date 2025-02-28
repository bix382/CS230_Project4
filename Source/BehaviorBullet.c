//------------------------------------------------------------------------------
//
// File Name:	BehaviorBullet.c
// Author(s):	Sam Normile (sam.normile)
// Project:		Project 3
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "BehaviorBullet.h"
#include "Behavior.h"
#include "Entity.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

enum BulletStates {
	cBulletInvalid = -1, cBulletIdle
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

static void BehaviorBulletOnInit(Behavior* behavior);
static void BehaviorBulletOnUpdate(Behavior* behavior, float dt);
static void BehaviorBulletOnExit(Behavior* behavior);
static void BehaviorBulletUpdateLifeTimer(Behavior* behavior, float dt);

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new (Bullet) behavior component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
Behavior* BehaviorBulletCreate(void) {
	Behavior* bullet = calloc(1, sizeof(Behavior));
	if (bullet) {
		bullet->stateCurr = cBulletInvalid;
		bullet->stateNext = cBulletInvalid;
		bullet->onInit = BehaviorBulletOnInit;
		bullet->onUpdate = BehaviorBulletOnUpdate;
		bullet->onExit = BehaviorBulletOnExit;
	}
	return bullet;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

static void BehaviorBulletOnInit(Behavior* behavior) {
	UNREFERENCED_PARAMETER(behavior);
}

static void BehaviorBulletOnUpdate(Behavior* behavior, float dt) {
	if (behavior) {
		switch (behavior->stateCurr) {
			case cBulletIdle:
				BehaviorBulletUpdateLifeTimer(behavior, dt);
				break;
		}
	}
}

static void BehaviorBulletOnExit(Behavior* behavior) {
	UNREFERENCED_PARAMETER(behavior);
}

static void BehaviorBulletUpdateLifeTimer(Behavior* behavior, float dt) {
	if (behavior) {
		if (behavior->timer > 0) {
			behavior->timer -= dt;
			if (behavior->timer <= 0) {
				EntityDestroy(behavior->parent);
			}
		}
	}
}
