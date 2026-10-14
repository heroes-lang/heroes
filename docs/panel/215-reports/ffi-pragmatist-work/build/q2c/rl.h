/* the declarations raylib.h makes that windows.h also makes (raylib's own
   documented clash: Rectangle, CloseWindow, ShowCursor, DrawText) */
typedef struct Rectangle { float x; float y; float width; float height; } Rectangle;
void CloseWindow(void);
void ShowCursor(void);
void DrawTextA(const char *text, int x, int y, int size, int color);
