#include "Model.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>
#include "Lights.h"

#include <iostream>

using namespace std;

int numVert, numNorm, numUV, numFace;
GLUquadric* quadric = gluNewQuadric();

Model::Model() {

	m_pVertexArray = NULL;
	m_pNormalArray = NULL;
	m_pEdgeArray = NULL;
	m_pQuadArray = NULL;

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
	m_pQuadArray = new PAE3D_Quad[m_nNumPolygon];

	m_nNumNormal = m_nNumPolygon;
	m_pNormalArray = new PAE3D_Normal[m_nNumNormal];

	m_pQuadArray[0].v1 = 0;
	m_pQuadArray[0].v2 = 1;
	m_pQuadArray[0].v3 = 2;
	m_pQuadArray[0].v4 = 3;
	m_pQuadArray[0].n = 0;
	m_pQuadArray[1].v1 = 7;
	m_pQuadArray[1].v2 = 6;
	m_pQuadArray[1].v3 = 5;
	m_pQuadArray[1].v4 = 4;
	m_pQuadArray[1].n = 1;
	m_pQuadArray[2].v1 = 1;
	m_pQuadArray[2].v2 = 5;
	m_pQuadArray[2].v3 = 6;
	m_pQuadArray[2].v4 = 2;
	m_pQuadArray[2].n = 2;
	m_pQuadArray[3].v1 = 4;
	m_pQuadArray[3].v2 = 0;
	m_pQuadArray[3].v3 = 3;
	m_pQuadArray[3].v4 = 7;
	m_pQuadArray[3].n = 3;
	m_pQuadArray[4].v1 = 4;
	m_pQuadArray[4].v2 = 5;
	m_pQuadArray[4].v3 = 1;
	m_pQuadArray[4].v4 = 0;
	m_pQuadArray[4].n = 4;
	m_pQuadArray[5].v1 = 3;
	m_pQuadArray[5].v2 = 2;
	m_pQuadArray[5].v3 = 6;
	m_pQuadArray[5].v4 = 7;
	m_pQuadArray[5].n = 5;

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

	for (int i = 0; i < m_nNumPolygon; i++) {
		m_pNormalArray[i] = QuadNormal(m_pQuadArray[i]);
		m_pQuadArray[i].selected = false;
	}
	// FOR TESTING. to select a face while picking isnt implemented yet
	//m_pQuadArray[3].selected = true;
}

Model::~Model(void) {
	if (m_pVertexArray != NULL)
		delete[] m_pVertexArray;
	if (m_pNormalArray != NULL)
		delete[] m_pNormalArray;
	if (m_pEdgeArray != NULL)
		delete[] m_pEdgeArray;
	if (m_pQuadArray != NULL)
		delete[] m_pQuadArray;
}

PAE3D_Normal Model::TriangleNormal(PAE3D_Triangle t) {
	PAE3D_Normal u;
	u.x = m_pVertexArray[t.v2].x - m_pVertexArray[t.v1].x;
	u.y = m_pVertexArray[t.v2].y - m_pVertexArray[t.v1].y;
	u.z = m_pVertexArray[t.v2].z - m_pVertexArray[t.v1].z;
	PAE3D_Normal v;
	v.x = m_pVertexArray[t.v3].x - m_pVertexArray[t.v1].x;
	v.y = m_pVertexArray[t.v3].y - m_pVertexArray[t.v1].y;
	v.z = m_pVertexArray[t.v3].z - m_pVertexArray[t.v1].z;
	PAE3D_Normal n;
	n.x = u.y * v.z - u.z * v.y;
	n.y = u.z * v.x - u.x * v.z;
	n.z = u.x * v.y - u.y * v.x;
	float m = sqrt(n.x*n.x + n.y*n.y + n.z*n.z);
	n.x /= m;
	n.y /= m;
	n.z /= m;
	return n;
}

PAE3D_Normal Model::QuadNormal(PAE3D_Quad q) {
	PAE3D_Triangle t;
	t.v1 = q.v1;
	t.v2 = q.v2;
	t.v3 = q.v3;
	PAE3D_Normal n1 = TriangleNormal(t);
	t.v1 = q.v2;
	t.v2 = q.v3;
	t.v3 = q.v4;
	PAE3D_Normal n2 = TriangleNormal(t);
	t.v1 = q.v3;
	t.v2 = q.v4;
	t.v3 = q.v1;
	PAE3D_Normal n3 = TriangleNormal(t);
	t.v1 = q.v4;
	t.v2 = q.v1;
	t.v3 = q.v2;
	PAE3D_Normal n4 = TriangleNormal(t);
	PAE3D_Normal n;
	n.x = n1.x + n2.x + n3.x + n4.x;
	n.x /= 4;
	n.y = n1.y + n2.y + n3.y + n4.y;
	n.y /= 4;
	n.z = n1.z + n2.z + n3.z + n4.z;
	n.z /= 4;
	return n;
}

PAE3D_Point Model::QuadCenter(PAE3D_Quad q) {
	PAE3D_Point center;
	center.x = (m_pVertexArray[q.v1].x+m_pVertexArray[q.v2].x+m_pVertexArray[q.v3].x+m_pVertexArray[q.v4].x)/4;
	center.y = (m_pVertexArray[q.v1].y+m_pVertexArray[q.v2].y+m_pVertexArray[q.v3].y+m_pVertexArray[q.v4].y)/4;
	center.z = (m_pVertexArray[q.v1].z+m_pVertexArray[q.v2].z+m_pVertexArray[q.v3].z+m_pVertexArray[q.v4].z)/4;
	return center;
}

void Model::RenderVertices(float zoom) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	glColor3f(0, 0, 0);
	for (int i = 0; i < m_nNumPoint; i++) {
		glPushMatrix();
		PAE3D_Point p = m_pVertexArray[i];
		glTranslatef(p.x, p.y, p.z);
		glutSolidSphere(1.0/height * zoom, 20, 20);
		glPopMatrix();
	}
}

void Model::RenderEdges(float zoom) {
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	glColor3f(0, 0, 0);
		for (int i = 0; i < m_nNumEdge; i++) {
			PAE3D_Edge edge = m_pEdgeArray[i];
			PAE3D_Point p1 = m_pVertexArray[edge.v1];
			PAE3D_Point p2 = m_pVertexArray[edge.v2];
			glPushMatrix();
			glTranslatef(p1.x, p1.y, p1.z);
			float length = sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y)+(p1.z-p2.z)*(p1.z-p2.z));
			float x = (p1.y-p2.y) * 1;
			float y = -(p1.x-p2.x)* 1;
			float z = 0;
			float angle = acos(-(p1.z-p2.z)) * 180 / M_PI;
			glRotatef(angle, x, y, z);
			gluCylinder(quadric, 0.5/height* zoom, 0.5/height* zoom, length, 20, 1);
			glPopMatrix();
		}
}

void Model::RenderFaces(bool highlightSelected) {
	glBegin(GL_QUADS);
	for (int i = 0; i < m_nNumPolygon; i++) {
		if (highlightSelected && m_pQuadArray[i].selected) {
			glColor3f(0.5, 0.3, 0.5);
		}
		else {
			glColor3f(0.8, 0.77, 0.5);
		}
		PAE3D_Quad quad = m_pQuadArray[i];
		PAE3D_Point p = m_pVertexArray[quad.v1];
		PAE3D_Normal n = m_pNormalArray[quad.n];
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(p.x, p.y, p.z);
		p = m_pVertexArray[quad.v2];
		glVertex3f(p.x, p.y, p.z);
		p = m_pVertexArray[quad.v3];
		glVertex3f(p.x, p.y, p.z);
		p = m_pVertexArray[quad.v4];
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();
}

void Model::RenderSelectedFacesHandle(float zoom) {
	int count = 0;
	PAE3D_Point center;
	center.x = 0;
	center.y = 0;
	center.z = 0;
	for (int i = 0; i < m_nNumPolygon; i++) {
		PAE3D_Quad quad = m_pQuadArray[i];
		if (quad.selected) {
			PAE3D_Point p = QuadCenter(quad);
			center.x += p.x;
			center.y += p.y;
			center.z += p.z;
			count++;
		}
	}
	if (count > 0) {
		center.x /= count;
		center.y /= count;
		center.z /= count;
		int height = glutGet(GLUT_WINDOW_HEIGHT);
		float width = 0.5/height* zoom;
		float length = 20.0/height* zoom;
		float square = length/2.0;
		glColor3f(0, 1, 0);
		glPushMatrix();
			// X-Axis
			glTranslatef(center.x, center.y, center.z);
			glPushMatrix();
				glRotatef(90, 0, 1, 0);
				glColor3f(1, 0, 0);
				Lights::SetUnlit(GL_LIGHT0);
				glBegin(GL_QUADS);
				glVertex3f(0, 0, 0);
				glVertex3f(-square, 0, 0);
				glVertex3f(-square, square, 0);
				glVertex3f(0, square, 0);
				glEnd();
				Lights::SetLit(GL_LIGHT0);
				gluCylinder(quadric, width, width, length, 20, 1);
				glTranslatef(0, 0, length);
				gluCylinder(quadric, width*4, 0, width*8, 20, 1);
			glPopMatrix();
			// Y-Axis
			glPushMatrix();
				glRotatef(90, -1, 0, 0);
				glColor3f(0, 1, 0);
				Lights::SetUnlit(GL_LIGHT0);
				glBegin(GL_QUADS);
				glVertex3f(0, 0, 0);
				glVertex3f(square, 0, 0);
				glVertex3f(square, -square, 0);
				glVertex3f(0, -square, 0);
				glEnd();
				Lights::SetLit(GL_LIGHT0);
				gluCylinder(quadric, width, width, length, 20, 1);
				glTranslatef(0, 0, length);
				gluCylinder(quadric, width*4, 0, width*8, 20, 1);
			glPopMatrix();
			// Z-Axis
			glColor3f(0, 0, 1);
			Lights::SetUnlit(GL_LIGHT0);
			glBegin(GL_QUADS);
			glVertex3f(0, 0, 0);
			glVertex3f(square, 0, 0);
			glVertex3f(square, square, 0);
			glVertex3f(0, square, 0);
			glEnd();
			Lights::SetLit(GL_LIGHT0);
			gluCylinder(quadric, width, width, length, 20, 1);
			glTranslatef(0, 0, length);
			gluCylinder(quadric, width*4, 0, width*8, 20, 1);
		glPopMatrix();
	}
}














