#include "raylib.h"
#include <cmath>

const int screenWidth = 800;
const int screenHeight = 600;
const int ballRadius = 10;
const float ballSpeed = 5.0f;
const float turnAngle = 360.0f / 24.0f * (PI / 180.0f); // 360�x/24�����W�A���ɕϊ�

// �X�l�[�N�̓��̏����ʒu
Vector2 ballPosition = { screenWidth / 2.0f, screenHeight / 2.0f };

// �{�[���̈ړ������x�N�g���ɑ΂��ă}�E�X�J�[�\�������E�ǂ���ɂ��邩�𔻒肷��֐�
// �Ԃ�l: ���̏ꍇ�͉E�A���̏ꍇ�͍��A0�̏ꍇ�͓��꒼����
float CheckMouseSide(Vector2 ballPosition, Vector2 direction, Vector2 mousePosition) {
    Vector2 toMouse = { mousePosition.x - ballPosition.x, mousePosition.y - ballPosition.y };
    return direction.x * toMouse.y - direction.y * toMouse.x;
}

// �ړ��������p�x�ŉ�]������֐�
Vector2 RotateVector(Vector2 vector, float angle) {
    float cosA = cos(angle);
    float sinA = sin(angle);
    return { vector.x * cosA - vector.y * sinA, vector.x * sinA + vector.y * cosA };
}

int main() {
    // �E�B���h�E�̏�����
    InitWindow(screenWidth, screenHeight, "Snake Game - Moving Head");

    // �t���[�����[�g�̐ݒ�
    SetTargetFPS(60);

    // �����̈ړ������i�E�����j
    Vector2 direction = { 1.0f, 0.0f };

    // �Q�[�����[�v
    while (!WindowShouldClose()) {
        // �}�E�X�ʒu�̎擾
        Vector2 mousePosition = GetMousePosition();

        // �}�E�X���{�[���̈ړ������ɑ΂��č��E�ǂ���ɂ��邩����
        float side = CheckMouseSide(ballPosition, direction, mousePosition);

        // �ړ������̉�]
        if (side > 0) {
            direction = RotateVector(direction, turnAngle); // �E�ɉ�]
        }
        else if (side < 0) {
            direction = RotateVector(direction, -turnAngle); // ���ɉ�]
        }

        // �{�[���̈ړ�
        ballPosition.x += direction.x * ballSpeed;
        ballPosition.y += direction.y * ballSpeed;

        // �`��
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawCircleV(ballPosition, ballRadius, DARKGREEN);

        if (side > 0) {
            DrawText("Mouse is on the RIGHT side", 10, 30, 20, DARKGRAY);
        }
        else if (side < 0) {
            DrawText("Mouse is on the LEFT side", 10, 30, 20, DARKGRAY);
        }
        else {
            DrawText("Mouse is on the LINE", 10, 30, 20, DARKGRAY);
        }

        DrawText("Move the mouse to guide the snake's head", 10, 10, 20, DARKGRAY);

        EndDrawing();
    }

    // �E�B���h�E�̏I������
    CloseWindow();

    return 0;
}
