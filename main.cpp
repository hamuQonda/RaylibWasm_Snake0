#include "raylib.h"
#include <raymath.h>

const int screenWidth = 800;
const int screenHeight = 600;
const int ballRadius = 5;
const float ballSpeed = 3.5f;
const float turnAngle = 360.0f / 24.0f * (PI / 180.0f); // 360度/24をラジアンに変換

// スネークの頭の初期位置
Vector2 ballPosition = { screenWidth / 2.0f, screenHeight / 2.0f };

// ボールの移動方向ベクトルに対してマウスカーソルが左右どちらにあるかを判定する関数
// 返り値: 正の場合は右、負の場合は左、0の場合は同一直線上
float CheckMouseSide(Vector2& ballPosition, Vector2& direction, Vector2& mousePosition) {
    Vector2 toMouse = { mousePosition.x - ballPosition.x, mousePosition.y - ballPosition.y };
    return direction.x * toMouse.y - direction.y * toMouse.x;
}

// 移動方向を角度で回転させる関数
Vector2 RotateVector(Vector2& vector, const float& angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    return { vector.x * cosA - vector.y * sinA, vector.x * sinA + vector.y * cosA };
}

int main() {
    // ウィンドウの初期化
    InitWindow(screenWidth, screenHeight, "Snake Game - Moving Head");

    // フレームレートの設定
    SetTargetFPS(60);

    // 初期の移動方向（右方向）
    Vector2 direction = { 1.0f, 0.0f };

    // マウス位置を保存
    Vector2 previousMousePos = { -1.0f, -1.0f };

    // ゲームループ
    while (!WindowShouldClose()) {
        // マウス位置の取得
        Vector2 mousePosition = GetMousePosition();

        if (Vector2Equals(mousePosition, previousMousePos)) {
        }
        else {
        }
            // マウスがボールの移動方向に対して左右どちらにあるか判定
            float side = CheckMouseSide(ballPosition, direction, mousePosition);

            // 移動方向の回転
            if (side > 0) {
                direction = RotateVector(direction, turnAngle); // 右に回転
            }
            else if (side < 0) {
                direction = RotateVector(direction, -turnAngle); // 左に回転
            }
            previousMousePos.x = mousePosition.x;
            previousMousePos.y = mousePosition.y;

        // ボールの移動
        ballPosition.x += direction.x * ballSpeed;
        ballPosition.y += direction.y * ballSpeed;

        // 描画
        BeginDrawing();
        ClearBackground(Color{10,10,30,255});

        DrawCircleV(ballPosition, ballRadius, Color{ 10,255,30,255 });

        //if (side > 0) {
        //    DrawText("Mouse is on the RIGHT side", 10, 30, 20, DARKGRAY);
        //}
        //else if (side < 0) {
        //    DrawText("Mouse is on the LEFT side", 10, 30, 20, DARKGRAY);
        //}
        //else {
        //    DrawText("Mouse is on the LINE", 10, 30, 20, DARKGRAY);
        //}

        //DrawText("Move the mouse to guide the snake's head", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    // ウィンドウの終了処理
    CloseWindow();

    return 0;
}
