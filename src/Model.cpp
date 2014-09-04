#include "Model.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GL/glut.h>

#include <iostream>

using namespace std;

int numVert, numNorm, numUV, numFace;

Model::Model(void) {
	m_pVertexArray = NULL;
	m_pNormalArray = NULL;
	m_pUVArray = NULL;
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
	m_nNumUV = m_nNumNormal = 0;
	m_glGeomList = 0;

	for (int i = 0; i < m_nNumPolygon; i++) {
		m_pNormalArray[i] = QuadNormal(m_pQuadArray[i]);
	}
}

Model::~Model(void) {
	if (m_pVertexArray != NULL)
		delete[] m_pVertexArray;
	if (m_pNormalArray != NULL)
		delete[] m_pNormalArray;
	if (m_pUVArray != NULL)
		delete[] m_pUVArray;
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

void Model::CreateGeometry() {
	if (m_glGeomList != 0){
		glDeleteLists(m_glGeomList, 1);
	}
	m_glGeomList = glGenLists(1);
	glNewList(m_glGeomList, GL_COMPILE);

	glBegin(GL_QUADS);
	glColor3f(1, 0, 0);

	for (int i = 0; i < m_nNumPolygon; i++) {
		PAE3D_Quad quad = m_pQuadArray[i];
		PAE3D_Point p = m_pVertexArray[quad.v1];
		PAE3D_Normal n = m_pNormalArray[quad.n];
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(p.x, p.y, p.z);
		p = m_pVertexArray[quad.v2];
		glVertex3f(p.x, p.y, p.z);
		//glNormal3f(n.x, n.y, n.z);
		p = m_pVertexArray[quad.v3];
		glVertex3f(p.x, p.y, p.z);
		//glNormal3f(n.x, n.y, n.z);
		p = m_pVertexArray[quad.v4];
		glVertex3f(p.x, p.y, p.z);
		//glNormal3f(n.x, n.y, n.z);
	}
	glEnd();
	glEndList();
}

void Model::RenderGeometry() {
		glShadeModel(GL_SMOOTH);
		glCallList(m_glGeomList);
}


















