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

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Build a single instance of the specified entity.
// (HINT: See project instructions.)
// Params:
//	 filename = The name of the file to be deserialized.
// Returns:
//	 If the filename is valid
//	   then return a pointer to a new instance of the specified entity,
//	   else NULL.
Entity* EntityFactoryBuild(const char* filename) {
	if (filename) {
		Stream entityStream = StreamOpen(filename);
		if (entityStream) {
			const char* firstToken = StreamReadToken(entityStream);
			int cmp = strncmp(firstToken, "Entity", _countof("Entity"));
			if (cmp == 0) {
				Entity* entity = EntityCreate();
				EntityRead(entity, entityStream);
				StreamClose(&entityStream);
				return entity;
			}
			StreamClose(&entityStream);
		}
	}
	return NULL;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

