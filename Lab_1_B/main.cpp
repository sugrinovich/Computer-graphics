#include "raylib.h"
#include <vector>
#include <cmath>

struct Point2f
{
    float x;
    float y;
};

const float PI_F = 3.14159265358979323846f;

// --------------------
// Поворот точки
// --------------------
Point2f RotatePoint(Point2f p, Point2f center, float angleDeg)
{
    float a = angleDeg * PI_F / 180.0f;
    float c = cosf(a);
    float s = sinf(a);

    float dx = p.x - center.x;
    float dy = p.y - center.y;

    return {
        center.x + dx * c - dy * s,
        center.y + dx * s + dy * c
    };
}

std::vector<Point2f> RotatePolygon(const std::vector<Point2f>& pts, Point2f center, float angleDeg)
{
    std::vector<Point2f> result;
    result.reserve(pts.size());

    for (const auto& p : pts)
        result.push_back(RotatePoint(p, center, angleDeg));

    return result;
}

// --------------------
// Брезенхем
// --------------------
void DrawLineBresenham(int x0, int y0, int x1, int y1, Color color)
{
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true)
    {
        DrawPixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) break;

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

void DrawPolygonContour(const std::vector<Point2f>& pts, Color color)
{
    if (pts.size() < 2) return;

    for (size_t i = 0; i + 1 < pts.size(); ++i)
    {
        DrawLineBresenham(
            (int)roundf(pts[i].x),     (int)roundf(pts[i].y),
            (int)roundf(pts[i + 1].x), (int)roundf(pts[i + 1].y),
            color
        );
    }

    DrawLineBresenham(
        (int)roundf(pts.back().x),   (int)roundf(pts.back().y),
        (int)roundf(pts.front().x),  (int)roundf(pts.front().y),
        color
    );
}

int main()
{
    const int screenWidth = 1000;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "Lab 1 - Task B");
    SetTargetFPS(60);

    std::vector<Point2f> triangle = {
        {350, 220},
        {270, 470},
        {500, 430}
    };

    std::vector<Point2f> rectangle = {
        {320, 220},
        {620, 220},
        {620, 460},
        {320, 460}
    };

    bool useRectangle = false;
    float angle = 0.0f;
    Point2f pivot = { 500.0f, 350.0f };

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_T)) useRectangle = false;
        if (IsKeyPressed(KEY_R)) useRectangle = true;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mp = GetMousePosition();
            pivot = { mp.x, mp.y };
        }

        if (IsKeyDown(KEY_Q)) angle -= 90.0f * GetFrameTime();
        if (IsKeyDown(KEY_E)) angle += 90.0f * GetFrameTime();

        angle += GetMouseWheelMove() * 5.0f;

        const std::vector<Point2f>& baseShape = useRectangle ? rectangle : triangle;
        std::vector<Point2f> rotatedShape = RotatePolygon(baseShape, pivot, angle);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Task B: rotation of a simple figure contour", 20, 20, 28, BLACK);
        DrawText("T - triangle | R - rectangle", 20, 60, 22, DARKGRAY);
        DrawText("LMB - set pivot | Q/E or mouse wheel - rotate", 20, 90, 22, DARKGRAY);

        // Исходная фигура
        DrawPolygonContour(baseShape, LIGHTGRAY);

        // Повернутая фигура
        DrawPolygonContour(rotatedShape, RED);

        // Центр поворота
        DrawCircle((int)pivot.x, (int)pivot.y, 5, BLUE);
        DrawCircleLines((int)pivot.x, (int)pivot.y, 10, BLUE);

        DrawText(TextFormat("Figure: %s", useRectangle ? "rectangle" : "triangle"), 20, 130, 24, DARKBLUE);
        DrawText(TextFormat("Angle: %.2f deg", angle), 20, 165, 24, MAROON);

        DrawText("Gray - original contour", 20, 205, 20, GRAY);
        DrawText("Red - rotated contour", 20, 230, 20, RED);
        DrawText("Blue - pivot", 20, 255, 20, BLUE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}