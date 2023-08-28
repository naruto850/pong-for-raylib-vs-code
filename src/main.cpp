#include <iostream>
#include <raylib.h>

#define endl '\n'

using namespace std;

Color green = Color{ 38, 185, 154, 255 };

int player_score = 0;
int cpu_score = 0;

class Ball
{	
public:
	float x, y;

	int speedx, speedy;

	int rad;

	void draw()
	{
		DrawCircle(x, y, rad, YELLOW);
	}

	void update()
	{
		x += speedx;
		
		y += speedy;

		if (y + rad >= GetScreenHeight() || y - rad <= 0)
		{
			speedy *= -1;
		}

		if (x + rad >= GetScreenWidth())
		{
			cpu_score++;
			resetball();
		}

		if (x - rad <= 0)
		{
			player_score++;
			resetball();
		}
	}

	void resetball()
	{
		x = GetScreenWidth() / 2;
		y = GetScreenHeight() / 2;

		int speedchoices[2] = { -1, 1 };

		speedx *= speedchoices[GetRandomValue(0, 1)];

		speedy *= speedchoices[GetRandomValue(0, 1)];
	}
};

class Paddle
{
protected:
	void checkifout()
	{
		if (y <= 0)
		{
			y = 0;
		}

		if (y + height >= GetScreenHeight())
		{
			y = GetScreenHeight() - height;
		}
	}
public:
	float x, y;
	float width, height;
	int speed;

	void draw()
	{
		DrawRectangleRounded(Rectangle{ x, y, (float)25, (float)120 }, 20, 0, WHITE);
	}

	void update()
	{
		if (IsKeyDown(KEY_UP))
		{
			y = y - speed;
		}
		if (IsKeyDown(KEY_DOWN))
		{
			y = y + speed;
		}

		checkifout();
	}

	
};


class CPUpaddle : public Paddle 
{
public:
	void update(int bally)
	{
		if (y + height / 2 > bally)
		{
			y = y - speed;
		}
		if (y + height / 2 <= bally)
		{
			y = y + speed;
		}

		checkifout();
	}
};


Ball ball;
Paddle player;
CPUpaddle cpu;

int main() 
{
	cout << "starting the game" << endl;

	const int screen_width = 1280;
	const int screen_height = 800;

	InitWindow(screen_width, screen_height, "Pong");
	SetTargetFPS(60);

	ball.rad = 20;
	ball.x = screen_width / 2;
	ball.y = screen_height / 2;
	ball.speedx = 7;
	ball.speedy = 7;

	player.width = 25;
	player.height = 120;
	player.x = screen_width - player.width - 10;
	player.y = screen_height / 2 - player.height / 2;
	player.speed = 6;

	cpu.height = 120;
	cpu.width = 25;
	cpu.x = 10;
	cpu.y = screen_height / 2 - cpu.height / 2;
	cpu.speed = 6;

	HideCursor();
	while (WindowShouldClose() == false)
	{

		BeginDrawing();

		ball.update();
		player.update();
		cpu.update(ball.y);

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.rad, Rectangle{ player.x, player.y, player.width, player.height }))
		{
			ball.speedx *= -1;
		}

		if (CheckCollisionCircleRec(Vector2{ ball.x, ball.y }, ball.rad, Rectangle{ cpu.x, cpu.y, cpu.width, cpu.height }))
		{
			ball.speedx *= -1;
		}

		ClearBackground(BLACK);

		DrawCircleLines(screen_width / 2, screen_height / 2, 150, WHITE);
		
		DrawLine(screen_width / 2, 0, screen_width/2, screen_height, WHITE);

		ball.draw();
		
		cpu.draw();

		player.draw();

		DrawText(TextFormat("%i", cpu_score), screen_width / 4 - 20, 20, 80, WHITE);
		DrawText(TextFormat("%i", player_score), 3*screen_width / 4 - 20, 20, 80, WHITE);

		DrawText(TextFormat("%i", GetFPS()), 0, 0, 10, WHITE);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}