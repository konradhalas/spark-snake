#ifndef POINT_H_
#define POINT_H_

class Point {
    
    public:
        Point(int x, int y): x(x), y(y) {};
        bool operator==(const Point & other) const { return this->x == other.x && this->y == other.y;};
        int x;
        int y;
};

#endif
