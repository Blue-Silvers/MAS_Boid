#include "raylib.h"
#include "Boid.h"
#include <iostream>
#include <vector>

using namespace std;

void Start();
void Update();
void Draw();
void End();

std::vector<Boid*> boidList;
int boidCount = 30;

bool debugMode = true;

int main() {

    Start();

    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }

    End();
    return 0;
}

void Start()
{
    InitWindow(1200, 800, "Boid MAS");
    SetTargetFPS(60);
}

void Update()
{
    if (boidList.size() < boidCount)
    {
        Vector2 randPos = Vector2{ (float)GetRandomValue(50, GetScreenWidth()-50), (float)GetRandomValue(50, GetScreenHeight() - 50) };
        Boid* newBoid = new Boid(randPos);
		float randSpeed = GetRandomValue(-20, 20);
        randSpeed = randSpeed / 10.0f;
		Vector2 startSpeed;
        if (randSpeed < 0) 
        {
            startSpeed = Vector2{ randSpeed, 2 + randSpeed };
        }
        else 
        {
			startSpeed = Vector2{ randSpeed, 2 - randSpeed };
		}
		newBoid->SetSpeed(startSpeed);

        //debug
		printf("New boid speed x: %f, y: %f\n",  startSpeed.x, startSpeed.y);
		

        boidList.push_back(newBoid);

	}

    for (int i = 0; i < boidList.size(); i++)
    {
        boidList[i]->Separation(boidList);
        boidList[i]->Update(GetScreenWidth(), GetScreenHeight());
		
        //printf("New boid angle: %f\n", boidList[i]->GetAngle());
    }
}

void Draw()
{
    BeginDrawing();

    ClearBackground(BLUE);


    for (int i = 0; i < boidList.size(); i++)
    {
		boidList[i]->Draw();
    }

    EndDrawing();
}

void End()
{
}
