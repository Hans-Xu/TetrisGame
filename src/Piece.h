#ifndef TETRIS_PIECE_H_
#define TETRIS_PIECE_H_

#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include "Interface.h"

using namespace std;

enum Shape {T, S, Z, J, L, I, O};

class Piece {
    public:
        Piece() = default;
        void initial(); // initial function
        void set_shape(Shape &cshape, int box_shape[][4], int &size_w, int &size_h); // set the shape of the box
        void score_next(); // display the shape and score of the next box
        void judge(); // judge whether there is a full layer
        void move(); // use direction keys to control movements
        void rotate(); // rotate box
        bool is_valid(); // check whether the next action is out of bound or overlapping other boxes
        bool is_empty(int row); // check whether the row is empty


    private:
        int score; 
        int shape; // the shape of the current box
        int next_shape; // the shape of the next box
        // the coordinate of current box
        int head_x; 
        int head_y;
        // the size of the current box
        int size_h;
        int size_w;
        // the size of the next box
        int next_size_h;
        int next_size_w;

        int box_shape[4][4]; // the array of the current box shape
        int next_box_shape[4][4]; // the array of the next box shape
        int box_map[30][45]; // the array marking all the boxes in the game

        bool game_over; // mark whether the game is over
        Interface *interface; // the interface this piece is on

        void display_box(int box_shape[][4], char symbol); // display box with symbol on the game window
        void move_down(); // move the box downward one step
        void move_left(); // move the box leftward one step
        void move_right(); // move the box rightward one step

        // swap a and b
        void swap(int &a, int &b) {
            int temp = a;
            a = b;
            b = temp;
        }

        // get a random number between min and max
        int getRand(int min, int max) {
            return (min + rand() % (max - min + 1));
        }
};

#endif