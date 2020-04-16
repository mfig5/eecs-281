/**
 * loops.cpp
 *
 * EECS 183
 * Lab 4: Loops exercises
 *
 * Michelle Figueroa
 * mfig
 *
 * This program will be print out a rectangle or different types of triangles, either right, right justifed, or isosceles.
 */

#include "loops.h"
#include <iostream>

using namespace std;


void printRectangle(int rows, int cols) {
    int i = 0;
    int j = 0;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            cout << "*";
        }
        cout << endl;
    }
    
    }
    


void printRight(int n) {
    int i = 1;
    int j = 1;
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= i; j++) {
            cout << "*";
    }
        cout << endl;
}
    
}

void printRightJustified(int n) {
    int i = n;
    int j = 1;
    for (i = n; i > 0; i--) {
        for (j = 1; j <= n; j++) {
            if (i == j || j > i) {
                cout << "*";
            } else {
                cout << " ";
           }
        }
        cout << endl;
   }
}


void printIsosceles(int n) {
    int i = n;
    int j = 1;
    int y = n;
    int x = n;
    for (i = n; i > 0; i--) {
        for (j = 1; j <= (n + n - 1); j++) {
            if (y != 0) {
                if (j >= i) {
                    cout << "*";
                    y--;
                } else {
                    cout << " ";
                    y--;
                }
            }
        }
        x++;
        y = x;
        cout << endl;
    }
    
    }


