#include "raylib.h"
#include "rcamera.h"
#include <vector>
#include <iostream>
#include <cmath>

float getRandom(float min, float max)
{
    float r = float(rand()) / float(RAND_MAX);
    return r * (max - min) + min;
}
Vector3 gridToWorld(int i, int j, int slices, float spacing)
{
    float radius = slices >> 1;
    return Vector3{-radius + spacing * (j + 0.5f), 0, -radius + spacing * (i + 0.5f)};
}
Vector3 operator+(Vector3 a, Vector3 b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}
Vector3 operator*(Vector3 a, float k)
{
    return {k * a.x, k * a.y, k * a.z};
}
float getDist(Vector3 a, Vector3 b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
Vector3 getGradient(Vector3 point)
{
    Vector3 grad{};

    grad.x = 2 * point.x;
    grad.z = 2 * point.z;
    float norm = getDist(grad, {});

    grad.x /= norm;
    grad.y /= norm;
    grad.z /= norm;

    return grad;
}
int main()
{

    const int screenWidth = 1280;
    const int screenHeight = 720;
    srand(time(nullptr));
    InitWindow(screenWidth, screenHeight, "Vector field");

    Camera camera{};
    camera.position   = {0.0f, 2.0f, 4.0f};
    camera.target     = {0.0f, 2.0f, 0.0f};
    camera.up         = {0.0f, 1.0f, 0.0f};
    camera.fovy       = 90.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    Vector3 origin{};

    SetTargetFPS(144);
    std::vector<Vector3> points;
    int gridSlices = 50;
    float spacing  = 1.0f;

    for (int i = 0; i < gridSlices; i++)
    {
        for (int j = 0; j < gridSlices; j++)
        {
            points.push_back(gridToWorld(i, j, gridSlices, spacing));
        }
    }

    DisableCursor();
    
    Vector3 particle = gridToWorld(getRandom(0, gridSlices - 1), getRandom(0, gridSlices - 1), gridSlices, spacing);
    Vector3 vel {};
    while (!WindowShouldClose())
    {

        float dt = GetFrameTime();
        Vector3 cameraMovement{};

        cameraMovement.x = (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) * 0.1f - (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) * 0.1f;
        cameraMovement.y = (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) * 0.1f - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) * 0.1f;
        cameraMovement.z = IsKeyDown(KEY_SPACE) * 0.1f - IsKeyDown(KEY_LEFT_SHIFT) * 0.1f;

        Vector3 cameraRotation{GetMouseDelta().x * 0.05f, GetMouseDelta().y * 0.05f, 0.0f};
        UpdateCameraPro(&camera, cameraMovement, cameraRotation, GetMouseWheelMove() * 2.0f);

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(camera);

        DrawSphere(origin, 0.25, BLUE);
        DrawSphere(particle, 0.15f, RED);
        particle = particle + vel * dt;
        vel      = vel  + getGradient(particle) * -dt;
        for (auto point : points)
        {
            Vector3 endPoint = point + getGradient(point) * 0.5f;

            DrawLine3D(point, endPoint, YELLOW);
        }
        DrawGrid(gridSlices, 1.0f);
        EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
