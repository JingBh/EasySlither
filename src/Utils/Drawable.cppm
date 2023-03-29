export module Utils.Drawable;

import <memory>;

import ThirdParty;

export enum class RenderAnchor {
    TOP_LEFT,
    TOP_MIDDLE,
    CENTER
};

export class Drawable {
    using
    enum RenderAnchor;

protected:
    windows::Rect *renderRect = nullptr;

public:
    virtual ~Drawable() {
        delete renderRect;
    }

    [[nodiscard]] virtual std::unique_ptr <easyx::Image> renderImage() const = 0;

    void renderAt(windows::Point point, // not ref, because value might be modified
                  easyx::Image *parentImage = nullptr,
                  const RenderAnchor anchor = TOP_LEFT) {
        const auto image = renderImage();

        switch (anchor) {
            case TOP_LEFT:
                break;
            case TOP_MIDDLE:
                point.x -= image->getwidth() / 2;
                break;
            case CENTER:
                point.x -= image->getwidth() / 2;
                point.y -= image->getheight() / 2;
                break;
        }

        easyx::setWorkingImage(parentImage);
        easyx::putImage(point, image.get());

        delete renderRect;
        renderRect = new windows::Rect{
            point.x,
            point.y,
            point.x + image->getwidth(),
            point.y + image->getheight()
        };
    }
};

export class LocationAwareDrawable : public Drawable {
public:
    virtual void render() {
        windows::Point renderLocation{0, 0};
        this->renderAt(renderLocation);
    };
};
