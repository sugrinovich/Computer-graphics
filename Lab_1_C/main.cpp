#include "raylib.h"
#include <vector>
#include <cmath>
#include <algorithm>

struct Point2f
{
    float x;
    float y;
};

const float PI_F = 3.14159265358979323846f;

// --------------------
// Математика
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

std::vector<Point2f> CreateStar(Point2f center, float outerR, float innerR, int rays)
{
    std::vector<Point2f> pts;
    pts.reserve(rays * 2);

    for (int i = 0; i < rays * 2; ++i)
    {
        float angle = -PI_F / 2.0f + i * PI_F / rays;
        float r = (i % 2 == 0) ? outerR : innerR;

        pts.push_back({
            center.x + r * cosf(angle),
            center.y + r * sinf(angle)
        });
    }

    return pts;
}

// --------------------
// Брезенхем (для исходного контура)
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

void DrawPolygonContourBresenham(const std::vector<Point2f>& pts, Color color)
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

// --------------------
// Алгоритм Ву
// --------------------
float Clamp01(float x)
{
    if (x < 0.0f) return 0.0f;
    if (x > 1.0f) return 1.0f;
    return x;
}

int IPart(float x)
{
    return (int)floorf(x);
}

float FPart(float x)
{
    return x - floorf(x);
}

float RFPart(float x)
{
    return 1.0f - FPart(x);
}

void PlotAA(int x, int y, float brightness, Color color)
{
    if (x < 0 || y < 0 || x >= GetScreenWidth() || y >= GetScreenHeight())
        return;

    DrawPixel(x, y, ColorAlpha(color, Clamp01(brightness)));
}

void DrawLineWu(float x0, float y0, float x1, float y1, Color color)
{
    bool steep = fabsf(y1 - y0) > fabsf(x1 - x0);

    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    float dx = x1 - x0;
    float dy = y1 - y0;

    if (fabsf(dx) < 1e-6f)
    {
        if (steep) DrawPixel((int)roundf(y0), (int)roundf(x0), color);
        else       DrawPixel((int)roundf(x0), (int)roundf(y0), color);
        return;
    }

    float gradient = dy / dx;

    float xEnd = roundf(x0);
    float yEnd = y0 + gradient * (xEnd - x0);
    float xGap = RFPart(x0 + 0.5f);

    int xpxl1 = (int)xEnd;
    int ypxl1 = IPart(yEnd);

    if (steep)
    {
        PlotAA(ypxl1,     xpxl1, RFPart(yEnd) * xGap, color);
        PlotAA(ypxl1 + 1, xpxl1, FPart(yEnd)  * xGap, color);
    }
    else
    {
        PlotAA(xpxl1, ypxl1,     RFPart(yEnd) * xGap, color);
        PlotAA(xpxl1, ypxl1 + 1, FPart(yEnd)  * xGap, color);
    }

    float intery = yEnd + gradient;

    xEnd = roundf(x1);
    yEnd = y1 + gradient * (xEnd - x1);
    xGap = FPart(x1 + 0.5f);

    int xpxl2 = (int)xEnd;
    int ypxl2 = IPart(yEnd);

    if (steep)
    {
        PlotAA(ypxl2,     xpxl2, RFPart(yEnd) * xGap, color);
        PlotAA(ypxl2 + 1, xpxl2, FPart(yEnd)  * xGap, color);
    }
    else
    {
        PlotAA(xpxl2, ypxl2,     RFPart(yEnd) * xGap, color);
        PlotAA(xpxl2, ypxl2 + 1, FPart(yEnd)  * xGap, color);
    }

    if (steep)
    {
        for (int x = xpxl1 + 1; x < xpxl2; ++x)
        {
            PlotAA(IPart(intery),     x, RFPart(intery), color);
            PlotAA(IPart(intery) + 1, x, FPart(intery),  color);
            intery += gradient;
        }
    }
    else
    {
        for (int x = xpxl1 + 1; x < xpxl2; ++x)
        {
            PlotAA(x, IPart(intery),     RFPart(intery), color);
            PlotAA(x, IPart(intery) + 1, FPart(intery),  color);
            intery += gradient;
        }
    }
}

void DrawPolygonContourWu(const std::vector<Point2f>& pts, Color color)
{
    if (pts.size() < 2) return;

    for (size_t i = 0; i + 1 < pts.size(); ++i)
    {
        DrawLineWu(pts[i].x, pts[i].y, pts[i + 1].x, pts[i + 1].y, color);
    }

    DrawLineWu(pts.back().x, pts.back().y, pts.front().x, pts.front().y, color);
}

int main()
{
    const int screenWidth = 1100;
    const int screenHeight = 750;

    InitWindow(screenWidth, screenHeight, "Lab 1 - Task V");
    SetTargetFPS(60);

    std::vector<Point2f> star = CreateStar({550.0f, 380.0f}, 170.0f, 75.0f, 5);

    Point2f pivot = {550.0f, 380.0f};
    float angle = 0.0f;
    float speed = 60.0f; // градусов в секунду
    bool animate = true;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_SPACE))
            animate = !animate;

        if (IsKeyDown(KEY_UP))
            speed += 40.0f * GetFrameTime();

        if (IsKeyDown(KEY_DOWN))
            speed -= 40.0f * GetFrameTime();

        if (speed < 0.0f) speed = 0.0f;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Vector2 mp = GetMousePosition();
            pivot = { mp.x, mp.y };
        }

        if (animate)
            angle += speed * GetFrameTime();

        std::vector<Point2f> rotatedStar = RotatePolygon(star, pivot, angle);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Task V: animated rotation of a complex polygon", 20, 20, 28, BLACK);
        DrawText("Figure: star", 20, 60, 22, DARKGRAY);
        DrawText("SPACE - pause/run | UP/DOWN - speed | LMB - set pivot", 20, 90, 22, DARKGRAY);
        DrawText("Gray - original contour (Bresenham)", 20, 125, 20, GRAY);
        DrawText("Red - rotated contour with antialiasing (Wu)", 20, 150, 20, RED);

        DrawPolygonContourBresenham(star, LIGHTGRAY);
        DrawPolygonContourWu(rotatedStar, RED);

        DrawCircle((int)pivot.x, (int)pivot.y, 5, BLUE);
        DrawCircleLines((int)pivot.x, (int)pivot.y, 10, BLUE);

        DrawText(TextFormat("Angle: %.2f deg", angle), 20, 190, 24, MAROON);
        DrawText(TextFormat("Speed: %.2f deg/sec", speed), 20, 225, 24, DARKBLUE);
        DrawText(animate ? "Animation: ON" : "Animation: PAUSE", 20, 260, 24, DARKGREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}