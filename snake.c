#include <curses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void main() {
    WINDOW* win = initscr();
    keypad(win, true);
    nodelay(win, true);

    int maxX = getmaxx(win);
    int maxY = getmaxy(win);

    uint16_t currentSize = 0;
    uint16_t newIndx = 0;

    const size_t maxSize = maxX * maxY;
    int foodX = rand() % 20;
    int foodY = rand() % 20;
    int dirX = 1;
    int dirY = 0;

    uint8_t isGameOver = 0;

    char characters[maxSize];
    characters[0] = '*';
    for (size_t i = 1; i < maxSize; i++) {
        characters[i] = '-'; 
    }

    int posX[maxSize];
    for (size_t i = 0; i < maxSize; i++) {
        posX[i] = 0; 
    }

    int posY[maxSize];
    for (size_t i = 0; i < maxSize; i++) {
        posY[i] = 0; 
    }

    int prevPosX[maxSize];
    for (size_t i = 0; i < maxSize; i++) {
        prevPosX[i] = 0; 
    }

    int prevPosY[maxSize];
    for (size_t i = 0; i < maxSize; i++) {
        prevPosY[i] = 0; 
    }

    size_t it = 0;
    while (true) {
        int pressed = wgetch(win);
        if (!isGameOver) {
            if (pressed == KEY_LEFT) {
            dirX = -1;
            dirY = 0;
            }
            else if (pressed == KEY_RIGHT) {
                dirX = 1;
                dirY = 0;
            }
            else if (pressed == KEY_UP) {
                dirX = 0;
                dirY = -1;
            }
            else if (pressed == KEY_DOWN) {
                dirX = 0;
                dirY = 1;
            }
            else if (pressed == KEY_BACKSPACE) {
                break;
            }
            prevPosX[0] = posX[0];
            prevPosY[0] = posY[0];
            posX[0] += dirX;
            posY[0] += dirY;

            if (posX[0] < 0 || posX[0] > maxX || posY[0] < 0 || posY[0] > maxY) {
                isGameOver = 1;
                continue;
            }
            for (it = 1; it < maxSize; it++) {
                if (characters[it] == '-') {
                    break;
                }
                prevPosX[it] = posX[it];
                prevPosY[it] = posY[it];
                posX[it] = prevPosX[it-1];
                posY[it] = prevPosY[it-1];
            }
            char test = mvwinch(win, posY[0], posX[0]);
            if (test == '*') {
                isGameOver = 1;
                continue;
            }

            erase();
            for (it = 0; it < maxSize; it++) {
                if (characters[it] == '-') {
                    break;
                }
                mvaddstr(posY[it], posX[it], "*");
            }
            mvaddstr(posY[0], posX[0], "*");
            mvaddstr(foodY, foodX, "&");
            if (foodX == posX[0] && foodY == posY[0]) {
                foodX = rand() % maxX;
                foodY = rand() % maxY;
                newIndx = currentSize + 1;
                characters[newIndx] = '*';
                posX[newIndx] = prevPosX[currentSize];
                posY[newIndx] = prevPosY[currentSize];
                currentSize++;
            }
        }
        else {
            if (pressed == KEY_BACKSPACE) {
                break;
            }
            mvaddstr(maxY/2, (maxX/2) - 5, "GAME OVER");
            mvaddstr((maxY/2) + 1, (maxX/2) - 13, "Press backspace to return");
        }
        
        
        usleep(100000);
    }

    endwin();
}