#define __SPECSTRINGS_STRICT_LEVEL 0 // to suppress warnings

#include <windows.h>

import Internal.ScreenMediator;
import Internal.UserInput;
import Screen;
import Screen.ScreenName;
import Utils.Frames;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    auto screen = Screen();

    // Event subjects
    auto subjectInputDirection = SubjectInputDirection::getInstance();
    auto subjectMouseMove = SubjectMouseMove::getInstance();
    auto subjectKeyPress = SubjectKeyPress::getInstance();

    // Mediators
    auto screenMediator = ScreenMediator::getInstance(&screen);
    screenMediator->notify(ScreenName::MAIN);

    while (true) {
        // process events
        subjectInputDirection->update();
        subjectMouseMove->update(screen.getHandle());
        subjectKeyPress->update();

        // render a frame
        screen.refresh();

        // target 60 fps
        sleepFor(60);
    }
}
