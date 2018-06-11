#ifndef QUEEN_H
#define QUEEN_H

#include "DepsHeaders.h"
#include "Test.h"

struct BOARD {
    int n;
    int** matrix;
};

class Queen : public TestAbstract {
public:
    Queen(size_t tableXY) :
        _szTableXY(tableXY)
    {}

    ~Queen();

    virtual void startTest() override;
    virtual void preparationBeforeTest() override;
    virtual void preparationAfterTest() override;

private:
    size_t _szTableXY;
    BOARD *b;
    float delta_t = 0.0;
    long int calls = 0;
    int i, j;

private:
    void init();
    void printBoard(BOARD* b);
    void queens(size_t n);
    bool placeQueen(BOARD** b, int line, long int* calls);
    bool isValid(BOARD* b, int x, int y);
};

#endif // QUEEN_H
