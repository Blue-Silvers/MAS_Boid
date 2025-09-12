#pragma once
#include "raylib.h"
#include "Obstacle.h"
#include <iostream>
#include <vector>

using namespace std;

class Boid {
private:
	Vector2 mBoidPosition = { 500, 500 };
	int mBoidRadius = 8;
	float mBoidAngle = 0;

	float mBoidSpeedX = 2;
	float mBoidSpeedY = -2;
	float mBoidSpeedMax = 5;
	float mBoidSpeedMin = 3;

	Color mBoidColor = WHITE;

	float mBoidMinimumDistance = 30;
	float mBoidMaxPerceiveDistance = 220;
	float mCohesionRadius = 580;

	float mAvoidFactor = 0.035;
	float mMatchingFactor = 0.145;
	float mCenteringFactor = 0.001;
	float mAvoidPredatorFactor = 0.035;
	float mFoodAttractFactor = 0.05;

	bool mBoidLunched = false;

	Texture2D mBoidSprite;

public:
	Boid(Vector2 pBoidPosition = Vector2{ 500, 500 }, Color pColor = WHITE);
	void Update(int screenWidth, int screenHeight);
	void Draw();
	void CollideBoid();

	inline void SetSpeed(Vector2 pNewSpeed) { mBoidSpeedX = pNewSpeed.x; mBoidSpeedY = pNewSpeed.y; }
	inline float GetAngle() { return mBoidAngle; }
	inline void SetBoidTexture(Texture2D pTexture) { mBoidSprite = pTexture; }
	inline void SetAvoidFactor(float pNewAvoidFactor) { mAvoidFactor = pNewAvoidFactor; }
	inline void SetMatchingFactor(float pNewMatchingFactor) { mMatchingFactor = pNewMatchingFactor; }
	inline void SetCenteringFactor(float pNewCenteringFactor) { mCenteringFactor = pNewCenteringFactor; }
	inline void SetAvoidPredatorFactor(float pNewAvoidPredatorFactor) { mAvoidPredatorFactor = pNewAvoidPredatorFactor; }
	inline void SetFoodAttractFactor(float pNewFoodAttractFactor) { mFoodAttractFactor = pNewFoodAttractFactor; }

	void ObstacleAvoid(vector<Obstacle*> pObstacles);
	void Separation(vector<Boid*> pBoids, vector<Boid*> pFoodBoids, vector<Boid*> pEvilBoids);
	void Alignment(vector<Boid*> pBoids, vector<Boid*> pFoodBoids);
	void Cohesion(vector<Boid*> pBoids);
};