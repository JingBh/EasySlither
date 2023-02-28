#include "shapes.h"

#include <cmath>

Ball::Ball(int x, int y, int vx, int vy, int r, COLORREF color)
        : x{x}, y{y}, vx{vx}, vy{vy}, r{r}, color{color} {}

void Ball::draw() const {
    setlinestyle(PS_SOLID, 2);
    setlinecolor(WHITE);
    setfillcolor(color);
    fillcircle(x, y, r);
}

void Ball::move() {
    Ball::move(vx, vy);
}

void Ball::move(const int dx, const int dy) {
    x += dx;
    y += dy;
}

bool Ball::isCollide(const Ball& ball) const {
    return std::pow(x - ball.x, 2) +
        std::pow(y - ball.y, 2) <=
        std::pow(r + ball.r, 2);
}

bool Ball::isCollide(const Rect& rect) const {
    return ((std::abs(x - rect.left) <= r || std::abs(x - rect.right) <= r) && (y + r) >= rect.top && (y - r) <= rect.bottom) ||
        ((std::abs(y - rect.top) <= r || std::abs(y - rect.bottom) <= r) && (x + r) >= rect.left && (x - r) <= rect.right);
}

bool Ball::processCollide(const Ball &ball) {
    if (isCollide(ball)) {
        vx = -vx;
        vy = -vy;

        return true;
    }

    return false;
}

bool Ball::processCollide(const Rect &rect) {
    bool isCollide = false;

    if ((std::abs(x - rect.left) <= r || std::abs(x - rect.right) <= r) && (y + r) >= rect.top && (y - r) <= rect.bottom) {
        vx = -vx;

        isCollide = true;
    }

    if ((std::abs(y - rect.top) <= r || std::abs(y - rect.bottom) <= r) && (x + r) >= rect.left && (x - r) <= rect.right) {
        vy = -vy;

        isCollide = true;
    }

    return isCollide;
}

Rect::Rect(int left, int top, int right, int bottom, COLORREF color)
        : left{left}, top{top}, right{right}, bottom{bottom}, color{color} {}

void Rect::draw() const {
    setlinestyle(PS_SOLID, 2);
    setlinecolor(WHITE);
    setfillcolor(color);
    fillrectangle(left, top, right, bottom);
}

void Rect::move(int dx, int dy) {
    left += dx;
    top += dy;
    right += dx;
    bottom += dy;
}

bool Rect::isCollide(const Ball &ball) const {
    return ball.isCollide(*this);
}

bool Rect::isCollide(const Rect &rect) const {
    return ((left < rect.left && right > rect.left) || (left < rect.right && right > rect.right) &&
        (top < rect.bottom || bottom > rect.top)) ||
        ((top < rect.top && bottom > rect.top) || (top < rect.bottom && bottom > rect.bottom) &&
        (left < rect.right || right > rect.left));
}
