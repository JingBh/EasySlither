export module Utils.Frames;

import <string_view>;
import <thread>;
import <map>;

import Utils.Time;

std::map<std::string_view, unsigned long> lastFrame;
std::map<std::string_view, unsigned long> lastTime;

const std::string_view defaultName = "default";

export void sleepFor(int fps, std::string_view name = defaultName) {
    if (lastFrame.find(name) == lastFrame.end()) {
        lastFrame[name] = 0;
        lastTime[name] = getTickCount();
    }

    unsigned long currentFrame = lastFrame[name];
    unsigned long currentTime = lastTime[name];

    unsigned long frameTime = 1000 / fps;
    unsigned long time = getTickCount();

    if (time - currentTime < frameTime) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(frameTime - (time - currentTime)));
    }

    lastFrame[name] = currentFrame + 1;
    lastTime[name] = getTickCount();
}
