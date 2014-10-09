#include "Model.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include "Handle.h"
#include "ImageLoader.h"

#include <iostream>

using namespace std;

int numVert, numNorm, numUV, numFace;
GLUquadric* quadric = gluNewQuadric();

Model::Model() {
	SelectedHandle = PAE3D_SELECT_NO_HANDLE;
	m_SelectMode = PAE3D_SELECT_FACES;
	m_hasSelected = false;
	picking = false;

	m_pVertexArray = NULL;
	m_pEdgeArray = NULL;
	m_pPolyArray= NULL;

	m_nNumPoint = 8;
	m_pVertexArray = new PAE3D_Point[m_nNumPoint];
	m_pVertexArray[0].x = 0.5;
	m_pVertexArray[0].y = 0.5;
	m_pVertexArray[0].z = 0.5;
	m_pVertexArray[1].x = -0.5;
	m_pVertexArray[1].y = 0.5;
	m_pVertexArray[1].z = 0.5;
	m_pVertexArray[2].x = -0.5;
	m_pVertexArray[2].y = -0.5;
	m_pVertexArray[2].z = 0.5;
	m_pVertexArray[3].x = 0.5;
	m_pVertexArray[3].y = -0.5;
	m_pVertexArray[3].z = 0.5;
	m_pVertexArray[4].x = 0.5;
	m_pVertexArray[4].y = 0.5;
	m_pVertexArray[4].z = -0.5;
	m_pVertexArray[5].x = -0.5;
	m_pVertexArray[5].y = 0.5;
	m_pVertexArray[5].z = -0.5;
	m_pVertexArray[6].x = -0.5;
	m_pVertexArray[6].y = -0.5;
	m_pVertexArray[6].z = -0.5;
	m_pVertexArray[7].x = 0.5;
	m_pVertexArray[7].y = -0.5;
	m_pVertexArray[7].z = -0.5;

	m_nNumPolygon = 6;
	m_pPolyArray = new PAE3D_Polygon[m_nNumPolygon];

	unsigned int* points = new unsigned int[4];
	points[0] = 0;
	points[1] = 1;
	points[2] = 2;
	points[3] = 3;
	m_pPolyArray[0].vertices = points;
	points = new unsigned int[4];
	points[0] = 7;
	points[1] = 6;
	points[2] = 5;
	points[3] = 4;
	m_pPolyArray[1].vertices = points;
	points = new unsigned int[4];
	points[0] = 1;
	points[1] = 5;
	points[2] = 6;
	points[3] = 2;
	m_pPolyArray[2].vertices = points;
	points = new unsigned int[4];
	points[0] = 4;
	points[1] = 0;
	points[2] = 3;
	points[3] = 7;
	m_pPolyArray[3].vertices = points;
	points = new unsigned int[4];
	points[0] = 4;
	points[1] = 5;
	points[2] = 1;
	points[3] = 0;
	m_pPolyArray[4].vertices = points;
	points = new unsigned int[4];
	points[0] = 3;
	points[1] = 2;
	points[2] = 6;
	points[3] = 7;
	m_pPolyArray[5].vertices = points;

	for (int i = 0 ; i < m_nNumPoint; i++) {
		m_pVertexArray[i].edgeCount = 0;
	}
	for (int i = 0; i < m_nNumPolygon; i++) {
		m_pPolyArray[i].selected = false;
		m_pPolyArray[i].vertexCount = 4;
		m_pPolyArray[i].n = PolyNormal(i);
		m_pPolyArray[i].mat = 0;
	}

	m_nNumEdge = 12;
	m_pEdgeArray = new PAE3D_Edge[m_nNumEdge];
	PAE3D_Edge e0;
	e0.v1 = 0;
	e0.v2 = 1;
	m_pEdgeArray[0] = e0;
	PAE3D_Edge e1;
	e1.v1 = 1;
	e1.v2 = 2;
	m_pEdgeArray[1] = e1;
	PAE3D_Edge e2;
	e2.v1 = 2;
	e2.v2 = 3;
	m_pEdgeArray[2] = e2;
	PAE3D_Edge e3;
	e3.v1 = 3;
	e3.v2 = 0;
	m_pEdgeArray[3] = e3;
	PAE3D_Edge e4;
	e4.v1 = 4;
	e4.v2 = 5;
	m_pEdgeArray[4] = e4;
	PAE3D_Edge e5;
	e5.v1 = 5;
	e5.v2 = 6;
	m_pEdgeArray[5] = e5;
	PAE3D_Edge e6;
	e6.v1 = 6;
	e6.v2 = 7;
	m_pEdgeArray[6] = e6;
	PAE3D_Edge e7;
	e7.v1 = 7;
	e7.v2 = 4;
	m_pEdgeArray[7] = e7;
	PAE3D_Edge e8;
	e8.v1 = 0;
	e8.v2 = 4;
	m_pEdgeArray[8] = e8;
	PAE3D_Edge e9;
	e9.v1 = 1;
	e9.v2 = 5;
	m_pEdgeArray[9] = e9;
	PAE3D_Edge e10;
	e10.v1 = 2;
	e10.v2 = 6;
	m_pEdgeArray[10] = e10;
	PAE3D_Edge e11;
	e11.v1 = 3;
	e11.v2 = 7;
	m_pEdgeArray[11] = e11;

	AssignIntermediatePointers();
}

void Model::AssignIntermediatePointers() {

	for (int i = 0; i < m_nNumEdge; i++) {
		//m_pEdgeArray[i].selected = false;
		m_pEdgeArray[i].hasPoly1= false;
		m_pEdgeArray[i].hasPoly2= false;
	}

	// points each edge to the neighboring polygons
	for (int i = 0; i < m_nNumPolygon; i++) {
		m_pPolyArray[i].edges = new unsigned int[m_pPolyArray[i].vertexCount];
		for (int j = 0; j < m_pPolyArray[i].vertexCount; j++) {
			int v1 = m_pPolyArray[i].vertices[j];
			int v2 = m_pPolyArray[i].vertices[(j+1)%m_pPolyArray[i].vertexCount];
			int edge = FindEdge(v1, v2);
			m_pPolyArray[i].edges[j] = edge;
			if (m_pEdgeArray[edge].hasPoly1) {
				m_pEdgeArray[edge].poly2 = i;
				m_pEdgeArray[edge].hasPoly2 = true;
			}
			else {
				m_pEdgeArray[edge].poly1 = i;
				m_pEdgeArray[edge].hasPoly1 = true;
			}
		}
	}

	for (int i = 0 ; i < m_nNumPoint; i++) {
		m_pVertexArray[i].edgeCount = 0;
	}

	// points each vertex to the neighboring edges
	for (int i = 0; i < m_nNumEdge; i++) {
		int v1 = m_pEdgeArray[i].v1;
		if (m_pVertexArray[v1].edgeCount == 0) {
			m_pVertexArray[v1].edges = new unsigned int[1];
		}
		else {
			unsigned int* old1 = m_pVertexArray[v1].edges;
			m_pVertexArray[v1].edges = new unsigned int[m_pVertexArray[v1].edgeCount+1];
			for (int j = 0; j < m_pVertexArray[v1].edgeCount; j++) {
				m_pVertexArray[v1].edges[j] = old1[j];
			}
			delete (old1);
		}
		m_pVertexArray[v1].edges[m_pVertexArray[v1].edgeCount] = i;
		m_pVertexArray[v1].edgeCount++;

		int v2 = m_pEdgeArray[i].v2;
		if (m_pVertexArray[v2].edgeCount == 0) {
			m_pVertexArray[v2].edges = new unsigned int[1];
		}
		else {
			unsigned int* old2 = m_pVertexArray[v2].edges;
			m_pVertexArray[v2].edges = new unsigned int[m_pVertexArray[v2].edgeCount+1];
			for(int j = 0; j < m_pVertexArray[v2].edgeCount; j++) {
				m_pVertexArray[v2].edges[j] = old2[j];
			}
			delete(old2);
		}
		m_pVertexArray[v2].edges[m_pVertexArray[v2].edgeCount] = i;
		m_pVertexArray[v2].edgeCount++;
	}

	for (int i = 0; i < m_nNumPoint; i++) {
		//m_pVertexArray[i].selected = false;
		CalculateNormal(i);
	}
}

Model::~Model(void) {
	if (m_pVertexArray != NULL)
		delete[] m_pVertexArray;
	if (m_pEdgeArray != NULL)
		delete[] m_pEdgeArray;
	if (m_pPolyArray != NULL)
		delete[] m_pPolyArray;
}

PAE3D_Normal Model::PolyNormal(int p) {
	PAE3D_Polygon poly = m_pPolyArray[p];
	PAE3D_Normal normal;
	normal.x = 0;
	normal.y = 0;
	normal.z = 0;
	for (int i = 0; i < poly.vertexCount; i++) {
		int p1 = poly.vertices[i];
		int p2 = poly.vertices[(i+1)%poly.vertexCount];
		int p3 = poly.vertices[(i+2)%poly.vertexCount];
		PAE3D_Normal u;
		u.x = m_pVertexArray[p2].x - m_pVertexArray[p1].x;
		u.y = m_pVertexArray[p2].y - m_pVertexArray[p1].y;
		u.z = m_pVertexArray[p2].z - m_pVertexArray[p1].z;
		PAE3D_Normal v;
		v.x = m_pVertexArray[p3].x - m_pVertexArray[p1].x;
		v.y = m_pVertexArray[p3].y - m_pVertexArray[p1].y;
		v.z = m_pVertexArray[p3].z - m_pVertexArray[p1].z;
		PAE3D_Normal n;
		n.x = u.y * v.z - u.z * v.y;
		n.y = u.z * v.x - u.x * v.z;
		n.z = u.x * v.y - u.y * v.x;
		float m = sqrt(n.x * n.x + n.y * n.y + n.z * n.z);
		n.x /= m;
		n.y /= m;
		n.z /= m;
		normal.x += n.x;
		normal.y += n.y;
		normal.z += n.z;
	}
	float m = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
	normal.x /= m;
	normal.y /= m;
	normal.z /= m;
	return normal;
}

PAE3D_Point Model::PolyCenter(int p) {
	PAE3D_Polygon poly = m_pPolyArray[p];
	PAE3D_Point center;
	center.x = 0;
	center.y = 0;
	center.z = 0;
	for (int i = 0; i < poly.vertexCount; i++) {
		center.x += m_pVertexArray[poly.vertices[i]].x;
		center.y += m_pVertexArray[poly.vertices[i]].y;
		center.z += m_pVertexArray[poly.vertices[i]].z;
	}
	center.x /= poly.vertexCount;
	center.y /= poly.vertexCount;
	center.z /= poly.vertexCount;
	return center;
}

void Model::AddEdge(PAE3D_Edge edge) {
	m_nNumEdge++;
	PAE3D_Edge* temp = new PAE3D_Edge[m_nNumEdge];
	for (int i = 0; i < m_nNumEdge-1; i++) {
		temp[i] = m_pEdgeArray[i];
	}
	temp[m_nNumEdge-1] = edge;
	delete(m_pEdgeArray);
	m_pEdgeArray = temp;
}

void Model::AddVertex(PAE3D_Point point) {
	m_nNumPoint++;
	PAE3D_Point* temp = new PAE3D_Point[m_nNumPoint];
	for (int i = 0; i < m_nNumPoint-1; i++) {
		temp[i] = m_pVertexArray[i];
	}
	temp[m_nNumPoint-1] = point;
	delete(m_pVertexArray);
	m_pVertexArray = temp;
}

void Model::AddPoly(PAE3D_Polygon quad) {
	m_nNumPolygon++;
	PAE3D_Polygon* temp = new PAE3D_Polygon[m_nNumPolygon];
	for (int i = 0; i < m_nNumPolygon-1; i++) {
		temp[i] = m_pPolyArray[i];
	}
	temp[m_nNumPolygon-1] = quad;
	delete(m_pPolyArray);
	m_pPolyArray = temp;
}
void Model::DeleteVertex(int index){
	/* set all  delete items to empty. Leave all references in and add handlers for empty cases for operations.
	 * cascade the changes to edges and faces setting them to empty if they have the vertex reference. All edges will
	 * empty because they need two vertices. Face will only empty if their vert count goes below 4.
	 *  */
	(void)index;
}
void Model::DeletePoly(int index){
	/* set face to empty. Just dont render it.
	 * */
	(void)index;
}
void Model::DeleteEdge(int index){
	/*set to empty. Leave vertices alone if they have references else where. If a vertex no longer has references
	 * set it to empty. Set both referenced edge faces to empty.
	 * */
	(void)index;
}
void Model::Smooth() {
	// Catmull-Clark
	unsigned int points = m_nNumPoint;
	unsigned int edges = m_nNumEdge;
	unsigned int polys = m_nNumPolygon;
	// finding face points
	for (int i = 0; i < m_nNumPolygon; i++) {
		m_pPolyArray[i].c = m_nNumPoint;
		AddVertex(PolyCenter(i));
	}
	// finding edge points
	for (int i = 0; i < m_nNumEdge; i++) {
		PAE3D_Point v1 = m_pVertexArray[m_pEdgeArray[i].v1];
		PAE3D_Point v2 = m_pVertexArray[m_pEdgeArray[i].v2];
		PAE3D_Point p;
		p.x = v1.x + v2.x;
		p.y = v1.y + v2.y;
		p.z = v1.z + v2.z;
		int count = 2;
		if (m_pEdgeArray[i].hasPoly1) {
			PAE3D_Point v3 = m_pVertexArray[m_pPolyArray[m_pEdgeArray[i].poly1].c];
			p.x += v3.x;
			p.y += v3.y;
			p.z += v3.z;
			count++;
		}
		if (m_pEdgeArray[i].hasPoly2) {
			PAE3D_Point v3 = m_pVertexArray[m_pPolyArray[m_pEdgeArray[i].poly2].c];
			p.x += v3.x;
			p.y += v3.y;
			p.z += v3.z;
			count++;
		}
		p.x /= count;
		p.y /= count;
		p.z /= count;
		m_pEdgeArray[i].c = m_nNumPoint;
		AddVertex(p);
	}
	// joining face points to edge points
	for (int i = 0; i < m_nNumPolygon; i++) {
		for (int j = 0; j < m_pPolyArray[i].vertexCount; j++) {
			PAE3D_Edge e;
			e.v1 = m_pPolyArray[i].c;
			e.v2 = m_pEdgeArray[m_pPolyArray[i].edges[j]].c;
			e.hasPoly1 = false;
			e.hasPoly2 = false;
			AddEdge(e);
		}
	}
	//adjusting original point positions
	for (unsigned int i = 0; i < points; i++) {
		PAE3D_Point f;
		f.x = 0;
		f.y = 0;
		f.z = 0;
		int neighboringPolys = 0;
		for (int j = 0; j < m_nNumPolygon; j++) {
				bool touches = false;
				for (int k = 0; k < m_pPolyArray[j].vertexCount; k++) {
					if (m_pPolyArray[j].vertices[k] == i) {
						touches = true;
					}
				}
				if (touches) {
					f.x += m_pVertexArray[m_pPolyArray[j].c].x;
					f.y += m_pVertexArray[m_pPolyArray[j].c].y;
					f.z += m_pVertexArray[m_pPolyArray[j].c].z;
					neighboringPolys++;
				}
		}
		if (neighboringPolys == 0) {
			continue;
		}
		f.x /= neighboringPolys;
		f.y /= neighboringPolys;
		f.z /= neighboringPolys;
		PAE3D_Point r;
		r.x = 0;
		r.y = 0;
		r.z = 0;
		int neighboringEdges = 0;
		for (int j = 0; j < m_nNumEdge; j++) {
			if (m_pEdgeArray[j].v1 == i || m_pEdgeArray[j].v2 == i) {
				r.x += m_pVertexArray[m_pEdgeArray[j].c].x;
				r.y += m_pVertexArray[m_pEdgeArray[j].c].y;
				r.z += m_pVertexArray[m_pEdgeArray[j].c].z;
				neighboringEdges++;
			}
		}
		if (neighboringEdges == 0) {
			continue;
		}
		r.x /= neighboringEdges;
		r.y /= neighboringEdges;
		r.z /= neighboringEdges;
		int n = neighboringPolys;
		float w1 = (n-2.0);
		float w2 = 1.0;
		float w3 = 1.0;
		m_pVertexArray[i].x = (w3*f.x + w2*r.x + w1*m_pVertexArray[i].x)/n;
		m_pVertexArray[i].y = (w3*f.y + w2*r.y + w1*m_pVertexArray[i].y)/n;
		m_pVertexArray[i].z = (w3*f.z + w2*r.z + w1*m_pVertexArray[i].z)/n;
	}

	// splitting original edges in half
	for (unsigned int i = 0; i < edges; i++) {
		PAE3D_Edge e;
		e.v1 = m_pEdgeArray[i].c;
		e.v2 = m_pEdgeArray[i].v2;
		e.hasPoly1 = false;
		e.hasPoly2 = false;
		AddEdge(e);
		m_pEdgeArray[i].v2 = m_pEdgeArray[i].c;
		m_pEdgeArray[i].hasPoly1 = false;
		m_pEdgeArray[i].hasPoly2 = false;
	}
	// creating new faces
	for (unsigned int i = 0; i < polys; i++) {
		unsigned int* oldVertices = m_pPolyArray[i].vertices;
		unsigned int* oldEdges = m_pPolyArray[i].edges;
		int oldVertexCount = m_pPolyArray[i].vertexCount;
		m_pPolyArray[i].vertexCount = 4;
		m_pPolyArray[i].vertices = new unsigned int[4];
		m_pPolyArray[i].vertices[0] = oldVertices[0];
		m_pPolyArray[i].vertices[1] = m_pEdgeArray[oldEdges[0]].c;
		m_pPolyArray[i].vertices[2] = m_pPolyArray[i].c;
		m_pPolyArray[i].vertices[3] = m_pEdgeArray[oldEdges[oldVertexCount-1]].c;
		m_pPolyArray[i].edges = new unsigned int[4];
		m_pPolyArray[i].n = PolyNormal(i);
		for (int j = 1; j < oldVertexCount; j++) {
			PAE3D_Polygon poly2;
			poly2.vertexCount = 4;
			poly2.vertices = new unsigned int[4];
			poly2.vertices[0] = oldVertices[j];
			poly2.vertices[1] = m_pEdgeArray[oldEdges[j]].c;
			poly2.vertices[2] = m_pPolyArray[i].c;
			poly2.vertices[3] = m_pEdgeArray[oldEdges[j-1]].c;
			poly2.selected = m_pPolyArray[i].selected;
			poly2.edges = new unsigned int[4];
			poly2.c = 0;
			poly2.mat = m_pPolyArray[i].mat;
			AddPoly(poly2);
			m_pPolyArray[m_nNumPolygon-1].n = PolyNormal(m_nNumPolygon-1);
		}
		delete(oldVertices);
		delete(oldEdges);
	}
	DeselectEverything();
	AssignIntermediatePointers();
}

void Model::Subdivide() {
	// make face points
	unsigned int points = m_nNumPoint;
	unsigned int edges = m_nNumEdge;
	unsigned int polys = m_nNumPolygon;
	// finding face points
	for (int i = 0; i < m_nNumPolygon; i++) {
		if (m_pPolyArray[i].selected) {
			m_pPolyArray[i].c = m_nNumPoint;
			PAE3D_Point p = PolyCenter(i);
			p.selected = true;
			AddVertex(p);
		}
	}
	// finding edge points
	for (int i = 0; i < m_nNumEdge; i++) {
		if (m_pEdgeArray[i].selected) {
			PAE3D_Point v1 = m_pVertexArray[m_pEdgeArray[i].v1];
			PAE3D_Point v2 = m_pVertexArray[m_pEdgeArray[i].v2];
			PAE3D_Point p;
			p.x = (v1.x + v2.x)/2;
			p.y = (v1.y + v2.y)/2;
			p.z = (v1.z + v2.z)/2;
			p.selected = true;
			m_pEdgeArray[i].c = m_nNumPoint;
			AddVertex(p);
		}
	}
	// joining face points to edge points
	for (int i = 0; i < m_nNumPolygon; i++) {
		if (m_pPolyArray[i].selected) {
			for (int j = 0; j < m_pPolyArray[i].vertexCount; j++) {
				PAE3D_Edge e;
				e.v1 = m_pPolyArray[i].c;
				e.v2 = m_pEdgeArray[m_pPolyArray[i].edges[j]].c;
				e.hasPoly1 = false;
				e.hasPoly2 = false;
				AddEdge(e);
			}
		}
	}
	// splitting original edges in half
	for (unsigned int i = 0; i < edges; i++) {
		if (m_pEdgeArray[i].selected) {
			int other = -1;
			if (m_pEdgeArray[i].hasPoly1 && !m_pPolyArray[m_pEdgeArray[i].poly1].selected) {
				other = m_pEdgeArray[i].poly1;
			}
			else if (m_pEdgeArray[i].hasPoly2 && !m_pPolyArray[m_pEdgeArray[i].poly2].selected) {
				other = m_pEdgeArray[i].poly2;
			}
			PAE3D_Edge e;
			e.v1 = m_pEdgeArray[i].c;
			e.v2 = m_pEdgeArray[i].v2;
			e.hasPoly1 = false;
			e.hasPoly2 = false;
			e.selected = true;
			int c = m_pEdgeArray[i].c;
			int en = m_nNumPoint;
			AddEdge(e);
			m_pEdgeArray[i].v2 = m_pEdgeArray[i].c;
			m_pEdgeArray[i].hasPoly1 = false;
			m_pEdgeArray[i].hasPoly2 = false;
			// fixing adding c to adjacent polygon
			if (other != -1) {
				m_pPolyArray[other].vertexCount++;
				unsigned int* oldVerts = m_pPolyArray[other].vertices;
				unsigned int* oldEdges = m_pPolyArray[other].edges;
				m_pPolyArray[other].vertices = new unsigned int[m_pPolyArray[other].vertexCount];
				m_pPolyArray[other].edges = new unsigned int[m_pPolyArray[other].vertexCount];
				int copied = 0;
				for (int j = 0; j < m_pPolyArray[other].vertexCount - 1; j++) {
					printf("%d\n", j);
					m_pPolyArray[other].vertices[j+copied] = oldVerts[j];
					if (-1 == FindEdge(oldVerts[j], oldVerts[(j+1)%(m_pPolyArray[other].vertexCount-1)])) {
						m_pPolyArray[other].edges[j+copied] = FindEdge(oldVerts[j], c);
						copied++;
						m_pPolyArray[other].vertices[j+copied] = c;
						m_pPolyArray[other].edges[j+copied] = FindEdge(oldVerts[(j+1)%(m_pPolyArray[other].vertexCount-1)], c);
					} else {
						m_pPolyArray[other].edges[j+copied] = oldEdges[j];
					}
				}
				printf("done\n");
			}
		}
	}
	printf("done2\n");
	// creating new faces
	for (unsigned int i = 0; i < polys; i++) {
		if (m_pPolyArray[i].selected) {
			unsigned int* oldVertices = m_pPolyArray[i].vertices;
			unsigned int* oldEdges = m_pPolyArray[i].edges;
			int oldVertexCount = m_pPolyArray[i].vertexCount;
			m_pPolyArray[i].vertexCount = 4;
			m_pPolyArray[i].vertices = new unsigned int[4];
			m_pPolyArray[i].vertices[0] = oldVertices[0];
			m_pPolyArray[i].vertices[1] = m_pEdgeArray[oldEdges[0]].c;
			m_pPolyArray[i].vertices[2] = m_pPolyArray[i].c;
			m_pPolyArray[i].vertices[3] =
					m_pEdgeArray[oldEdges[oldVertexCount - 1]].c;
			m_pPolyArray[i].edges = new unsigned int[4];
			m_pPolyArray[i].n = PolyNormal(i);
			for (int j = 1; j < oldVertexCount; j++) {
				PAE3D_Polygon poly2;
				poly2.vertexCount = 4;
				poly2.vertices = new unsigned int[4];
				poly2.vertices[0] = oldVertices[j];
				poly2.vertices[1] = m_pEdgeArray[oldEdges[j]].c;
				poly2.vertices[2] = m_pPolyArray[i].c;
				poly2.vertices[3] = m_pEdgeArray[oldEdges[j - 1]].c;
				poly2.selected = m_pPolyArray[i].selected;
				poly2.edges = new unsigned int[4];
				poly2.c = 0;
				poly2.mat = m_pPolyArray[i].mat;
				poly2.selected = true;
				AddPoly(poly2);
				m_pPolyArray[m_nNumPolygon - 1].n = PolyNormal(m_nNumPolygon - 1);
			}
			delete (oldVertices);
			delete (oldEdges);
		}
	}
	AssignIntermediatePointers();
	for (int i = 0; i < m_nNumPolygon; i++) {
		m_pPolyArray[i].n = PolyNormal(i);
	}
	for (int i = 0; i < m_nNumPoint; i++) {
		CalculateNormal(i);
	}
}

void Model::MoveSelected(float dx, float dy, float dz) {
	for (int i = 0; i < m_nNumPoint; i++) {
		if (m_pVertexArray[i].selected) {
			m_pVertexArray[i].x += dx;
			m_pVertexArray[i].y += dy;
			m_pVertexArray[i].z += dz;
		}
	}
	m_selectedCenter.x += dx;
	m_selectedCenter.y += dy;
	m_selectedCenter.z += dz;
	for (int i = 0; i < m_nNumPolygon; i++) {
		m_pPolyArray[i].n = PolyNormal(i);
	}
	for (int i = 0; i < m_nNumPoint; i++) {
		CalculateNormal(i);
	}
}

void Model::ScaleSelected(float dx, float dy, float dz) {
	for (int i = 0; i < m_nNumPoint; i++) {
		if (m_pVertexArray[i].selected) {
			m_pVertexArray[i].x += dx * (m_pVertexArray[i].x - m_selectedCenter.x);
			m_pVertexArray[i].y += dy * (m_pVertexArray[i].y - m_selectedCenter.y);
			m_pVertexArray[i].z += dz * (m_pVertexArray[i].z - m_selectedCenter.z);
		}
	}
	for (int i = 0; i < m_nNumPolygon; i++) {
		m_pPolyArray[i].n = PolyNormal(i);
	}
	for (int i = 0; i < m_nNumPoint; i++) {
		CalculateNormal(i);
	}
}

void Model::CalculateNormal(int id) {
	PAE3D_Normal n;
	n.x = 0;
	n.y = 0;
	n.z = 0;
	int count = 0;
	for (int i = 0; i < m_pVertexArray[id].edgeCount; i++) {
		int edge = m_pVertexArray[id].edges[i];
		if (m_pEdgeArray[edge].hasPoly1) {
			PAE3D_Normal n1 = m_pPolyArray[m_pEdgeArray[edge].poly1].n;
			n.x += n1.x;
			n.y += n1.y;
			n.z += n1.z;
			count++;
		}
		if (m_pEdgeArray[edge].hasPoly2) {
			PAE3D_Normal n2 = m_pPolyArray[m_pEdgeArray[edge].poly2].n;
			n.x += n2.x;
			n.y += n2.y;
			n.z += n2.z;
			count++;
		}
	}

	n.x /= count;
	n.y /= count;
	n.z /= count;
	m_pVertexArray[id].n = n;
}

unsigned int Model::FindEdgeUsingDependancies(unsigned int v1, unsigned  int v2) {
	for (int i = 0; i < m_pVertexArray[v1].edgeCount; i++) {
		if (m_pEdgeArray[m_pVertexArray[v1].edges[i]].v1 == v2 || m_pEdgeArray[m_pVertexArray[v1].edges[i]].v1 == v2) {
			return m_pVertexArray[v1].edges[i];
		}
	}
	return -1;
}

unsigned int Model::FindEdge(unsigned int v1, unsigned  int v2) {
	for (int i = 0; i < m_nNumEdge; i++) {
		if ((m_pEdgeArray[i].v1 == v1 && m_pEdgeArray[i].v2 == v2)
				|| (m_pEdgeArray[i].v2 == v1 && m_pEdgeArray[i].v1 == v2)) {
			return i;
		}
	}
	return -1;
}

void Model::RenderVertices(float zoom) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	for (int i = 0; i < m_nNumPoint; i++) {
		float radius = 1;
		if(picking){
			int id = i + PAE3D_COLORPADDING;
			int r = (id & 0x000000FF) >> 0;
			int g = (id & 0x0000FF00) >> 8;
			int b = (id & 0x00FF0000) >> 16;
			glColor3f(r/255.0, g/255.0, b/255.0);
			radius = 4;
		}
		else if (m_hasSelected && m_pVertexArray[i].selected) {
			glColor3f(1.0, 0.0, 0.0);
		}
		else {
			glColor3f(0, 0, 0);
		}
		glPushMatrix();
		PAE3D_Point p = m_pVertexArray[i];
		glTranslatef(p.x, p.y, p.z);
		glutSolidSphere(radius/height * zoom, 4, 4);
		glPopMatrix();
	}
}

void Model::RenderEdges(float zoom) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	for (int i = 0; i < m_nNumEdge; i++) {
		if(picking){
			int id = i + PAE3D_COLORPADDING;
			int r = (id & 0x000000FF) >> 0;
			int g = (id & 0x0000FF00) >> 8;
			int b = (id & 0x00FF0000) >> 16;
			glColor3f(r/255.0, g/255.0, b/255.0);
		}
		else if (m_hasSelected && m_pEdgeArray[i].selected) {
			glColor3f(1.0, 0.0, 0.0);
		}
		else {
			glColor3f(0, 0, 0);
		}
		PAE3D_Edge edge = m_pEdgeArray[i];
		PAE3D_Point p1 = m_pVertexArray[edge.v1];
		PAE3D_Point p2 = m_pVertexArray[edge.v2];
		if (p1.z < p2.z) {
			PAE3D_Point tempP = p1;
			p1 = p2;
			p2 = tempP;
		}
		glPushMatrix();
		glTranslatef(p2.x, p2.y, p2.z);
		float length = sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z));
		PAE3D_Point t;
		PAE3D_Point v1;
		PAE3D_Point v2;
		v1.x = 0;
		v1.y = 0;
		v1.z = 1;
		v2.x = (p1.x - p2.x)/length;
		v2.y = (p1.y - p2.y)/length;
		v2.z = (p1.z - p2.z)/length;
		float dot = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
		float crossX = v1.y * v2.x - v2.y * v1.z;
		float crossY = (v1.z * v2.x - v2.z * v1.x);
		float crossZ = v1.x * v2.y - v2.x * v1.y;
		t.x = crossX;
		t.y = crossY;
		t.z = crossZ;
		float tLength = sqrt(t.x*t.x+t.y*t.y+t.z*t.z);
		t.x /= tLength;
		t.y /= tLength;
		t.z /= tLength;
		float angle = acos(dot);
		angle = angle * 180 / M_PI;
		glRotatef(angle, t.x, t.y, t.z);
		gluCylinder(quadric, 0.5 / height * zoom, 0.5 / height * zoom, length, 4, 1);
		glPopMatrix();
	}
}

void Model::RenderFaces(Color* cols, bool phong) {
	for (int i = 0; i < m_nNumPolygon; i++) {
		PAE3D_Polygon poly = m_pPolyArray[i];
		PAE3D_Material* mat = cols->GetMaterial(poly.mat);
		if(picking){
			int id = i + PAE3D_COLORPADDING;
			int r = (id & 0x000000FF) >> 0;
			int g = (id & 0x0000FF00) >> 8;
			int b = (id & 0x00FF0000) >> 16;
			glColor3f(r/255.0, g/255.0, b/255.0);
		}
		else if (m_hasSelected && poly.selected) {
			glColor3f(0.5, 0.3, 0.5);
		}
		else {
			glColor4f(mat->col.r, mat->col.g, mat->col.b, mat->col.a);
		}
		if (phong) {
			/*glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GLglTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			glBindTexture(GL_TEXTURE_2D, cubeMap);*/
			glEnable(GL_TEXTURE_CUBE_MAP);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			glEnable(GL_TEXTURE_GEN_R);
		} else {
			glNormal3f(poly.n.x, poly.n.y, poly.n.z);
		}
		glBegin(GL_POLYGON);
		for(int j = 0 ; j < poly.vertexCount; j++) {
			PAE3D_Point p = m_pVertexArray[poly.vertices[j]];
			if (phong) {
				glNormal3f(p.n.x, p.n.y, p.n.z);
			}
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
		if (phong) {
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_GEN_R);
			glDisable(GL_TEXTURE_CUBE_MAP);
		}
		glColor3f(1, 0, 0);
		// RENDER NORMALS
		if (PAE3D_RENDER_NORMALS) {
			glBegin(GL_LINES);
			PAE3D_Point p = PolyCenter(i);
			PAE3D_Normal n = poly.n;
			glVertex3f(p.x, p.y, p.z);
			glVertex3f(p.x + n.x, p.y + n.y, p.z + n.z);
			glEnd();
		}
	}
}

void Model::RenderPicker(float zoom, int handleMode) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	picking = true;
	switch (m_SelectMode) {
	case PAE3D_SELECT_FACES:
		RenderFaces(NULL, false);
		break;
	case PAE3D_SELECT_EDGES:
		RenderEdges(zoom);
		break;
	case PAE3D_SELECT_VERTICES:
		RenderVertices(zoom);
		break;
	}
	if (m_hasSelected) {
		glClear(GL_DEPTH_BUFFER_BIT);
		float width = 0.5 / height * zoom;
		float length = 20.0 / height * zoom;
		glPushMatrix();
			glTranslatef(m_selectedCenter.x, m_selectedCenter.y, m_selectedCenter.z);
			switch(handleMode){
			case PAE3D_HANLE_MOVE:
				glColor3f(PAE3D_SELECT_X_HANDLE/255.0, 0, 0);
				Handle::RenderXHandleMove(quadric, width, length);
				glColor3f(PAE3D_SELECT_Y_HANDLE/255.0, 0, 0);
				Handle::RenderYHandleMove(quadric, width, length);
				glColor3f(PAE3D_SELECT_Z_HANDLE/255.0, 0, 0);
				Handle::RenderZHandleMove(quadric, width, length);
				break;
			case PAE3D_HANLE_SCALE:
				glColor3f(PAE3D_SELECT_X_HANDLE / 255.0, 0, 0);
				Handle::RenderXHandleScale(quadric, width, length);
				glColor3f(PAE3D_SELECT_Y_HANDLE / 255.0, 0, 0);
				Handle::RenderYHandleScale(quadric, width, length);
				glColor3f(PAE3D_SELECT_Z_HANDLE / 255.0, 0, 0);
				Handle::RenderZHandleScale(quadric, width, length);
				break;
			}
		glPopMatrix();
	}
	picking = false;
}

void Model::DeselectEverything() {
	m_hasSelected = false;
	for (int i = 0; i < m_nNumPolygon; i++) {
		m_pPolyArray[i].selected = false;
	}
	for (int i = 0; i < m_nNumEdge; i++) {
		m_pEdgeArray[i].selected = false;
	}
	for (int i = 0; i < m_nNumPoint; i++) {
		m_pVertexArray[i].selected = false;
	}
}

void Model::ProcessSelection(int cursorX, int cursorY, bool shift, bool leftClick, int mat){
	SelectedHandle = PAE3D_SELECT_NO_HANDLE;
	GLint viewport[4];
	GLubyte pixel[3];
	glGetIntegerv(GL_VIEWPORT,viewport);
	glReadPixels(cursorX,viewport[3]-cursorY,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);
	int id = (pixel[0] << 0) + (pixel[1] << 8) + (pixel[2] << 16) - PAE3D_COLORPADDING;
	if (leftClick && mat != -1 && id < m_nNumPolygon) {
		m_pPolyArray[id].mat = mat;
	}
	switch (m_SelectMode) {
	case PAE3D_SELECT_FACES:
		if (id < m_nNumPolygon && !leftClick) {
			if (m_pPolyArray[id].selected) {
				if (shift) {
					/*deselects face from group*/
					m_pPolyArray[id].selected = false;
					m_hasSelected = false;
					for (int i = 0; i < m_nNumPolygon; i++) {
						if (m_pPolyArray[i].selected) {
							m_hasSelected = true;
							break;
						}
					}
					CascadeFaceSelection(id);
				}
				else {
					/*single select, deselecting any group*/
					DeselectEverything();
					m_pPolyArray[id].selected = true;
					m_hasSelected = true;
					CascadeFaceSelection(id);
				}
			}
			else {
				if (shift) {
					/*adds to selection group*/
					m_pPolyArray[id].selected = true;
					m_hasSelected = true;
					CascadeFaceSelection(id);
				}
				else {
					/*single select, deselecting any group*/
					DeselectEverything();
					m_pPolyArray[id].selected = true;
					m_hasSelected = true;
					CascadeFaceSelection(id);
				}
			}
		}
		break;
	case PAE3D_SELECT_EDGES:
		if (id < m_nNumEdge && !leftClick) {
			if (m_pEdgeArray[id].selected) {
				if (shift) {
					m_pEdgeArray[id].selected = false;
					m_hasSelected = false;
					for (int i = 0; i < m_nNumEdge; i++) {
						if (m_pEdgeArray[i].selected) {
							m_hasSelected = true;
						break;
						}
					}
					CascadeEdgeSelection(id);
				}
				else {
					for (int i = 0; i < m_nNumEdge; i++) {
						m_pEdgeArray[i].selected = false;
					}
					m_pEdgeArray[id].selected = true;
					m_hasSelected = true;
					CascadeEdgeSelection(id);
				}
			} else {
				if (shift) {
					m_pEdgeArray[id].selected = true;
					m_hasSelected = true;
					CascadeEdgeSelection(id);
				} else {
					for (int i = 0; i < m_nNumEdge; i++) {
						m_pEdgeArray[i].selected = false;
					}
					for (int i = 0; i < m_nNumPoint; i++) {
						m_pVertexArray[i].selected = false;
					}
					m_pEdgeArray[id].selected = true;
					m_hasSelected = true;
					CascadeEdgeSelection(id);
				}
			}
		}
		break;

	case PAE3D_SELECT_VERTICES:
		if (id < m_nNumPoint && !leftClick) {
			if (m_pVertexArray[id].selected) {
				if (shift) {
					/*deselects face from group*/
					m_pVertexArray[id].selected = false;
					m_hasSelected = false;
					CascadeVertexSelection();
					for (int i = 0; i < m_nNumPoint; i++) {
						if (m_pVertexArray[i].selected) {
							m_hasSelected = true;

						break;
						}
					}
				} else {
					/*single select, deselecting any group*/
					for (int i = 0; i < m_nNumPoint; i++) {
						m_pVertexArray[i].selected = false;
					}
					m_pVertexArray[id].selected = true;
					m_hasSelected = false;
					CascadeVertexSelection();
				}
			} else {
				if (shift) {
					/*adds to selection group*/
					m_pVertexArray[id].selected = true;
					m_hasSelected = true;
					CascadeVertexSelection();
				} else {
					/*single select, deselecting any group*/
					for (int i = 0; i < m_nNumPoint; i++) {
						m_pVertexArray[i].selected = false;
					}
					m_pVertexArray[id].selected = true;
					m_hasSelected = true;
					CascadeVertexSelection();
				}
			}
		}
		break;
	}
	if (id < 0) {
		id += PAE3D_COLORPADDING;
		switch (id) {
		case PAE3D_SELECT_X_HANDLE:
		case PAE3D_SELECT_Y_HANDLE:
		case PAE3D_SELECT_Z_HANDLE:
			SelectedHandle = id;
			break;
		}
	}
	ResetHandlePosition();
}
void Model::ResetHandlePosition(){
	if(m_hasSelected){
		int count = 0;
		m_selectedCenter.x = 0;
		m_selectedCenter.y = 0;
		m_selectedCenter.z = 0;
		for (int i = 0; i < m_nNumPoint; i++) {
			PAE3D_Point p = m_pVertexArray[i];
			if (p.selected) {
				m_selectedCenter.x += p.x;
				m_selectedCenter.y += p.y;
				m_selectedCenter.z += p.z;
				count++;
			}
		}
		m_selectedCenter.x /= count;
		m_selectedCenter.y /= count;
		m_selectedCenter.z /= count;
	}
}

void Model::RenderSelectedFacesHandle(float zoom, int handleMode) {
	if (m_hasSelected) {
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		float width = 0.5/height* zoom;
		float length = 20.0/height* zoom;
		glPushMatrix();
			switch(handleMode) {
			case PAE3D_HANLE_MOVE:
				glTranslatef(m_selectedCenter.x, m_selectedCenter.y, m_selectedCenter.z);
				glColor3f(1, 0, 0);
				Handle::RenderXHandleMove(quadric, width, length);
				glColor3f(0, 1, 0);
				Handle::RenderYHandleMove(quadric, width, length);
				glColor3f(0, 0, 1);
				Handle::RenderZHandleMove(quadric, width, length);
				break;
			case PAE3D_HANLE_SCALE:
				glTranslatef(m_selectedCenter.x, m_selectedCenter.y, m_selectedCenter.z);
				glColor3f(1, 0, 0);
				Handle::RenderXHandleScale(quadric, width, length);
				glColor3f(0, 1, 0);
				Handle::RenderYHandleScale(quadric, width, length);
				glColor3f(0, 0, 1);
				Handle::RenderZHandleScale(quadric, width, length);
				break;
			}
		glPopMatrix();
	}
}

void Model::SetSelectType(int type){
	m_SelectMode = type;
}

void Model::CascadeFaceSelection(int face){
	PAE3D_Polygon poly = m_pPolyArray[face];
	//PAE3D_PrintPoly(poly);
	if (poly.selected) {
		for (int i = 0; i < poly.vertexCount; i++) {
			m_pVertexArray[poly.vertices[i]].selected = true;
		}
		for (int i = 0; i < poly.vertexCount; i++) {
			m_pEdgeArray[poly.edges[i]].selected = true;
		}
	}
	else {
		for (int i = 0; i < m_nNumEdge; i++) {
			m_pEdgeArray[i].selected = false;
		}
		for (int i = 0; i < m_nNumPoint; i++) {
			m_pVertexArray[i].selected = false;
		}
		for (int i = 0 ; i < m_nNumPolygon; i++) {
			if (m_pPolyArray[i].selected) {
				CascadeFaceSelection(i);
			}
		}
	}
}
void Model::CascadeEdgeSelection(int edge){
	PAE3D_Edge currentEdge = m_pEdgeArray[edge];

	if (currentEdge.selected) {
		m_pVertexArray[currentEdge.v1].selected = true;
		m_pVertexArray[currentEdge.v2].selected = true;

		for (int i = 0; i < m_nNumPolygon; i++) {
			PAE3D_Polygon poly = m_pPolyArray[i];
			bool hasVerts = true;
			if (poly.vertexCount>2) {
				for (int k = 0; k < poly.vertexCount; k++) {
					if (!m_pEdgeArray[poly.edges[k]].selected) {
						hasVerts = false;
						break;
					}
				}
			}
			m_pPolyArray[i].selected = hasVerts;
		}


	} else {
		m_pVertexArray[currentEdge.v1].selected = false;
		m_pVertexArray[currentEdge.v2].selected = false;

		for (int i = 0 ; i < m_nNumEdge; i++) {
			if (m_pEdgeArray[i].selected) {
				CascadeEdgeSelection(i);
			}
		}
	}
}
void Model::CascadeVertexSelection(){
	for (int i = 0; i < m_nNumPolygon; i++) {
		PAE3D_Polygon poly = m_pPolyArray[i];
		bool hasVerts = true;
		for (int k = 0; k < poly.vertexCount; k++) {
			if (!m_pVertexArray[poly.vertices[k]].selected) {
				hasVerts = false;
				break;
			}
		}
		m_pPolyArray[i].selected = hasVerts;
	}
	for(int i = 0; i < m_nNumEdge; i++){
		PAE3D_Edge currentEdge = m_pEdgeArray[i];
		if(m_pVertexArray[currentEdge.v1].selected && m_pVertexArray[currentEdge.v2].selected){
			m_pEdgeArray[i].selected = true;
		}else{
			m_pEdgeArray[i].selected = false;
		}
	}
}

void Model::Extrude(){
	int innerVertCount = 0;int outerVertCount = 0;
	int innerEdgeCount = 0;int outerEdgeCount = 0;
	int faceCount = 0;
	//deselect erroneous
	for (int i = 0; i < m_nNumPoint; i++) {
		if (m_pVertexArray[i].selected) {
			m_pVertexArray[i].selected = false;
			}
	}
	for (int i = 0; i < m_nNumEdge; i++) {
		if (m_pEdgeArray[i].selected) {
			m_pEdgeArray[i].selected = false;
			}
	}
	//face Select and Count
	for (int i = 0; i < m_nNumPolygon; i++) {
		if (m_pPolyArray[i].selected) {
			faceCount++;
			CascadeFaceSelection(i);
		}

	}
	if(faceCount ==0 ){	return;}

	for (int i = 0; i < m_nNumEdge; i++) {
		if (m_pEdgeArray[i].selected) {
			if(m_pPolyArray[m_pEdgeArray[i].poly1].selected && m_pPolyArray[m_pEdgeArray[i].poly2].selected){
				innerEdgeCount++;
			}else{
				outerEdgeCount++;
			}
		}
	}
	unsigned int* outerEdgeInd = new unsigned int[outerEdgeCount];
	unsigned int* innerEdgeInd = new unsigned int[innerEdgeCount];
	int outerPosition = 0; int innerPosition = 0;

	for (int i = 0; i < m_nNumEdge; i++) {
		if (m_pEdgeArray[i].selected) {
			if(m_pPolyArray[m_pEdgeArray[i].poly1].selected && m_pPolyArray[m_pEdgeArray[i].poly2].selected){
				innerEdgeInd[innerPosition] = i;
				innerPosition++;
			}else{
				outerEdgeInd[outerPosition] = i;
				outerPosition++;
			}
		}
	}


	for (int i = 0; i < m_nNumPoint; i++) {
		if (m_pVertexArray[i].selected) {
			bool outer = false;
			for (int k = 0; k < outerEdgeCount; k++) {
				if (m_pEdgeArray[outerEdgeInd[k]].v1 == i || m_pEdgeArray[outerEdgeInd[k]].v2 == i){
					outer = true;
					break;
				}
			}
			if(outer){
				outerVertCount++;
			}else{
				innerVertCount++;
			}
		}
	}

	unsigned int* outerVertsInd = new unsigned int[outerVertCount];
	unsigned int* innerVertsInd = new unsigned int[innerVertCount];
	outerPosition = 0; innerPosition = 0;

	for (int i = 0; i < m_nNumPoint; i++) {
		if (m_pVertexArray[i].selected) {
			bool outer = false;
			for (int k = 0; k < outerEdgeCount; k++) {
				if (m_pEdgeArray[outerEdgeInd[k]].v1 == i || m_pEdgeArray[outerEdgeInd[k]].v2 == i){
					outer = true;
					break;
				}
			}
			if(outer){
				outerVertsInd[outerPosition] = i;
				outerPosition++;
			}else{
				innerVertsInd[innerPosition] = i;
				innerPosition++;
			}
		}
	}

	//printf("end sizes ov %i iv %i oe %i ie %i \n", outerVertCount, innerVertCount, outerEdgeCount,innerEdgeCount);

	PAE3D_Point* tempVerts = new PAE3D_Point[outerVertCount];
	PAE3D_Edge* tempEdges = new PAE3D_Edge[outerEdgeCount];
	//PAE3D_Polygon* tempFaces = new PAE3D_Polygon[faceCount];

	//vertices add
	for (int i = 0; i < outerVertCount; i++) {
			PAE3D_Point temp;
			temp.x = m_pVertexArray[outerVertsInd[i]].x;
			temp.y = m_pVertexArray[outerVertsInd[i]].y;
			temp.z = m_pVertexArray[outerVertsInd[i]].z;
			m_pVertexArray[outerVertsInd[i]].selected = false;
			temp.selected = true;
			tempVerts[i] = temp;
	}

	for(int i = 0; i < outerVertCount; i++){
		AddVertex(tempVerts[i]);
	}

	for(int i = 0; i < outerEdgeCount;i++){
		PAE3D_Edge temp;
		temp.selected = true;
		for(int k = 0; k < outerVertCount; k++){
			if(outerVertsInd[k]== m_pEdgeArray[outerEdgeInd[i]].v1){
				temp.v1 = (m_nNumPoint - outerVertCount) + k;
			}
			if(outerVertsInd[k]== m_pEdgeArray[outerEdgeInd[i]].v2){
				temp.v2 = (m_nNumPoint - outerVertCount) + k;
			}

		}
		m_pEdgeArray[outerEdgeInd[i]].selected = false;
		tempEdges[i] = temp;
	}

	for(int i = 0; i < outerEdgeCount; i++){
		AddEdge(tempEdges[i]);
	}

	int pos = 0;
	for (int i = 0; i < m_nNumPolygon; i++) {
		if (m_pPolyArray[i].selected) {
			for (int k = 0; k < m_pPolyArray[i].vertexCount; k++) {
				for (int j = 0; j < outerVertCount; j++) {
					if (outerVertsInd[j] == m_pPolyArray[i].vertices[k]) {
						m_pPolyArray[i].vertices[k] = (m_nNumPoint- outerVertCount) + j;
						break;
					}
				}
				for (int j = 0; j < outerEdgeCount; j++) {
					if (outerEdgeInd[j] == m_pPolyArray[i].edges[k]) {
						m_pPolyArray[i].edges[k] = (m_nNumEdge - outerEdgeCount)+ j;
						break;
					}
				}
			}
			m_pPolyArray[i].selected = true;
			pos++;
		}
	}
	for(int i = 0; i < innerEdgeCount;i++){
		for(int k = 0; k < outerVertCount; k++){

			if(outerVertsInd[k]== m_pEdgeArray[innerEdgeInd[i]].v1){
				m_pEdgeArray[innerEdgeInd[i]].v1 = (m_nNumPoint - outerVertCount) + k;
			}
			if(outerVertsInd[k]== m_pEdgeArray[innerEdgeInd[i]].v2){
				m_pEdgeArray[innerEdgeInd[i]].v2 = (m_nNumPoint - outerVertCount) + k;
			}
		}
	}

	for(int i = 0; i < outerEdgeCount; i++){
		PAE3D_Polygon left = m_pPolyArray[m_pEdgeArray[outerEdgeInd[i]].poly1];
		PAE3D_Polygon right = m_pPolyArray[m_pEdgeArray[outerEdgeInd[i]].poly2];

		unsigned int v1 = m_pEdgeArray[outerEdgeInd[i]].v1;
		unsigned int v2 = m_pEdgeArray[outerEdgeInd[i]].v2;
		unsigned int v1s = m_pEdgeArray[(m_nNumEdge - outerEdgeCount) + i].v1;
		unsigned int v2s = m_pEdgeArray[(m_nNumEdge - outerEdgeCount) + i].v2;

		int v1lp = -1;int v2lp = -1;
		int v1rp = -1; int v2rp = -1;

		for (int k = 0; k < left.vertexCount; k++) {
			if (left.vertices[k] == v1s) {
				v1lp = k;
			}
			if (left.vertices[k] == v2s) {
				v2lp = k;
			}
			if (left.vertices[k] == v1) {
				v1lp = k;
			}
			if (left.vertices[k] == v2) {
				v2lp = k;
			}
		}
		for (int k = 0; k < right.vertexCount; k++) {
			if (right.vertices[k] == v1s) {
				v1rp = k;
			}
			if (right.vertices[k] == v2s) {
				v2rp = k;
			}
			if (right.vertices[k] == v1) {
				v1rp = k;
			}
			if (right.vertices[k] == v2) {
				v2rp = k;
			}
		}

		PAE3D_Polygon temp;
		temp.vertices = new unsigned int[4];
		temp.vertexCount = 4;

		if (left.selected) {
			if ((v1lp == 0 && v2lp == 3) || (v2lp == 0 && v1lp == 3)) {
				if (v1lp == 0 && v2lp == 3) {
					temp.vertices[0] = v2;
					temp.vertices[1] = v1;
				} else {
					temp.vertices[0] = v1;
					temp.vertices[1] = v2;
				}
			} else if (v1lp < v2lp) {
				temp.vertices[0] = v1;
				temp.vertices[1] = v2;
			} else if (v1lp > v2lp) {
				temp.vertices[0] = v2;
				temp.vertices[1] = v1;
			}

			if ((v1rp == 0 && v2rp == 3) || (v2rp == 0 && v1rp == 3)) {
				if (v1rp == 0 && v2rp == 3) {
					temp.vertices[2] = v2s;
					temp.vertices[3] = v1s;
				} else {
					temp.vertices[2] = v1s;
					temp.vertices[3] = v2s;
				}
			} else if (v1rp < v2rp) {
				temp.vertices[2] = v1s;
				temp.vertices[3] = v2s;
			} else if (v1rp > v2rp) {
				temp.vertices[2] = v2s;
				temp.vertices[3] = v1s;
			}
			temp.selected = false;
			AddPoly(temp);
			m_pPolyArray[m_nNumPolygon-1].n = PolyNormal(m_nNumPolygon-1);
		}
		if (right.selected) {
			if ((v1lp == 0 && v2lp == 3) || (v2lp == 0 && v1lp == 3)) {
				if (v1lp == 0 && v2lp == 3) {
					temp.vertices[0] = v2s;
					temp.vertices[1] = v1s;
				} else {
					temp.vertices[0] = v1s;
					temp.vertices[1] = v2s;
				}
			} else if (v1lp < v2lp) {
				temp.vertices[0] = v1s;
				temp.vertices[1] = v2s;
			} else if (v1lp > v2lp) {
				temp.vertices[0] = v2s;
				temp.vertices[1] = v1s;
			}
			if ((v1rp == 0 && v2rp == 3) || (v2rp == 0 && v1rp == 3)) {
				if (v1rp == 0 && v2rp == 3) {
					temp.vertices[2] = v2;
					temp.vertices[3] = v1;
				} else {
					temp.vertices[2] = v1;
					temp.vertices[3] = v2;
				}
			} else if (v1rp < v2rp) {
				temp.vertices[2] = v1;
				temp.vertices[3] = v2;
			} else if (v1rp > v2rp) {
				temp.vertices[2] = v2;
				temp.vertices[3] = v1;
			}
			temp.selected = false;
			AddPoly(temp);
			m_pPolyArray[m_nNumPolygon-1].n = PolyNormal(m_nNumPolygon-1);
		}
	}

	for(int i = 0; i < outerVertCount; i++){
		PAE3D_Edge temp;
		temp.selected = false;
		temp.v1 = outerVertsInd[i];
		temp.v2 = (m_nNumPoint - outerVertCount) + i;
		AddEdge(temp);
	}

	AssignIntermediatePointers();
	ResetHandlePosition();
}

void Model::Duplicate(){
	int vertCount = 0;
	int edgeCount = 0;
	int faceCount =0;
	for (int i = 0; i < m_nNumPoint; i++) {
		if (m_pVertexArray[i].selected == true) {vertCount++;}
	}
	for (int i = 0; i < m_nNumEdge; i++) {
		if (m_pEdgeArray[i].selected) {edgeCount++;}
	}
	for (int i = 0; i < m_nNumPolygon; i++) {
		if (m_pPolyArray[i].selected) {faceCount++;}
	}
	unsigned int* tempVertsInd = new unsigned int[vertCount];
	unsigned int* tempEdgeInd = new unsigned int[edgeCount];
	PAE3D_Point* tempVerts = new PAE3D_Point[vertCount];
	PAE3D_Edge* tempEdges = new PAE3D_Edge[edgeCount];
	PAE3D_Polygon* tempFaces = new PAE3D_Polygon[faceCount];

	int pos = 0;
	for (int i = 0; i < m_nNumPoint; i++) {
		if (m_pVertexArray[i].selected == true) {
			PAE3D_Point temp;
			temp.x = m_pVertexArray[i].x;
			temp.y = m_pVertexArray[i].y;
			temp.z = m_pVertexArray[i].z;
			m_pVertexArray[i].selected = false;
			temp.selected = true;
			tempVertsInd[pos]= i;
			tempVerts[pos] = temp;
			pos++;
		}
	}

	for(int i = 0; i < vertCount; i++){
		AddVertex(tempVerts[i]);
	}
	pos = 0;
	for(int i = 0; i< m_nNumEdge;i++){
		if (m_pEdgeArray[i].selected) {
			PAE3D_Edge temp;
			for(int k = 0; k< vertCount; k++){
				if(tempVertsInd[k]== m_pEdgeArray[i].v1){
					temp.v1 = (m_nNumPoint - vertCount) + k;
				}
				if(tempVertsInd[k]== m_pEdgeArray[i].v2){
					temp.v2 = (m_nNumPoint - vertCount) + k;
				}
			}
			m_pEdgeArray[i].selected = false;
			temp.selected = true;
			tempEdgeInd[pos]= i;
			tempEdges[pos] = temp;
			pos++;
		}
	}
	for(int i = 0; i < vertCount; i++){
		AddEdge(tempEdges[i]);
	}
	pos = 0;
	for (int i = 0; i < m_nNumPolygon; i++) {
		if (m_pPolyArray[i].selected) {
			PAE3D_Polygon temp;
			temp.vertices = new unsigned int[m_pPolyArray[i].vertexCount];
			temp.vertexCount = m_pPolyArray[i].vertexCount;
			for(int k =0; k < m_pPolyArray[i].vertexCount; k++){
					for(int j= 0; j< vertCount;j++){
						if(tempVertsInd[j]==m_pPolyArray[i].vertices[k]){
							temp.vertices[k] = (m_nNumPoint - vertCount)+j;
							break;
						}
					}
			}
			m_pPolyArray[i].selected = false;
			temp.selected = true;
			tempFaces[pos] = temp;
			pos++;
		}
	}
	for(int i = 0; i < faceCount; i++){
		AddPoly(tempFaces[i]);
	}
}











