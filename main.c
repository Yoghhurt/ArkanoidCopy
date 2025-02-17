#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "raylib.h"
#include "raymath.h"

#define RAYMATH_IMPLEMENTATION
#define RAYMATH_STATIC_INLINE
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

// Structs
typedef struct Ball {
    Vector2 position;
    Vector2 speed;
} Ball;

typedef struct Paddle {
    Vector2 position;
} Paddle;

typedef struct Brick {
    Rectangle rect;
    bool isActive;
} Brick;

typedef struct PowerUp {
    Vector2 position;
    bool isActive;
    float speed;
} PowerUp;

//Variables
Ball ball;
Paddle paddle;
Brick bricks[NUM_ROWS * BRICKS_PER_ROW];
PowerUp powerUp;
int lives = MAX_LIVES;
int score = 0;
bool gameOver = false;

// Functions
void InitializeGame();
void UpdateGame();
void DrawGame();
void ResetBall();
void ResetBricks();
void ResetPowerUp();
void CheckCollisions();
void RestartGame();
void DrawUI();
bool CheckBrickCollision(Rectangle rect);
void DrawPowerUp();

int main(void)
{
    // Initialization
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arkanoid Game");
    SetTargetFPS(60); // Set game to run at 60 FPS
    InitializeGame();

    // Main game loop
    while (!WindowShouldClose())
    {
        if (gameOver)
        {
            //Restart
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

    // De-Initialization
    CloseWindow(); // Close the window and OpenGL context
    return 0;
}

void InitializeGame()
{
    // Set initial positions
    paddle.position = (Vector2){SCREEN_WIDTH / 2 - PADDLE_WIDTH / 2, SCREEN_HEIGHT - PADDLE_HEIGHT - 20};
    ball.position = (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    ball.speed = (Vector2){5.0f, -5.0f};  // Initial ball speed

    ResetBricks();
    ResetPowerUp();
}

void UpdateGame()
{
    // Move paddle with left and right arrow keys
    if (IsKeyDown(KEY_RIGHT) && paddle.position.x + PADDLE_WIDTH < SCREEN_WIDTH)
        paddle.position = Vector2Add(paddle.position, (Vector2){8.0f, 0});  // Move right
    if (IsKeyDown(KEY_LEFT) && paddle.position.x > 0)
        paddle.position = Vector2Add(paddle.position, (Vector2){-8.0f, 0});  // Move left

    // Update ball position
    ball.position = Vector2Add(ball.position, ball.speed);

    // Ball-wall collision (left, right, top walls)
    if (ball.position.x - BALL_RADIUS <= 0 || ball.position.x + BALL_RADIUS >= SCREEN_WIDTH)
        ball.speed.x *= -1;

    if (ball.position.y - BALL_RADIUS <= 0)
        ball.speed.y *= -1;

    // Ball-bottom wall collision (player loses a life)
    if (ball.position.y + BALL_RADIUS >= SCREEN_HEIGHT)
    {
        lives--;
        if (lives <= 0)
        {
            gameOver = true;
        }
        else
        {
            ResetBall();
        }
    }

    // Ball-paddle collision
    if (ball.position.y + BALL_RADIUS >= paddle.position.y &&
        ball.position.x >= paddle.position.x &&
        ball.position.x <= paddle.position.x + PADDLE_WIDTH)
    {
        ball.speed.y *= -1;
    }

    // Ball-brick collision
    CheckCollisions();

    // Powerup movement
    if (powerUp.isActive)
    {
        powerUp.position.y += powerUp.speed;  // Move powerup down

        // Check for ball-powerup collision
        if (CheckCollisionCircleRec(ball.position, BALL_RADIUS, (Rectangle){powerUp.position.x, powerUp.position.y, 20, 20}))
        {
            lives++;  //Extra life
            powerUp.isActive = false;  // Deactivate powerup
        }

        // If powerup goes off the screen, deactivate it
        if (powerUp.position.y > SCREEN_HEIGHT)
        {
            powerUp.isActive = false;
        }
    }
}

void DrawGame()
{
    BeginDrawing();

    ClearBackground(DARKGRAY);

    // Draw paddle
    DrawRectangleV(paddle.position, (Vector2){PADDLE_WIDTH, PADDLE_HEIGHT}, BLUE);

    // Draw ball
    DrawCircleV(ball.position, BALL_RADIUS, RED);

    // Draw bricks
    for (int i = 0; i < NUM_ROWS * BRICKS_PER_ROW; i++)
    {
        if (bricks[i].isActive)
        {
            DrawRectangleRec(bricks[i].rect, GREEN);
        }
    }

    // Draw power-up
    if (powerUp.isActive)
    {
        DrawPowerUp();
    }

    // Draw UI (score, lives)
    DrawUI();

    // Game Over message
    if (gameOver)
    {
        DrawText("GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, 40, RED);
        DrawText("Press R to Restart", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 50, 20, WHITE);
    }

    EndDrawing();
}

void ResetBall()
{
    ball.position = (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    ball.speed = (Vector2){5.0f, -5.0f};
}

void ResetBricks()
{
    for (int i = 0; i < NUM_ROWS * BRICKS_PER_ROW; i++)
    {
        bricks[i].rect = (Rectangle){(i % BRICKS_PER_ROW) * BRICK_WIDTH, (i / BRICKS_PER_ROW) * BRICK_HEIGHT, BRICK_WIDTH, BRICK_HEIGHT};
        bricks[i].isActive = true;
    }
}

void ResetPowerUp()
{
    // Randomly spawn the powerup
    powerUp.position = (Vector2){rand() % (SCREEN_WIDTH - 20), -20};  // Start offscreen
    powerUp.isActive = true;
    powerUp.speed = 2.0f;  // Fall speed of the powerup
}

void CheckCollisions()
{
    for (int i = 0; i < NUM_ROWS * BRICKS_PER_ROW; i++)
    {
        if (bricks[i].isActive && CheckCollisionCircleRec(ball.position, BALL_RADIUS, bricks[i].rect))
        {
            ball.speed.y *= -1;
            bricks[i].isActive = false;
            score += 10; // Increase score for each brick destroyed
        }
    }
}

bool CheckBrickCollision(Rectangle rect)
{
    return CheckCollisionCircleRec(ball.position, BALL_RADIUS, rect);
}

void RestartGame()
{
    lives = MAX_LIVES;
    score = 0;
    gameOver = false;
    ResetBall();
    ResetBricks();
    ResetPowerUp();
}

void DrawUI()
{
    DrawText(TextFormat("Lives: %d", lives), 10, 10, 20, WHITE);
    DrawText(TextFormat("Score: %d", score), SCREEN_WIDTH - 150, 10, 20, WHITE);
}

void DrawPowerUp()
{
    DrawRectangleV(powerUp.position, (Vector2){20, 20}, YELLOW);  // Draw the powerup as a yellow square
}