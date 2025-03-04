//------------------------------------------------------------------------------
//
// File Name:	Mesh.c
// Author(s):	Sam Normile (sam.normile)
// Project:		Project 3
// Course:		CS230S25
//
// Copyright � 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Mesh.h"
#include "DGL.h"
#include "Stream.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

// You are free to change the contents of this structure as long as you do not
//   change the public interface declared in the header.
typedef struct Mesh
{
	// The name of the Mesh.  This will be used later in the semester.
	char name[32];

	// Pointer to the DGL_Mesh resource.
	const DGL_Mesh* meshResource;

	// The draw mode to use when rendering the mesh (Usually "DGL_DM_TRIANGLELIST").
	DGL_DrawMode drawMode;
} Mesh;

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

// Dynamically allocate a new Mesh object but leave it empty.
// (Hint: Use calloc() to ensure that all member variables are initialized to 0.)
// Returns:
//	 If the mesh was created successfully,
//	   then return a pointer to the created Mesh,
//	   else return NULL.
Mesh* MeshCreate() {
	Mesh* mesh = calloc(1, sizeof(Mesh));

	return mesh;
}

// Build a quadrilateral mesh and store it in the specified Mesh object.
// (NOTE: The DGL_Mesh object must be created using DGL_Graphics_StartMesh,
//    DGL_Graphics_AddTriangle, and DGL_Graphics_EndMesh.)
// (NOTE: The Mesh name can be stored using strcpy_s(). For example:
//    strcpy_s(mesh->name, _countof(mesh->name), name); )
// (NOTE: The drawMode should be set to DGL_DM_TRIANGLELIST.)
// Params:
//   mesh = Pointer to an existing, empty Mesh object.
//	 xHalfSize = The X half-size of the mesh.
//	 yHalfSize = The Y half-size of the mesh.
//   uSize = The U size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//   vSize = The V size of the mesh, relative to texture coordinates (0.0 .. 1.0).
//	 name = A name for the mesh.
void MeshBuildQuad(Mesh* mesh, float xHalfSize, float yHalfSize, float uSize, float vSize, const char* name) {
	if (mesh) { 
		DGL_Graphics_StartMesh();
		DGL_Graphics_AddTriangle(
			&(DGL_Vec2) { -xHalfSize, -yHalfSize }, &(DGL_Color) { 0, 0, 0, 1 }, & (DGL_Vec2) { 0.f, vSize },
			&(DGL_Vec2){  xHalfSize,  yHalfSize }, &(DGL_Color) { 0, 0, 0, 1 }, &(DGL_Vec2){ uSize, 0.f},
			&(DGL_Vec2){  xHalfSize, -yHalfSize }, &(DGL_Color) { 0, 0, 0, 1 }, &(DGL_Vec2){ uSize, vSize });
		DGL_Graphics_AddTriangle(
			&(DGL_Vec2){ -xHalfSize, -yHalfSize}, &(DGL_Color) { 0, 0, 0, 1 }, &(DGL_Vec2){ 0.f, vSize },
			&(DGL_Vec2){ -xHalfSize,  yHalfSize}, &(DGL_Color) { 0, 0, 0, 1 }, &(DGL_Vec2){ 0.f, 0.f },
			&(DGL_Vec2){  xHalfSize,  yHalfSize}, &(DGL_Color) { 0, 0, 0, 1 }, &(DGL_Vec2){ uSize, 0.f });
		mesh->meshResource = DGL_Graphics_EndMesh();
		mesh->drawMode = DGL_DM_TRIANGLELIST;
		strcpy_s(mesh->name, _countof(mesh->name), name);
	}
}

// Build a "spaceship" mesh and store it in the specified Mesh object.
// (NOTE: This must be a "unit"-sized triangular mesh with the same characteristics as
//    the "triangular, colored mesh" created in DemoScene.c.)
// (NOTE: The DGL_Mesh object must be created using DGL_Graphics_StartMesh,
//    DGL_Graphics_AddTriangle, and DGL_Graphics_EndMesh.)
// (NOTE: The Mesh name can be stored using strcpy_s().)
// (NOTE: The drawMode should be set to DGL_DM_TRIANGLELIST.)
// Params:
//   mesh = Pointer to an existing, empty Mesh object.
void MeshBuildSpaceship(Mesh* mesh) {
	DGL_Graphics_StartMesh();
	DGL_Graphics_AddTriangle(
		&(DGL_Vec2){  0.5f,  0.0f }, &(DGL_Color) { 1, 1, 0, 1 }, & (DGL_Vec2) { 0.0f, 0.0f },
		&(DGL_Vec2){ -0.5f, -0.5f }, &(DGL_Color) { 1, 0, 0, 1 }, &(DGL_Vec2){ 0.0f, 0.0f },
		&(DGL_Vec2){ -0.5f,  0.5f }, &(DGL_Color) { 1, 0, 0, 1 }, &(DGL_Vec2){ 0.0f, 0.0f });
	mesh->meshResource = DGL_Graphics_EndMesh();
	strcpy_s(mesh->name, _countof(mesh->name), "spaceship");
	mesh->drawMode = DGL_DM_TRIANGLELIST;
}

// Read the properties of a Mesh object from a file.
// (NOTE: First, read a token from the file and verify that it is "Mesh".)
// (NOTE: Second, read a token and store it in the Mesh's name variable.)
// (NOTE: Third, read an integer indicating the number of vertices to be read.)
// (NOTE: For each vertex, read a Vector2D (position), a DGL_Color (color), and a Vector2D (UV).)
// (HINT: Call DGL_Graphics_AddVertex() to add a single vertex to the mesh.)
// Params:
//   mesh = Pointer to the Mesh.
//	 stream = The data stream used for reading.
void MeshRead(Mesh* mesh, Stream stream) {
	if (mesh && stream) {
		const char* token = StreamReadToken(stream);
		if (strncmp(token, "Mesh", sizeof("Mesh")) == 0) {
			*mesh->name = *StreamReadToken(stream);
			int vertices = StreamReadInt(stream);
			DGL_Graphics_StartMesh();
			for (int i = 0; i < vertices; ++i) {
				Vector2D pos, uv = { 0 };
				DGL_Color color = { 0 };
				StreamReadVector2D(stream, &pos);
				StreamReadColor(stream, &color);
				StreamReadVector2D(stream, &uv);
				DGL_Graphics_AddVertex(&pos, &color, &uv);
			}
			mesh->meshResource = DGL_Graphics_EndMesh();
		}
	}
}

// Determines if a Mesh has the specified name.
// (HINT: This function is similar to one in Entity.c.)
// Params:
//	 mesh = Pointer to the Mesh object.
//	 name = Pointer to the name to be compared.
// Returns:
//	 If the mesh and name pointers are valid,
//		then perform a string comparison and return the result (match = true),
//		else return false.
bool MeshIsNamed(const Mesh* mesh, const char* name) {
	if (mesh && name) {
		if (strcmp(mesh->name, name) == 0) {
			return true;
		}
	}
	return false;
}

// Render a mesh.
// (NOTE: This is done using DGL_Graphics_DrawMesh().)
// Params:
//   mesh = Pointer to a Mesh to be rendered.
void MeshRender(const Mesh* mesh) {
	if (mesh) {
		DGL_Graphics_DrawMesh(mesh->meshResource, mesh->drawMode);
	}
}

// Free the memory associated with a mesh.
// (NOTE: The DGL_Mesh resource must be freed using DGL_Graphics_FreeMesh().)
// (NOTE: The Mesh object must be freed using free().
// (NOTE: The Mesh pointer must be set to NULL.)
// Params:
//   mesh = Pointer to the Mesh pointer.
void MeshFree(Mesh** mesh) {
	if (mesh) {
		DGL_Graphics_FreeMesh(&(*mesh)->meshResource);
		free(*mesh);
		*mesh = NULL;
	}
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------

