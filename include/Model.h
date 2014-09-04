#pragma once

#include "define.h"
#include <GL/glut.h>

class Model
{
public:
	Model(void);
	~Model(void);

	// Array for Geometry
	PAE3D_Point* m_pVertexArray;		// Vertex Array
	PAE3D_Normal* m_pNormalArray;	// Normal Array
	PAE3D_Quad* m_pQuadArray;	// Trianglem_glGeomListMap Array
	PAE3D_UVcoord* m_pUVArray;	    // Texture Coordinate Array

	// Data for Geometry
	int m_nNumPoint;
	int m_nNumUV;
	int m_nNumPolygon;
	int m_nNumNormal;

	// Data for Rendering
	int m_glGeomList;	// Display List for Polygon

public:
	PAE3D_Point TriangleNormal(PAE3D_Triangle);
	PAE3D_Normal QuadNormal(PAE3D_Quad);
	void RenderGeometry();
};



