#ifndef ASSIGNMENT_UTILS_H
#define ASSIGNMENT_UTILS_H

#include <windows.h>

#include <easyx.h>

void initDpiScaledGraph(int width, int height);

COORD getDpiScaledMousePosition(ExMessage *msg);

#endif //ASSIGNMENT_UTILS_H
