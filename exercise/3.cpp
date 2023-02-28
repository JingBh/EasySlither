#include "3.h"

void doMouse() {
    auto msg = new ExMessage;

    initDpiScaledGraph(1000, 1000);
    setlinestyle(PS_SOLID, 2);
    setlinecolor(WHITE);
    setfillcolor(WHITE);

    COORD last_coord { 0, 0 };

    while (true) {
        peekmessage(msg, EX_MOUSE);

        const auto coord = getDpiScaledMousePosition(msg);

        if (last_coord.X && last_coord.Y) {
            line(last_coord.X, last_coord.Y, coord.X, coord.Y);
        }

        last_coord = coord;
    }
}
