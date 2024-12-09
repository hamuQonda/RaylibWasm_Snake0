#include "raylib.h"
#include <vector>
#include <raymath.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

struct SnakeSegment {
    Vector2 position;
};

struct Food {
    Vector2 position;
    bool active;
};

struct Obstacle {
    Vector2 position;
};

std::vector<SnakeSegment> snake;
std::vector<Food> food;
std::vector<Obstacle> obstacles;

Vector2 direction = {};
float speed = 2.0f;
bool gameOver = false;
int score = 0;

static void InitGame(std::vector<SnakeSegment>& snake, std::vector<Food>& food, std::vector<Obstacle>& obstacles) {
    snake.clear();
    food.clear();
    obstacles.clear();
    direction = { 1,0 };
    speed = 2.0f;
    gameOver = false;
    score = 0;

    snake.push_back({ {GetScreenWidth() / 8.0f     , GetScreenHeight() / 4.0f} });
    snake.push_back({ {GetScreenWidth() / 8.0f - 10, GetScreenHeight() / 4.0f} });
    snake.push_back({ {GetScreenWidth() / 8.0f - 20, GetScreenHeight() / 4.0f} });

    for (int i = 0; i < 10; i++) {
        food.push_back({ {(float)(rand() % GetScreenWidth()), (float)(rand() % GetScreenHeight())}, true });
    }

    for (int i = 0; i < 15; i++) {
        obstacles.push_back({ {(float)(rand() % GetScreenWidth()), (float)(rand() % GetScreenHeight())} });
    }
}

static void UpdateGame(std::vector<SnakeSegment>& snake, std::vector<Food>& food, std::vector<Obstacle>& obstacles) {
    if (gameOver) return;

    Vector2 mousePosition = GetMousePosition();
    direction = Vector2Normalize(Vector2Subtract(mousePosition, snake.front().position));
    speed = IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? 3.0f : 2.0f;

    for (int i = (int)snake.size() - 1; i > 0; i--) {
        snake[i].position = snake[static_cast<std::vector<SnakeSegment, std::allocator<SnakeSegment>>::size_type>(i) - 1].position;
    }

    snake.front().position.x += direction.x * speed;
    snake.front().position.y += direction.y * speed;

    for (Food& f : food) {
        if (f.active && CheckCollisionCircles(snake.front().position, 5, f.position, 5)) {
            f.active = false;
            snake.push_back({ {snake.back().position} });
            food.push_back({ {(float)(rand() % GetScreenWidth()), (float)(rand() % GetScreenHeight())}, true });
            score++;
            obstacles.push_back({ {(float)(rand() % GetScreenWidth()), (float)(rand() % GetScreenHeight())} });
        }
    }

    for (const Obstacle& o : obstacles) {
        if (CheckCollisionCircles(snake.front().position, 5, o.position, 5)) {
            gameOver = true;
        }
    }
}

static void DrawGame(const std::vector<SnakeSegment>& snake, const std::vector<Food>& food, const std::vector<Obstacle>& obstacles) {
    for (const auto& s : snake) {
        if (&s == &snake.front()) {
            DrawCircleV(s.position, 5, PINK);  // 先頭をピンクに描画
        }
        else {
            DrawCircleV(s.position, 5, GREEN);
        }
    }

    for (const auto& f : food) {
        if (f.active) {
            DrawCircleV(f.position, 5, RED);
        }
    }

    for (const auto& o : obstacles) {
        DrawCircleV(o.position, 5, DARKGRAY);
    }

    DrawText(TextFormat("Score: %i", score), 10, 10, 20, WHITE);

    if (gameOver) {
        DrawText("GAME OVER !", GetScreenWidth() / 2 - MeasureText("GAME OVER !", 64) / 2, GetScreenHeight() / 2 - 64, 64, RED);
        DrawText("Press R to Retry", GetScreenWidth() / 2 - MeasureText("Press R to Retry", 32) / 2, GetScreenHeight() / 2 + 16, 32, RED);
    }
}


static void UpdateDrawFrame() {
    if (IsKeyPressed(KEY_R)) {
        InitGame(snake, food, obstacles);
    }

    UpdateGame(snake, food, obstacles);

    BeginDrawing();
    ClearBackground(BLACK);  // バックグラウンドを黒に設定
    DrawGame(snake, food, obstacles);
    EndDrawing();
}


int main() {
    InitWindow(800, 600, "Snake Game");


    InitGame(snake, food, obstacles);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        UpdateDrawFrame();
    }
#endif


    CloseWindow();
    return 0;
}
