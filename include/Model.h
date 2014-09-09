#pragma once

#include "define.h"
#include <GL/glut.h>
#define PAE3D_SELECT_DISABLE 0
#define PAE3D_SELECT_VERICES 1
#define PAE3D_SELECT_EDGES 2
#define PAE3D_SELECT_FACES 3
#define PAE3D_SELECT_NO_HANDLE -1
#define PAE3D_SELECT_X_HANDLE 0
#define PAE3D_SELECT_Y_HANDLE 1
#define PAE3D_SELECT_Z_HANDLE 2
#define PAE3D_COLORPADDING 3

class Model
{
private:
	PAE3D_Point* m_pVertexArray;
	PAE3D_Quad* m_pQuadArray;
	PAE3D_Edge* m_pEdgeArray;
	int m_nNumPoint;
	int m_nNumEdge;
	int m_nNumPolygon;
	PAE3D_Point m_selectedCenter;
	int m_SelectMode;
	bool m_hasSelected;
public:
	int SelectedHandle;
	Model(void);
	~Model(void);
	PAE3D_Point TriangleNormal(PAE3D_Triangle);
	PAE3D_Normal QuadNormal(PAE3D_Quad);
	PAE3D_Point QuadCenter(PAE3D_Quad);
	void MoveSelected(float, float, float);
	void RenderVertices(float);
	void RenderEdges(float);
	void RenderFaces();
	void RenderSelectedFacesHandle(float);
	void Smooth();
	void AddVertex(PAE3D_Point);
	void AddEdge(PAE3D_Edge);
	void AddQuad(PAE3D_Quad);
	void AddNormal();
	unsigned int FindEdge(unsigned int, unsigned int);
	void RenderPicker(float);
	void ProcessSelection(int, int, bool, bool);
};
