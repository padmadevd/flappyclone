#include <cstdlib>
#include <stdio.h>
#include <cmath>
#include <raylib/raylib.h>
#include <raylib/raymath.h>
#include <sys/types.h>
#include "GLFW/glfw3.h"
#include "raylibX.hpp"

// -------------------- STRUCT AND MACROS ----------------------------------

// Texture Co ordinates - Mapping different sub-textures in the txr_flappy
Rectangle tc[] = {
	//FLAPPY.PNG
	//sky_day
	{0, 0, 144, 256},
	//sky_dark
	{146, 0, 144, 256},
	//platform
	{292, 0, 168, 56},
	//menu
	{462, 26, 40, 14},
	//ok
	{462, 42, 40, 14},
	//game_over
	{395, 59, 96, 21},
	//get_ready
	{295, 59, 92, 25},
	//title
	{351, 91, 89, 24},
	//play
	{354, 118, 52, 29},
	//pause
	{121, 306, 13, 14},
	//pipe_green
	{84, 323, 26, 160},
	//pipe_red
	{0, 323, 26, 160},
	//bird_blue
	{87, 491, 17, 12}, //up
	{115, 329, 17, 12}, //mid
	{115, 355, 17, 12}, //down
	//bird_red
	{115, 381, 17, 12},	//up
	{115, 407, 17, 12},	//mid
	{115, 433, 17, 12},	//down
	//bird_yellow
	{3, 491, 17, 12}, //up
	{31, 491, 17, 12}, //mid
	{59, 491, 17, 12}, //down
	//number_big
	{496, 60, 12, 18},
	{134, 455, 12, 18},
	{292, 160, 12, 18},
	{306, 160, 12, 18},
	{320, 160, 12, 18},
	{334, 160, 12, 18},
	{292, 184, 12, 18},
	{306, 184, 12, 18},
	{320, 184, 12, 18},
	{334, 184, 12, 18},
	//number_small
	{137, 306, 7, 10},
	{138, 477, 7, 10},
	{137, 489, 7, 10},
	{131, 501, 7, 10},
	{502, 0, 7, 10},
	{502, 12, 7, 10},
	{505, 26, 7, 10},
	{505, 42, 7, 10},
	{293, 242, 7, 10},
	{311, 206, 7, 10},

	//KEY.PNG && KEY2.PNG
	//key_left
	{33, 2, 13, 12},
	//key_right
	{49, 2, 13, 12},
	//key_down
	{17, 2, 13, 12},
	//key_j
	{17, 50, 13, 12},
	//key_space
	{65, 34, 30, 12},
	//key_enter
	{98, 18, 28, 12}
};

// Texture Map Index
#define sky_day 0
#define sky_dark 1
#define platform 2
#define menu 3
#define ok 4
#define game_over 5
#define get_ready 6
#define title 7
#define play 8
#define pause 9
#define pipe_green 10
#define pipe_red 11

#define bird_blue 12
#define bird_red 15
#define bird_yellow 18
#define up 0
#define mid 1
#define down 2

#define number_big 21
#define number_small 31

#define key_left 41
#define key_right 42
#define key_down 43
#define key_j 44
#define key_space 45
#define key_enter 46

#define STATE_GAMEOVER 0
#define STATE_READY 1
#define STATE_RUN 2
#define STATE_PAUSE 3

#define SCALE 2

struct Box{
	Rectangle rec;
	float rotation;
	float scale;
	Vector2 pivot;
};


// ----------------- RESOURCE VARIABLES --------------------------

Texture2D txr_flappy;
Texture2D txr_key;
Texture2D txr_key2;
Font font;
Sound s_birdWing;
Sound s_birdDie;
Sound s_birdHit;
Sound s_point;
Sound s_level;

// ---------------------- GAME STATES -------------------------------

float screenWidth;
float screenHeight;

int gameState;

// --------------------- GAMEPLAY STATES ---------------------------

// bird properties
int bird = bird_red;
float birdFPS;
float birdFrame;
Box birdBox;

int sky = sky_day;
float skyScroll;
Box skyBox;

float platformScroll;
Box platformBox;

// pipe properties
Box pipeBox[6];
Vector2 pipePos[3];
int lastPipe;
int scorePipe = -1;

float gravity;
Vector2 velocity;
float rotVelocity;

float jumpVelocity;
float jumpRVelocity;

double lastKeyTime;
int keyCount = 0;

// GAME STATE PROPERTIES

bool showTitle;
bool fadeTitle;
float titleAlpha;

unsigned int score;

bool moveGameOver;
Vector2 gameOverPos;
float finalScore;
bool animFinalScore;
bool playDie;

void initGame();
void processFrame(double delta);
void drawFrame();
void drawInt(int n, int size, float scale, Vector2 position);
void drawInt(int n, int size, float scale, Vector2 position, Color color);
void drawInt(int n, int size);
float sigmoid(float x);

int main(){

	initGame();

	double FPS = 120;
	double SPF = 1/FPS;

	double lastTime = GetTime();
	double delta = 0;
	while (!WindowShouldClose()) {

		PollInputEvents();
        processFrame(delta);
        drawFrame();
		SwapScreenBuffer();

		delta = GetTime() - lastTime;
		lastTime = GetTime();
		if(SPF - delta > 0)
			WaitTime(SPF - delta);
	}
}

void initGame(){

	screenWidth = tc[sky_day].width*2;
	screenHeight = tc[sky_day].height*2;

	InitWindow(screenWidth, screenHeight, "flappybird");
	InitAudioDevice();

	// SetWindowState(FLAG_WINDOW_RESIZABLE);

	// LOADING RESOURCE
	txr_flappy = LoadTexture("assets/flappy.png");
	txr_key = LoadTexture("assets/key.png");
	txr_key2 = LoadTexture("assets/key2.png");
	font = LoadFont("assets/ObelusCompact.ttf");
	s_birdWing = LoadSound("assets/bird_wing.wav");
	s_birdHit = LoadSound("assets/bird_hit.wav");
	s_birdDie = LoadSound("assets/bird_die.wav");
	s_point = LoadSound("assets/bird_point.wav");
	s_level = LoadSound("assets/bird_swooshing.wav");
	// SetSoundPitch(s_point, 1);
	// GAME STATES

	gameState = STATE_READY;
	// bird properties
	bird = bird_red;
	birdFPS = 9;
	birdFrame = mid;
	birdBox.rec.x = screenWidth/2;
	birdBox.rec.y = screenHeight/2;
	birdBox.rec.width = tc[bird_blue].width*2;
	birdBox.rec.height = tc[bird_blue].height*2;
	birdBox.pivot.x = tc[bird_blue].width;
	birdBox.pivot.y = tc[bird_blue].height;
	birdBox.rotation = 0;
	birdBox.scale = 1;

	sky = sky_day;
	skyScroll = 0;
	skyBox.rec.x = 0;
	skyBox.rec.y = 0;
	skyBox.rec.width = tc[sky_day].width*2;
	skyBox.rec.height = tc[sky_day].height*2;
	skyBox.pivot.x = 0;
	skyBox.pivot.y = 0;
	skyBox.rotation = 0;
	skyBox.scale = 1;

	platformScroll = 0;
	platformBox.rec.width = tc[platform].width*2;
	platformBox.rec.height = tc[platform].height*2;
	platformBox.rec.x = screenWidth - platformBox.rec.width;
	platformBox.rec.y = screenHeight - platformBox.rec.height;
	platformBox.pivot.x = 0;
	platformBox.pivot.y = 0;
	platformBox.rotation = 0;
	platformBox.scale = 1;

	// pipe properties
	pipePos[0].x = screenWidth + tc[pipe_green].width*SCALE*4;
	pipePos[1].x = screenWidth + tc[pipe_green].width*SCALE*7; 
	pipePos[2].x = screenWidth + tc[pipe_green].width*SCALE*10;

	pipePos[0].y = (screenHeight-platformBox.rec.height)*sigmoid(GetRandomValue(30, 70)/10.0);
	pipePos[1].y = (screenHeight-platformBox.rec.height)*sigmoid(GetRandomValue(30, 70)/10.0);
	pipePos[2].y = (screenHeight-platformBox.rec.height)*sigmoid(GetRandomValue(30, 70)/10.0);

	for(int i = 0; i < 6; i++){
		pipeBox[i].rotation = 0;
		pipeBox[i].scale = 1;
		pipeBox[i].pivot.x = 0;
		pipeBox[i].pivot.y = 0;
	}

	lastPipe = 2;
	scorePipe = -1;

	gravity = 1600;
	velocity = Vector2{125, 0};
	rotVelocity = 0;

	jumpVelocity = -400;
	jumpRVelocity = -600;

	// GAME STATE PROPERTIES

	showTitle = true;
	fadeTitle = true;
	titleAlpha = 255;

	score = 0;

	moveGameOver = true;
	gameOverPos.x = screenWidth/2 - tc[game_over].width*2;
	gameOverPos.y = -tc[game_over].height*2;

	finalScore = 0;
	animFinalScore = true;

	lastKeyTime = GetTime();
}

void processFrame(double delta){

	if(gameState == STATE_READY){

		skyScroll -= velocity.x*0.2*delta;
		platformScroll -= velocity.x*delta;

		if(skyScroll < -skyBox.rec.width)
			skyScroll = 0;
		if(platformScroll < -platformBox.rec.width)
			platformScroll = 0;

		birdFrame += birdFPS*delta;
		if(birdFrame >= 3)
			birdFrame = 0;

		if(IsKeyPressed(KEY_LEFT)){
			if(bird == 12)
				bird = 18;
			else
				bird -= 3;
		}
		if(IsKeyPressed(KEY_RIGHT)){
			if(bird == 18)
				bird = 12;
			else
				bird += 3;
		}
		if(IsKeyPressed(KEY_DOWN)){
			if(sky == sky_day)
				sky = sky_dark;
			else
				sky = sky_day;
		}
		if(IsKeyPressed(KEY_J)){
			StopSound(s_birdWing);
			PlaySound(s_birdWing);
			lastKeyTime = GetTime();
			keyCount = 1;
			gameState = STATE_RUN;
			fadeTitle = true;
		}
	}
	if(gameState == STATE_RUN){

		skyScroll -= velocity.x*0.2*delta;
		platformScroll -= velocity.x*delta;

		if(skyScroll < -skyBox.rec.width)
			skyScroll = 0;
		if(platformScroll < -platformBox.rec.width)
			platformScroll = 0;

		for(int i = 0; i < 3; i++){
			pipePos[i].x -= velocity.x*delta;
			if(pipePos[i].x < -tc[pipe_green].width*SCALE){
				pipePos[i].x = pipePos[lastPipe].x+tc[pipe_green].width*SCALE*3;
				pipePos[i].y = (screenHeight-tc[platform].height*SCALE)*sigmoid(GetRandomValue(-70, 70)/10.0);
				lastPipe = i;
			}
		}

		birdFrame += birdFPS*delta;
		if(birdFrame >= 3)
			birdFrame = 0;

		velocity.y += gravity*delta;
		rotVelocity += 1.25*gravity*delta;
		birdBox.rec.y += velocity.y*delta;
		birdBox.rotation += rotVelocity*delta;

		if(birdBox.rotation > 90	)
			birdBox.rotation = 90;
		if(birdBox.rotation < -50)
			birdBox.rotation = -50;

		if(IsKeyPressed(KEY_J) && birdBox.rec.y > 0){
			
			StopSound(s_birdWing);
			PlaySound(s_birdWing);

			double keyTime = GetTime() - lastKeyTime;
			lastKeyTime = GetTime();

			if(keyTime < 0.25){
				keyCount += 1;
				jumpVelocity = -400-25*keyCount;
			}else{
				keyCount = 1;
				jumpVelocity = -400;
			}

			// printf("%f\n", jumpVelocity);
			// fflush(stdout);

			velocity.y = jumpVelocity;
			rotVelocity = jumpRVelocity;
		}

		if(IsKeyPressed(KEY_SPACE)){
			showTitle = false;
			gameState = STATE_PAUSE;
			StopSound(s_level);
			PlaySound(s_level);
			return;
		}

		if(showTitle){
			if(fadeTitle)
				titleAlpha -= 255*delta;
			if(titleAlpha <= 0.1)
				showTitle = false;
		}

		//CHECKING COLLISIONS
		Rectangle pipeRec;
		bool collided = CheckCollisionCircleRec(Vector2{birdBox.rec.x, birdBox.rec.y}, birdBox.rec.height/2, platformBox.rec);
		for(int i = 0; i < 3; i++){

			pipeRec.x = pipePos[i].x;
			pipeRec.y = pipePos[i].y + screenHeight/10;
			pipeRec.width = tc[pipe_green].width*SCALE;
			pipeRec.height = tc[pipe_green].height*SCALE;
			collided = collided || CheckCollisionCircleRec(Vector2{birdBox.rec.x, birdBox.rec.y}, birdBox.rec.height/2, pipeRec);
			pipeBox[i*2].rec = pipeRec;

			pipeRec.x = pipePos[i].x;
			pipeRec.y = pipePos[i].y - screenHeight/10 - tc[pipe_green].height*SCALE;
			pipeRec.width = tc[pipe_green].width*SCALE;
			pipeRec.height = tc[pipe_green].height*SCALE;
			collided = collided || CheckCollisionCircleRec(Vector2{birdBox.rec.x, birdBox.rec.y}, birdBox.rec.height/2, pipeRec);
			pipeBox[i*2+1].rec = pipeRec;
		}

		if(collided){

			StopSound(s_birdHit);
			PlaySound(s_birdHit);
			velocity.x = 0;
			velocity.y = 0;
			rotVelocity = 250;
			moveGameOver = true;
			gameOverPos.x = screenWidth/2 - tc[game_over].width*SCALE/2;
			gameOverPos.y = -tc[game_over].height*2;
			finalScore = 0;
			animFinalScore = false;
			playDie = true;
			gameState = STATE_GAMEOVER;
			return;
		}

		for(int i = 0; i < 3; i++){
			if(i != scorePipe && pipePos[i].x < birdBox.rec.x -tc[pipe_green].width*SCALE -10){
				StopSound(s_point);
				PlaySound(s_point);
				score += 1;
				scorePipe = i;
			}
		}
	}
	if(gameState == STATE_PAUSE){
		if(IsKeyPressed(KEY_SPACE)){
			StopSound(s_level);
			PlaySound(s_level);
			gameState = STATE_RUN;
		}

		if(IsKeyPressed(KEY_J) && birdBox.rec.y > 0){

			StopSound(s_birdWing);
			PlaySound(s_birdWing);

			lastKeyTime = GetTime();
			keyCount = 1;
			jumpVelocity = -400;

			velocity.y = jumpVelocity;
			rotVelocity = jumpRVelocity;

			gameState = STATE_RUN;
		}
	}
	if(gameState == STATE_GAMEOVER){

		if(!CheckCollision(birdBox.rec, birdBox.rotation, 1, birdBox.pivot, platformBox.rec)){
			if(playDie){
				StopSound(s_birdDie);
				PlaySound(s_birdDie);
				playDie = false;
			}
			velocity.y += gravity*delta;
			rotVelocity += gravity*delta;
			birdBox.rec.y += velocity.y*delta;
		}

		birdBox.rotation += 0.8*rotVelocity*delta;
		if(birdBox.rotation > 90)
			birdBox.rotation = 90;
		if(birdBox.rotation < -30)
			birdBox.rotation = -30;

		if(moveGameOver){
			gameOverPos.y += screenHeight*delta;
			if(gameOverPos.y > screenHeight/3){
				moveGameOver = false;
				animFinalScore = true;
			}
		}

		if(animFinalScore){
			finalScore += score*delta*3;
			if(finalScore >= score){
				finalScore = score;
				animFinalScore = false;
			}
		}

		if(!moveGameOver && !animFinalScore && IsKeyPressed(KEY_J)){

			StopSound(s_level);
			PlaySound(s_level);

			score = 0;

			showTitle = true;
			fadeTitle = true;
			titleAlpha = 255;

			birdBox.rec.y = screenHeight/2;
			birdBox.rotation = 0;

			pipePos[0].x = screenWidth + tc[pipe_green].width*2*4;
			pipePos[1].x = screenWidth + tc[pipe_green].width*2*7; 
			pipePos[2].x = screenWidth + tc[pipe_green].width*2*10;

			pipePos[0].y = (screenHeight-platformBox.rec.height)*sigmoid(GetRandomValue(30, 70)/10.0);
			pipePos[1].y = (screenHeight-platformBox.rec.height)*sigmoid(GetRandomValue(30, 70)/10.0);
			pipePos[2].y = (screenHeight-platformBox.rec.height)*sigmoid(GetRandomValue(30, 70)/10.0);

			lastPipe = 2;

			velocity.x = 125;
			velocity.y = 0;
			rotVelocity = 0;

			gameState = STATE_READY;
		}
	}
}

void drawFrame(){
	if(gameState == STATE_READY){
		BeginDrawing();
			ClearBackground(BLACK);

			DrawTexture(txr_flappy, tc[sky], Vector2{skyScroll, 0}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[sky], Vector2{skyScroll+tc[sky_day].width*SCALE, 0}, 0, SCALE, WHITE);

			DrawTexture(txr_flappy, tc[platform], Vector2{platformScroll, platformBox.rec.y}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[platform], Vector2{platformScroll+tc[platform].width*SCALE, platformBox.rec.y}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[platform], Vector2{platformScroll+tc[platform].width*2*SCALE, platformBox.rec.y}, 0, SCALE, WHITE);

			DrawTexture(txr_flappy, tc[title], Vector2{screenWidth/2.0f-tc[title].width*SCALE/2, screenHeight*0.2f}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[get_ready], Vector2{screenWidth/2.0f-tc[title].width*SCALE/2, screenHeight*0.325f}, 0, SCALE, WHITE);

			DrawTexture(txr_flappy, tc[bird+int(birdFrame)], birdBox.rec, birdBox.rotation, birdBox.scale, birdBox.pivot, WHITE);

			DrawTexture(txr_key, tc[key_down], Vector2{screenWidth/2.0f-tc[key_j].width*SCALE/2, screenHeight*0.575f}, 0, SCALE, Color{255, 255, 255, 155});
			DrawTexture(txr_key, tc[key_left], Vector2{screenWidth*0.3f-tc[key_j].width*SCALE/2, birdBox.rec.y}, 0, SCALE, Color{255, 255, 255, 155});
			DrawTexture(txr_key, tc[key_right], Vector2{screenWidth*0.7f-tc[key_j].width*SCALE/2, birdBox.rec.y}, 0, SCALE, Color{255, 255, 255, 155});

			DrawTexture(txr_key, tc[key_j], Vector2{screenWidth*0.47f-tc[key_j].width*SCALE/2, screenHeight*0.89f}, 0, SCALE, Color{255, 255, 255, 155});
			DrawTextEx(font, "press   to jump", Vector2{screenWidth*0.1f, screenHeight*0.87f}, 32, 4, Color{0, 0, 0, 155});

		EndDrawing();
	}
	if(gameState == STATE_RUN){
		BeginDrawing();
			ClearBackground(BLACK);

			DrawTexture(txr_flappy, tc[sky], Vector2{skyScroll, 0}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[sky], Vector2{skyScroll+tc[sky_day].width*SCALE, 0}, 0, SCALE, WHITE);

			for(int i = 0; i < 3; i++){
				Vector2 upPos, downPos;
				upPos.x = pipePos[i].x + tc[pipe_green].width*SCALE;
				downPos.x = pipePos[i].x;
				upPos.y = pipePos[i].y - screenHeight/10;
				downPos.y = pipePos[i].y + screenHeight/10;
				DrawTexture(txr_flappy, tc[pipe_green], upPos, 180, SCALE, WHITE);
				DrawTexture(txr_flappy, tc[pipe_green], downPos, 0, SCALE, WHITE);

			}

			if(showTitle){
				DrawTexture(txr_flappy, tc[title], Vector2{screenWidth/2.0f-tc[title].width*SCALE/2, screenHeight/5}, 0, SCALE, Color{255, 255, 255, (unsigned char)(titleAlpha)});
				DrawTexture(txr_flappy, tc[get_ready], Vector2{screenWidth/2.0f-tc[title].width*SCALE/2, screenHeight*0.325f}, 0, SCALE, Color{255, 255, 255, (unsigned char)(titleAlpha)});
			}

			DrawTexture(txr_flappy, tc[bird+int(birdFrame)], birdBox.rec, birdBox.rotation, birdBox.scale, birdBox.pivot, WHITE);

			DrawTexture(txr_flappy, tc[platform], Vector2{platformScroll, platformBox.rec.y}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[platform], Vector2{platformScroll+tc[platform].width*SCALE, platformBox.rec.y}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[platform], Vector2{platformScroll+tc[platform].width*2*SCALE, platformBox.rec.y}, 0, SCALE, WHITE);

			DrawTexture(txr_key2, tc[key_space], Vector2{screenWidth*0.2f-tc[key_j].width*SCALE/2, screenHeight*0.89f}, 0, SCALE, Color{255, 255, 255, 155});
			DrawTextEx(font, "    to pause", Vector2{screenWidth*0.2f-tc[key_j].width*SCALE/2, screenHeight*0.87f}, 32, 4, Color{0, 0, 0, 155});

			// DrawRectangle(birdBox.rec, birdBox.rotation, 1, birdBox.pivot, BLACK);
			// for(int i = 0; i < 6; i++){
			// 	DrawRectangle(pipeBox[i].rec, pipeBox[i].rotation, 1, pipeBox[i].pivot, BLACK);
			// }

			drawInt(score, number_small);
		EndDrawing();
	}
	if(gameState == STATE_PAUSE){
		BeginDrawing();
			ClearBackground(BLACK);

			DrawTexture(txr_flappy, tc[sky], Vector2{skyScroll, 0}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[sky], Vector2{skyScroll+tc[sky_day].width*SCALE, 0}, 0, SCALE, WHITE);

			for(int i = 0; i < 3; i++){
				Vector2 upPos, downPos;
				upPos.x = pipePos[i].x + tc[pipe_green].width*SCALE;
				downPos.x = pipePos[i].x;
				upPos.y = pipePos[i].y - screenHeight/10;
				downPos.y = pipePos[i].y + screenHeight/10;
				DrawTexture(txr_flappy, tc[pipe_green], upPos, 180, SCALE, WHITE);
				DrawTexture(txr_flappy, tc[pipe_green], downPos, 0, SCALE, WHITE);

			}

			DrawTexture(txr_flappy, tc[bird+int(birdFrame)], birdBox.rec, birdBox.rotation, birdBox.scale, birdBox.pivot, WHITE);

			DrawTexture(txr_flappy, tc[platform], Vector2{platformScroll, platformBox.rec.y}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[platform], Vector2{platformScroll+tc[platform].width*SCALE, platformBox.rec.y}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[platform], Vector2{platformScroll+tc[platform].width*2*SCALE, platformBox.rec.y}, 0, SCALE, WHITE);

			DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 100});

			DrawTexture(txr_key2, tc[key_space], Vector2{screenWidth*0.2f-tc[key_j].width*SCALE/2, screenHeight*0.89f}, 0, SCALE, Color{255, 255, 255, 155});
			DrawTextEx(font, "    to resume", Vector2{screenWidth*0.2f-tc[key_j].width*SCALE/2, screenHeight*0.87f}, 32, 4, Color{255, 255, 255, 155});

			// DrawRectangle(birdBox.rec, birdBox.rotation, 1, birdBox.pivot, BLACK);
			// for(int i = 0; i < 6; i++){
			// 	DrawRectangle(pipeBox[i].rec, pipeBox[i].rotation, 1, pipeBox[i].pivot, BLACK);
			// }

			drawInt(score, number_small, 2, Vector2{screenWidth/2, screenHeight/2}, Color{255, 255, 255, 155});
		EndDrawing();
	}
	if(gameState == STATE_GAMEOVER){
		BeginDrawing();
			ClearBackground(BLACK);

			DrawTexture(txr_flappy, tc[sky], Vector2{skyScroll, 0}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[sky], Vector2{skyScroll+tc[sky_day].width*SCALE, 0}, 0, SCALE, WHITE);

			for(int i = 0; i < 3; i++){
				Vector2 upPos, downPos;
				upPos.x = pipePos[i].x + tc[pipe_green].width*SCALE;
				downPos.x = pipePos[i].x;
				upPos.y = pipePos[i].y - screenHeight/10;
				downPos.y = pipePos[i].y + screenHeight/10;
				DrawTexture(txr_flappy, tc[pipe_green], upPos, 180, SCALE, WHITE);
				DrawTexture(txr_flappy, tc[pipe_green], downPos, 0, SCALE, WHITE);
			}

			DrawTexture(txr_flappy, tc[bird+int(birdFrame)], birdBox.rec, birdBox.rotation, birdBox.scale, birdBox.pivot, WHITE);

			DrawTexture(txr_flappy, tc[platform], Vector2{platformScroll, platformBox.rec.y}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[platform], Vector2{platformScroll+tc[platform].width*SCALE, platformBox.rec.y}, 0, SCALE, WHITE);
			DrawTexture(txr_flappy, tc[platform], Vector2{platformScroll+tc[platform].width*2*SCALE, platformBox.rec.y}, 0, SCALE, WHITE);

			DrawTexture(txr_flappy, tc[game_over], gameOverPos, 0, SCALE, WHITE);
			
			DrawTexture(txr_key, tc[key_j], Vector2{screenWidth*0.2f-tc[key_j].width*SCALE/2, screenHeight*0.89f}, 0, SCALE, Color{255, 255, 255, 155});
			DrawTextEx(font, "  to restart", Vector2{screenWidth*0.2f-tc[key_j].width*SCALE/2, screenHeight*0.87f}, 32, 4, Color{0, 0, 0, 155});

			drawInt(finalScore, number_big, 1, Vector2{screenWidth/2, screenHeight/2});

		EndDrawing();
	}
}

void drawInt(int n, int size){
	char str[50];
	int digits = sprintf(str, "%d", n);

	int spacing = 1;
	int width = digits* tc[size].width*SCALE + digits*spacing - spacing;
	int height = tc[size].height*SCALE;

	for(int i = 0; i < digits; i++){
		Vector2 pos{screenWidth*0.8f - width/2.0f + i*(tc[size].width*SCALE+spacing), screenWidth/5.0f - height/2.0f};
		DrawTexture(txr_flappy, tc[size + str[i]-'0'], pos, 0, SCALE, WHITE);
	}
}

void drawInt(int n, int size, float scale, Vector2 position){
	char str[50];
	int digits = sprintf(str, "%d", n);

	int spacing = 1;
	int width = digits* tc[size].width*SCALE*scale + digits*spacing - spacing;
	int height = tc[size].height*SCALE*scale;

	for(int i = 0; i < digits; i++){
		Vector2 pos{position.x - width/2.0f + i*(tc[size].width*SCALE*scale+spacing), position.y - height/2.0f};
		DrawTexture(txr_flappy, tc[size + str[i]-'0'], pos, 0, SCALE*scale, WHITE);
	}
}

void drawInt(int n, int size, float scale, Vector2 position, Color color){
	char str[50];
	int digits = sprintf(str, "%d", n);

	int spacing = 1;
	int width = digits* tc[size].width*SCALE*scale + digits*spacing - spacing;
	int height = tc[size].height*SCALE*scale;

	for(int i = 0; i < digits; i++){
		Vector2 pos{position.x - width/2.0f + i*(tc[size].width*SCALE*scale+spacing), position.y - height/2.0f};
		DrawTexture(txr_flappy, tc[size + str[i]-'0'], pos, 0, SCALE*scale, color);
	}
}

float sigmoid(float x){
	float res = pow(1.2, -x);
	res += 1;
	// printf("%f\n", 1/res);
	// fflush(stdout);
	return 1/res;
}