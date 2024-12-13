#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <deque>

const int screenWidth = 800;
const int screenHeight = 600;
const int ballRadius = 5;
const float ballSpeed = 150.0f; // �{�[���̈ړ����x�i�s�N�Z�����b�j
const float turnAngle = 360.0f / 24.0f * (PI / 180.0f); // �Ȃ���p�x�̑���// ���W�A���ɕϊ�
const int foodCount = 10;
const int foodRadius = 5;
const int rockRadius = 5;
const int initialLives = 3;
const int initialBodySegments = 3;
const int scoreToGrow = 3;

// �X�l�[�N�̓��̏����ʒu
Vector2 headBallPos = { screenWidth / 3.0f, screenHeight / 3.0f };

// �G�T�̏����ʒu�������_���ɐ���
Vector2 foods[foodCount];

// ��̏����ʒu�������_���ɐ���
std::deque<Vector2> rocks;
int rockCount = 5;

// �X�R�A�ƃ��C�t
int score = 0;
int lives = initialLives;

// �X�l�[�N�̑�
std::deque<Vector2> snakeBody;
int segmentsToAdd = 0;

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

// �����_���Ȉʒu�ɃG�T�𐶐�����֐�
void GenerateRandomFood(Vector2& food) {
    food.x = GetRandomValue(foodRadius, screenWidth - foodRadius);
    food.y = GetRandomValue(foodRadius, screenHeight - foodRadius);
}

// �����_���Ȉʒu�Ɋ�𐶐�����֐�
void GenerateRandomRock(Vector2& rock) {
    rock.x = GetRandomValue(rockRadius, screenWidth - rockRadius);
    rock.y = GetRandomValue(rockRadius, screenHeight - rockRadius);
}

int main() {
    // �E�B���h�E�̏�����
    InitWindow(screenWidth, screenHeight, "Snake Game - Moving Head");

    // �����V�[�h�̏�����
    srand(time(NULL));

    // �G�T�̏�����
    for (int i = 0; i < foodCount; i++) {
        GenerateRandomFood(foods[i]);
    }

    // ��̏�����
    for (int i = 0; i < rockCount; i++) {
        Vector2 rock;
        GenerateRandomRock(rock);
        rocks.push_back(rock);
    }

    // �t���[�����[�g�̐ݒ�
    SetTargetFPS(60);

    // �����̈ړ������i�E�����j
    Vector2 direction = { 1.0f, 0.0f };

    // �X�l�[�N�̑̂̏�����
    for (int i = 0; i < initialBodySegments; i++) {
        snakeBody.push_back(headBallPos);
    }

    bool gameOver = false;

    // �Q�[�����[�v
    while (!WindowShouldClose()) {
        if (!gameOver) {

            // �}�E�X�ʒu�̎擾
            Vector2 mousePosition = GetMousePosition();

            // �}�E�X���{�[���̈ړ������ɑ΂��č��E�ǂ���ɂ��邩����
            float side = CheckMouseSide(headBallPos, direction, mousePosition);

            // �ړ������̉�]
            if (side > 0) {
                direction = RotateVector(direction, turnAngle); // �E�ɉ�]
            }
            else if (side < 0) {
                direction = RotateVector(direction, -turnAngle); // ���ɉ�]
            }

            // �{�[���̈ړ��i�t���[�����Ƃ̎��Ԃ��l�����đ��x�𒲐��j
            float deltaTime = GetFrameTime();
            headBallPos.x += direction.x * ballSpeed * deltaTime;
            headBallPos.y += direction.y * ballSpeed * deltaTime;

            // �X�l�[�N�̑̂��X�V
            snakeBody.push_front(headBallPos);
            while (snakeBody.size() > initialBodySegments + segmentsToAdd) {
                snakeBody.pop_back();
            }

            // �G�T�Ƃ̏Փ˔���
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

            // ��Ƃ̏Փ˔���
            for (auto& rock : rocks) {
                if (CheckCollisionCircles(headBallPos, ballRadius, rock, rockRadius)) {
                    lives--;
                    GenerateRandomRock(rock);
                }
            }

            // �Q�[���I�[�o�[����
            if (lives <= 0) {
                BeginDrawing();
                ClearBackground(BLACK);

                DrawText("GAME OVER", screenWidth / 2 - 100, screenHeight / 2 - 20, 40, RED);
                DrawText(TextFormat("Final Score: %d", score), screenWidth / 2 - 100, screenHeight / 2 + 30, 30, DARKGRAY);
                DrawText("Press R to Retry", screenWidth / 2 - 100, screenHeight / 2 + 70, 20, DARKGRAY);

                EndDrawing();

                // ���g���C����
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

        // �`��
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

    // �E�B���h�E�̏I������
    CloseWindow();

    return 0;
}
