#include "raylib.h"
#include "Boid.h"
#include "Obstacle.h"
#include <iostream>
#include <vector>

using namespace std;

void Start();
void Update();
void Draw();
void End();

std::vector<Boid*> boidList;
std::vector<Boid*> fugitiveBoidsList;
std::vector<Boid*> hunterBoidsList;
int boidCount = 20;
int fugitiveBoidCount = 20;
int hunterBoidCount = 20;
Texture2D boidSprite;

std::vector<Obstacle*> obstacleList;

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

    boidSprite = LoadTexture("resources/Picture/WhiteFish.png");

    for (int i = 0; i < boidCount; i++)
    {
        Vector2 randPos = Vector2{ (float)GetRandomValue(75, GetScreenWidth() - 75), (float)GetRandomValue(75, GetScreenHeight() - 75) };
        Boid* newBoid = new Boid(randPos);
        float randSpeed = GetRandomValue(-20, 20);
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
        newBoid->SetBoidTexture(boidSprite);


        boidList.push_back(newBoid);
    }

    for (int i = 0; i < fugitiveBoidCount; i++)
    {
        Vector2 randPos = Vector2{ (float)GetRandomValue(75, GetScreenWidth() - 75), (float)GetRandomValue(75, GetScreenHeight() - 75) };
        Boid* newBoid = new Boid(randPos, RED);
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
        newBoid->SetBoidTexture(boidSprite);
        newBoid->SetAvoidPredatorFactor(0.5f);

        fugitiveBoidsList.push_back(newBoid);
    }

    for (int i = 0; i < hunterBoidCount; i++)
    {
        Vector2 randPos = Vector2{ (float)GetRandomValue(75, GetScreenWidth() - 75), (float)GetRandomValue(75, GetScreenHeight() - 75) };
        Boid* newBoid = new Boid(randPos, GREEN);
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
        newBoid->SetBoidTexture(boidSprite);
        newBoid->SetFoodAttractFactor(0.2f);

        hunterBoidsList.push_back(newBoid);
	}
}

void Update()
{
	obstacleList.push_back(new Obstacle(Vector2{ 500, 200 }, Vector2{ 50, 100 }));
    obstacleList.push_back(new Obstacle(Vector2{ 800, 500 }, Vector2{ 50, 25 }));

    for (int i = 0; i < boidList.size(); i++)
    {
        boidList[i]->Separation(boidList, fugitiveBoidsList, hunterBoidsList);
        boidList[i]->ObstacleAvoid(obstacleList);
        boidList[i]->Update(GetScreenWidth(), GetScreenHeight());
    }

    for (int i = 0; i < fugitiveBoidsList.size(); i++)
    {
        fugitiveBoidsList[i]->Separation(fugitiveBoidsList, hunterBoidsList, boidList);
        fugitiveBoidsList[i]->ObstacleAvoid(obstacleList);
        fugitiveBoidsList[i]->Update(GetScreenWidth(), GetScreenHeight());
    }

    for (int i = 0; i < hunterBoidsList.size(); i++)
    {
        hunterBoidsList[i]->Separation(hunterBoidsList, boidList, fugitiveBoidsList);
        hunterBoidsList[i]->ObstacleAvoid(obstacleList);
        hunterBoidsList[i]->Update(GetScreenWidth(), GetScreenHeight());
    }
}

void Draw()
{
    BeginDrawing();

    ClearBackground(DARKBLUE);

    for (int i = 0; i < boidList.size(); i++)
    {
		boidList[i]->Draw();

        if (boidList[i]->GetIsAlive() == false) 
        {
            boidList[i]->SetIsAlive(true);
			boidList[i]-> SetColor(hunterBoidsList[0]->GetColor());
            hunterBoidsList.push_back(boidList[i]);
            boidList.erase(boidList.begin() + i);
            i--;
            
			continue;
        }
    }

    for (int i = 0; i < fugitiveBoidsList.size(); i++)
    {
        fugitiveBoidsList[i]->Draw();

        if (fugitiveBoidsList[i]->GetIsAlive() == false)
        {
            fugitiveBoidsList[i]->SetIsAlive(true);
            fugitiveBoidsList[i]->SetColor(boidList[0]->GetColor());
            boidList.push_back(fugitiveBoidsList[i]);
            fugitiveBoidsList.erase(fugitiveBoidsList.begin() + i);
            i--;

            continue;
        }
    }

    for (int i = 0; i < hunterBoidsList.size(); i++)
    {
        hunterBoidsList[i]->Draw();

        if (hunterBoidsList[i]->GetIsAlive() == false)
        {
			hunterBoidsList[i]->SetIsAlive(true);
            hunterBoidsList[i]->SetColor(fugitiveBoidsList[0]->GetColor());
            fugitiveBoidsList.push_back(hunterBoidsList[i]);
            hunterBoidsList.erase(hunterBoidsList.begin() + i);
            i--;

            continue;
        }
    }


    for (int i = 0; i < obstacleList.size(); i++)
    {
        obstacleList[i]->Draw();
    }


    EndDrawing();
}

void End()
{
}
