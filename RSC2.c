#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#define DUNGEON_WIDTH 20
#define DUNGEON_HEIGHT 19
// #define RADIUS 16
#define RADIUS 17

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
//     "#..................#",
//     "####################"
// };

// char dungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {
//     "####################",
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
//     "#..................#",
//     "#........##........#",
//     "#........###..#..###",
//     "#........###.......#",
//     "#........###.......#",
//     "#..................#",
//     "####################"
// };

// char dungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {
//     "####################",
//     "#..................#",
//     "#.......###........#",
//     "#.......###........#",
//     "###..#..###........#",
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
//     "#..................#",
//     "####################"
// };

// char dungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {
//     "####################",
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
//     "#..................#",
//     "#........##........#",
//     "###..#..###........#",
//     "#.......###........#",
//     "#.......###........#",
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
    "#.............####.#",
    "#.............####.#",
    "#..............###.#",
    "#..................#",
    "#..................#",
    "#..............#...#",
    "#..................#",
    "#..............#...#",
    "#..............#...#",
    "####################"
};

// char dungeon[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {
//     "####################",
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
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "#..................#",
//     "####################"
// };

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

float calculateNonNegativeSlope(int x1, int y1, int x2, int y2);

float altCalculateNonNegativeSlope(int x1, int y1, int x2, int y2);

void printDungeon();

void printVisionMap(int playerX, int playerY);

int main(int argc, char *argv[]) {
    
    // octant 1
    //int playerX = 18, playerY = 17;
    // octant 6
    //int playerX = 18, playerY = 1;
    // octant 2
    //int playerX = 1, playerY = 17;
    // octant 5
    //int playerX = 1, playerY = 1;
    // octant 3
    int playerX = 1, playerY = 17;
    printDungeon();
    findFOV(playerX, playerY);
    printVisionMap(playerX, playerY);
    return 0;
}


void findFOV(int playerX, int playerY) {

    // Row-wise (Left to Right)
    //castLight(1.0, 0.0, playerX, playerY, LeftToRight, RowUp, RADIUS, 0);  // Octant 1
    //castLight(1.0, 0.0, playerX, playerY, LeftToRight, RowDown, RADIUS, 0); // Octant 6

    // // Row-wise (Right to Left)
    //castLight(1.0, 0.0, playerX, playerY, RightToLeft, RowUp, RADIUS, 0);  // Octant 2
    //castLight(1.0, 0.0, playerX, playerY, RightToLeft, RowDown, RADIUS, 0); // Octant 5

    // // Column-wise (Top to Bottom)
    castLight(1.0, 0.0, playerX, playerY, TopToBottom, ColRight, RADIUS, 0); // Octant 3
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

    // iterate by row or col
    int changeInX, changeInY;
    int startX, startY, endX, endY;

    // octant 1
    if (itrDir == LeftToRight && fromDir == RowUp){

        changeInX = 1;
        changeInY = 0;
        startX = playerX + (startSlope * -currentDistance);
        startY = playerY - currentDistance;
        endX = playerX + (endSlope * -currentDistance);
        endY = playerY - currentDistance;
    }
    // octant 6
    else if (itrDir == LeftToRight && fromDir == RowDown){

        changeInX = 1;
        changeInY = 0;
        startX = playerX + (startSlope * -currentDistance);
        startY = playerY + currentDistance;
        endX = playerX + (endSlope * -currentDistance);
        endY = playerY + currentDistance;
    }
    // octant 2
    else if (itrDir == RightToLeft && fromDir == RowUp){

        changeInX = -1;
        changeInY = 0;
        startX = playerX + (startSlope * +currentDistance);
        startY = playerY - currentDistance;
        endX = playerX + (endSlope * +currentDistance);
        endY = playerY - currentDistance;
    }
    // octant 5
    else if (itrDir == RightToLeft && fromDir == RowDown){

        changeInX = -1;
        changeInY = 0;
        startX = playerX + (startSlope * +currentDistance);
        startY = playerY + currentDistance;
        endX = playerX + (endSlope * +currentDistance);
        endY = playerY + currentDistance;
    }
    // octant 3
    else if (itrDir == TopToBottom && fromDir == ColRight){

        changeInX = 0;
        changeInY = 1;
        startX = playerX + currentDistance;
        startY = playerY - (startSlope * +currentDistance);
        endX = playerX + currentDistance;
        endY = playerY - (endSlope * +currentDistance);
    }

    bool rowColBlockedInstance = false;
    float newStartSlope = startSlope;

    // iterate by row or col
    int tempX = startX; 
    int tempY = startY;
    int previousX = INT_MAX;
    int previousY = INT_MAX;
    bool reachedEnd = false;
    do {
        if (tempX == endX && tempY == endY) reachedEnd = true;
        // non blocking cell
        if (!isBlocked(tempX, tempY)) {

            becomeVisible(tempX, tempY);

            // first non blocking after series of blocking cells
            if (rowColBlockedInstance) {

                rowColBlockedInstance = false;

                if (itrDir == TopToBottom && fromDir == ColRight){
                    newStartSlope = altCalculateNonNegativeSlope(tempX, tempY, playerX, playerY);
                }
                else {
                    newStartSlope = calculateNonNegativeSlope(tempX, tempY, playerX, playerY);
                }                
            }
        }
        // blocking cell
        else {

            if (rowColBlockedInstance) {

                previousX = tempX;
                previousY = tempY;
                tempX += changeInX;
                tempY += changeInY;
                //printVisionMap(0, 0);
                continue;
            }

            rowColBlockedInstance = true;
            // start on a new iteration with starting blocking cell
            if (previousX == INT_MAX || previousY == INT_MAX){

                if (itrDir == TopToBottom && fromDir == ColRight){
                    newStartSlope = altCalculateNonNegativeSlope(tempX, tempY, playerX, playerY);
                }
                else {
                    newStartSlope = calculateNonNegativeSlope(tempX, tempY, playerX, playerY);
                }  
            }
            // due to perhaps a not so precise calculation/interpretation of slopes as explained in the webpage,
            // values here can be adjusted to prevent undesired vision maps (e.g. see through wall gaps)
            else {
                float newEndSlope;
                // octant 1
                if (itrDir == LeftToRight && fromDir == RowUp){
                    newEndSlope = calculateNonNegativeSlope(previousX - 1, previousY - 1, playerX, playerY);
                }
                // octant 6
                else if (itrDir == LeftToRight && fromDir == RowDown){
                    newEndSlope = calculateNonNegativeSlope(previousX, previousY - 1, playerX, playerY);
                }
                // octant 2
                else if (itrDir == RightToLeft && fromDir == RowUp){
                    newEndSlope = calculateNonNegativeSlope(previousX + 1, previousY - 1, playerX, playerY);
                }
                // octant 5
                else if (itrDir == RightToLeft && fromDir == RowDown){
                    newEndSlope = calculateNonNegativeSlope(previousX, previousY - 1, playerX, playerY);
                }
                // octant 3
                else if (itrDir == TopToBottom && fromDir == ColRight){
                    newEndSlope = altCalculateNonNegativeSlope(previousX + 1, previousY, playerX, playerY);
                }
                else {
                    newEndSlope = calculateNonNegativeSlope(previousX, previousY, playerX, playerY);
                }
                castLight(newStartSlope, newEndSlope, playerX, playerY, itrDir, fromDir, radius, currentDistance + 1);
            }
        }
        previousX = tempX;
        previousY = tempY;
        tempX += changeInX;
        tempY += changeInY;
        //printVisionMap(0, 0);
    } while (!reachedEnd);

    if (!rowColBlockedInstance) castLight(newStartSlope, endSlope, playerX, playerY, itrDir, fromDir, radius, currentDistance + 1);
}

bool isBlocked(int x, int y){

    return dungeon[y][x] == '#';
}

void becomeVisible(int x, int y){
    //printf("show light");
    lightMap[y][x] = true;
}

float calculateNonNegativeSlope(int x1, int y1, int x2, int y2){

    if (((y2 - y1) == 0) || ((x2 - x1) == 0)){
        return 0.0f;
    }

    float result = (float)(x1 - x2) / (float)(y1 - y2);
    //printf("%.2f\n", result);
    return (result > 0) ? result : -result;
}

float altCalculateNonNegativeSlope(int x1, int y1, int x2, int y2){

    if (((y2 - y1) == 0) || ((x2 - x1) == 0)){
        return 0.0f;
    }

    float result = (float)(y1 - y2) / (float)(x1 - x2);
    //printf("%.2f\n", result);
    return (result > 0) ? result : -result;
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

