#pragma once
#include "raylib.h"
#include <iostream>
#include <vector>

using namespace std;

class Boid {
private:
	Vector2 mBoidPosition = { 500, 500 };
	int mBoidRadius = 5;
	float mBoidAngle = 0;

	float mBoidSpeedX = 2;
	float mBoidSpeedY = -2;
	float mBoidSpeedMax = 5;
	float mBoidSpeedMin = 3;

	Color mBoidColor = WHITE;

	float mBoidMinimumDistance = 30;
	float mBoidMaxPerceiveDistance = 120;
	float mCohesionRadius = 60;

	float mAvoidFactor = 0.035;
	float mMatchingFactor = 0.045;
	float mCenteringFactor = 0.001;

	bool mBoidLunched = false;


public:
	Boid(Vector2 pBoidPosition = Vector2{ 500, 500 });
	void Update(int screenWidth, int screenHeight);
	void Draw();
	void CollideBoid();
	inline void SetSpeed(Vector2 pNewSpeed) { mBoidSpeedX = pNewSpeed.x; mBoidSpeedY = pNewSpeed.y; }
	inline float GetAngle() { return mBoidAngle; }
	void Separation(vector<Boid*> pBoids);
	void Alignment(vector<Boid*> pBoids);
	void Cohesion(vector<Boid*> pBoids);
};

