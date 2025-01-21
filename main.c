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
    Rectangle rect;
    _Bool active;
} Brick;

static void UpdatePaddle(Paddle *paddle) {
    if (IsKeyDown(KEY_LEFT) && paddle->position.x > 0) {
        paddle->position.x -= 5.0f;
    }
    if (IsKeyDown(KEY_RIGHT) && paddle->position.x < GetScreenWidth() - paddle->width) {
        paddle->position.x += 5.0f;
    }
}

static void UpdateBall(Ball *ball, Paddle paddle, Brick bricks[], int *score, int *lives) {
    ball->position.x += ball->speed.x;
    ball->position.y += ball->speed.y;

    if (ball->position.x < 0 || ball->position.x > GetScreenWidth()) {
        ball->speed.x *= -1;
    }
    if (ball->position.y < 0) {
        ball->speed.y *= -1;
    } else if (ball->position.y > GetScreenHeight()) {
        (*lives)--;
        ball->position = (Vector2){GetScreenWidth() / 2, GetScreenHeight() / 2};
        ball->speed = (Vector2){200.0f, 200.0f};
        if (*lives <= 0) {
            ball->speed = (Vector2){0, 0};
        }
    }

    if (CheckCollisionCircles(ball->position, ball->radius, (Vector2){
    paddle.position.x + paddle.width / 2, paddle.position.y}, paddle.height)) {
        ball->speed.y *= -1;
    }

    for (int i = 0; i < MAX_BRICKS; i++) {
        if (bricks[i].active && CheckCollisionCircleRec(ball->position, ball->radius, bricks[i].rect)) {
            ball->speed.y *= -1;
            bricks[i].active = false;
            (*score) += 10;
        }
    }
}

int main() {
    InitWindow(800, 600, "ArkanoidCopy");


    int score = 0;
    int lives = 3;
    bool gameOver = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (!gameOver) {
            UpdatePaddle(&paddle);
            UpdateBall(&ball, paddle, bricks, &score, &lives);

            if (lives <= 0) {
                gameOver = true;
            }
        }
    }
}