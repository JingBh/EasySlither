export module Menu.MenuItem;

import Internal.ScreenMediator;
import Menu;

void MenuItem::action() const {
    ScreenMediator::getInstance()->notify(this->event);
}
