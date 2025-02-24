//------------------------------------------------------------------------------
//
// File Name:	Vector2D.c
// Author(s):	Sam Normile (sam.normile)
// Project:		Project 3
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Vector2D.h"
#include "Trace.h"
#define _USE_MATH_DEFINES
#include "math.h"


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

// This function sets the coordinates of the 2D vector (pResult) to 0
void Vector2DZero(Vector2D* pResult){
	if (pResult) {
		pResult->x = 0;
		pResult->y = 0;
	}
	else {
		TraceMessage("Error: Vector2DZero: bad param");
	}
}

// This function sets the coordinates of the 2D vector (pResult) to x & y
void Vector2DSet(Vector2D* pResult, float x, float y) {
	if (pResult) {
		pResult->x = x;
		pResult->y = y;
	}
	else {
		TraceMessage("Error: Vector2DSet: bad param");
	}
}

// In this function, pResult will be set to the opposite of pVec0 (e.g. x = -x & y = -y)
void Vector2DNeg(Vector2D* pResult, const Vector2D* pVec0) {
	if (pResult && pVec0) {
		pResult->x = -pVec0->x;
		pResult->y = -pVec0->y;
	}
	else {
		TraceMessage("Error: Vector2DNeg: bad param");
	}
}

// In this function, pResult will be the sum of pVec0 and pVec1 (x result = x0 + x1)
void Vector2DAdd(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1) {
	if (pResult && pVec0 && pVec1) {
		pResult->x = pVec0->x + pVec1->x;
		pResult->y = pVec0->y + pVec1->y;
	}
	else {
		TraceMessage("Error: Vector2DAdd: bad param");
	}
}

// In this function, pResult will be the difference between pVec0 and pVec1 (specifically, pVec0 - pVec1)
void Vector2DSub(Vector2D* pResult, const Vector2D* pVec0, const Vector2D* pVec1) {
	if (pResult && pVec0 && pVec1) {
		pResult->x = pVec0->x - pVec1->x;
		pResult->y = pVec0->y - pVec1->y;
	}
	else {
		TraceMessage("Error: Vector2DSub: bad param");
	}
}

// In this function, pResult will be the unit vector of pVec0
// (NOTE: Care must be taken when pResult = pVec0!)
void Vector2DNormalize(Vector2D* pResult, const Vector2D* pVec0) {
	if (pResult && pVec0) {
		float length = sqrtf(powf(pVec0->x, 2.f) + powf(pVec0->y, 2.f));
		pResult->x = pVec0->x / length;
		pResult->y = pVec0->y / length;
	}
	else {
		TraceMessage("Error: Vector2DNormalize: bad param");
	}
}

// In this function, pResult will be the vector pVec0 scaled by the value 'scale'
void Vector2DScale(Vector2D* pResult, const Vector2D* pVec0, float scale) {
	if (pResult && pVec0) {
		pResult->x = pVec0->x * scale;
		pResult->y = pVec0->y * scale;
	}
	else {
		TraceMessage("Error: Vector2DScale: bad param");
	}
}

// In this function, pResult will be the vector pVec0 scaled by 'scale' and added to pVec1
void Vector2DScaleAdd(Vector2D* pResult, const Vector2D* pVec0, float scale, const Vector2D* pVec1) {
	if (pResult && pVec0) {
		pResult->x = pVec0->x * scale + pVec1->x;
		pResult->y = pVec0->y * scale + pVec1->y;
	}
	else {
		TraceMessage("Error: Vector2DScaleAdd: bad param");
	}
}

// In this function, pResult will be the vector pVec0 scaled by 'scale' and pVec1 will be subtracted from it
void Vector2DScaleSub(Vector2D* pResult, const Vector2D* pVec0, float scale, const Vector2D* pVec1) {
	if (pResult && pVec0) {
		pResult->x = pVec0->x * scale - pVec1->x;
		pResult->y = pVec0->y * scale - pVec1->y;
	}
	else {
		TraceMessage("Error: Vector2DScaleSub: bad param");
	}
}

// This function returns the length of the vector pVec0
float Vector2DLength(const Vector2D* pVec0) {
	if (pVec0) {
		float length = sqrtf(powf(pVec0->x, 2) + powf(pVec0->y, 2));
		return length;
	}
	else {
		TraceMessage("Error: Vector2DLength: bad param");
	}
	return 0;
}

// This function returns the square of pVec0's length.
// NOTE: The square root function must NOT be called by this function.
float Vector2DSquareLength(const Vector2D* pVec0) {
	if (pVec0) {
		float length = powf(pVec0->x, 2) + powf(pVec0->y, 2);
		return length;
	}
	else {
		TraceMessage("Error: Vector2DSquareLength: bad param");
	}
	return 0;
}

// This function returns the distance between two points.
float Vector2DDistance(const Vector2D* pVec0, const Vector2D* pVec1) {
	if (pVec0 && pVec1) {
		Vector2D sides = { pVec1->x - pVec0->x, pVec1->y - pVec0->y };
		float sideC = Vector2DLength(&sides);
		return sideC;
	}
	else {
		TraceMessage("Error: Vector2DDistance: bad param");
	}
	return 0;
}

// This function returns the distance squared between two points.
// NOTE: The square root function must NOT be called by this function.
float Vector2DSquareDistance(const Vector2D* pVec0, const Vector2D* pVec1) {
	if (pVec0 && pVec1) {
		Vector2D sides = { pVec1->x - pVec0->x, pVec1->y - pVec0->y };
		float sideC = Vector2DSquareLength(&sides);
		return sideC;
	}
	else {
		TraceMessage("Error: Vector2DSquareDistance: bad param");
	}
	return 0;
}

// This function returns the dot product between pVec0 and pVec1
float Vector2DDotProduct(const Vector2D* pVec0, const Vector2D* pVec1) {
	if (pVec0 && pVec1) {
		float dot = (pVec0->x * pVec1->x) + (pVec0->y * pVec1->y);
		return dot;
	}
	else {
		TraceMessage("Error: Vector2DDotProduct: bad param");
	}
	return 0;
}

// This function computes the coordinates of the unit vector represented by the angle "angle", which is in Degrees.
// Converting from degrees to radians can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
// M_PI is defined in "math.h", which may be included as follows:
//   #define _USE_MATH_DEFINES
//   #include <math.h>
void Vector2DFromAngleDeg(Vector2D* pResult, float angle) {
	if (pResult) {
		float rads = (float)((angle * M_PI) / 180.f);
		pResult->x = cosf(rads);
		pResult->y = sinf(rads);
	}
	else {
		TraceMessage("Error: Vector2DFromAngleDeg: bad param");
	}
}

// This function computes the coordinates of the unit vector represented by the angle "angle", which is in Radians.
// HINT: x = cos(angle), y = sin(angle).
void Vector2DFromAngleRad(Vector2D* pResult, float angle) {
	if (pResult) {
		pResult->x = cosf(angle);
		pResult->y = sinf(angle);
	}
	else {
		TraceMessage("Error: Vector2DFromAngleRad: bad param");
	}
}

// This function computes the angle, in radians, of the specified vector.
// HINT: Angle (radians) = atan2f(direction Y, direction X).
// NOTE: If the pointer is NULL, then return 0.0f.
float Vector2DToAngleRad(const Vector2D* pVec) {
	if (pVec) {
		float angle = atan2f(pVec->y, pVec->x);
		return angle;
	}
	else {
		TraceMessage("Error: Vector2DToAngleRad: bad param");
	}
	return 0;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

