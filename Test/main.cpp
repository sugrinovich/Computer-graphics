// Подключаем библиотеку Raylib
#include "raylib.h"
#include <cmath>

// Главная функция программы
int main() {
    // --- Настройки окна ---
    const int screenWidth = 800;
    const int screenHeight = 600;

    // Создаём окно с заголовком
    InitWindow(screenWidth, screenHeight, "Raylib Test Drive");

    // Устанавливаем частоту кадров (60 FPS)
    SetTargetFPS(60);

    // --- Инициализация переменных для анимации ---
    // Позиция и скорость мячика (вектор)
    Vector2 ballPosition = { (float)screenWidth / 2, (float)screenHeight / 2 };
    Vector2 ballSpeed = { 2.5f, 2.0f };
    float ballRadius = 20.0f;

    // Цвет фона (можно менять в процессе)
    Color backgroundColor = BLACK;
    // Счётчик кадров для демонстрации
    int frameCounter = 0;

    // --- Главный цикл ---
    // Выполняется, пока окно открыто (пока не нажали крестик)
    while (!WindowShouldClose()) {
        // --- Обновление логики (ввод, анимация) ---

        // Увеличиваем счётчик кадров
        frameCounter++;

        // Обработка нажатий клавиш
        if (IsKeyPressed(KEY_SPACE)) {
            // При нажатии пробела меняем цвет фона случайным образом
            backgroundColor.r = GetRandomValue(0, 255);
            backgroundColor.g = GetRandomValue(0, 255);
            backgroundColor.b = GetRandomValue(0, 255);
        }

        // Обработка нажатий мыши
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            // При клике левой кнопкой мыши получаем позицию курсора
            Vector2 mousePos = GetMousePosition();
            // Можно вывести в консоль (отладка)
            TraceLog(LOG_INFO, "Mouse clicked at (%.0f, %.0f)", mousePos.x, mousePos.y);
        }

        // Анимация мячика: движение и отскок от стенок
        ballPosition.x += ballSpeed.x;
        ballPosition.y += ballSpeed.y;

        // Проверка столкновения с границами окна
        if (ballPosition.x + ballRadius >= screenWidth || ballPosition.x - ballRadius <= 0) {
            ballSpeed.x *= -1; // Меняем направление по X
        }
        if (ballPosition.y + ballRadius >= screenHeight || ballPosition.y - ballRadius <= 0) {
            ballSpeed.y *= -1; // Меняем направление по Y
        }

        // --- Рисование ---
        // Начинаем рисование в текущий кадр
        BeginDrawing();

        // Очищаем экран текущим цветом фона
        ClearBackground(backgroundColor);

        // --- Рисуем различные примитивы ---

        // 1. Линия от верхнего левого угла до нижнего правого
        DrawLine(0, 0, screenWidth, screenHeight, RED);

        // 2. Круг (мячик) с изменяющимся цветом (радужный эффект)
        Color ballColor = {
            (unsigned char)((sin(frameCounter * 0.01) + 1) * 127.5), // красный меняется по синусу
            (unsigned char)((sin(frameCounter * 0.02) + 1) * 127.5), // зелёный
            (unsigned char)((sin(frameCounter * 0.03) + 1) * 127.5), // синий
            255
        };
        DrawCircleV(ballPosition, ballRadius, ballColor);

        // 3. Прямоугольник в левом верхнем углу (полупрозрачный)
        DrawRectangle(50, 50, 150, 100, {0, 255, 0, 128}); // RGBA: зелёный, полупрозрачный

        // 4. Контур прямоугольника
        DrawRectangleLines(50, 200, 150, 100, BLUE);

        // 5. Треугольник (закрашенный)
        DrawTriangle(
            {100, 400},  // вершина A
            {200, 350},  // вершина B
            {150, 450},  // вершина C
            YELLOW
        );

        // 6. Текст с полезной информацией
        DrawText("=== Raylib Test ===", 10, 10, 20, LIGHTGRAY);

        // Текст с координатами мыши
        Vector2 mouse = GetMousePosition();
        DrawText(TextFormat("Mouse: (%.0f, %.0f)", mouse.x, mouse.y), 10, 40, 20, LIGHTGRAY);

        // Текст с FPS (встроенная переменная)
        DrawText(TextFormat("FPS: %i", GetFPS()), 10, 70, 20, LIGHTGRAY);

        // Текст с номером кадра
        DrawText(TextFormat("Frame: %i", frameCounter), 10, 100, 20, LIGHTGRAY);

        // Подсказка по управлению
        DrawText("Press SPACE to change background color", 10, 130, 20, LIGHTGRAY);
        DrawText("Click mouse left button - see console output", 10, 160, 20, LIGHTGRAY);

        // Завершаем рисование кадра
        EndDrawing();
    }

    // --- Завершение работы ---
    // Закрываем окно и освобождаем ресурсы
    CloseWindow();

    return 0;
}