#include "Model.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include "Handle.h"

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

	for (int i = 0; i < m_nNumPolygon; i++) {
		m_pPolyArray[i].selected = false;
		m_pPolyArray[i].vertexCount = 4;
		m_pPolyArray[i].n = PolyNormal(i);
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

	for (int i = 0; i < m_nNumEdge; i++) {
		m_pEdgeArray[i].selected = false;
	}
	for (int i = 0; i < m_nNumPoint; i++) {
		m_pVertexArray[i].selected = false;
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
		center.x = m_pVertexArray[poly.vertices[i]].x;
		center.y = m_pVertexArray[poly.vertices[i]].y;
		center.z = m_pVertexArray[poly.vertices[i]].z;
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

void Model::Smooth() {
	/*for (int i = 0; i < m_nNumPolygon; i++) {
		PAE3D_Quad q = m_pQuadArray[i];
		PAE3D_Point v1 = m_pVertexArray[q.v1];
		PAE3D_Point v2 = m_pVertexArray[q.v2];
		PAE3D_Point v3 = m_pVertexArray[q.v3];
		PAE3D_Point v4 = m_pVertexArray[q.v4];
		PAE3D_Point p;
		p.x = (v1.x + v2.x + v3.x + v4.x) / 4;
		p.y = (v1.y + v2.y + v3.y + v4.y) / 4;
		p.z = (v1.z + v2.z + v3.z + v4.z) / 4;
		q.c = m_nNumPoint;
		AddVertex(p);
	}
	for (int i = 0; i < m_nNumEdge; i++) {
		PAE3D_Edge e = m_pEdgeArray[i];
		PAE3D_Point v1 = m_pVertexArray[e.v1];
		PAE3D_Point v2 = m_pVertexArray[e.v2];
		PAE3D_Point p;
		p.x = (v1.x + v2.x) / 2;
		p.y = (v1.y + v2.y) / 2;
		p.z = (v1.z + v2.z) / 2;
		e.c = m_nNumPoint;
		AddVertex(p);
	}
	//for (int i = 0; i < m_nNumPolygon; i++) {
		PAE3D_Quad q = m_pQuadArray[0];
		int ei1 = FindEdge(q.v1, q.v2);
		int ei2 = FindEdge(q.v2, q.v3);
		int ei3 = FindEdge(q.v3, q.v4);
		int ei4 = FindEdge(q.v4, q.v1);
		printf("%d, %d, %d, %d\n", ei1, ei2, ei3, ei4);

		PAE3D_Edge e1;
		e1.v1 = q.c;
		e1.v2 = 0;//m_pEdgeArray[ei1].c;
		e1.c = 0;
		PAE3D_Edge e2;
		e2.v1 = q.c;
		e2.v2 = 0;//m_pEdgeArray[ei2].c;
		e2.c = 0;
		PAE3D_Edge e3;
		e3.v1 = q.c;
		e3.v2 = 0;//m_pEdgeArray[ei3].c;
		e3.c = 0;
		PAE3D_Edge e4;
		e4.v1 = q.c;
		e4.v2 = 0;//m_pEdgeArray[ei4].c;
		e4.c = 0;
		AddEdge(e1);
		AddEdge(e2);
		AddEdge(e3);
		AddEdge(e4);
		m_pQuadArray[i].v3 = m_pQuadArray[i].c;
		m_pQuadArray[i].v2 = m_pEdgeArray[FindEdge(m_pQuadArray[i].v1, m_pQuadArray[i].v2)].c;
		m_pQuadArray[i].v4 = m_pEdgeArray[FindEdge(m_pQuadArray[i].v4, m_pQuadArray[i].v1)].c;
	//}*/
}

void Model::MoveSelected(float dx, float dy, float dz) {
		for (int i = 0; i < m_nNumPoint; i++) {
			if (m_pVertexArray[i].selected) {
				m_pVertexArray[i].x += dx;
				m_pVertexArray[i].y += dy;
				m_pVertexArray[i].z += dz;
			}
		}

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
		if(picking){
			int id = i + PAE3D_COLORPADDING;
			int r = (id & 0x000000FF) >> 0;
			int g = (id & 0x0000FF00) >> 8;
			int b = (id & 0x00FF0000) >> 16;
			glColor3f(r/255.0, g/255.0, b/255.0);
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
		glutSolidSphere(1.0/height * zoom, 20, 20);
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
		gluCylinder(quadric, 0.5 / height * zoom, 0.5 / height * zoom, length, 20, 1);
		glPopMatrix();
	}
}

void Model::RenderFaces() {
	for (int i = 0; i < m_nNumPolygon; i++) {
		PAE3D_Polygon poly = m_pPolyArray[i];
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
			glColor3f(0.77, 0.77, 0.5);
		}
		glNormal3f(poly.n.x, poly.n.y, poly.n.z);
		glBegin(GL_POLYGON);
		for(int j = 0 ; j < poly.vertexCount; j++) {
			PAE3D_Point p = m_pVertexArray[poly.vertices[j]];
			glVertex3f(p.x, p.y, p.z);
		}
		glEnd();
	}
}

void Model::RenderPicker(float zoom) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	picking = true;
	switch (m_SelectMode) {
	case PAE3D_SELECT_FACES:
		RenderFaces();
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
			glColor3f(PAE3D_SELECT_X_HANDLE/255.0, 0, 0);
			Handle::RenderXHandle(quadric, width, length);
			glColor3f(PAE3D_SELECT_Y_HANDLE/255.0, 0, 0);
			Handle::RenderYHandle(quadric, width, length);
			glColor3f(PAE3D_SELECT_Z_HANDLE/255.0, 0, 0);
			Handle::RenderZHandle(quadric, width, length);
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

void Model::ProcessSelection(int cursorX, int cursorY, bool shift, bool onlyHandles){
	SelectedHandle = PAE3D_SELECT_NO_HANDLE;
	GLint viewport[4];
	GLubyte pixel[3];
	glGetIntegerv(GL_VIEWPORT,viewport);
	glReadPixels(cursorX,viewport[3]-cursorY,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);
	int id = (pixel[0] << 0) + (pixel[1] << 8) + (pixel[2] << 16) - PAE3D_COLORPADDING;
	switch (m_SelectMode) {
	case PAE3D_SELECT_FACES:
		if (id < m_nNumPolygon && !onlyHandles) {
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
		if (id < m_nNumEdge && !onlyHandles) {
			if (m_pEdgeArray[id].selected) {
				if (shift) {
					m_pEdgeArray[id].selected = false;
					CascadeEdgeSelection(id);
					m_hasSelected = false;
					for (int i = 0; i < m_nNumEdge; i++) {
						if (m_pEdgeArray[i].selected) {
							m_hasSelected = true;
							CascadeEdgeSelection(i);
						break;
						}
					}
				} else {
					for (int i = 0; i < m_nNumEdge; i++) {
						m_pEdgeArray[i].selected = false;
						CascadeEdgeSelection(i);
					}
					m_pEdgeArray[id].selected = true;
					CascadeEdgeSelection(id);
					m_hasSelected = false;
				}
			} else {
				if (shift) {
					m_pEdgeArray[id].selected = true;
					CascadeEdgeSelection(id);
					m_hasSelected = true;
				} else {
					for (int i = 0; i < m_nNumEdge; i++) {
						m_pEdgeArray[i].selected = false;
						CascadeEdgeSelection(i);
					}
					m_pEdgeArray[id].selected = true;
					CascadeEdgeSelection(id);
					m_hasSelected = true;
				}
			}
		}
		break;

	case PAE3D_SELECT_VERTICES:
		if (id < m_nNumPoint && !onlyHandles) {
			if (m_pVertexArray[id].selected) {
				if (shift) {
					/*deselects face from group*/
					m_pVertexArray[id].selected = false;
					m_hasSelected = false;
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
				}
			} else {
				if (shift) {
					/*adds to selection group*/
					m_pVertexArray[id].selected = true;
					m_hasSelected = true;
				} else {
					/*single select, deselecting any group*/
					for (int i = 0; i < m_nNumPoint; i++) {
						m_pVertexArray[i].selected = false;
					}
					m_pVertexArray[id].selected = true;
					m_hasSelected = true;
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

void Model::RenderSelectedFacesHandle(float zoom) {
	if (m_hasSelected) {
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		float width = 0.5/height* zoom;
		float length = 20.0/height* zoom;
		glPushMatrix();
			glTranslatef(m_selectedCenter.x, m_selectedCenter.y, m_selectedCenter.z);
			glColor3f(1, 0, 0);
			Handle::RenderXHandle(quadric, width, length);
			glColor3f(0, 1, 0);
			Handle::RenderYHandle(quadric, width, length);
			glColor3f(0, 0, 1);
			Handle::RenderZHandle(quadric, width, length);
		glPopMatrix();
	}
}

void Model::SetSelectType(int type){
	m_SelectMode = type;
}

void Model::CascadeFaceSelection(int face){
	PAE3D_Polygon poly = m_pPolyArray[face];
	if (poly.selected) {
		for (int i = 0; i < poly.vertexCount; i++) {
			m_pVertexArray[poly.vertices[i]].selected = true;
		}
		for (int i = 1; i < poly.vertexCount; i++) {
			int edge = FindEdge(poly.vertices[i - 1], poly.vertices[i]);
			if (edge >= 0) {
				m_pEdgeArray[edge].selected = true;
			}
		}
		int edge = FindEdge(poly.vertices[0],
				poly.vertices[poly.vertexCount - 1]);
		if (edge > 0) {
			m_pEdgeArray[edge].selected = true;
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
	PAE3D_Edge e = m_pEdgeArray[edge];
	if (e.selected) {
		m_pVertexArray[e.v1].selected = true;
		m_pVertexArray[e.v2].selected = true;
	}else{
		m_pVertexArray[e.v1].selected = false;
		m_pVertexArray[e.v2].selected = false;
	}

}











