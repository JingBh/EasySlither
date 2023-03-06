module;

#include <Windows.h>

#include <easyx.h>

export module Screen;

import <memory>;
import <vector>;
import Utils.Drawable;

export class Screen {
public:
    int width, height;

private:
    HWND handle;
    std::vector<std::unique_ptr<Drawable>> drawables;

public:
    Screen() {
        RECT screenRect;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);
        const float scale_factor = static_cast<float>(GetDpiForSystem()) / 96.0f;

        width = static_cast<int>((screenRect.right - screenRect.left) * 0.7);
        height = static_cast<int>((screenRect.bottom - screenRect.top) * 0.7);
        handle = initgraph(width, height);
        setaspectratio(scale_factor, scale_factor);
    }

    ~Screen() {
        closegraph();
    }

    void addElement(std::unique_ptr<Drawable> drawable) {
        drawables.push_back(std::move(drawable));
    }

    void removeElement(std::unique_ptr<Drawable> drawable) {
        for (auto it = drawables.begin(); it != drawables.end(); it++) {
            if (it->get() == drawable.get()) {
                drawables.erase(it);
                break;
            }
        }
    }

    void refresh() {
        BeginBatchDraw();

        cleardevice();

        for (auto &drawable: drawables) {
            drawable->render();
        }

        EndBatchDraw();
    }
};
