#include "utils.h"

#include <cmath>

const float scale_factor = static_cast<float>(GetDpiForSystem()) / 96;

void initDpiScaledGraph(const int width, const int height) {
    const int width_scaled = std::floor(static_cast<float>(width) * scale_factor);
    const int height_scaled = std::floor(static_cast<float>(height) * scale_factor);

    initgraph(width_scaled, height_scaled);
    setaspectratio(scale_factor, scale_factor);
}

COORD getDpiScaledMousePosition(ExMessage *msg) {
    const auto mouseX = static_cast<short>(
        static_cast<float>(msg->x) / scale_factor
    );

    const auto mouseY = static_cast<short>(
        static_cast<float>(msg->y) / scale_factor
    );

    return {mouseX, mouseY};
}
