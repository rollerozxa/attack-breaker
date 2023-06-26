#include "raylib.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define SCREEN_WIDTH (854)
#define SCREEN_HEIGHT (480)

typedef struct Block {
	Rectangle rect;
	char type;
	Color colour;
	bool broken;
} Block;

Color randomColour() {
	int i = rand() % 5;
	switch (i) {
		case 0: return YELLOW;
		case 1: return RED;
		case 2: return GOLD;
		case 3: return BLUE;
		case 4: return LIME;
		case 5: return DARKPURPLE;
	}
}

int main(void)
{
	srand(time(NULL));

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "gay attack breaker clone");
	SetTargetFPS(60);

	Vector2 mousePosition = { -100.0f, -100.0f };

	Rectangle top		= { 0,-10, SCREEN_WIDTH,10 };
	Rectangle bottom	= { 0,SCREEN_HEIGHT, SCREEN_WIDTH,10 };
	Rectangle left 		= { -10,0, 10,SCREEN_HEIGHT };
	Rectangle right		= { SCREEN_WIDTH,0, 10,SCREEN_HEIGHT };

	Rectangle ball = { 300,300, 25,25 };
	Rectangle ballpoint = { 300,300, 1,1 };
	float velAngle = PI/3;

	Rectangle paddle = { 50, 460, 100, 20 };

	Block blocks[256];

	int n = 0;
	for (int x = 0; x < 17; x++) {
		for (int y = 0; y < 6; y++) {
#define BLOCK_WIDTH 40
#define BLOCK_HEIGHT 20

			blocks[n].rect.x = 40+(x*(BLOCK_WIDTH+5));
			blocks[n].rect.y = 50+(y*(BLOCK_HEIGHT+5));
			blocks[n].rect.width = BLOCK_WIDTH;
			blocks[n].rect.height = BLOCK_HEIGHT;
			blocks[n].type = 1;
			blocks[n].colour = randomColour();
			blocks[n].broken = false;

			n++;
		}
	}

	int cooldownCollision = 0;

	while (!WindowShouldClose()) {
		for (int i = 0; i < 16; i++) {

			mousePosition = GetMousePosition();
			paddle.x = mousePosition.x - paddle.width/2;

			#define SPEED 8/16

			ball.x += cos(velAngle) *SPEED;
			ball.y += sin(velAngle) *SPEED;
			ballpoint.x = ball.x;
			ballpoint.y = ball.y;

			for (int i = 0; i < 256; i++) {
				if (!blocks[i].broken && CheckCollisionRecs(ball, blocks[i].rect)) {
					blocks[i].broken = true;

					Rectangle bt = {blocks[i].rect.x,blocks[i].rect.y,blocks[i].rect.width,1};
					Rectangle bb = {blocks[i].rect.x,blocks[i].rect.y+blocks[i].rect.height-1,blocks[i].rect.width,1};
					Rectangle bl = {blocks[i].rect.x,blocks[i].rect.y,1,blocks[i].rect.height};
					Rectangle br = {blocks[i].rect.x+blocks[i].rect.width-1,blocks[i].rect.y,1,blocks[i].rect.height};

					if (CheckCollisionRecs(ball, bb)) {
						if (cos(velAngle) *SPEED > 0) {
							velAngle += PI/(2+rand()%1);
						} else {
							velAngle -= PI/(2+rand()%1);
						}
						cooldownCollision = 0;
					}
					else if (CheckCollisionRecs(ball, bt)) {
						if (cos(velAngle) *SPEED < 0) {
							velAngle += PI/(2+rand()%1);
						} else {
							velAngle -= PI/(2+rand()%1);
						}
						cooldownCollision = 0;
					}
					else if (CheckCollisionRecs(ball, br)) {
						if (sin(velAngle) *SPEED < 0) {
							velAngle += PI/(2+rand()%1);
						} else {
							velAngle -= PI/(2+rand()%1);
						}
						cooldownCollision = 0;
					}
					else if (CheckCollisionRecs(ball, bl)) {
						if (sin(velAngle) *SPEED > 0) {
							velAngle += PI/(2+rand()%1);
						} else {
							velAngle -= PI/(2+rand()%1);
						}
						cooldownCollision = 0;
					}
				}
			}

			if (CheckCollisionRecs(ball, top) && cooldownCollision != 1) {
				cooldownCollision = 1;
				if (cos(velAngle) *SPEED > 0) {
					velAngle += PI/(2+rand()%1);
				} else {
					velAngle -= PI/(2+rand()%1);
				}
				//printf("%f\n", velAngle/PI);
			}
			if (CheckCollisionRecs(ball, bottom) && cooldownCollision != 2) {
				cooldownCollision = 2;
				if (cos(velAngle) *SPEED < 0) {
					velAngle += PI/(2+rand()%1);
				} else {
					velAngle -= PI/(2+rand()%1);
				}
				//printf("%f\n", velAngle/PI);
			}
			if (CheckCollisionRecs(ball, left) && cooldownCollision != 3) {
				cooldownCollision = 3;
				if (sin(velAngle) *SPEED < 0) {
					velAngle += PI/(2+rand()%1);
				} else {
					velAngle -= PI/(2+rand()%1);
				}
				//printf("%f\n", velAngle/PI);
			}
			if (CheckCollisionRecs(ball, right) && cooldownCollision != 4) {
				cooldownCollision = 4;
				if (sin(velAngle) *SPEED > 0) {
					velAngle += PI/(2+rand()%1);
				} else {
					velAngle -= PI/(2+rand()%1);
				}
				//printf("%f\n", velAngle/PI);
			}
			if (CheckCollisionRecs(ball, paddle) && cooldownCollision != 5) {
				cooldownCollision = 5;
				if (cos(velAngle) *SPEED < 0) {
					velAngle += PI/(2+rand()%1);
				} else {
					velAngle -= PI/(2+rand()%1);
				}
				//printf("%f\n", velAngle/PI);
			}

			/*if (CheckCollisionRecs(ball, top)
			|| CheckCollisionRecs(ball, bottom)
			|| CheckCollisionRecs(ball, left)
			|| CheckCollisionRecs(ball, right)
			|| CheckCollisionRecs(ball, paddle)) {
				velAngle -= PI/(2+rand()%1);
			}*/
		}

		BeginDrawing();

		ClearBackground(BLACK);

		DrawRectangle(paddle.x, paddle.y, paddle.width, paddle.height, GRAY);

		for (int i = 0; i < 256; i++) {
			if (!blocks[i].broken) {
				DrawRectangle(blocks[i].rect.x, blocks[i].rect.y, blocks[i].rect.width, blocks[i].rect.height, blocks[i].colour);
			}
		}

		DrawCircle(ball.x+(ball.width/2), ball.y+(ball.height/2), ball.width/2, GRAY);

		EndDrawing();
	}

	CloseWindow();

	return 0;
}
