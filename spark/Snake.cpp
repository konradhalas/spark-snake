#include <stdlib.h>

#include "Snake.h"

Snake::Snake(int length, int boardWidth, int boardHeight) : startLength(length), boardWidth(boardWidth), boardHeight(boardHeight), 
    direction(0, -1), turned(false), food(NULL), alive(true), fed(false), prevTail(NULL) {
    for (int i = 0; i < this->startLength; ++i) {
        Point *point = new Point(this->boardWidth/2, this->boardHeight/2 + i);
        this->body.add(point);
    }
    this->generateFood();
}

void Snake::step() {
    this->fed = false;
    Point *oldHead = this->body.get(0);
    Point *newHead = new Point(oldHead->x + this->direction.x, oldHead->y + this->direction.y);
    if (this->isOutsideBoard(newHead) || this->isBodyPart(newHead)) {
        this->alive = false;
    } else {
        this->body.unshift(newHead);
        if (*newHead == *(this->food)) {
            this->fed = true;
            this->generateFood();
        } else {
            if (this->prevTail != NULL) {
                delete(this->prevTail);
            }
            this->prevTail = this->body.pop();
        }
        this->turned = false;
    }
}

void Snake::turnLeft() {
    if (!this->turned) {
        this->turned = true;
        int tmpX = this->direction.x;
        this->direction.x = this->direction.y;
        this->direction.y = -tmpX;
    }
}

void Snake::turnRight() {
    if (!this->turned) {
        this->turned = true;
        int tmpX = this->direction.x;
        this->direction.x = -this->direction.y;
        this->direction.y = tmpX;
    }   
}

void Snake::generateFood() {
    if (this->food != NULL) {
        delete(this->food);
        this->food = NULL;
    }
    Point startPoint(rand() % this->boardWidth, rand() % this->boardHeight);
    Point currentPoint(startPoint.x, startPoint.y);
    bool found = false;
    do {
        do {
            found = !this->isBodyPart(&currentPoint);
            if (found) {
               break;
            }
            currentPoint.y = (currentPoint.y + 1) % this->boardHeight;    
        } while (currentPoint.y != startPoint.y);
        if (found) {
           break;
        }
        currentPoint.x = (currentPoint.x + 1) % this->boardWidth;
    } while (currentPoint.x != startPoint.x);
    if (found) {
        this->food = new Point(currentPoint.x, currentPoint.y);
    }
}

Snake::~Snake() {
    if (this->food != NULL) {
        delete(this->food);
    }
    for (int i = 0; i < this->body.size(); ++i) {
        Point *point = this->body.get(i);
        delete(point);
    }
    if (this->prevTail != NULL) {
        delete(this->prevTail);
    }
    this->body.clear();
}

bool Snake::isBodyPart(Point *point) {
    bool result = false;
    for (int i = 0; i < this->body.size(); ++i) {
        Point *bodyPart = this->body.get(i);
        if (*bodyPart == *point) {
            result = true;
            break;
        }
    }
    return result;
}

bool Snake::isOutsideBoard(Point *point) {
    return point->x < 0 || point->x >= this->boardWidth || point->y < 0 || point->y >= this->boardHeight;
}

bool Snake::isAlive() {
    return this->alive;
}

int Snake::getScore() {
    return this->body.size() - this->startLength;
}

bool Snake::isFed() {
    return this->fed;
}

Point* Snake::getFood() {
    return this->food;
}

LinkedList<Point*>* Snake::getBody() {
    return &(this->body);
}

int Snake::getBoardWidth() {
    return this->boardWidth;
}

int Snake::getBoardHeight() {
    return this->boardHeight;
}

Point* Snake::getPrevTail() {
    return this->prevTail;
}

