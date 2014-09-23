#pragma once

#include "define.h"
#include "Color.h"
#include <GL/glut.h>
#define PAE3D_SELECT_DISABLE 0
#define PAE3D_SELECT_VERTICES 1
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
	PAE3D_Polygon* m_pPolyArray;
	PAE3D_Edge* m_pEdgeArray;
	int m_nNumPoint;
	int m_nNumEdge;
	int m_nNumPolygon;
	PAE3D_Point m_selectedCenter;
	int m_SelectMode;
	bool m_hasSelected;
	bool picking;
public:
	int SelectedHandle;
	Model(void);
	~Model(void);
	PAE3D_Normal PolyNormal(int);
	PAE3D_Point PolyCenter(int);
	void MoveSelected(float, float, float);
	void ScaleSelected(float, float, float);
	void DeselectEverything();
	void RenderVertices(float);
	void RenderEdges(float);
	void RenderFaces(Color*);
	void RenderSelectedFacesHandle(float, int);
	void Smooth();
	void AddVertex(PAE3D_Point);
	void AddEdge(PAE3D_Edge);
	void AddPoly(PAE3D_Polygon);
	void AddNormal();
	unsigned int FindEdge(unsigned int, unsigned int);
	void RenderPicker(float, int);
	void ProcessSelection(int, int, bool, bool);
	void SetSelectType(int);
	void CascadeFaceSelection(int);
	void CascadeEdgeSelection(int);
	void CascadeVertexSelection();
	void Extrude();

	//debug
	void PAE3D_PrintPoly (PAE3D_Polygon p);
	void PAE3D_PrintEdge(PAE3D_Edge e);
};
