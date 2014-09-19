#pragma once

// Default Window
#define PAE3D_WIN_WIDTH	640
#define PAE3D_WIN_HEIGHT 480

// Projection parameters
#define PAE3D_FOVY		20.0
#define PAE3D_ZNEAR_3D	1
#define PAE3D_ZFAR_3D	1000.0
#define PAE3D_ZNEAR_2D	-50.0
#define PAE3D_ZFAR_2D	50.0

#define PAE3D_HANLE_MOVE 0
#define PAE3D_HANLE_SCALE 1

struct PAE3D_Point {
	float x;
	float y;
	float z;
	bool selected;
};

typedef PAE3D_Point PAE3D_Normal;

struct PAE3D_Edge {
	unsigned int v1;
	unsigned int v2;
	unsigned int c;			// used for subdivision
	bool hasPoly1;			// used for initialization
	bool hasPoly2;			// used for initialization
	unsigned int poly1;
	unsigned int poly2;
	bool selected;
	unsigned int nextEdge;	// used for subdivision
};

struct PAE3D_Polygon {
	bool selected;
	PAE3D_Normal n;
	int vertexCount;
	unsigned int* vertices;
	unsigned int* edges;
	int c;					// used for subdivision
};
