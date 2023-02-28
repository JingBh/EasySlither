#ifndef ASSIGNMENT_SHAPES_H
#define ASSIGNMENT_SHAPES_H

#include <easyx.h>

class Ball;
class Rect;

class Ball {
public:
    int x, y;
    int vx, vy;
    int r;
    COLORREF color;

    Ball(int x, int y, int vx, int vy, int r, COLORREF color);

    void draw() const;

    void move();
    void move(int dx, int dy);

    [[nodiscard]] bool isCollide(const Ball &ball) const;

    [[nodiscard]] bool isCollide(const Rect &rect) const;

    bool processCollide(const Ball &ball);

    bool processCollide(const Rect &rect);
};

class Rect {
public:
    int left, top, right, bottom;
    COLORREF color;

    Rect(int left, int top, int right, int bottom, COLORREF color);

    void draw() const;

    void move(int dx, int dy);

    [[nodiscard]] bool isCollide(const Ball &ball) const;

    [[nodiscard]] bool isCollide(const Rect &rect) const;
};

#endif //ASSIGNMENT_SHAPES_H
