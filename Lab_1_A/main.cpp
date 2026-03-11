#include "raylib.h"
#include <iostream>
#include <cmath>

struct Point
{
    int x;
    int y;
};

const float PI_F = 3.14159265358979323846f;

// Поворот точки p вокруг центра c на angleDeg градусов
Point RotatePoint(Point p, Point c, float angleDeg)
{
    float angleRad = angleDeg * PI_F / 180.0f;

    float cosA = std::cos(angleRad);
    float sinA = std::sin(angleRad);

    float dx = p.x - c.x;
    float dy = p.y - c.y;

    Point result;
    result.x = (int)std::round(c.x + dx * cosA - dy * sinA);
    result.y = (int)std::round(c.y + dx * sinA + dy * cosA);

    return result;
}

// Алгоритм Брезенхема
void DrawLineBresenham(int x0, int y0, int x1, int y1, Color color)
{
    int dx = std::abs(x1 - x0);
    int dy = std::abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true)
    {
        DrawPixel(x0, y0, color);

        if (x0 == x1 && y0 == y1)
            break;

        int e2 = 2 * err;

        if (e2 > -dy)
        {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx)
        {
            err += dx;
            y0 += sy;
        }
    }
}

int main()
{
    Point p1, p2, center;
    float angle;

    std::cout << "=== Задача A: поворот отрезка ===\n";
    std::cout << "Введите координаты начала отрезка (x1 y1): ";
    std::cin >> p1.x >> p1.y;

    std::cout << "Введите координаты конца отрезка (x2 y2): ";
    std::cin >> p2.x >> p2.y;

    std::cout << "Введите центр поворота (cx cy): ";
    std::cin >> center.x >> center.y;

    std::cout << "Введите угол поворота в градусах: ";
    std::cin >> angle;

    Point rp1 = RotatePoint(p1, center, angle);
    Point rp2 = RotatePoint(p2, center, angle);

    const int screenWidth = 1000;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "Task A - Rotate Line (Bresenham)");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Оси
        DrawLine(0, screenHeight / 2, screenWidth, screenHeight / 2, LIGHTGRAY);
        DrawLine(screenWidth / 2, 0, screenWidth / 2, screenHeight, LIGHTGRAY);

        // Исходный отрезок
        DrawLineBresenham(p1.x, p1.y, p2.x, p2.y, GRAY);

        // Повернутый отрезок
        DrawLineBresenham(rp1.x, rp1.y, rp2.x, rp2.y, RED);

        // Центр поворота
        DrawCircle(center.x, center.y, 5, BLUE);
        DrawCircleLines(center.x, center.y, 10, BLUE);

        DrawText("GRAY - original line", 20, 20, 20, DARKGRAY);
        DrawText("RED - rotated line", 20, 50, 20, MAROON);
        DrawText("BLUE - rotation center", 20, 80, 20, DARKBLUE);
        DrawText(TextFormat("Angle: %.2f degrees", angle), 20, 120, 22, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}