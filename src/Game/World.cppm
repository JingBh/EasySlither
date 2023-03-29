export module Game.World;

import Game;

void World::addSnake(Snake *snake) {
    this->snakes[snake->id] = snake;
    if (snake->isPlayer) {
        this->player = snake;
    }
}

void World::removeSnake(Snake *snake) {
    this->snakes.erase(snake->id);
}
