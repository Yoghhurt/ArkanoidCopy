#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "raylib.h"
#include "raymath.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PADDLE_WIDTH 80
#define PADDLE_HEIGHT 20
#define BALL_RADIUS 10
#define BRICK_WIDTH 60
#define BRICK_HEIGHT 20
#define BRICKS_PER_ROW 10
#define NUM_ROWS 5
#define MAX_LIVES 3

typedef struct Ball {
    Vector2 position;
    Vector2 speed;
} Ball;

typedef struct Paddle {
    Vector2 position;
}Paddle;

typedef struct Brick {
    Rectangle rect;
    bool isActive;
}Brick;

