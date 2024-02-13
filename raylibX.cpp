#include "raylibX.hpp"
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include <stdio.h>

bool CheckCollision(Rectangle rec1, float angle, Rectangle rec2){
	bool res = false;
	Vector2 points[4] = {
		{0, 0},
		{rec1.width, 0},
		{rec1.width, rec1.height},
		{0, rec1.height}
	};
	Vector2 points2[4] = {
		{rec2.x, rec2.y},
		{rec2.x+rec2.width, rec2.y},
		{rec2.x+rec2.width, rec2.y+rec2.height},
		{rec2.x, rec2.y+rec2.height}
	};

	for(int i = 0; i < 4; i++){
		points[i] = Vector2Rotate(points[i], angle*DEG2RAD);
		points[i] = Vector2Add(points[i], Vector2{rec1.x, rec1.y});
		res = res || CheckCollisionPointRec(points[i], rec2);
	}
	for(int i = 0; i < 4; i++){
		res = res || CheckCollisionPointTriangle(points2[i], points[2], points[3], points[0]);
		res = res || CheckCollisionPointTriangle(points2[i], points[0], points[1], points[2]);
	}

	return res;
}

bool CheckCollision(Rectangle rec1, float angle1, Rectangle rec2, float angle2){
	bool res = false;
	Vector2 points[4] = {
		{0, 0},
		{rec1.width, 0},
		{rec1.width, rec1.height},
		{0, rec1.height}
	};
	for(int i = 0; i < 4; i++){
		points[i] = Vector2Rotate(points[i], angle1*DEG2RAD);
		points[i] = Vector2Add(points[i], Vector2{rec1.x, rec1.y});
	}
	Vector2 points2[4] = {
		{0, 0},
		{rec2.width, 0},
		{rec2.width, rec2.height},
		{0, rec2.height}
	};
	for(int i = 0; i < 4; i++){
		points2[i] = Vector2Rotate(points2[i], angle2*DEG2RAD);
		points2[i] = Vector2Add(points2[i], Vector2{rec2.x, rec2.y});
	}
	for(int i = 0; i < 4; i++){
		res = res || CheckCollisionPointTriangle(points[i], points2[2], points2[3], points2[0]);
		res = res || CheckCollisionPointTriangle(points[i], points2[0], points2[1], points2[2]);
	}
	for(int i = 0; i < 4; i++){
		res = res || CheckCollisionPointTriangle(points2[i], points[2], points[3], points[0]);
		res = res || CheckCollisionPointTriangle(points2[i], points[0], points[1], points[2]);
	}

	return res;
}

bool CheckCollisionByLines(Rectangle rec1, float angle1, Rectangle rec2, float angle2){
	bool res = false;
	Vector2 points[4] = {
		{0, 0},
		{rec1.width, 0},
		{rec1.width, rec1.height},
		{0, rec1.height}
	};
	for(int i = 0; i < 4; i++){
		points[i] = Vector2Rotate(points[i], angle1*DEG2RAD);
		points[i] = Vector2Add(points[i], Vector2{rec1.x, rec1.y});
	}
	Vector2 points2[4] = {
		{0, 0},
		{rec2.width, 0},
		{rec2.width, rec2.height},
		{0, rec2.height}
	};
	for(int i = 0; i < 4; i++){
		points2[i] = Vector2Rotate(points2[i], angle2*DEG2RAD);
		points2[i] = Vector2Add(points2[i], Vector2{rec2.x, rec2.y});
	}

	Vector2 temp;
	res = res || CheckCollisionLines(points[0], points[1], points2[0], points2[1], &temp);
	res = res || CheckCollisionLines(points[0], points[1], points2[1], points2[2], &temp);
	res = res || CheckCollisionLines(points[0], points[1], points2[2], points2[3], &temp);
	res = res || CheckCollisionLines(points[0], points[1], points2[3], points2[0], &temp);
	res = res || CheckCollisionLines(points[1], points[2], points2[0], points2[1], &temp);
	res = res || CheckCollisionLines(points[1], points[2], points2[1], points2[2], &temp);
	res = res || CheckCollisionLines(points[1], points[2], points2[2], points2[3], &temp);
	res = res || CheckCollisionLines(points[1], points[2], points2[3], points2[0], &temp);
	res = res || CheckCollisionLines(points[2], points[3], points2[0], points2[1], &temp);
	res = res || CheckCollisionLines(points[2], points[3], points2[1], points2[2], &temp);
	res = res || CheckCollisionLines(points[2], points[3], points2[2], points2[3], &temp);
	res = res || CheckCollisionLines(points[2], points[3], points2[3], points2[0], &temp);
	res = res || CheckCollisionLines(points[3], points[0], points2[0], points2[1], &temp);
	res = res || CheckCollisionLines(points[3], points[0], points2[1], points2[2], &temp);
	res = res || CheckCollisionLines(points[3], points[0], points2[2], points2[3], &temp);
	res = res || CheckCollisionLines(points[3], points[0], points2[3], points2[0], &temp);

	return res;
}

bool CheckCollision(Rectangle rec1, float degree1, float scale1, Vector2 pivot1, Rectangle rec2){

	bool res = false;
	Vector2 points[4] = {
		{0, 0},
		{rec1.width, 0},
		{rec1.width, rec1.height},
		{0, rec1.height}
	};
	Vector2 points2[4] = {
		{rec2.x, rec2.y},
		{rec2.x+rec2.width, rec2.y},
		{rec2.x+rec2.width, rec2.y+rec2.height},
		{rec2.x, rec2.y+rec2.height}
	};

	for(int i = 0; i < 4; i++){
		points[i] = Vector2Subtract(points[i], pivot1);
		points[i] = Vector2Scale(points[i], scale1);
		points[i] = Vector2Rotate(points[i], degree1*DEG2RAD);
		points[i] = Vector2Add(points[i], Vector2{rec1.x, rec1.y});
		res = res || CheckCollisionPointRec(points[i], rec2);
	}
	for(int i = 0; i < 4; i++){
		res = res || CheckCollisionPointTriangle(points2[i], points[2], points[3], points[0]);
		res = res || CheckCollisionPointTriangle(points2[i], points[0], points[1], points[2]);
	}

	return res;
}

bool CheckCollision(Rectangle rec1, float degree1, float scale1, Vector2 pivot1, Rectangle rec2, float degree2, float scale2, Vector2 pivot2){
	bool res = false;
	Vector2 points[4] = {
		{0, 0},
		{rec1.width, 0},
		{rec1.width, rec1.height},
		{0, rec1.height}
	};
	for(int i = 0; i < 4; i++){
		points[i] = Vector2Subtract(points[i], pivot1);
		points[i] = Vector2Scale(points[i], scale1);
		points[i] = Vector2Rotate(points[i], degree1*DEG2RAD);
		points[i] = Vector2Add(points[i], Vector2{rec1.x, rec1.y});
	}
	Vector2 points2[4] = {
		{0, 0},
		{rec2.width, 0},
		{rec2.width, rec2.height},
		{0, rec2.height}
	};
	for(int i = 0; i < 4; i++){
		points2[i] = Vector2Subtract(points2[i], pivot2);
		points2[i] = Vector2Scale(points2[i], scale2);
		points2[i] = Vector2Rotate(points2[i], degree2*DEG2RAD);
		points2[i] = Vector2Add(points2[i], Vector2{rec2.x, rec2.y});
	}
	for(int i = 0; i < 4; i++){
		res = res || CheckCollisionPointTriangle(points[i], points2[2], points2[3], points2[0]);
		res = res || CheckCollisionPointTriangle(points[i], points2[0], points2[1], points2[2]);
	}
	for(int i = 0; i < 4; i++){
		res = res || CheckCollisionPointTriangle(points2[i], points[2], points[3], points[0]);
		res = res || CheckCollisionPointTriangle(points2[i], points[0], points[1], points[2]);
	}

	return res;
}

bool CheckCollisionByLines(Rectangle rec1, float degree1, float scale1, Vector2 pivot1, Rectangle rec2, float degree2, float scale2, Vector2 pivot2){

	bool res = false;
	
	Vector2 points[4] = {
		{0, 0},
		{rec1.width, 0},
		{rec1.width, rec1.height},
		{0, rec1.height}
	};
	for(int i = 0; i < 4; i++){
		points[i] = Vector2Subtract(points[i], pivot1);
		points[i] = Vector2Scale(points[i], scale1);
		points[i] = Vector2Rotate(points[i], degree1*DEG2RAD);
		points[i] = Vector2Add(points[i], Vector2{rec1.x, rec1.y});
	}
	Vector2 points2[4] = {
		{0, 0},
		{rec2.width, 0},
		{rec2.width, rec2.height},
		{0, rec2.height}
	};
	for(int i = 0; i < 4; i++){
		points2[i] = Vector2Subtract(points2[i], pivot2);
		points2[i] = Vector2Scale(points2[i], scale2);
		points2[i] = Vector2Rotate(points2[i], degree2*DEG2RAD);
		points2[i] = Vector2Add(points2[i], Vector2{rec2.x, rec2.y});
	}

	Vector2 temp;
	res = res || CheckCollisionLines(points[0], points[1], points2[0], points2[1], &temp);
	res = res || CheckCollisionLines(points[0], points[1], points2[1], points2[2], &temp);
	res = res || CheckCollisionLines(points[0], points[1], points2[2], points2[3], &temp);
	res = res || CheckCollisionLines(points[0], points[1], points2[3], points2[0], &temp);
	res = res || CheckCollisionLines(points[1], points[2], points2[0], points2[1], &temp);
	res = res || CheckCollisionLines(points[1], points[2], points2[1], points2[2], &temp);
	res = res || CheckCollisionLines(points[1], points[2], points2[2], points2[3], &temp);
	res = res || CheckCollisionLines(points[1], points[2], points2[3], points2[0], &temp);
	res = res || CheckCollisionLines(points[2], points[3], points2[0], points2[1], &temp);
	res = res || CheckCollisionLines(points[2], points[3], points2[1], points2[2], &temp);
	res = res || CheckCollisionLines(points[2], points[3], points2[2], points2[3], &temp);
	res = res || CheckCollisionLines(points[2], points[3], points2[3], points2[0], &temp);
	res = res || CheckCollisionLines(points[3], points[0], points2[0], points2[1], &temp);
	res = res || CheckCollisionLines(points[3], points[0], points2[1], points2[2], &temp);
	res = res || CheckCollisionLines(points[3], points[0], points2[2], points2[3], &temp);
	res = res || CheckCollisionLines(points[3], points[0], points2[3], points2[0], &temp);

	return res;
}

void DrawRectangle(Rectangle rec, float degree, float scale, Vector2 pivot, Color color){

	Vector2 points[4] = {
		{0, 0},
		{rec.width, 0},
		{rec.width, rec.height},
		{0, rec.height}
	};
	for(int i = 0; i < 4; i++){
		points[i] = Vector2Subtract(points[i], pivot);
		points[i] = Vector2Scale(points[i], scale);
		points[i] = Vector2Rotate(points[i], degree*DEG2RAD);
		points[i] = Vector2Add(points[i], Vector2{rec.x, rec.y});
	}

	DrawTriangleLines(points[0], points[1], points[2],color);
	DrawTriangleLines(points[2], points[3], points[0],color);
}

void DrawRectangle(Rectangle rec, float degree, float scale, Color color){

	Vector2 points[4] = {
		{0, 0},
		{rec.width, 0},
		{rec.width, rec.height},
		{0, rec.height}
	};
	for(int i = 0; i < 4; i++){
		points[i] = Vector2Scale(points[i], scale);
		points[i] = Vector2Rotate(points[i], degree*DEG2RAD);
		points[i] = Vector2Add(points[i], Vector2{rec.x, rec.y});
	}

	DrawTriangleLines(points[0], points[1], points[2],color);
	DrawTriangleLines(points[2], points[3], points[0],color);
}

void DrawTexture(Texture2D txr_flappy, Rectangle srcRec, Vector2 position, float rotation, float scale, Color tint){
	DrawTexturePro(txr_flappy, srcRec, Rectangle{position.x, position.y, srcRec.width*scale, srcRec.height*scale}, Vector2{0, 0}, rotation, tint);
}

void DrawTexture(Texture2D texture, Rectangle texCoord, Rectangle rec, float rotation, float scale, Color tint){
	rec.width *= scale;
	rec.height *= scale;
	DrawTexturePro(texture, texCoord, rec, Vector2{0, 0}, rotation, tint);
}

void DrawTexture(Texture2D texture, Rectangle texCoord, Rectangle rec, float degree, float scale, Vector2 pivot, Color tint){
	Vector2 t;
	t.x = -pivot.x;
	t.y = -pivot.y;
	t.x *= scale;
	t.y *= scale;
	t = Vector2Rotate(t, DEG2RAD*degree);
	rec.x += t.x;
	rec.y += t.y;
	rec.width *= scale;
	rec.height *= scale;
	DrawTexturePro(texture, texCoord, rec, Vector2{0, 0}, degree, tint);
}