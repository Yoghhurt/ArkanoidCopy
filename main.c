#include "raylib.h"

typedef struct Paddle {
    Vector2 position;
    float width;
    float height;
} Paddle;

typedef struct Ball {
    Vector2 position;
    Vector2 speed;
    float radius;
} Ball;

#define MAX_BRICKS 100
typedef struct Brick {
    Rectangle rect:
    bool active;
} Brick;

static void UpdatePaddle(Paddle *paddle) {
    if (IsKeyDown(KEY_LEFT) && paddle->position.x > 0) {
        paddle->position.x -= 5.0f;
    }
    if (IsKeyDown(KEY_RIGHT) && paddle->position.x < GetScreenWidth() - paddle->width) {
        paddle->position.x += 5.0f;
    }
}