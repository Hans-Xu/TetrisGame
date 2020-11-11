#include "Piece.h"

using namespace std;

void Piece::set_shape(Shape &cshape, int shape[][4], int &size_w, int &size_h) {
    // initialize 4*4 array to zero
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            shape[i][j] = 0;
        }
    }

    // set 7 types of boxes and their corresponding sizes
    switch(cshape) {
        case I:
            size_h = 1;
            size_w = 4;
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[0][2] = 1;
            shape[0][3] = 1;
            break;
        case J:
            size_h = 2;
            size_w = 3;
            shape[0][0] = 1;
            shape[1][0] = 1;
            shape[1][1] = 1;
            shape[1][2] = 1;
            break; 
        case L:
            size_h = 2;
            size_w = 3;
            shape[0][2] = 1;
            shape[1][0] = 1;
            shape[1][1] = 1;
            shape[1][2] = 1;
            break;   
        case S:
            size_h = 2;
            size_w = 3;
            shape[0][1] = 1;
            shape[0][2] = 1;
            shape[1][0] = 1;
            shape[1][1] = 1;
            break; 
        case Z:
            size_h = 2;
            size_w = 3;
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[1][1] = 1;
            shape[1][2] = 1;
            break;   
        case O:
            size_h = 2;
            size_w = 2;
            shape[0][0] = 1;
            shape[0][1] = 1;
            shape[1][0] = 1;
            shape[1][1] = 1;
            break;  
        case T:
            size_h = 2;
            size_w = 3;
            shape[0][1] = 1;
            shape[1][0] = 1;
            shape[1][1] = 1;
            shape[1][2] = 1;
            break;          
    }

    // set the initial position of the boxes
    head_x = interface->GAME_WINDOW_WIDTH / 2;
    head_y = 1;

    // if the box position is invalid, set the game_over to true
    if (!is_valid()) {
        game_over = true;
    }
}


void Piece::display_box(int box_shape[][4], char symbol) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (box_shape[i][j] == 1) {
                mvwaddch(interface->game_window, head_y + i, head_x + j, symbol);
                wrefresh(interface->game_window);
            }
        }
    }
}


void Piece::rotate() {
    int temp[4][4] = {0}; // temporary array
    int backUp[4][4] = {0}; // backup array
    // backup the original width and height
    int size_w_backup, size_h_backup; 
    size_w_backup = size_w;
    size_h_backup = size_h;
    // backup the box before rotation
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            backUp[i][j] = box_shape[i][j];
        }
    }
    // diagonal symmetry
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            temp[i][j] = box_shape[j][i];
        }
    }
    // swap height and width
    swap(size_h, size_w);
    // bilateral symmetry
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            box_shape[i][size_w - 1 - j] = temp[i][j];
        }
    } 

    // now the rotation is done, we need to check whether this new array is valid
    // if it is invalid, use the backup data to restore the origin array
    if (!is_valid()) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                box_shape[i][j] = backUp[i][j];
            }
        }
        size_w = size_w_backup;
        size_h = size_h_backup;
    }
    // else, the rotation is valid, display it on the window
    else {
        // erase the last box
        display_box(backUp, ' ');
        // display new box
        display_box(box_shape, '#');
    }
}

void Piece::move() {
    // use fd_set and select to achieve waking up the thread blocked by getch()
    fd_set set;
    FD_ZERO(&set); // clear fd_set
    FD_SET(0, &set); // put 0 into fd_set
    // set waiting time to 500,000 us
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 500000;
    // if there is no key input in 500,000us, run the next code instead of waiting.
    if (select(1, &set, NULL, NULL, &timeout) == 0) {
        //move downward
        move_down();
    }
    if (FD_ISSET(0, &set)) {
        while((interface->key = getch()) == -1);
        // once there is a key input, the following code will be run
        switch(interface->key) {
            case KEY_LEFT:
                move_left();
                break;
            case KEY_RIGHT:
                move_right();
                break;
            case KEY_DOWN:
                move_down();
                break;
            case KEY_UP:
                rotate();
                break;
        }
        // Need more boudary check here??
    }
}

bool Piece::is_valid() {
    for (int i = 0; i < size_h; ++i) {
        for (int j = 0; j < size_w; j++) {
            if (box_shape[i][j] == 1) {
                // invalid if the box is over bottom boudary
                if (head_y + i > interface->GAME_WINDOW_HEIGHT- 2) {
                    return false;
                }
                // invalid if the box is over left/right boudary
                if (head_x + j > interface->GAME_WINDOW_WIDTH - 2 || head_x + i < 1) {
                    return false;
                }
                // invalid if the bos is overlapping with other existing boxes
                if (box_map[head_y + i][head_x + j] == 1) {
                    return false;
                }
            }
        }
    }
    return true;
}



void Piece::move_down() {
    head_y++;
    // check whether the move is valid
    if (!is_valid()) {
        // if it's not valid, restore the original position
        head_y--;
        // update box map and conduct next round
        for (int i = 0; i < size_h; ++i) {
            for (int j = 0; j < size_w; ++j) {
                if (box_shape[i][j] == 1) {
                    box_map[head_y + i][head_x + j] = 1;
                }
            }
        }
        score_next();
    } else {
        // else display the box after moving downward
        for (int i = size_h - 1; i >= 0; --i) {
            for (int j = 0; j < size_w; ++j) {
                if (box_shape[i][j] == 1) {
                    mvwaddch(interface->game_window, head_y - 1 + i, head_x + j, ' ');
                    mvwaddch(interface->game_window, head_y + i, head_x + j, '#');
                }
            }
        }
    }
}

void Piece::move_left() {
    head_x--;
    // check whether it is valid
    if (!is_valid()) {
        // if it's invalid, restore the original position
        head_x++;
    } else {
        // else display the box after moving leftward
        for (int i = 0; i < size_h; ++i) {
            for (int j = 0; j < size_w; ++j) {
                if (box_shape[i][j] == 1) {
                    mvwaddch(interface->game_window, head_y + i, head_x + j + 1, ' ');
                    mvwaddch(interface->game_window, head_y + i, head_x + j, '#');
                }
            }
        }
        wrefresh(interface->game_window);
    }
}

void Piece::move_right() {
    head_x++;
    // check whether it is valid
    if (!is_valid()) {
        // if it's invalid, restore the original position
        head_x--;
    } else {
        // else display the box after moving leftward
        for (int i = 0; i < size_h; ++i) {
            for (int j = size_w - 1; j >= 0; -j) {
                if (box_shape[i][j] == 1) {
                    mvwaddch(interface->game_window, head_y + i, head_x + j - 1, ' ');
                    mvwaddch(interface->game_window, head_y + i, head_x + j, '#');
                }
            }
        }
        wrefresh(interface->game_window);
    }
}