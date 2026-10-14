/* Route I's stand-in for `extern "raylib.h"` in examples/raylib/main.hero. */
typedef struct Color { uint8_t r; uint8_t g; uint8_t b; uint8_t a; } Color;
typedef struct Vector2 { float x; float y; } Vector2;
typedef struct Rectangle { float x; float y; float width; float height; } Rectangle;
typedef struct AutomationEvent { uint32_t frame; uint32_t type; int32_t params[4]; } AutomationEvent;
void SetRandomSeed(uint32_t a0);
int32_t GetRandomValue(int32_t a0, int32_t a1);
int32_t ColorToInt(Color a0);
Color ColorAlpha(Color a0, float a1);
Vector2 GetSplinePointLinear(Vector2 a0, Vector2 a1, float a2);
bool CheckCollisionPointRec(Vector2 a0, Rectangle a1);
