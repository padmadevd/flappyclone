#ifndef RAYLIBX_H
#define RAYLIBX_H

#include <raylib/raylib.h>
#include <raylib/raymath.h>

bool CheckCollision(Rectangle rec1, float degree1, Rectangle rec2);
bool CheckCollision(Rectangle rec1, float degree1, Rectangle rec2, float degree2);
bool CheckCollisionByLines(Rectangle rec1, float angle1, Rectangle rec2, float degree2);

bool CheckCollision(Rectangle rec1, float degree1, float scale1, Vector2 pivot1, Rectangle rec2);
bool CheckCollision(Rectangle rec1, float degree1, float scale1, Vector2 pivot1, Rectangle rec2, float degree2, float scale2, Vector2 pivot2);
bool CheckCollisionByLines(Rectangle rec1, float degree1, float scale1, Vector2 pivot1, Rectangle rec2, float degree2, float scale2, Vector2 pivot2);

void DrawRectangle(Rectangle rec, float degree, float scale, Vector2 pivot, Color color);
void DrawRectangle(Rectangle rec, float degree, float scale, Color color);

void DrawTexture(Texture2D texture, Rectangle texCoord, Vector2 position, float rotation, float scale, Color tint);
// void DrawTexture(Texture2D texture, Rectangle texCoord, Vector2 position, float rotation, float scale, Vector2 pivot, Color tint);
void DrawTexture(Texture2D texture, Rectangle texCoord, Rectangle rec, float rotation, float scale, Color tint);
void DrawTexture(Texture2D texture, Rectangle texCoord, Rectangle rec, float rotation, float scale, Vector2 pivot, Color tint);

#endif