#include "Boid.h"
#include "Math.h"

Boid::Boid(Vector2 pBoidPosition)
{
	mBoidPosition = pBoidPosition;
}

void Boid::Update(int screenWidth, int screenHeight)
{
    if (IsKeyDown(KEY_SPACE))
    {
        mBoidLunched = true;
    }

    if (mBoidLunched)
    {
        mBoidPosition.x += mBoidSpeedX;
        mBoidPosition.y += mBoidSpeedY;


		//Screen bounds collision
        if (mBoidPosition.x + mBoidRadius > screenWidth)
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
            mBoidPosition.y = mBoidRadius ;
        }
    }
    // arcsin(a / c)
    mBoidAngle = sin(mBoidSpeedX / sqrt(mBoidSpeedX * mBoidSpeedX + mBoidSpeedY * mBoidSpeedY)) * 90;

}

void Boid::Draw()
{
	//DrawRectangle(mBoidPosition.x - mBoidRadius, mBoidPosition.y - mBoidRadius, mBoidRadius * 2, mBoidRadius * 2, mBoidColor);
	DrawRectanglePro(Rectangle{ mBoidPosition.x - mBoidRadius, mBoidPosition.y - mBoidRadius, (float)mBoidRadius * 2, (float)mBoidRadius * 2 }, Vector2{ (float)mBoidRadius, (float)mBoidRadius }, mBoidAngle, mBoidColor);

	DrawCircleLines(mBoidPosition.x - mBoidRadius, mBoidPosition.y - mBoidRadius, mBoidMinimumDistance , RED);
    DrawCircleLines(mBoidPosition.x - mBoidRadius, mBoidPosition.y - mBoidRadius, mBoidMaxPerceiveDistance, GREEN);
}

void Boid::CollideBoid()
{
}

void Boid::Separation(vector<Boid*> pBoids)
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
                //debug
                //printf("Boid too close! Distance: %f\n", dist);

                //mBoidSpeedX *= -1;
				//mBoidSpeedY *= -1;
                close_dx += mBoidPosition.x - pBoids[i]->mBoidPosition.x;
                close_dy += mBoidPosition.y - pBoids[i]->mBoidPosition.y;


                //mBoidPosition.x += mBoidSpeedX;
                //mBoidPosition.y += mBoidSpeedY;

            }
        }
    }

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

    Alignment(pBoids);
}

void Boid::Alignment(vector<Boid*> pBoids)
{
    /*At the start of the update for a particular boid, three variables(xvel_avg, yvel_avg, and neighboring_boids) are zeroed
        We loop thru every other boid.If the distance to a particular boid is less than the visible range, then
        xvel_avg += otherboid.vx
        yvel_avg += otherboid.vy
        neighboring_boids += 1
        Once we've looped through all other boids, we do the following if neighboring_boids>0:
        xvel_avg = xvel_avg / neighboring_boids
        yvel_avg = yvel_avg / neighboring_boids
        We then update the velocity according to :
    boid.vx += (xvel_avg - boid.vx) * matchingfactor
        boid.vy += (yvel_avg - boid.vy) * matchingfactor
        (where matchingfactor is a tunable parameter)*/

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
                //debug
                //printf("Boid too close! Distance: %f\n", dist);

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

    mBoidSpeedX += (boidSpeedX_Avg - mBoidSpeedX) * mMatchingfactor;
    mBoidSpeedY += (boidSpeedY_Avg - mBoidSpeedY) * mMatchingfactor;
}
