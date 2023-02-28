#include "4.h"

const Rect border = {0, 0, 400, 600, BLACK};

int flag_mouse_down = false;
int timer_bullet_spawn = 0;

void processInput(Ball &jet);
void draw(Ball jet, const std::vector<Rect> &bullets);
void calculate(Ball &jet, std::vector<Rect> &bullets);

[[noreturn]] void doShoot() {
    initDpiScaledGraph(400, 600);

    Ball jet { 200, 500, 0, 0, 20, RED };

    std::vector<Rect> bullets;

    while (true) {
        processInput(jet);

        draw(jet, bullets);

        Sleep(10);

        calculate(jet, bullets);
    }
}

void processInput(Ball &jet) {
    auto msg = new ExMessage;

    COORD coord;

    if (peekmessage(msg, EX_MOUSE, false)) {
        switch (msg->message) {
            case WM_MOUSEMOVE:
                coord = getDpiScaledMousePosition(msg);
                jet.x = coord.X;
                jet.y = coord.Y;
                break;

            case WM_LBUTTONDOWN:
                flag_mouse_down = true;
                break;

            case WM_LBUTTONUP:
                flag_mouse_down = false;
                break;
        }
    }

    flushmessage(EX_MOUSE);
}

void draw(const Ball jet, const std::vector<Rect> &bullets) {
    BeginBatchDraw();

    cleardevice();

    jet.draw();

    for (const auto& bullet : bullets) {
        bullet.draw();
    }

    EndBatchDraw();
}

void calculate(Ball &jet, std::vector<Rect> &bullets) {
    if (flag_mouse_down && timer_bullet_spawn <= 0) {
        bullets.emplace_back(
            jet.x - 1,
            jet.y - jet.r - 10,
            jet.x + 1,
            jet.y - jet.r,
            RGB(221, 160, 90)
        );
        timer_bullet_spawn = 10;
    }

    for (int i = 0; i < bullets.size(); i++) {
        auto &bullet = bullets[i];

        if (bullet.bottom < 0) {
            bullets.erase(bullets.begin() + i, bullets.begin() + i + 1);
            i--;
        }

        bullet.move(0, -10);
    }

    if (timer_bullet_spawn > 0) {
        timer_bullet_spawn--;
    }
}
