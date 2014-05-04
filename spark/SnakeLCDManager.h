#ifndef SNAKE_LCD_MANAGER_H_
#define SNAKE_LCD_MANAGER_H_

#include "Snake.h"
#include "Adafruit_PCD8544.h"

class SnakeLCDManager {

    public:
        SnakeLCDManager(Snake *snake, Adafruit_PCD8544 *display);
        void initialDraw();
        void drawAfterStep();
    private:
        void drawBodyPart(Point *point, int color=BLACK);
        void drawFood(Point *point);
        void drawBoard();
        Snake *snake;
        Adafruit_PCD8544 *display;
        int edge;
};

#endif
