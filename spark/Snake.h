#ifndef SNAKE_H
#define SNAKE_H

#include "LinkedList.h"

#include "Point.h"

class Snake {
    
    public: 
        Snake(int length, int boardWidth, int boardHeight);
        ~Snake();
        void step();
        void turnLeft();
        void turnRight();
        bool isAlive();
        bool isFed();
        int getScore();
        int getBoardWidth();
        int getBoardHeight();
        Point *getFood();
        LinkedList<Point*>* getBody();
        Point *getPrevTail();
    private:
        void generateFood();
        bool isBodyPart(Point *point);
        bool isOutsideBoard(Point *point);
        
        int boardWidth;
        int boardHeight;
        int startLength;
        LinkedList<Point*> body;
        Point direction;
        Point *food;
        Point *prevTail;
        bool turned;
        bool alive;
        bool fed;
};

#endif
