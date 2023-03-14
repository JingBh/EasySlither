export module Screen.Screen;

import <list>;

import Screen;
import Screen.ScaleFactor;
import ThirdParty;
import Utils.Drawable;

Screen::Screen() {
    const auto screenRect = windows::getScreenRect();

    width = static_cast<int>((screenRect.right - screenRect.left) * 0.7);
    height = static_cast<int>((screenRect.bottom - screenRect.top) * 0.7);

#ifdef DEBUG
    handle = easyx::initGraph(width, height, easyx::WINDOW_SHOWCONSOLE);
#else
    handle = easyx::initGraph(width, height);
#endif //DEBUG

    easyx::setAspectRatio(SCALE_FACTOR, SCALE_FACTOR);
}

Screen::~Screen() {
    easyx::closeGraph();
}

void Screen::addElement(LocationAwareDrawable *drawable) {
    drawables.push_back(drawable);
}

void Screen::removeElement(LocationAwareDrawable *drawable) {
    drawables.remove(drawable);
}

void Screen::refresh() const {
    easyx::beginBatchDraw();

    easyx::clearDevice();

    for (auto &drawable: drawables) {
        drawable->render();
    }

    easyx::endBatchDraw();
}

[[nodiscard]] int Screen::getWidth() const {
    return static_cast<int>(static_cast<float>(this->width) / SCALE_FACTOR);
}

[[nodiscard]] int Screen::getHeight() const {
    return static_cast<int>(static_cast<float>(this->height) / SCALE_FACTOR);
}

[[nodiscard]] windows::WindowHandle Screen::getHandle() const {
    return this->handle;
}
