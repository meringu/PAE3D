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
	PAE3D_Quad* m_pQuadArray;	// Quad Array
	PAE3D_Edge* m_pEdgeArray;	    // Texture Coordinate Array

	// Data for Geometry
	int m_nNumPoint;
	int m_nNumEdge;
	int m_nNumPolygon;
	int m_nNumNormal;

public:
	PAE3D_Point TriangleNormal(PAE3D_Triangle);
	PAE3D_Normal QuadNormal(PAE3D_Quad);
	PAE3D_Point QuadCenter(PAE3D_Quad q);
	void RenderVertices(float);
	void RenderEdges(float);
	void RenderFaces(bool);
	void RenderSelectedFacesHandle(float);
};
