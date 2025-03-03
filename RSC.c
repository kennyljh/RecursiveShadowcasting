#include <stdio.h>
#include <stdbool.h>

#define DUNGEON_WIDTH 20
#define DUNGEON_HEIGHT 18
#define RADIUS 10

// char dungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {
//     "####################",
//     "#..................#",
//     "#........###.......#",
//     "#........###.......#",
//     "#........###..#..###",
//     "#........##........#",
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "####################"
// };

char dungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {
    "####################",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "#..................#",
    "####################"
};

bool lightMap[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {false};

typedef enum {
    LeftToRight,
    RightToLeft,
    TopToBottom,
    BottomToTop
} IterateDirection;

typedef enum {
    RowUp,
    RowDown,
    ColLeft,
    ColRight
} FromDirection;

void findFOV(int playerX, int playerY);

void castLight(float startSlope, float endSlope, int startX, int startY, 
    IterateDirection itrDir, FromDirection fromDir,
    int radius, int currentRowCol);

bool isBlocked(int x, int y);

void becomeVisible(int x, int y);

float calculateSlope(int x1, int y1, int x2, int y2);

void printDungeon();

void printVisionMap(int playerX, int playerY);

int main(int argc, char *argv[]) {
    
    int playerX = 18, playerY = 16;
    printDungeon();
    findFOV(playerX, playerY);
    printVisionMap(playerX, playerY);
    return 0;
}


void findFOV(int playerX, int playerY) {

    // Row-wise (Left to Right)
    castLight(1.0, 0.0, playerX, playerY, LeftToRight, RowUp, RADIUS, 1);  // Octant 1
    // castLight(1.0, 0.0, playerX, playerY, LeftToRight, RowDown, RADIUS, 1); // Octant 6

    // // Row-wise (Right to Left)
    // castLight(1.0, 0.0, playerX, playerY, RightToLeft, RowUp, RADIUS, 1);  // Octant 2
    // castLight(1.0, 0.0, playerX, playerY, RightToLeft, RowDown, RADIUS, 1); // Octant 5

    // // Column-wise (Top to Bottom)
    // castLight(1.0, 0.0, playerX, playerY, TopToBottom, ColRight, RADIUS, 1); // Octant 3
    // castLight(1.0, 0.0, playerX, playerY, TopToBottom, ColLeft, RADIUS, 1); // Octant 8

    // // Column-wise (Bottom to Top)
    // castLight(1.0, 0.0, playerX, playerY, BottomToTop, ColRight, RADIUS, 1); // Octant 4
    // castLight(1.0, 0.0, playerX, playerY, BottomToTop, ColLeft, RADIUS, 1); // Octant 7
}

/*
             Shared
             edge by
  Shared     1 & 2      Shared
  edge by\      |      /edge by
  1 & 8   \     |     / 2 & 3
           \1111|2222/
           8\111|222/3
           88\11|22/33
           888\1|2/333
  Shared   8888\|/3333  Shared
  edge by-------@-------edge by
  7 & 8    7777/|\4444  3 & 4
           777/6|5\444
           77/66|55\44
           7/666|555\4
           /6666|5555\
  Shared  /     |     \ Shared
  edge by/      |      \edge by
  6 & 7      Shared     4 & 5
             edge by 
             5 & 6
    Rough implementation of recursive shadowcasting is based of this explanation by Björn Bergström
    https://www.roguebasin.com/index.php/FOV_using_recursive_shadowcasting
*/
void castLight(float startSlope, float endSlope, int playerX, int playerY, 
                IterateDirection itrDir, FromDirection fromDir,
                int radius, int currentDistance){
    
    if (currentDistance > radius) return;

    bool rowColBlocked = false;
    float newStartSlope = startSlope;

    for (int step = currentDistance; step <= radius; step++){

        int x, y;

        // iteration direction based on octant
        if (itrDir == LeftToRight) {
            x = playerX - step;
        } else if (itrDir == RightToLeft) {
            x = playerX - step;
        } else if (itrDir == TopToBottom) {
            y = playerY + step;
        } else if (itrDir == BottomToTop) {
            y = playerY - step;
        }

        // iteration from direction of player
        if (fromDir == RowUp) {
            y = playerY - (startSlope * step);
        } else if (fromDir == RowDown) {
            y = playerY + (startSlope * step);
        } else if (fromDir == ColLeft) {
            x = playerX - (startSlope * step);
        } else if (fromDir == ColRight) {
            x = playerX + (startSlope * step);
        }

        // check out of bounds
        if (x < 0 || x >= DUNGEON_WIDTH || y < 0 || y >= DUNGEON_HEIGHT) continue;

        // cell not blocked
        if (!isBlocked(x, y)){

            becomeVisible(x, y);

            if (rowColBlocked){

                rowColBlocked = false;
                newStartSlope = calculateSlope(playerX, playerY, x, y);
            }
        }
        // cell is blocked
        else {

            if (!rowColBlocked){

                rowColBlocked = true;
                castLight(newStartSlope, calculateSlope(playerX, playerY, x, y),
                            playerX, playerY, itrDir, fromDir, radius, currentDistance + 1);
            }
        }

        // once end is reached, start new scan if and only if last cell was non blocking
        if (!rowColBlocked){

            castLight(newStartSlope, endSlope, playerX, playerY, itrDir, fromDir, radius, currentDistance + 1);
        }
    }
}

bool isBlocked(int x, int y){

    return dungeon[y][x] == '#';
}

void becomeVisible(int x, int y){
    
    lightMap[y][x] = true;
}

float calculateSlope(int x1, int y1, int x2, int y2){

    return (float)(y2 - y1) / (x2 - x1);
}

void printDungeon(){

    printf("Original Dungeon:\n");

    for (int y = 0; y < DUNGEON_HEIGHT; y++){

        for (int x = 0; x < DUNGEON_WIDTH; x++){

            printf("%c", dungeon[y][x]);
        }
        printf("\n");
    }
}

void printVisionMap(int playerX, int playerY){

    printf("\nVision Map:\n");
    for (int y = 0; y < DUNGEON_HEIGHT; y++){

        for (int x = 0; x < DUNGEON_WIDTH; x++){

            if (x == playerX && y == playerY){

                printf("@");
            }
            else if (lightMap[y][x]){

                printf(".");
            }
            else{

                printf("#");
            }
        }
        printf("\n");
    }
}


