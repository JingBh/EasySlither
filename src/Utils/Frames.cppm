module;

#include <Windows.h>

export module Utils.Frames;

import <string>;
import <map>;

std::map<std::string, unsigned long> lastFrame;
std::map<std::string, unsigned long> lastTime;

const std::string defaultName = "default";

export void sleepFor(int fps, const std::string &name = defaultName) {
    if (lastFrame.find(name) == lastFrame.end()) {
        lastFrame[name] = 0;
        lastTime[name] = GetTickCount();
    }

    unsigned long currentFrame = lastFrame[name];
    unsigned long currentTime = lastTime[name];

    unsigned long frameTime = 1000 / fps;
    unsigned long time = GetTickCount();

    if (time - currentTime < frameTime) {
        Sleep(frameTime - (time - currentTime));
    }

    lastFrame[name] = currentFrame + 1;
    lastTime[name] = GetTickCount();
}
