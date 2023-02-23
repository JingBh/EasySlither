#include <iostream>
#include <numbers>
#include <string>

#include <windows.h>

#include <easyx.h>

[[noreturn]] void doCircle();
void drawCircle(int center, int radius, COLORREF color);

[[noreturn]] void doBoard();

[[noreturn]] void doClock();
void initClock(int center);
void drawClock(int center, SYSTEMTIME *time);

int main()
{
    std::cout << "EasyX 练习作业" << std::endl
        << "\t22080206 敬博浩" << std::endl
        << "\t2023-02-21" << std::endl;

    std::cout << std::endl
        << "1. 绘制圆" << std::endl
        << "2. 绘制棋盘" << std::endl
        << "3. 绘制时钟" << std::endl
        << "请选择项目：";

    auto choice = 0;
    std::cin >> choice;

    switch (choice)
    {
    case 1:
        doCircle();
    case 2:
        doBoard();
    case 3:
        doClock();
    default:
        std::cout << "Invalid choice." << std::endl;
        return 0;
    }
}

[[noreturn]] void doCircle()
{
    const auto size = 300;
    const auto center = 150;
    const auto radius = 100;

    const float scale_factor = static_cast<float>(GetDpiForSystem()) / 96;
    initgraph(size * scale_factor, size * scale_factor);
    setaspectratio(scale_factor, scale_factor);

    while (true) {
        for (float hue = 0; hue < 360.0; hue += 0.5)
        {
            drawCircle(center, radius, HSLtoRGB(hue, 1, 0.5));

            Sleep(1);
        }
    }
}

void drawCircle(const int center, const int radius, const COLORREF color)
{
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 2);
    setfillcolor(color);
    fillcircle(center, center, radius);
}

[[noreturn]] void doBoard()
{
    const float scale_factor = static_cast<float>(GetDpiForSystem()) / 96;
    initgraph(400 * scale_factor, 400 * scale_factor);
    setaspectratio(scale_factor, scale_factor);

    setlinestyle(PS_NULL);
    setlinecolor(BLACK);
    setfillcolor(RGB(242, 206, 135));
    fillrectangle(0, 0, 400, 400);

    setlinestyle(PS_SOLID, 1);
    for (int i = 0; i < 20; i++)
    {
        line(20, 20 + i * 20, 380, 20 + i * 20);
        line(20 + i * 20, 20, 20 + i * 20, 380);
    }

    setlinestyle(PS_NULL);
    setfillcolor(BLACK);
    fillcircle(80, 80,   3);
    fillcircle(200, 80,  3);
    fillcircle(320, 80,  3);
    fillcircle(80, 200,  3);
    fillcircle(200, 200, 3);
    fillcircle(320, 200, 3);
    fillcircle(80, 320,  3);
    fillcircle(200, 320, 3);
    fillcircle(320, 320, 3);

    while (true)
    {
        Sleep(1000);
    }
}

[[noreturn]] void doClock()
{
    SYSTEMTIME time;

    const auto size = 400;
    const auto center = size / 2;

    const float scale_factor = static_cast<float>(GetDpiForSystem()) / 96;
    initgraph(size * scale_factor, size * scale_factor);
    setaspectratio(scale_factor, scale_factor);

    initClock(center);

    while (true)
    {
        // Get current time
        GetLocalTime(&time);

        BeginBatchDraw();
        drawClock(center, &time);
        EndBatchDraw();

        // Delay
        Sleep(1);
    }
}

void initClock(const int center)
{
    // Step 1. Draw panel
    setlinecolor(WHITE);
    setlinestyle(PS_SOLID, 2);
    setfillcolor(WHITE);
    fillcircle(center, center, 150);

    // Step 2. Draw numbers

    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(30, 0, _T("Arial"));
    for (int i = 1; i <= 12; i++)
    {
        const auto number_str = std::to_string(i);

        outtextxy(
            center + 128 * std::sin(i * 30 * std::numbers::pi / 180) - 10,
            center - 128 * std::cos(i * 30 * std::numbers::pi / 180) - 10,
            number_str.c_str()
        );
    }

    // Step 3. Draw ticks
    setlinecolor(BLACK);
    setlinestyle(PS_SOLID, 1);
    for (int i = 0; i < 60; i++)
    {
        if (i % 5 != 0)
            line(
                center + 140 * std::sin(i * 6 * std::numbers::pi / 180),
                center - 140 * std::cos(i * 6 * std::numbers::pi / 180),
                center + 150 * std::sin(i * 6 * std::numbers::pi / 180),
                center - 150 * std::cos(i * 6 * std::numbers::pi / 180)
            );
        else
            line(
                center + 145 * std::sin(i * 6 * std::numbers::pi / 180),
                center - 145 * std::cos(i * 6 * std::numbers::pi / 180),
                center + 150 * std::sin(i * 6 * std::numbers::pi / 180),
                center - 150 * std::cos(i * 6 * std::numbers::pi / 180)
            );
    }
}

void drawClock(const int center, SYSTEMTIME *time)
{
    setlinestyle(PS_NULL);
    setfillcolor(WHITE);
    fillcircle(center, center, 103);

    setlinecolor(BLACK);
    setfillcolor(BLACK);
    fillcircle(center, center, 5);

    // Draw hour hand
    setlinestyle(PS_SOLID, 5);
    const auto hour_angle = (
        static_cast<double>(time->wHour % 12) +
        static_cast<double>(time->wMinute) / 60
    ) * 30;
    line(
        center,
        center,
        center + 50 * std::sin(hour_angle * std::numbers::pi / 180),
        center - 50 * std::cos(hour_angle * std::numbers::pi / 180)
    );

    // Draw minute hand
    setlinestyle(PS_SOLID, 3);
    const auto minute_angle = (
        static_cast<double>(time->wMinute) +
        static_cast<double>(time->wSecond) / 60
    ) * 6;
    line(
        center,
        center,
        center + 100 * std::sin(minute_angle * std::numbers::pi / 180),
        center - 100 * std::cos(minute_angle * std::numbers::pi / 180)
    );

    // Draw second hand
    setlinestyle(PS_SOLID, 1);
    const auto second_angle = (
        static_cast<double>(time->wSecond) +
        static_cast<double>(time->wMilliseconds) / 1000
    ) * 6;
    line(
        center,
        center,
        center + 100 * std::sin(second_angle * std::numbers::pi / 180),
        center - 100 * std::cos(second_angle * std::numbers::pi / 180)
    );
}
