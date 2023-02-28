#include "1.h"

#include <random>

void draw(std::vector<Ball> &balls);
void calculate(std::vector<Ball> &balls);

const int size = 500;
const Rect border = {0, 0, size, size, BLACK};

void doBalls() {
    initDpiScaledGraph(size, size);

    std::vector<Ball> balls;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, size);

    for (int i = 0; i < 20; i++) {
        const Ball ball_new {
            dis(gen), dis(gen),
            dis(gen) % 2 == 0 ? 1 : -1,
            dis(gen) % 2 == 0 ? 1 : -1,
            10,
            WHITE
        };

        bool is_valid = true;

        for (const auto& ball : balls) {
            if (ball_new.isCollide(ball) || ball_new.isCollide(border)) {
                i--;
                is_valid = false;
                break;
            }
        }

        if (is_valid) {
            balls.push_back(ball_new);
        }
    }

    while (true) {
        draw(balls);

        Sleep(10);

        calculate(balls);
    }
}

void calculate(std::vector<Ball> &balls) {
    for (auto& ball : balls) {
        ball.move();
    }

    for (auto& ball : balls) {
        for (const auto& other : balls) {
            if (&ball == &other) {
                continue;
            }

            if (ball.processCollide(other)) {
                break;
            }
        }
    }

    for (auto& ball : balls) {
        ball.processCollide(border);
    }
}

void draw(std::vector<Ball> &balls) {
    BeginBatchDraw();

    cleardevice();

    for (const auto& ball : balls) {
        ball.draw();
    }

    EndBatchDraw();
}
