import Middleware.ScreenMediator;
import Middleware.UserInput;
import Screen;
import Screen.ScreenName;
import Utils.Frames;

int main() {
    auto screen = Screen();

    // Events (for observers)
    auto subjectInputDirection = SubjectInputDirection::getInstance();
    auto subjectMouseMove = SubjectMouseMove::getInstance();
    auto subjectKeyPress = SubjectKeyPress::getInstance();

    // Mediators
    auto screenMediator = ScreenMediator::getInstance(&screen);
    screenMediator->notify(ScreenName::MAIN);

    while (true) {
        subjectInputDirection->update();
        subjectMouseMove->update(screen.getHandle());
        subjectKeyPress->update();

        screen.refresh();

        sleepFor(60);
    }
}
