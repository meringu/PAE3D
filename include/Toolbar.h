#pragma once

class Toolbar {
public:
	Toolbar(void (*i)(int), unsigned int, void (*d) (unsigned char, int,int), void (*u) (unsigned char, int,int), int (*pi) (int));
	~Toolbar(void);
	void DrawButtons(bool);
	int getCurrentButton();
	void repaint();
};
