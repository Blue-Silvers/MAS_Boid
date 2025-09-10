#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>

using namespace std;

class Boid {
private:
	Vector2 mBoidPosition = { 500, 500 };
	float mBoidSpeedX = 2;
	float mBoidSpeedY = -2;
	float mBoidSpeedMax = 10;
	float mBoidAngle = 0;
	Color mBoidColor = WHITE;

	float mBoidDetectionZone = 20;

public:
	bool mBoidLunched = false;
	int mBoidRadius = 5;

	Boid(Vector2 pBoidPosition = Vector2{ 500, 500 });
	void Update(int screenWidth, int screenHeight);
	void Draw();
	void CollideBoid();
	inline void SetSpeed(Vector2 pNewSpeed) { mBoidSpeedX = pNewSpeed.x; mBoidSpeedY = pNewSpeed.y; }
	inline float GetAngle() { return mBoidAngle; }
	void Separation(vector<Boid*> pBoids);
};

