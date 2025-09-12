#include "Boid.h"
#include "Math.h"
#include "Obstacle.h"

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
        mBoidPosition.x += mBoidSpeedX*5 * GetFrameTime();
        mBoidPosition.y += mBoidSpeedY*5 * GetFrameTime();

        /*float close_dx = 0;
        float close_dy = 0;

        if (mBoidPosition.x + mBoidRadius > screenWidth - 50)
        {
            //mBoidSpeedX *= -1;
            close_dx += mBoidPosition.x - (screenWidth + 50);
        }
        else if (mBoidPosition.x - mBoidRadius < 50)
        {
            //mBoidSpeedX *= -1;
            close_dx += mBoidPosition.x + 50;
        }
        if (mBoidPosition.y + mBoidRadius > screenHeight - 50)
        {
            //mBoidSpeedY *= -1;
            close_dy += mBoidPosition.y - (screenHeight + 50);
        }
        else if (mBoidPosition.y - mBoidRadius < 50)
        {
            //mBoidSpeedY *= -1;
            close_dy += mBoidPosition.y + 50;
        }

        mBoidSpeedX += close_dx * mAvoidFactor;
        mBoidSpeedY += close_dy * mAvoidFactor;*/

        /*float speed = sqrt(mBoidSpeedX * mBoidSpeedX + mBoidSpeedY * mBoidSpeedY);
        if (speed > mBoidSpeedMax)
        {
            mBoidSpeedX = (mBoidSpeedX / speed) * mBoidSpeedMax;
            mBoidSpeedY = (mBoidSpeedY / speed) * mBoidSpeedMin;
        }
        else if (speed < mBoidSpeedMax)
        {
            mBoidSpeedX = (mBoidSpeedX / speed) * mBoidSpeedMin;
            mBoidSpeedY = (mBoidSpeedY / speed) * mBoidSpeedMin;
        }*/

		//Screen bounds collision
        /*if (mBoidPosition.x + mBoidRadius > screenWidth)
        {
            mBoidSpeedX *= -1;
            mBoidPosition.x = screenWidth - mBoidRadius;
        }
        else if (mBoidPosition.x - mBoidRadius < 0)
        {
            mBoidSpeedX *= -1;
            mBoidPosition.x = mBoidRadius;
        }
        else if (mBoidPosition.y + mBoidRadius > screenHeight)
        {
            mBoidSpeedY *= -1;
            mBoidPosition.y = screenHeight - mBoidRadius;
        }
        else if (mBoidPosition.y - mBoidRadius < 0)
        {
            mBoidSpeedY *= -1;
            mBoidPosition.y = mBoidRadius;
        }*/

		//Screen teleportation
        if (mBoidPosition.x + mBoidRadius > screenWidth)
        {
            mBoidPosition.x = mBoidRadius;
        }
        else if (mBoidPosition.x - mBoidRadius < 0)
        {
            mBoidPosition.x = screenWidth - mBoidRadius;
        }
        else if (mBoidPosition.y + mBoidRadius > screenHeight)
        {
            mBoidPosition.y = mBoidRadius;
        }
        else if (mBoidPosition.y - mBoidRadius < 0)
        {
            mBoidPosition.y = screenHeight - mBoidRadius;
        }
    }


    //calcul angle
    //mBoidAngle = sin(mBoidSpeedX / sqrt(mBoidSpeedX * mBoidSpeedX + mBoidSpeedY * mBoidSpeedY))*90;
	mBoidAngle = atan2(mBoidSpeedY,mBoidSpeedX)*90;
}

void Boid::Draw()
{

    //Separation zone Debug
    //DrawCircleLines(mBoidPosition.x - mBoidRadius, mBoidPosition.y - mBoidRadius, mBoidMinimumDistance, RED);
	DrawLine(mBoidPosition.x, mBoidPosition.y, mBoidPosition.x + mBoidSpeedX , mBoidPosition.y + mBoidSpeedY , mBoidColor);
    //Cohesion zone Debug
    //DrawCircleLines(mBoidPosition.x - mBoidRadius, mBoidPosition.y - mBoidRadius, mCohesionRadius, ORANGE);
    //Alignement zone Debug
    //DrawCircleLines(mBoidPosition.x - mBoidRadius, mBoidPosition.y - mBoidRadius, mBoidMaxPerceiveDistance, GREEN);

    //Draw boid
	//DrawRectangle(mBoidPosition.x - mBoidRadius, mBoidPosition.y - mBoidRadius, mBoidRadius * 2, mBoidRadius * 2, mBoidColor);
	//DrawRectanglePro(Rectangle{ mBoidPosition.x - mBoidRadius, mBoidPosition.y - mBoidRadius, (float)mBoidRadius * 2, (float)mBoidRadius * 2 }, Vector2{ (float)mBoidRadius, (float)mBoidRadius }, mBoidAngle, mBoidColor);
    DrawTexturePro(mBoidSprite, Rectangle{ 0, 0, 512, 512 }, Rectangle{ mBoidPosition.x - mBoidRadius, mBoidPosition.y - mBoidRadius, (float)mBoidRadius * 2, (float)mBoidRadius * 2 }, Vector2{ (float)mBoidRadius, (float)mBoidRadius }, mBoidAngle+180, mBoidColor);

}

void Boid::CollideBoid()
{
}

void Boid::ObstacleAvoid(vector<Obstacle*> pObstacles)
{
    float close_dx = 0;
    float close_dy = 0;

    for (int i = 0; i < pObstacles.size(); i++)
    {
        if (pObstacles[i]->GetPosition().x - pObstacles[i]->GetSize().x < mBoidPosition.x + mBoidRadius 
            && pObstacles[i]->GetPosition().x + pObstacles[i]->GetSize().x > mBoidPosition.x
            && pObstacles[i]->GetPosition().y - pObstacles[i]->GetSize().y < mBoidPosition.y + mBoidRadius
            && pObstacles[i]->GetPosition().y + pObstacles[i]->GetSize().y > mBoidPosition.y)
        {
            close_dx += mBoidPosition.x - pObstacles[i]->GetPosition().x;
            close_dy += mBoidPosition.y - pObstacles[i]->GetPosition().y;
        }
    }

	//printf("close_dx: %f, close_dy: %f\n", close_dx, close_dy);

    mBoidSpeedX += close_dx * mAvoidFactor;
    mBoidSpeedY += close_dy * mAvoidFactor;

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
