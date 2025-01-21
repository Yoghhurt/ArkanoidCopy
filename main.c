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

Ball ball;
Paddle paddle;
Brick bricks[NUM_ROWS * BRICKS_PER_ROW];
int lives = MAX_LIVES;
int score = 0;
bool gameOver = false;

void InitializeGame();
void UpdateGame();
void DrawGame();
void ResetBall();
void ResetBricks();
void CheckCollisions();
void RestartGame();
void DrawUI();
bool CheckBrickCollision(Rectangle rect);

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ArkanoidCopy");
    SetTargetFPS(60);
    InitializeGame();

    while (!WindowShouldClose()) {
        if (gameOver)
        {
            if (IsKeyPressed(KEY_R))
            {
                RestartGame();
            }
        }

        else

        {
            UpdateGame();
        }
        DrawGame();
    }
    CloseWindow();
    return 0;
}

void InitializeGame() {
    paddle.position = (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,};
    ball.position = (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    ball.speed = (Vector2){5.0f, 5.0f};

    ResetBricks();
}

void UpdateGame() {
    if (IsKeyDown(KEY_RIGHT) && paddle.position.x + PADDLE_WIDTH < SCREEN_WIDTH)
        paddle.position.x += 8;
    if (IsKeyDown(KEY_LEFT) && paddle.position.x > 0)
        paddle.position.x -= 8;

    ball.position.x += ball.speed.x;
    ball.position.y += ball.speed.y;

    if (ball.position.x - BALL_RADIUS <= 0 || ball.position.x + BALL_RADIUS >= SCREEN_WIDTH)
        ball.speed.x *= -1;
    if (ball.position.y - BALL_RADIUS <= 0)
        ball.speed.y *= -1;

    if (ball.position.y + BALL_RADIUS >= SCREEN_HEIGHT) {
        lives--;
        if (lives <= 0) {
            gameOver = true;
        }
        else {
            ResetBall();
        }
    }
    if (ball.position.y + BALL_RADIUS >= paddle.position.y &&
        ball.position.x >= paddle.position.x &&
        ball.position.x <= paddle.position.x + PADDLE_WIDTH) {
        ball.speed.x *= -1;
    }
    CheckCollisions();
}
