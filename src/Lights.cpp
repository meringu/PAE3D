#include "Lights.h"

using namespace std;

void Lights::AddLight(int light) {
	float direction[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	glLightfv(light, GL_POSITION, direction);
	SetLit(light);
	glEnable(light);
}

void Lights::SetUnlit(int light) {
	float diffintensity[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(light, GL_DIFFUSE, diffintensity);
	glLightfv(light, GL_AMBIENT, ambient);
}

void Lights::SetLit(int light) {
	float diffintensity[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	float ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };

	glLightfv(light, GL_DIFFUSE, diffintensity);
	glLightfv(light, GL_AMBIENT, ambient);
}
