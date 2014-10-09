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

struct PAE3D_Normal {
	float x;
	float y;
	float z;
};

struct PAE3D_Point {
	float x;
	float y;
	float z;
	bool selected;
	bool empty;
	unsigned int* edges;
	int edgeCount;
	PAE3D_Normal n;
};

struct PAE3D_Vertex {
	float x;
	float y;
	float z;
	bool selected;
};

struct PAE3D_Edge {
	unsigned int v1;
	unsigned int v2;
	unsigned int c;			// used for subdivision
	bool hasPoly1;			// used for initialization
	bool hasPoly2;			// used for initialization
	unsigned int poly1;
	unsigned int poly2;
	bool selected;
	bool empty;
	unsigned int nextEdge;	// used for subdivision
};

struct PAE3D_Polygon {
	bool selected;
	bool empty;
	PAE3D_Normal n;
	int vertexCount;
	unsigned int* vertices;
	unsigned int* edges;
	int c;					// used for subdivision
	int mat;
};

struct PAE3D_RGBA {
        float r;
        float g;
        float b;
        float a;
};

struct PAE3D_Material {
	PAE3D_RGBA col;
	float specular;
	float shininess;
};
