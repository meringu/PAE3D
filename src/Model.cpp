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
	m_pEdgeArray = NULL;
	m_pQuadArray = NULL;

	m_nNumPoint = 8;
	m_pVertexArray = new PAE3D_Point[m_nNumPoint];
	m_pVertexArray[0].x = 0.6;
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

	m_pQuadArray[0].v1 = 0;
	m_pQuadArray[0].v2 = 1;
	m_pQuadArray[0].v3 = 2;
	m_pQuadArray[0].v4 = 3;
	m_pQuadArray[1].v1 = 7;
	m_pQuadArray[1].v2 = 6;
	m_pQuadArray[1].v3 = 5;
	m_pQuadArray[1].v4 = 4;
	m_pQuadArray[2].v1 = 1;
	m_pQuadArray[2].v2 = 5;
	m_pQuadArray[2].v3 = 6;
	m_pQuadArray[2].v4 = 2;
	m_pQuadArray[3].v1 = 4;
	m_pQuadArray[3].v2 = 0;
	m_pQuadArray[3].v3 = 3;
	m_pQuadArray[3].v4 = 7;
	m_pQuadArray[4].v1 = 4;
	m_pQuadArray[4].v2 = 5;
	m_pQuadArray[4].v3 = 1;
	m_pQuadArray[4].v4 = 0;
	m_pQuadArray[5].v1 = 3;
	m_pQuadArray[5].v2 = 2;
	m_pQuadArray[5].v3 = 6;
	m_pQuadArray[5].v4 = 7;

	for (int i = 0; i < m_nNumPolygon; i++) {
		m_pQuadArray[i].n = QuadNormal(m_pQuadArray[i]);
		m_pQuadArray[i].selected = false;
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

	// FOR TESTING. to select a face while picking isn't implemented yet
	//m_pQuadArray[0].selected = true;
	//m_pQuadArray[5].selected = true;
}

Model::~Model(void) {
	if (m_pVertexArray != NULL)
		delete[] m_pVertexArray;
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

void Model::AddQuad(PAE3D_Quad quad) {
	m_nNumPolygon++;
	PAE3D_Quad* temp = new PAE3D_Quad[m_nNumPolygon];
	for (int i = 0; i < m_nNumPolygon-1; i++) {
		temp[i] = m_pQuadArray[i];
	}
	temp[m_nNumPolygon-1] = quad;
	delete(m_pQuadArray);
	m_pQuadArray = temp;
}

void Model::Smooth() {
	for (int i = 0; i < m_nNumPolygon; i++) {
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
	for (int i = 0; i < m_nNumPolygon; i++) {
		PAE3D_Quad q = m_pQuadArray[i];
		int ei1 = FindEdge(q.v1, q.v2);
		int ei2 = FindEdge(q.v2, q.v3);
		int ei3 = FindEdge(q.v3, q.v4);
		int ei4 = FindEdge(q.v4, q.v1);
		PAE3D_Edge e1;
		e1.v1 = q.c;
		e1.v2 = m_pEdgeArray[ei1].c;
		e1.c = 0;
		PAE3D_Edge e2;
		e2.v1 = q.c;
		e2.v2 = m_pEdgeArray[ei1].c;
		e2.c = 0;
		PAE3D_Edge e3;
		e3.v1 = q.c;
		e3.v2 = m_pEdgeArray[ei1].c;
		e3.c = 0;
		PAE3D_Edge e4;
		e4.v1 = q.c;
		e4.v2 = m_pEdgeArray[ei1].c;
		e4.c = 0;
		/*AddEdge(e1);
		AddEdge(e2);
		AddEdge(e3);
		AddEdge(e4);*/
		/*m_pQuadArray[i].v3 = facePoints[i];
		m_pQuadArray[i].v2 = edgePoints[FindEdge(m_pQuadArray[i].v1, m_pQuadArray[i].v2)];
		m_pQuadArray[i].v4 = edgePoints[FindEdge(m_pQuadArray[i].v4, m_pQuadArray[i].v1)];*/
	}
}

unsigned int Model::FindEdge(unsigned int v1, unsigned  int v2) {
	for (int i = 0; i < m_nNumEdge; i++) {
		if (m_pEdgeArray[i].v1 == v1 && m_pEdgeArray[i].v2 == v2
				|| m_pEdgeArray[i].v2 == v1 && m_pEdgeArray[i].v2 == v1) {
			return i;
		}
	}
	return 0;
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
		PAE3D_Normal n = quad.n;
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

void Model::RenderPickerFaces() {
	glBegin(GL_QUADS);
	for (int i = 0; i < m_nNumPolygon; i++) {

		glColor3f((1/256.0f)+(i)/256.0f, 0.5, 0.5);
		//printf("%i %f \n",i,(i+0.5)/256.0f);

		PAE3D_Quad quad = m_pQuadArray[i];
		PAE3D_Point p = m_pVertexArray[quad.v1];
		PAE3D_Normal n = quad.n;
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

void Model::ProcessSelection(int cursorX, int cursorY){
	GLint viewport[4];
	GLubyte pixel[3];

	glGetIntegerv(GL_VIEWPORT,viewport);

	glReadPixels(cursorX,viewport[3]-cursorY,1,1,GL_RGB,GL_UNSIGNED_BYTE,(void *)pixel);

	printf("%d %d %d\n",pixel[0],pixel[1],pixel[2]);
	printf("id %d \n", pixel[0]);
	m_pQuadArray[pixel[0]].selected = true;

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














