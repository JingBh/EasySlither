export module Utils.Drawable;

export class Drawable {
public:
    virtual ~Drawable() = default;

    virtual void render() = 0;
};
