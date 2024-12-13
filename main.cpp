#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <deque>

const int screenWidth = 800;
const int screenHeight = 600;
const int ballRadius = 5;
const float ballSpeed = 150.0f; // ボールの移動速度（ピクセル毎秒）
const float turnAngle = 360.0f / 24.0f * (PI / 180.0f); // 曲がる角度の増分// ラジアンに変換
const int foodCount = 10;
const int foodRadius = 5;
const int rockRadius = 5;
const int initialLives = 3;
const int initialBodySegments = 3;
const int scoreToGrow = 3;

// スネークの頭の初期位置
Vector2 headBallPos = { screenWidth / 3.0f, screenHeight / 3.0f };

// エサの初期位置をランダムに生成
Vector2 foods[foodCount];

// 岩の初期位置をランダムに生成
std::deque<Vector2> rocks;
int rockCount = 5;

// スコアとライフ
int score = 0;
int lives = initialLives;

// スネークの体
std::deque<Vector2> snakeBody;
int segmentsToAdd = 0;

// ボールの移動方向ベクトルに対してマウスカーソルが左右どちらにあるかを判定する関数
// 返り値: 正の場合は右、負の場合は左、0の場合は同一直線上
float CheckMouseSide(Vector2 ballPosition, Vector2 direction, Vector2 mousePosition) {
    Vector2 toMouse = { mousePosition.x - ballPosition.x, mousePosition.y - ballPosition.y };
    return direction.x * toMouse.y - direction.y * toMouse.x;
}

// 移動方向を角度で回転させる関数
Vector2 RotateVector(Vector2 vector, float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    return { vector.x * cosA - vector.y * sinA, vector.x * sinA + vector.y * cosA };
}

// ランダムな位置にエサを生成する関数
void GenerateRandomFood(Vector2& food) {
    food.x = GetRandomValue(foodRadius, screenWidth - foodRadius);
    food.y = GetRandomValue(foodRadius, screenHeight - foodRadius);
}

// ランダムな位置に岩を生成する関数
void GenerateRandomRock(Vector2& rock) {
    rock.x = GetRandomValue(rockRadius, screenWidth - rockRadius);
    rock.y = GetRandomValue(rockRadius, screenHeight - rockRadius);
}

int main() {
    // ウィンドウの初期化
    InitWindow(screenWidth, screenHeight, "Snake Game - Moving Head");

    // 乱数シードの初期化
    srand(time(NULL));

    // エサの初期化
    for (int i = 0; i < foodCount; i++) {
        GenerateRandomFood(foods[i]);
    }

    // 岩の初期化
    for (int i = 0; i < rockCount; i++) {
        Vector2 rock;
        GenerateRandomRock(rock);
        rocks.push_back(rock);
    }

    // フレームレートの設定
    SetTargetFPS(60);

    // 初期の移動方向（右方向）
    Vector2 direction = { 1.0f, 0.0f };

    // スネークの体の初期化
    for (int i = 0; i < initialBodySegments; i++) {
        snakeBody.push_back(headBallPos);
    }

    bool gameOver = false;

    // ゲームループ
    while (!WindowShouldClose()) {
        if (!gameOver) {

            // マウス位置の取得
            Vector2 mousePosition = GetMousePosition();

            // マウスがボールの移動方向に対して左右どちらにあるか判定
            float side = CheckMouseSide(headBallPos, direction, mousePosition);

            // 移動方向の回転
            if (side > 0) {
                direction = RotateVector(direction, turnAngle); // 右に回転
            }
            else if (side < 0) {
                direction = RotateVector(direction, -turnAngle); // 左に回転
            }

            // ボールの移動（フレームごとの時間を考慮して速度を調整）
            float deltaTime = GetFrameTime();
            headBallPos.x += direction.x * ballSpeed * deltaTime;
            headBallPos.y += direction.y * ballSpeed * deltaTime;

            // スネークの体を更新
            snakeBody.push_front(headBallPos);
            while (snakeBody.size() > initialBodySegments + segmentsToAdd) {
                snakeBody.pop_back();
            }

            // エサとの衝突判定
            for (int i = 0; i < foodCount; i++) {
                if (CheckCollisionCircles(headBallPos, ballRadius, foods[i], foodRadius)) {
                    score++;
                    GenerateRandomFood(foods[i]);
                    if (score % scoreToGrow == 0) {
                        segmentsToAdd++;
                         Vector2 newRock;
                         GenerateRandomRock(newRock);
                         rocks.push_back(newRock);
                    }
                }
            }

            // 岩との衝突判定
            for (auto& rock : rocks) {
                if (CheckCollisionCircles(headBallPos, ballRadius, rock, rockRadius)) {
                    lives--;
                    GenerateRandomRock(rock);
                }
            }

            // ゲームオーバー判定
            if (lives <= 0) {
                BeginDrawing();
                ClearBackground(BLACK);

                DrawText("GAME OVER", screenWidth / 2 - 100, screenHeight / 2 - 20, 40, RED);
                DrawText(TextFormat("Final Score: %d", score), screenWidth / 2 - 100, screenHeight / 2 + 30, 30, DARKGRAY);
                DrawText("Press R to Retry", screenWidth / 2 - 100, screenHeight / 2 + 70, 20, DARKGRAY);

                EndDrawing();

                // リトライ処理
                if (IsKeyPressed(KEY_R)) {
                    headBallPos = { screenWidth / 2.0f, screenHeight / 2.0f };
                    score = 0;
                    lives = initialLives;
                    snakeBody.clear();
                    rocks.clear();
                    for (int i = 0; i < initialBodySegments; i++) snakeBody.push_back(headBallPos);
                    for (int i = 0; i < foodCount; i++) GenerateRandomFood(foods[i]);
                    for (int i = 0; i < rockCount; i++) {
                        Vector2 rock;
                        GenerateRandomRock(rock);
                        rocks.push_back(rock);
                    }
                }

                gameOver = true;
            }
        }

        // 描画
        BeginDrawing();
        ClearBackground(BLACK);

        for (Vector2 segment : snakeBody) {
            DrawCircleV(segment, ballRadius, GREEN);
        }

        DrawCircleV(headBallPos, ballRadius, PINK);

        for (int i = 0; i < foodCount; i++) {
            DrawCircleV(foods[i], foodRadius, RED);
        }

        for (auto& rock : rocks) {
            DrawCircleV(rock, rockRadius, LIGHTGRAY);
        }

        DrawText(TextFormat("Lives: %d", lives), 10, 10, 20, WHITE);
        DrawText(TextFormat("Score: %d", score), 100, 10, 20, WHITE);

        EndDrawing();
    }

    // ウィンドウの終了処理
    CloseWindow();

    return 0;
}
