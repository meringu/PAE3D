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
/*
// Shading mode : 0 Polygon, 1 Wireframe
#define PAE3D_SHADE_POLYGON 0
#define PAE3D_SHADE_WIREFRAME 1
*/
// Define number of vertex
//#define PAE3D_NUM_VERTEX_PER_FACE 3 // Triangle = 3, Quad = 4

// Define Basic Structures
struct PAE3D_Point {
	float x;
	float y;
	float z;
	bool selected;
};

struct vec4{
	float x;
	float y;
	float z;
	float w;
};

/*struct PAE3D_RGBA {
	float r;
	float g;
	float b;
	float a;
};*/

typedef PAE3D_Point PAE3D_Normal;

/*struct PAE3D_UVcoord {

	float u;
	float v;
};*/

/*
 * Normals and textures need to be defined by face, not by vertex.
 * Reminder: these are just indicies into the normal and texture arrays.
 * n1 and t1 are the normals and texture co-ordinates for vertex 1 of this face.
 * Same goes for (v2,n2,t2) etc.
 */
struct PAE3D_Triangle {
	unsigned int v1;
	unsigned int v2;
	unsigned int v3;
	unsigned int c;
	/*unsigned int n1;
	unsigned int n2;
	unsigned int n3;
	unsigned int t1;
	unsigned int t2;
	unsigned int t3;*/
};

struct PAE3D_Edge {
	unsigned int v1;
	unsigned int v2;
	unsigned int c;
	bool selected;
};

struct PAE3D_Quad {
	bool selected;
	PAE3D_Normal n;
	unsigned int v1;
	unsigned int v2;
	unsigned int v3;
	unsigned int v4;
	unsigned int c;
	/*unsigned int n1;
	unsigned int n2;
	unsigned int n3;
	unsigned int n4;
	unsigned int t1;
	unsigned int t2;
	unsigned int t3;
	unsigned int t4;*/
};

struct PAE3D_Polygon {
	bool selected;
	PAE3D_Normal n;
	int vertexCount;
	unsigned int* vertices;
};



/*struct lightParameters
{
   vec4 ambient;              // Aclarri
   vec4 diffuse;              // Dcli
   vec4 specular;             // Scli
   vec4 position;             // Ppli
   vec4 halfVector;           // Derived: Hi
   vec4 spotDirection;        // Sdli
   float spotExponent;        // Srli
   float spotCutoff;          // Crli
                              // (range: [0.0,90.0], 180.0)
   float spotCosCutoff;       // Derived: cos(Crli)
                              // (range: [1.0,0.0],-1.0)
   float constantAttenuation; // K0
   float linearAttenuation;   // K1
   float quadraticAttenuation;// K2
};


struct materialParameters
{
   vec4 emission;    // Ecm
   vec4 ambient;     // Acm
   vec4 diffuse;     // Dcm
   vec4 specular;    // Scm
   float shininess;  // Srm
};*/
