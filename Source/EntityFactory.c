//------------------------------------------------------------------------------
//
// File Name:	EntityFactory.c
// Author(s):	Sam Normile (sam.normile)
// Project:		Project 3
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "EntityFactory.h"
#include "Stream.h"
#include "Entity.h"
#include "EntityContainer.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

static EntityContainer* archetypes = NULL;

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Build a single instance of the specified Entity.
// (NOTE: Use sprintf_s() to construct a path name using objectName.)
// (HINT: The correct path name should be constructed using "Data/&s.txt".)
// (HINT: See project instructions.)
// Params:
//	 entityName = The name of the Entity to be constructed.
// Returns:
//	 If the objectName is not NULL,
//	   then return a pointer to a new instance of the specified Entity,
//	   else NULL.
Entity* EntityFactoryBuild(const char* entityName) {
	if (!entityName) {
		return NULL;
	}
	if (!archetypes) {
		archetypes = EntityContainerCreate();
	}

	Entity* found = EntityContainerFindByName(archetypes, entityName);
	
	if (!found) {
		char pathName[FILENAME_MAX] = "";
		sprintf_s(pathName, _countof(pathName), "Data/%s.txt", entityName);
		Stream entityStream = StreamOpen(pathName);
		if (entityStream) {
			const char* firstToken = StreamReadToken(entityStream);
			if (strncmp(firstToken, "Entity", _countof("Entity")) == 0) {
				Entity* entity = EntityCreate();
				EntityRead(entity, entityStream);
				EntityContainerAddEntity(archetypes, entity);
				StreamClose(&entityStream);
			}
		}
	}

	found = EntityContainerFindByName(archetypes, entityName);

	if (found) {
		Entity* clone = EntityClone(found);
		return clone;
	}
	return NULL;
}

// Free all archetype Entities.
// (Hint: If the "archetypes" container exists, then the EntityContainerFreeAll
//    function must be called.)
void EntityFactoryFreeAll() {
	if (archetypes) {
		EntityContainerFreeAll(archetypes);
	}
	//@@@ EntityContainerFree(archetypes);
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

