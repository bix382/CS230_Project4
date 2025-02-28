//------------------------------------------------------------------------------
//
// File Name:	Behavior.c
// Author(s):	Sam Normile (sam.normile)
// Project:		Project 3
// Course:		CS230S25
//
// Copyright � 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Behavior.h"
#include "Stream.h"

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

// Dynamically allocate a clone of an existing behavior.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
Behavior* BehaviorClone(Behavior* other) {
	if (other) {
		Behavior* behavior = calloc(1, sizeof(Behavior));
		if (behavior) {
			*behavior = *other;
			return behavior;
		}
	}
	return NULL;
}

// Free the memory associated with a Behavior component.
// (Also, set the behavior pointer to NULL.)
// Params:
//	 behavior = Pointer to the Behavior component.
void BehaviorFree(Behavior** behavior) {
	free(*behavior);
	*behavior = NULL;
}

// Read the properties of a Behavior component from a file.
// [NOTE: Read the stateCurr and stateNext values using StreamReadInt.]
// [NOTE: Read the timer value using StreamReadFloat.]
// Params:
//	 behavior = Pointer to the Behavior component.
//	 stream = Pointer to the data stream used for reading.
void BehaviorRead(Behavior* behavior, Stream stream) {
	if (behavior && stream) {
		behavior->stateCurr = StreamReadInt(stream);
		behavior->stateNext = StreamReadInt(stream);
		behavior->timer = StreamReadFloat(stream);
	}
}

// Set the parent Entity for a Behavior component.
// Params:
//	 behavior = Pointer to the Behavior component.
//	 parent = Pointer to the parent Entity.
void BehaviorSetParent(Behavior* behavior, Entity* parent) {
	if (behavior && parent) {
		behavior->parent = parent;
	}
}

// Update the Behavior component.
// (Hint: Refer to the Word document for detailed instructions regarding this function.)
// Params:
//	 behavior = Pointer to the Behavior component.
//	 dt = Change in time (in seconds) since the last game loop.
void BehaviorUpdate(Behavior* behavior, float dt) {
	if (behavior) {
		if (behavior->stateCurr != behavior->stateNext) {
			if (behavior->onExit) {
				behavior->onExit(behavior); //@@@
				behavior->stateCurr = behavior->stateNext;
				if (behavior->onInit) {
					behavior->onInit(behavior); //@@@
				}
			}
		}
		if (behavior->onUpdate) {
			behavior->onUpdate(behavior, dt); //@@@
		}
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

