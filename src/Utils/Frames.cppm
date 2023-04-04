export module Utils.Frames;

import <string_view>;
import <thread>;
import <map>;

import Utils.Time;

std::map<std::string_view, unsigned long long> lastFrame;
std::map<std::string_view, unsigned long long> lastTime;
std::map<std::string_view, unsigned long long> lastTimeSpan;

const std::string_view defaultName = "default";

export void sleepFor(int fps, std::string_view name = defaultName) {
    if (lastFrame.find(name) == lastFrame.end()) {
        lastFrame[name] = 0;
        lastTime[name] = getTickCount();
        lastTimeSpan[name] = 0;
    }

    unsigned long long currentFrame = lastFrame[name];
    unsigned long long currentTime = lastTime[name];

    unsigned long long frameTime = 1000 / fps;
    unsigned long long time = getTickCount();

    if (time - currentTime < frameTime) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(frameTime - (time - currentTime)));
    }

    time = getTickCount();
    lastFrame[name] = currentFrame + 1;
    lastTime[name] = time;
    lastTimeSpan[name] = time - currentTime;
}

export unsigned long long getFrameCount(std::string_view name = defaultName) {
    if (lastFrame.find(name) == lastFrame.end()) {
        return 0;
    }

    return lastFrame[name];
}

export unsigned long long getTimeSpan(std::string_view name = defaultName) {
    if (lastFrame.find(name) == lastFrame.end()) {
        return 0;
    }

    return lastTimeSpan[name];
}
