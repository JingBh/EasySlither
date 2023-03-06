import Screen;
import Utils.Frames;

int main() {
    auto screen = Screen();

    while (true) {
        screen.refresh();

        sleepFor(60);
    }
}
