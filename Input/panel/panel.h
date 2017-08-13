#ifndef RWE_INPUT_PANEL_PANEL
#define RWE_INPUT_PANEL_PANEL
#include "Frame/main/main.h"

void RWEMouse(int x, int y);
void RWEDrag(int x, int y);
void RWEClick(int button, int state, int x, int y);
void RWEWheel(int wheel, int dir, int x, int y);
void RWESpecialDown(int key, int x, int y);
void RWESpecialUp(int key, int x, int y);
void RWEKeyDown(unsigned char cAscii, int x, int y);
void RWEKeyUp(unsigned char cAscii, int x, int y);

#endif