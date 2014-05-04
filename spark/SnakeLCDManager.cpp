#include "Adafruit_PCD8544.h"

#include "Point.h"
#include "SnakeLCDManager.h"

SnakeLCDManager::SnakeLCDManager(Snake *snake, Adafruit_PCD8544 *display): snake(snake), display(display) {
    this->edge = display->width() / snake->getBoardWidth();
}

void SnakeLCDManager::initialDraw() {
    display->clearDisplay();
    for (int i = 0; i < this->snake->getBody()->size(); ++i) {
        Point *point = this->snake->getBody()->get(i);
        this->drawBodyPart(point);
    }
    if (this->snake->getFood() != NULL) {
        this->drawFood(this->snake->getFood());
    }
    this->drawBoard();
    display->display();
}

void SnakeLCDManager::drawAfterStep() {
    if (this->snake->getFood() != NULL) {
        this->drawFood(this->snake->getFood());
    }
    Point *head = this->snake->getBody()->get(0);
    this->drawBodyPart(head);
    if (this->snake->getPrevTail() != NULL) {
        this->drawBodyPart(this->snake->getPrevTail(), WHITE);
    }
    display->display();
}

void SnakeLCDManager::drawBodyPart(Point *point, int color) {
    display->fillRect(point->x * edge + 1, point->y * edge + 1, edge, edge, color);
}

void SnakeLCDManager::drawFood(Point *point) {
    display->drawRect(point->x * edge + 1, point->y * edge + 1, edge, edge, BLACK);
}

void SnakeLCDManager::drawBoard(){
    display->drawRect(0, 0, display->width(), display->height(), BLACK);
}
