#ifndef MOUSE_H
#define MOUSE_H
#include<windows.h>

enum { LEFT , BOTH , RIGHT };
void initMouse(void);
void click(POINT pt , int type);
#endif
