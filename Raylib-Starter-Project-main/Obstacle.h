#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>

using namespace std;

class Obstacle
{
private:
	Vector2 mObstaclePosition = { 500, 500 };
	Vector2 mObstacleSize = { 50, 100 };
	float mObstacleAngle = 0;

	Color mObstacleColor = BLACK;


public:
	Obstacle(Vector2 pObstaclePosition = Vector2{ 500, 500 }, Vector2 pObstacleSize = { 50, 100 });
	void Draw();

	inline Vector2 GetPosition() { return mObstaclePosition; }
	inline Vector2 GetSize() { return mObstacleSize; }
};

