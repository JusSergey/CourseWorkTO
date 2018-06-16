#include "Queen.h"
#include <iostream>
#include <stdio.h>

void Queen::startTest() {
    queens(_szTableXY);
    std::cout << std::endl;
}

void Queen::preparationBeforeTest()
{
    init();
}

void Queen::preparationAfterTest()
{
    for (int i = 0; i < _szTableXY; ++i)
    delete []b->matrix[i];
    delete []b->matrix;
    delete b;
}

Queen::~Queen() {
}

void Queen::init() {
    b = new BOARD;
    b->n = _szTableXY;
    b->matrix = new int*[_szTableXY];
    for (i = 0; i < _szTableXY; i++) {
        b->matrix[i] = new int[_szTableXY];
        for (j = 0; j < _szTableXY; j++) {
            b->matrix[i][j] = 0;
        }
    }
}

bool Queen::placeQueen(BOARD** b, int line, long int* calls) {
    int i;
    if (line >= (*b)->n) return true;
    for (i = 0; i < (*b)->n; i++) {
        (*b)->matrix[line][i] = 1;
        (*calls)++;
        if (isValid((*b), line, i) && placeQueen(b, line+1, calls)) {
            return true;
        }
        (*b)->matrix[line][i] = 0;
    }
    return false;
}

void Queen::printBoard(BOARD* b) {
    int i, j;
    for (i = 0; i < b->n; i++) {
        for (j = 0; j < b->n; j++) {
            if (b->matrix[i][j]) {
                printf("* ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
}

void Queen::queens(size_t n)
{
    // benchmarking
    if (placeQueen(&b, 0, &calls)) {
        // no print for test
        // printBoard(b);
    }
}

bool Queen::isValid(BOARD* b, int x, int y) {

    int i, j;
    // check horizontal
    for (i = 0; i < b->n; i++) {
        if (i != y && b->matrix[x][i] == 1) return false;
    }

    // check vertical
    for (i = 0; i < b->n; i++) {
        if (i != x && b->matrix[i][y] == 1) return false;
    }
    // check diagonals
    // check top left
    i = x;
    j = y;
    while (i >= 0 && j >= 0) {
        if (i != x && j != y && b->matrix[i][j] == 1) {
            return false;
        }
        i--;
        j--;
    }
    // check top right
    i = x;
    j = y;
    while (i >= 0 && j < b->n) {
        if (i != x && j != y && b->matrix[i][j] == 1) {
            return false;
        }
        i--;
        j++;
    }
    // check bottom left
    i = x;
    j = y;
    while (i < b->n && j >= 0) {
        if (i != x && j != y && b->matrix[i][j] == 1) {
            return false;
        }
        i++;
        j--;
    }
    // check bottom right
    i = x;
    j = y;
    while (i < b->n && j < b->n) {
        if (i != x && j != y && b->matrix[i][j] == 1) {
            return false;
        }
        i++;
        j++;
    }
    return true;
}
