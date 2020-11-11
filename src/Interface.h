#ifndef TETRIS_INTERFACE_H_
#define TETRIS_INTERFACE_H_

using namespace std;

class Interface {
    public:

    private:
    WINDOW *game_window;
    const int GAME_WINDOW_WIDTH;
    const int GAME_WINDOW_HEIGHT;
    int key; // control input info

    friend class Piece;

};

#endif