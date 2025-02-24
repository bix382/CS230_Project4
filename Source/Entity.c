//------------------------------------------------------------------------------
//
// File Name:	Entity.c
// Author(s):	Sam Normile (sam.normile)
// Project:		Project 3
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Entity.h"
#include "Physics.h"
#include "Sprite.h"
#include "Transform.h"
#include "Stream.h"
#include "Animation.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Entity
{
	// The name of the entity.
	// A buffer is used to allow each entity to have a unique name.
	// The buffer is hardcoded to an arbitrary length that will be sufficient
	//	 for all CS230 assignments.  You may, instead, use dynamically-allocated
	//	 arrays for this purpose but the additional complexity is unnecessary
	//	 and it is your responsibility to ensure that the memory is successfully
	//	 allocated and deallocated in all possible situations.
	// [NOTE: When setting the name, use strcpy_s() to reduce the risk of
	//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
	//	 calling this function!  Instead, use the _countof() macro to get the
	//	 size of the "name" array.]
	char name[32];

	// Pointer to an attached Animation component.
	Animation* animation;

	// Pointer to an attached physics component.
	Physics* physics;

	// Pointer to an attached sprite component.
	Sprite* sprite;

	// Pointer to an attached transform component.
	Transform* transform;

} Entity;

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Dynamically allocate a new Entity.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Entity* EntityCreate(void) {
	Entity* entity = calloc(1, sizeof(Entity));
	return entity;
}

// Free the memory associated with an Entity.
// (NOTE: All attached components must be freed using the corresponding Free() functions.)
// (NOTE: The Entity pointer must be set to NULL.)
// Params:
//	 entity = Pointer to the Entity pointer.
void EntityFree(Entity** entity) {
	PhysicsFree(&(*entity)->physics);
	SpriteFree(&(*entity)->sprite);
	TransformFree(&(*entity)->transform);
	free(*entity);
	*entity = NULL;
}

// Read (and construct) the components associated with a entity.
// [NOTE: See project instructions for implementation instructions.]
// Params:
//	 entity = Pointer to the Entity.
//	 stream = The data stream used for reading.
void EntityRead(Entity* entity, Stream stream) {
	if (entity && stream) {
		const char* entityToken = StreamReadToken(stream);
		EntitySetName(entity, entityToken);
		while (1) {
			entityToken = StreamReadToken(stream);
			if (strncmp(entityToken, "Transform", _countof("Transform")) == 0) {
				Transform* transform = TransformCreate();
				TransformRead(transform, stream);
				EntityAddTransform(entity, transform);
			}
			else if (strncmp(entityToken, "Physics", _countof("Physics")) == 0) {
				Physics* physics = PhysicsCreate();
				PhysicsRead(physics, stream);
				EntityAddPhysics(entity, physics);
			}
			else if (strncmp(entityToken, "Sprite", _countof("Sprite")) == 0) {
				Sprite* sprite = SpriteCreate();
				SpriteRead(sprite, stream);
				EntityAddSprite(entity, sprite);
			}
			else if (strncmp(entityToken, "Animation", _countof("Animation")) == 0) {
				Animation* ani = AnimationCreate();
				AnimationRead(ani, stream);
				EntityAddAnimation(entity, ani);
			}
			else if (strncmp(entityToken, "", 0) == 0) {
				break;
			}
		}
	}
}

// Attach an Animation component to an Entity.
// (NOTE: This function must set the animation component's parent pointer by
//	  calling the AnimationSetParent() function.)
// Params:
//	 entity = Pointer to the Entity.
//   animation = Pointer to the Animation component to be attached.
void EntityAddAnimation(Entity* entity, Animation* animation) {
	if (entity && animation) {
		entity->animation = animation;
		AnimationSetParent(animation, entity);
	}
}

// Attach a Physics component to an Entity.
// Params:
//	 entity = Pointer to the Entity.
//   physics = Pointer to the Physics component to be attached.
void EntityAddPhysics(Entity* entity, Physics* physics) {
	if (entity) {
		entity->physics = physics;
	}
}

// Attach a sprite component to an Entity.
// Params:
//	 entity = Pointer to the Entity.
//   sprite = Pointer to the Sprite component to be attached.
void EntityAddSprite(Entity* entity, Sprite* sprite) {
	if (entity) {
		entity->sprite = sprite;
	}
}

// Attach a transform component to an Entity.
// Params:
//	 entity = Pointer to the Entity.
//   transform = Pointer to the Transform component to be attached.
void EntityAddTransform(Entity* entity, Transform* transform) {
	if (entity) {
		entity->transform = transform;
	}
}

// Set the Entity's name.
// [NOTE: Verify that both pointers are valid before setting the name.]
// [NOTE: When setting the name, use strcpy_s() to reduce the risk of
//	 buffer overruns. Additionally, do NOT hardcode the number "32" when
//	 calling this function!  Instead, use the _countof() macro to get the
//	 size of the "name" array.]
// Params:
//	 entity = Pointer to the Entity.
//	 name = Pointer to the Entity's new name.
void EntitySetName(Entity* entity, const char* name) {
	if (entity && name) {
		strcpy_s(entity->name, _countof(entity->name), name);
	}
}

// Get the Entity's name.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the Entity's name,
//		else return NULL.
const char* EntityGetName(const Entity* entity) {
	if (entity) {
		return entity->name;
	}
	return NULL;
}

// Get the Animation component attached to an Entity.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached Animation component,
//		else return NULL.
Animation* EntityGetAnimation(const Entity* entity) {
	if (entity) {
		return entity->animation;
	}
	return NULL;
}

// Get the Physics component attached to an Entity.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached physics component,
//		else return NULL.
Physics* EntityGetPhysics(const Entity* entity) {
	if (entity) {
		return entity->physics;
	}
	return NULL;
}

// Get the Sprite component attached to a Entity.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached Sprite component,
//		else return NULL.
Sprite* EntityGetSprite(const Entity* entity) {
	if (entity) {
		return entity->sprite;
	}
	return NULL;
}

// Get the Transform component attached to a Entity.
// Params:
//	 entity = Pointer to the Entity.
// Returns:
//	 If the Entity pointer is valid,
//		then return a pointer to the attached Transform component,
//		else return NULL.
Transform* EntityGetTransform(const Entity* entity) {
	if (entity) {
		return entity->transform;
	}
	return NULL;
}

// Update any components attached to the Entity.
// (Hint: You will need to call PhysicsUpdate().)
// (NOTE: You must first check for a valid pointer before calling this function.)
// Params:
//	 entity = Pointer to the Entity.
//	 dt = Change in time (in seconds) since the last game loop.
void EntityUpdate(Entity* entity, float dt) {
	if (entity) {
		if (entity->physics) {
			PhysicsUpdate(entity->physics, entity->transform, dt);
		}
		if (entity->animation) {
			AnimationUpdate(entity->animation, dt);
		}
	}
}

// Render any visible components attached to the Entity.
// (Hint: You will need to call SpriteRender(), passing the Sprite and Transform components.)
// (NOTE: You must first check for valid pointers before calling this function.)
// Params:
//	 entity = Pointer to the Entity.
void EntityRender(Entity* entity) {
	if (entity) {
		SpriteRender(entity->sprite, entity->transform);
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

