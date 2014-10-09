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
#define PAE3D_RENDER_NORMALS false

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
	void RenderFaces(Color*, bool);
	void RenderSelectedFacesHandle(float, int);
	void Smooth();
	void AddVertex(PAE3D_Point);
	void AddEdge(PAE3D_Edge);
	void AddPoly(PAE3D_Polygon);
	void DeletePoly(int);
	void DeleteVertex(int);
	void DeleteEdge(int);
	void AddNormal();
	unsigned int FindEdge(unsigned int, unsigned int);
	void RenderPicker(float, int);
	void ProcessSelection(int, int, bool, bool, int);
	void SetSelectType(int);
	void CascadeFaceSelection(int);
	void CascadeEdgeSelection(int);
	void CascadeVertexSelection();
	void Extrude();
	void Duplicate();
	void CalculateNormal(int);
	void AssignIntermediatePointers();
	void Subdivide();
	unsigned int FindEdgeUsingDependancies(unsigned int, unsigned  int);
	void ResetHandlePosition();
};
