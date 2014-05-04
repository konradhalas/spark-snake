#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"
#include "LinkedList.h"
#include "Song.h"

#include "Snake.h"
#include "Point.h"
#include "SnakeLCDManager.h"

// MissionImp
char *splashScreenSong = "d=16,o=6,b=190:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,"
                         "g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,"
                         "2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";

Adafruit_PCD8544 display = Adafruit_PCD8544(SS, D2, D3);

int left = D5;
int right = D6;
int light = A0;
int speaker = D7;

void setup()   {
    Serial.begin(9600);
    display.begin();
    display.setContrast(50);
    display.clearDisplay();
    pinMode(left, INPUT);
    pinMode(right, INPUT);
    pinMode(light, OUTPUT);
    pinMode(speaker, OUTPUT);
    analogWrite(light, 100);
    display.clearDisplay();
}

void loop() {
    splashScreenLoop();
    gameLoop();
}

void splashScreenLoop() {
    display.setCursor(12, 5);
    display.println("SNAKE v0.1");
    display.setCursor(5, 15);
    display.println("@KONRADHALAS");
    display.setCursor(3, 35);
    display.println("PRESS ANY KEY");
    display.display();
    delay(200);
    Song song(splashScreenSong, speaker);
    while (digitalRead(left) != HIGH && digitalRead(right) != HIGH) { song.playNote(); };
    display.clearDisplay();
}

void gameLoop() {
    delay(200); 
    Snake snake(4, 41, 23);
    int cycle = 0;
    int cyclePerStep = 3;
    int cycleDuration = 70;
    int eatSound = 478;
    bool leftButtonReleased = true;
    bool rightButtonReleased = true;
    SnakeLCDManager lcdManager(&snake, &display);
    lcdManager.initialDraw();
    Spark.publish("game", "game_start");
    while (snake.isAlive()) {
        ++cycle;
        if (cycle == cyclePerStep) {
            snake.step();
            lcdManager.drawAfterStep();
            cycle = 0;
        }
        if (digitalRead(left) == HIGH) {
            if (leftButtonReleased) {
                snake.turnLeft();
                leftButtonReleased = false;
            }
        } else {
            leftButtonReleased = true;
        }
        if (digitalRead(right) == HIGH) {
            if (rightButtonReleased) {
                snake.turnRight();
                rightButtonReleased = false;
            }
        } else {
            rightButtonReleased = true;
        }
        
        if (snake.isFed()) {
            publishScore(snake.getScore());
            Song::tone(speaker, eatSound, cycleDuration - 50);
        } else {
            delay(cycleDuration);
        }
    }
    Spark.publish("game", "game_over");
    display.clearDisplay();
    display.setCursor(10, 5);
    display.println("SCORE");
    display.setTextSize(2);
    display.setCursor(15, 20);
    display.print(snake.getScore());
    display.setTextSize(1);
    display.display();
    Song gameOverSong("16c7,16b,16a,16g,16f,16e,4c", speaker);
    while (gameOverSong.playNote()) {};
    while (digitalRead(left) != HIGH && digitalRead(right) != HIGH) { delay(10); };
    display.clearDisplay();
}

void publishScore(int score) {
    String scoreStr(score, (uint8_t)10);
    char charBuf[5];
    scoreStr.toCharArray(charBuf, 5);
    Spark.publish("game", charBuf);
}
