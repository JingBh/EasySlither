#include "2.h"

#include <limits>
#include <random>

const Rect border { 0, 0, 600, INT_MAX, BLACK };
const Rect border_fail { 0, 400, 600, INT_MAX, BLACK };

void gameLoop();
void processInput(Rect &user);
bool calculate(Rect user, Ball &ball, std::vector<Rect> &bricks);
void draw(Rect user, Ball ball, std::vector<Rect> &bricks);
void gameOver();

[[noreturn]] void doBreakout() {
    initDpiScaledGraph(600, 400);

    while (true) {
        gameLoop();
    }
}

void gameLoop() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_v(0, 1);

    Ball ball {
        300, 220,
        dis_v(gen) == 0 ? -2 : 2,
        dis_v(gen) == 0 ? -2 : 2,
        5,
        WHITE
    };

    Rect user {
        200, 380, 400, 400,
        RGB(255, 255, 255)
    };

    std::vector<Rect> bricks;
    const int brick_width = 60;
    const int brick_height = 20;

    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 10; col++) {
            bricks.emplace_back(
                col * brick_width, row * brick_height,
                (col + 1) * brick_width, (row + 1) * brick_height,
                RGB(191, 157, 90)
            );
        }
    }

    while (true) {
        processInput(user);

        draw(user, ball, bricks);

        Sleep(10);

        if (!calculate(user, ball, bricks)) {
            return gameOver();
        }
    }
}

void processInput(Rect &user) {
    int dx = 0;

    XINPUT_STATE state;
    ZeroMemory(&state, sizeof(XINPUT_STATE));
    auto dwResult = XInputGetState(0, &state);
    if (dwResult == ERROR_SUCCESS) {
        if (state.Gamepad.sThumbLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
            dx = -10;
        } else if (state.Gamepad.sThumbLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) {
            dx = 10;
        }

        if (state.Gamepad.wButtons & 0x0100 && !(state.Gamepad.wButtons & 0x0200)) {
            dx = -10;
        } else if (state.Gamepad.wButtons & 0x0200 && !(state.Gamepad.wButtons & 0x0100)) {
            dx = 10;
        }
    }

    if (GetKeyState(VK_LEFT) & 0x8000 || GetKeyState('A') & 0x8000) {
        dx = -10;
    } else if (GetKeyState(VK_RIGHT) & 0x8000 || GetKeyState('D') & 0x8000) {
        dx = 10;
    }

    user.move(dx, 0);
    if (user.isCollide(border)) {
        user.move(-dx, 0);
    }
}

bool calculate(Rect user, Ball &ball, std::vector<Rect> &bricks) {
    ball.move();

    if (ball.isCollide(border_fail)) {
        return false;
    }

    ball.processCollide(border);
    ball.processCollide(user);

    for (int i = 0; i < bricks.size(); i++) {
        const auto& brick = bricks[i];

        if (ball.processCollide(brick)) {
            bricks.erase(bricks.begin() + i, bricks.begin() + i + 1);
            i--;
        }
    }

    return true;
}

void draw(Rect user, Ball ball, std::vector<Rect> &bricks) {
    BeginBatchDraw();

    cleardevice();

    user.draw();
    ball.draw();

    for (const auto& brick : bricks) {
        brick.draw();
    }

    EndBatchDraw();
}

void gameOver() {
    BeginBatchDraw();

    settextcolor(WHITE);
    settextstyle(30, 0, _T("Consolas"));
    outtextxy(235, 200, _T("Game Over"));

    EndBatchDraw();

    while (true) {
        XINPUT_STATE state;
        ZeroMemory(&state, sizeof(XINPUT_STATE));
        auto dwResult = XInputGetState(0, &state);
        if (dwResult == ERROR_SUCCESS) {
            if (state.Gamepad.wButtons & 0x1000) {
                return;
            }
        }

        if (GetKeyState(VK_SPACE) & 0x8000) {
            return;
        }

        Sleep(10);
    }
}
