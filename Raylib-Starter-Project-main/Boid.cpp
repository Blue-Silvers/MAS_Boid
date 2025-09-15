#include "Boid.h"
#include "Math.h"
#include "Obstacle.h"
#include <algorithm>

Boid::Boid(Vector2 pBoidPosition, Color pColor)
{
	mBoidPosition = pBoidPosition;
    mBoidColor = pColor;
}


void Boid::Update(int screenWidth, int screenHeight)
{
    if (IsKeyDown(KEY_SPACE))
    {
        mBoidLunched = !mBoidLunched;
    }

    if (mBoidLunched)
    {
        //Calcul angle
        mBoidDesiredAngle = atan2(mBoidSpeedY, mBoidSpeedX) * 180.0f / PI;

        float diff = NormalizeAngle(mBoidDesiredAngle - mBoidActualAngle);
        if (diff < -mBoidMaxTurn)
        {
            diff = -mBoidMaxTurn;
        }
        else if (diff > mBoidMaxTurn)
        {
            diff = mBoidMaxTurn;
        }
        mBoidAngle = mBoidActualAngle + diff;

        mBoidSpeedX = cos(mBoidAngle * PI / 180.0f);
        mBoidSpeedY = sin(mBoidAngle * PI / 180.0f);

        mBoidActualAngle = atan2(mBoidSpeedY, mBoidSpeedX) * 180.0f / PI;


        //Convert Normalize Speed to classic Speed
        float speed = sqrt(mBoidSpeedX * mBoidSpeedX + mBoidSpeedY * mBoidSpeedY);
        if (speed > mBoidSpeedMax)
        {
            mBoidSpeedX = (mBoidSpeedX / speed) * mBoidSpeedMax;
            mBoidSpeedY = (mBoidSpeedY / speed) * mBoidSpeedMin;
        }
        else if (speed < mBoidSpeedMax)
        {
            mBoidSpeedX = (mBoidSpeedX / speed) * mBoidSpeedMin;
            mBoidSpeedY = (mBoidSpeedY / speed) * mBoidSpeedMin;
        }

        mBoidPosition.x += mBoidSpeedX * 5 * GetFrameTime();
        mBoidPosition.y += mBoidSpeedY * 5 * GetFrameTime();


		//Screen teleportation
        if (mBoidPosition.x  > screenWidth)
        {
            mBoidPosition.x = mBoidRadius;
        }
        else if (mBoidPosition.x  < 0)
        {
            mBoidPosition.x = screenWidth - mBoidRadius;
        }
        else if (mBoidPosition.y  > screenHeight)
        {
            mBoidPosition.y = mBoidRadius;
        }
        else if (mBoidPosition.y  < 0)
        {
            mBoidPosition.y = screenHeight - mBoidRadius;
        }
    }

}

void Boid::Draw()
{
    //Draw boid
	DrawTexturePro(mBoidSprite, Rectangle{ 0, 0, 512, 512 }, Rectangle{ mBoidPosition.x - mBoidRadius, mBoidPosition.y - mBoidRadius, (float)mBoidRadius * 2, (float)mBoidRadius * 2 }, Vector2{ (float)mBoidRadius, (float)mBoidRadius }, mBoidActualAngle+180, mBoidColor);
}

float Boid::NormalizeAngle(float angle)
{
    while (angle > 180.0f) 
    {
        angle -= 360.0f;
    }
    while (angle < -180.0f) 
    {
        angle += 360.0f;
    }
    return angle;
}

void Boid::ObstacleAvoid(vector<Obstacle*> pObstacles)
{
    float close_dx = 0;
    float close_dy = 0;

    for (int i = 0; i < pObstacles.size(); i++)
    {
        if (pObstacles[i]->GetPosition().x - pObstacles[i]->GetSize().x < mBoidPosition.x + (mBoidMinimumDistance + mBoidRadius)
            && pObstacles[i]->GetPosition().x + pObstacles[i]->GetSize().x > mBoidPosition.x - (mBoidMinimumDistance + mBoidRadius)
            && pObstacles[i]->GetPosition().y - pObstacles[i]->GetSize().y < mBoidPosition.y + (mBoidMinimumDistance + mBoidRadius)
            && pObstacles[i]->GetPosition().y + pObstacles[i]->GetSize().y > mBoidPosition.y - (mBoidMinimumDistance + mBoidRadius))
        {
            close_dx += mBoidPosition.x - pObstacles[i]->GetPosition().x;
            close_dy += mBoidPosition.y - pObstacles[i]->GetPosition().y;
        }
    }

    mBoidSpeedX += close_dx * mAvoidFactor;
    mBoidSpeedY += close_dy * mAvoidFactor;
}

void Boid::Separation(vector<Boid*> pBoids, vector<Boid*> pFoodBoids, vector<Boid*> pEvilBoids)
{
    float close_dx = 0;
    float close_dy = 0;

    for (int i = 0; i < pBoids.size(); i++)
    {
        if (pBoids[i] != this)
        {
            float dist = sqrt(((pBoids[i]->mBoidPosition.x + mBoidRadius - mBoidPosition.x + mBoidRadius) * (pBoids[i]->mBoidPosition.x + mBoidRadius - mBoidPosition.x + mBoidRadius)) +
                ((pBoids[i]->mBoidPosition.y + mBoidRadius - mBoidPosition.y + mBoidRadius) * (pBoids[i]->mBoidPosition.y + mBoidRadius - mBoidPosition.y + mBoidRadius)));
            if (dist < mBoidMinimumDistance )
            {
                close_dx += mBoidPosition.x - pBoids[i]->mBoidPosition.x;
                close_dy += mBoidPosition.y - pBoids[i]->mBoidPosition.y;
            }
        }
    }

    mBoidSpeedX += close_dx * mAvoidFactor;
    mBoidSpeedY += close_dy * mAvoidFactor;

    close_dx = 0;
    close_dy = 0;
    for (int i = 0; i < pEvilBoids.size(); i++)
    {
        if (pEvilBoids[i] != this)
        {
            float dist = sqrt(((pEvilBoids[i]->mBoidPosition.x + mBoidRadius - mBoidPosition.x + mBoidRadius) * (pEvilBoids[i]->mBoidPosition.x + mBoidRadius - mBoidPosition.x + mBoidRadius)) +
                ((pEvilBoids[i]->mBoidPosition.y + mBoidRadius - mBoidPosition.y + mBoidRadius) * (pEvilBoids[i]->mBoidPosition.y + mBoidRadius - mBoidPosition.y + mBoidRadius)));
            if (dist < mBoidMaxPerceiveDistance)
            {
                close_dx += mBoidPosition.x - pEvilBoids[i]->mBoidPosition.x;
                close_dy += mBoidPosition.y - pEvilBoids[i]->mBoidPosition.y;
            }
        }
    }
    mBoidSpeedX += close_dx * mAvoidPredatorFactor;
    mBoidSpeedY += close_dy * mAvoidPredatorFactor;

    Alignment(pBoids, pFoodBoids);
    Cohesion(pBoids);
}

void Boid::Alignment(vector<Boid*> pBoids, vector<Boid*> pFoodBoids)
{
    float boidSpeedX_Avg = 0;
    float boidSpeedY_Avg = 0;
    float neighboringBoids = 0;

    for (int i = 0; i < pBoids.size(); i++)
    {
        if (pBoids[i] != this)
        {
            float dist = sqrt(((pBoids[i]->mBoidPosition.x + mBoidRadius - mBoidPosition.x + mBoidRadius) * (pBoids[i]->mBoidPosition.x + mBoidRadius - mBoidPosition.x + mBoidRadius)) +
                ((pBoids[i]->mBoidPosition.y + mBoidRadius - mBoidPosition.y + mBoidRadius) * (pBoids[i]->mBoidPosition.y + mBoidRadius - mBoidPosition.y + mBoidRadius)));
            if (dist < mBoidMaxPerceiveDistance)
            {
                boidSpeedX_Avg += pBoids[i]->mBoidSpeedX;
                boidSpeedY_Avg += pBoids[i]->mBoidSpeedY;
                neighboringBoids += 1;
            }
        }
    }

    if (neighboringBoids > 0) 
    {
        boidSpeedX_Avg = boidSpeedX_Avg / neighboringBoids;
        boidSpeedY_Avg = boidSpeedY_Avg / neighboringBoids;
    }

    mBoidSpeedX += (boidSpeedX_Avg - mBoidSpeedX) * mMatchingFactor;
    mBoidSpeedY += (boidSpeedY_Avg - mBoidSpeedY) * mMatchingFactor;



    boidSpeedX_Avg = 0;
    boidSpeedY_Avg = 0;
    neighboringBoids = 0;

    for (int i = 0; i < pFoodBoids.size(); i++)
    {
        if (pFoodBoids[i] != this)
        {
            float dist = sqrt(((pFoodBoids[i]->mBoidPosition.x + mBoidRadius - mBoidPosition.x + mBoidRadius) * (pFoodBoids[i]->mBoidPosition.x + mBoidRadius - mBoidPosition.x + mBoidRadius)) +
                ((pFoodBoids[i]->mBoidPosition.y + mBoidRadius - mBoidPosition.y + mBoidRadius) * (pFoodBoids[i]->mBoidPosition.y + mBoidRadius - mBoidPosition.y + mBoidRadius)));
            if (dist < mBoidMaxPerceiveDistance)
            {
                boidSpeedX_Avg += pFoodBoids[i]->mBoidSpeedX;
                boidSpeedY_Avg += pFoodBoids[i]->mBoidSpeedY;
                neighboringBoids += 1;
            }
            if (dist <= mBoidMinimumDistance)
            {
				pFoodBoids[i]->SetIsAlive(false);
            }
        }
    }

    if (neighboringBoids > 0)
    {
        boidSpeedX_Avg = boidSpeedX_Avg / neighboringBoids;
        boidSpeedY_Avg = boidSpeedY_Avg / neighboringBoids;
    }

    mBoidSpeedX += (boidSpeedX_Avg - mBoidSpeedX) * mFoodAttractFactor;
    mBoidSpeedY += (boidSpeedY_Avg - mBoidSpeedY) * mFoodAttractFactor;
}

void Boid::Cohesion(vector<Boid*> pBoids)
{
    float boidPosX_Avg = 0;
    float boidPosY_Avg = 0;
    float neighboringBoids = 0;

    for (int i = 0; i < pBoids.size(); i++)
    {
        if (pBoids[i] != this)
        {
            float dist = sqrt(((pBoids[i]->mBoidPosition.x + mBoidRadius - mBoidPosition.x + mBoidRadius) * (pBoids[i]->mBoidPosition.x + mBoidRadius - mBoidPosition.x + mBoidRadius)) +
                ((pBoids[i]->mBoidPosition.y + mBoidRadius - mBoidPosition.y + mBoidRadius) * (pBoids[i]->mBoidPosition.y + mBoidRadius - mBoidPosition.y + mBoidRadius)));
            if (dist < mCohesionRadius)
            {
                boidPosX_Avg += pBoids[i]->mBoidPosition.x;
                boidPosY_Avg += pBoids[i]->mBoidPosition.y;
                neighboringBoids += 1;
            }
        }
    }

    if (neighboringBoids > 0)
    {
        boidPosX_Avg = boidPosX_Avg / neighboringBoids;
        boidPosY_Avg = boidPosY_Avg / neighboringBoids;
    }

    mBoidSpeedX += (boidPosX_Avg - mBoidPosition.x) * mCenteringFactor;
    mBoidSpeedY += (boidPosY_Avg - mBoidPosition.y) * mCenteringFactor;
}
