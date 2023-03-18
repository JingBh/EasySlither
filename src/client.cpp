#define __SPECSTRINGS_STRICT_LEVEL 0 // to suppress warnings

#include <windows.h>

import Middleware.ScreenMediator;
import Middleware.UserInput;
import Screen;
import Screen.ScreenName;
import Utils.Frames;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    auto screen = Screen();

    // Events (for observers)
    auto subjectInputDirection = SubjectInputDirection::getInstance();
    auto subjectMouseMove = SubjectMouseMove::getInstance();
    auto subjectKeyPress = SubjectKeyPress::getInstance();

    // Mediators
    auto screenMediator = ScreenMediator::getInstance(&screen);
    screenMediator->notify(ScreenName::MAIN);

    while (true) {
        subjectInputDirection->update();
        subjectMouseMove->update(screen.getHandle());
        subjectKeyPress->update();

        screen.refresh();

        sleepFor(60);
    }
}
