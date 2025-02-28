//------------------------------------------------------------------------------
//
// File Name:	Transform.c
// Author(s):	Sam Normile (sam.normile)
// Project:		Project 3
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Transform.h"
#include "Vector2D.h"
#include "Stream.h"
#include "Matrix2D.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Transform
{
	// The translation (or world position) of an entity.
	Vector2D	translation;

	// The rotation (or orientation) of an entity (in radians).
	float	rotation;

	// The scale (or size) of an entity.
	// (Hint: This should be initialized to (1, 1).)
	Vector2D	scale;

	Matrix2D matrix;

	bool isDirty;

} Transform;

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

// Dynamically allocate a new Transform component.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// (Hint: You must initialize the scale values to non-zero values.)
// Returns:
//	 If the memory allocation was successful,
//	   then return a pointer to the allocated memory,
//	   else return NULL.
Transform* TransformCreate(void) {
	Transform* transform = calloc(1, sizeof(Transform));
	if (transform) {
		transform->scale = (Vector2D){ 1, 1 };
		transform->isDirty = true;
	}

	return transform;
}

// Dynamically allocate a 
// 
// 
// of an existing Transform.
// (Hint: Perform a shallow copy of the member variables.)
// Params:
//	 other = Pointer to the component to be cloned.
// Returns:
//	 If 'other' is valid and the memory allocation was successful,
//	   then return a pointer to the cloned component,
//	   else return NULL.
Transform* TransformClone(const Transform* other) {
	if (other) {
		Transform* trans = calloc(1, sizeof(Transform));
		if (trans) {
			*trans = *other;
			return trans;
		}
	}
	return NULL;
}

// Free the memory associated with a Transform component.
// (NOTE: The Transform pointer must be set to NULL.)
// Params:
//	 transform = Pointer to the Transform pointer.
void TransformFree(Transform** transform) {
	free(*transform);
	*transform = NULL;
}

// Read the properties of a Transform component from a file.
// [NOTE: Read the translation value using StreamReadVector2D.]
// [NOTE: Read the rotation value using StreamReadFloat.]
// [NOTE: Read the scale value using StreamReadVector2D.]
// Params:
//	 transform = Pointer to the Transform component.
//	 stream = The data stream used for reading.
void TransformRead(Transform* transform, Stream stream) {
	if (transform && stream) {
		Vector2D vector;
		StreamReadVector2D(stream, &vector);
		transform->translation = vector;

		transform->rotation = StreamReadFloat(stream);

		StreamReadVector2D(stream, &vector);
		transform->scale = vector;
	}
}

// Get the transform matrix, based upon translation, rotation and scale settings.
// (HINT: If the isDirty flag is true, then recalculate the transform matrix.)
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return a pointer to the component's matrix structure,
//		else return a NULL pointer.
const Matrix2D* TransformGetMatrix(Transform* transform) {
	if (transform) {
		if (transform->isDirty) {
			Matrix2D scale = { 0 };
			Matrix2D rotation = { 0 };
			Matrix2D translation = { 0 };
			Matrix2DScale(&scale, transform->scale.x, transform->scale.y);
			Matrix2DRotRad(&rotation, transform->rotation);
			Matrix2DTranslate(&translation, transform->translation.x, transform->translation.y);

			Matrix2D result = { 0 };
			Matrix2DConcat(&result, &rotation, &scale);
			Matrix2DConcat(&transform->matrix, &translation, &result);
			transform->isDirty = false;
		}
		return &transform->matrix;
	}
	return NULL;
}

// Get the translation of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return a pointer to the component's translation,
//		else return a NULL pointer.
const Vector2D* TransformGetTranslation(const Transform* transform) {
	if (transform) {
		return &transform->translation;
	}
	return NULL;
}

// Get the rotation value of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return the component's rotation value (in radians),
//		else return 0.0f.
float TransformGetRotation(const Transform* transform) {
	if (transform) {
		return transform->rotation;
	}
	return 0;
}

// Get the scale of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
// Returns:
//	 If the Transform pointer is valid,
//		then return a pointer to the component's scale,
//		else return a NULL pointer.
const Vector2D* TransformGetScale(const Transform* transform) {
	if (transform) {
		return &transform->scale;
	}
	return NULL;
}

// Set the translation of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
//	 translation = Pointer to the new translation.
void TransformSetTranslation(Transform* transform, const Vector2D* translation) {
	if (transform) {
		transform->translation.x = translation->x;
		transform->translation.y = translation->y;
		transform->isDirty = true;
	}
}

// Set the rotation of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
//	 rotation = The rotation value (in radians).
void TransformSetRotation(Transform* transform, float rotation) {
	if (transform) {
		transform->rotation = rotation;
		transform->isDirty = true;
	}
}

// Set the scale of a Transform component.
// Params:
//	 transform = Pointer to the Transform component.
//	 scale = Pointer to the new scale.
void TransformSetScale(Transform* transform, const Vector2D* scale) {
	if (transform) {
		transform->scale.x = scale->x;
		transform->scale.y = scale->y;
		transform->isDirty = true;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

