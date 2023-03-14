export module Menu.MenuItem;

import Menu;

void MenuItem::action() const {
    this->_mediator->notify(this->event);
}
