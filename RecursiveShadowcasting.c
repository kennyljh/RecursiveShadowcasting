#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#define DUNGEON_HORIZONTAL 10
#define DUNGEON_VERTICAL 10
#define MAX_RADIUS 5

bool lightMap[DUNGEON_VERTICAL][DUNGEON_HORIZONTAL] = {false};
char dungeon[DUNGEON_VERTICAL][DUNGEON_HORIZONTAL] = {
    "##########",
    "#........#",
    "#....@...#",
    "#........#",
    "#........#",
    "##########"
};

void setVisible(int x, int y){

    if (x >= 0 && x < DUNGEON_HORIZONTAL && y >= 0 && y < DUNGEON_VERTICAL){
        lightMap[y][x] = true;
    }
}

bool isBlocked(int x, int y){

    if (x < 0 || x >= DUNGEON_HORIZONTAL || y < 0 || y >= DUNGEON_VERTICAL){
        return true;
    }
    return dungeon[y][x] == '#';
}

void castLight(int row, float startSlope, float endSlope, int startX, int startY, int radius, int octant){

    if (startSlope < endSlope) return;

    float nextStartSlope = startSlope;
    for (int i = row; i <= radius; i++){

        bool blocked = false;

        for (int dx = -i; dx <= 0; dx++){

            int xx = startX, yy = startY;

            switch (octant) {
                case 0:
                    xx += dx;
                    yy -= i;
                    break;
                case 1:
                    xx += i;
                    yy += dx;
                    break;
                case 2:
                    xx += i;
                    yy -= dx;
                    break;
                case 3:
                    xx -= dx;
                    yy += i;
                    break;
                case 4:
                    xx -= dx;
                    yy -= i;
                    break;
                case 5:
                    xx -= i;
                    yy -= dx;
                    break;
                case 6:
                    xx -= i;
                    yy += dx;
                    break;
                case 7:
                    xx += dx;
                    yy += i;
                    break;
            }

            float leftSlope = (dx - 0.5) / i;
            float rightSlope = (dx + 0.5) / i;

            if (leftSlope > startSlope) continue;
            if (rightSlope < endSlope) break;

            setVisible(xx, yy);

            if (blocked){

                if (isBlocked(xx, yy)){

                    nextStartSlope = rightSlope;
                }
                else {

                    blocked = false;
                    startSlope = nextStartSlope;
                }
            }
            else {

                if (isBlocked(xx, yy) && i < radius){

                    blocked = true;
                    castLight(i + 1, startSlope, leftSlope, startX, startY, radius, octant);
                    nextStartSlope = rightSlope;
                }
            }
        }
        if (blocked) break;
    }
}

// Initialize FOV
void calculateFOV(int startX, int startY, int radius) {
    // Reset light map
    for (int y = 0; y < DUNGEON_VERTICAL; y++)
        for (int x = 0; x < DUNGEON_HORIZONTAL; x++)
            lightMap[y][x] = false;

    setVisible(startX, startY); // Player is always visible

    // Cast light for all 8 octants
    for (int octant = 0; octant < 8; octant++)
        castLight(1, 1.0, 0.0, startX, startY, radius, octant);
}

// Print dungeon with FOV
void printDungeon() {
    for (int y = 0; y < DUNGEON_VERTICAL; y++) {
        for (int x = 0; x < DUNGEON_HORIZONTAL; x++) {
            if (x == 4 && y == 2) {
                printf("@"); // Player
            } else if (lightMap[y][x]) {
                printf("."); // Visible
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

int main() {
    int playerX = 4, playerY = 2;
    calculateFOV(playerX, playerY, MAX_RADIUS);
    printDungeon();
    return 0;
}