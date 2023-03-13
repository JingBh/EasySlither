/**
 * CLion performs poorly with Windows headers,
 * so Windows and EasyX API is wrapped.
 */

module;

#define __SPECSTRINGS_STRICT_LEVEL 0 // to suppress warnings

#include <Windows.h>
#include <Xinput.h>
#include "easyx.h"

export module ThirdParty;

import <iostream>;
import <string>;

export namespace windows {
    /**
     * Constants
     */

    constexpr auto XINPUT_SUCCESS = ERROR_SUCCESS;

    constexpr auto XINPUT_LEFT_THUMB_DEADZONE = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
    constexpr auto XINPUT_RIGHT_THUMB_DEADZONE = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

    constexpr auto XINPUT_DPAD_UP = XINPUT_GAMEPAD_DPAD_UP;
    constexpr auto XINPUT_DPAD_DOWN = XINPUT_GAMEPAD_DPAD_DOWN;
    constexpr auto XINPUT_DPAD_LEFT = XINPUT_GAMEPAD_DPAD_LEFT;
    constexpr auto XINPUT_DPAD_RIGHT = XINPUT_GAMEPAD_DPAD_RIGHT;

    constexpr auto VKEY_UP = VK_UP;
    constexpr auto VKEY_DOWN = VK_DOWN;
    constexpr auto VKEY_LEFT = VK_LEFT;
    constexpr auto VKEY_RIGHT = VK_RIGHT;

    /**
     * Data structures
     */

    using Point = POINT;
    using Rect = RECT;
    using WindowHandle = HWND;
    using XinputState = XINPUT_STATE;

    /**
     * Functions
     */

    void createZeroMemory(void *p, const std::size_t size) {
        ZeroMemory(p, size);
    }

    auto getActiveCodePage() {
        return GetACP();
    }

    auto getMousePosition(const HWND handle) {
        RECT rect;
        POINT point;

        if (handle == GetForegroundWindow()
            && GetCursorPos(&point)
            && GetWindowRect(handle, &rect)
            && PtInRect(&rect, point)
            && ScreenToClient(handle, &point)) {
            return point;
        }

        return POINT{-1, -1};
    }

    float getScaleFactor() {
        return static_cast<float>(GetDpiForSystem()) / 96.0f;
    }

    auto getScreenRect() {
        RECT screenRect;
        SystemParametersInfo(SPI_GETWORKAREA, 0, &screenRect, 0);

        return screenRect;
    }

    bool isKeyDown(const int vKey) {
        return static_cast<bool>(GetAsyncKeyState(vKey) & 0x8000);
    }

    auto XinputGetState(XinputState *state) {
        return XInputGetState(0, state);
    }
}

export namespace easyx {
    constexpr int WINDOW_SHOWCONSOLE = EX_SHOWCONSOLE;
    constexpr int WINDOW_NOCLOSE = EX_NOCLOSE;
    constexpr int WINDOW_NOMINIMIZE = EX_NOMINIMIZE;
    constexpr int WINDOW_DBLCLKS = EX_DBLCLKS;

    constexpr unsigned int TEXT_BOTTOM = DT_BOTTOM;
    constexpr unsigned int TEXT_CALCRECT = DT_CALCRECT;
    constexpr unsigned int TEXT_CENTER = DT_CENTER;
    constexpr unsigned int TEXT_EDITCONTROL = DT_EDITCONTROL;
    constexpr unsigned int TEXT_END_ELLIPSIS = DT_END_ELLIPSIS;
    constexpr unsigned int TEXT_EXPANDTABS = DT_EXPANDTABS;
    constexpr unsigned int TEXT_EXTERNALLEADING = DT_EXTERNALLEADING;
    constexpr unsigned int TEXT_INTERNAL = DT_INTERNAL;
    constexpr unsigned int TEXT_LEFT = DT_LEFT;
    constexpr unsigned int TEXT_MODIFYSTRING = DT_MODIFYSTRING;
    constexpr unsigned int TEXT_NOCLIP = DT_NOCLIP;
    constexpr unsigned int TEXT_NOPREFIX = DT_NOPREFIX;
    constexpr unsigned int TEXT_PATH_ELLIPSIS = DT_PATH_ELLIPSIS;
    constexpr unsigned int TEXT_RIGHT = DT_RIGHT;
    constexpr unsigned int TEXT_RTLREADING = DT_RTLREADING;
    constexpr unsigned int TEXT_SINGLELINE = DT_SINGLELINE;
    constexpr unsigned int TEXT_TABSTOP = DT_TABSTOP;
    constexpr unsigned int TEXT_TOP = DT_TOP;
    constexpr unsigned int TEXT_VCENTER = DT_VCENTER;
    constexpr unsigned int TEXT_WORDBREAK = DT_WORDBREAK;
    constexpr unsigned int TEXT_WORD_ELLIPSIS = DT_WORD_ELLIPSIS;

    using Color = COLORREF;
    using FillStyle = FILLSTYLE;
    using Image = IMAGE;
    using LineStyle = LINESTYLE;

    /**
     * 绘图设备相关函数
     */

    void clearDevice() {
        cleardevice();
    }

    void closeGraph() {
        closegraph();
    }

    HWND initGraph(const int width, const int height, const int flag = 0) {
        return initgraph(width, height, flag);
    }

    void setAspectRatio(const float x, const float y) {
        setaspectratio(x, y);
    }

    /**
     * 颜色模型
     */

    COLORREF fromRGB(const int r, const int g, const int b) {
        return RGB(r, g, b);
    }

    COLORREF fromHSL(const float h, const float s, const float l) {
        return HSLtoRGB(h, s, l);
    }

    /**
     * 图形颜色及样式设置相关函数
     */

    void setBackgroundStyle(const bool isOpaque) {
        isOpaque
        ? setbkmode(OPAQUE)
        : setbkmode(TRANSPARENT);
    }

    void setBackgroundColor(const COLORREF color) {
        setbkcolor(color);
    }

    void setLineStyle(const int thickness) {
        thickness
        ? setlinestyle(PS_SOLID, thickness)
        : setlinestyle(PS_NULL);
    }

    void setLineColor(const COLORREF color) {
        setlinecolor(color);
    }

    void setFillStyle(const bool isEnabled) {
        isEnabled
        ? setfillstyle(BS_SOLID)
        : setfillstyle(BS_NULL);
    }

    void setFillColor(const COLORREF color) {
        setfillcolor(color);
    }

    /**
     * 图形绘制相关函数
     */

    /**
     * 文字输出相关函数
     */

    int drawText(std::string_view text, RECT *location, const unsigned int style = 0) {
        return drawtext(text.data(), location, style);
    }

    void setFont(const long size,
                 const bool bold = false,
                 const bool italic = false,
                 const bool underline = false) {
        LOGFONT font;

        gettextstyle(&font);

        font.lfHeight = size;
        font.lfWeight = bold ? 700 : 400;
        font.lfItalic = italic;
        font.lfUnderline = underline;
        font.lfQuality = ANTIALIASED_QUALITY;
        font.lfPitchAndFamily = DEFAULT_PITCH;
        // font.lfFaceName[0] = '\0'; // reset font

        settextstyle(&font);
    }

    /**
     * 图像处理相关函数
     */

    void putImage(const int x, const int y, const IMAGE *image, const DWORD operation = SRCCOPY) {
        putimage(x, y, image, operation);
    }

    void putImage(const POINT &point, const IMAGE *image, const DWORD operation = SRCCOPY) {
        putimage(point.x, point.y, image, operation);
    }

    void setWorkingImage(IMAGE *image = nullptr) {
        SetWorkingImage(image);
    }

    /**
     * 其他函数
     */
    void beginBatchDraw() {
        BeginBatchDraw();
    }

    void endBatchDraw() {
        EndBatchDraw();
    }
}

export std::string Utf8ToGbk(const char *src_str) {
    /**
     * For the record,
     * this function is taken from https://blog.csdn.net/u012234115/article/details/83186386
     * in order to implement Chinese display.
     */
    int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, nullptr, 0);
    auto *wszGBK = new wchar_t[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, nullptr, 0, nullptr, nullptr);
    auto *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, nullptr, nullptr);
    std::string strTemp(szGBK);
    delete[] wszGBK;
    delete[] szGBK;
    return strTemp;
}
