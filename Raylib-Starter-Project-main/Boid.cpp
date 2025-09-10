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

	DrawCircleLines(mBoidPosition.x - mBoidRadius, mBoidPosition.y - mBoidRadius, mBoidDetectionZone, RED);
}

void Boid::CollideBoid()
{
}

void Boid::Separation(vector<Boid*> pBoids)
{
    for (int i = 0; i < pBoids.size(); i++)
    {
        if (pBoids[i] != this)
        {
            float dist = sqrt(((pBoids[i]->mBoidPosition.x + mBoidRadius - mBoidPosition.x + mBoidRadius) * (pBoids[i]->mBoidPosition.x + mBoidRadius - mBoidPosition.x + mBoidRadius)) +
                ((pBoids[i]->mBoidPosition.y + mBoidRadius - mBoidPosition.y + mBoidRadius) * (pBoids[i]->mBoidPosition.y + mBoidRadius - mBoidPosition.y + mBoidRadius)));
            if (dist < mBoidDetectionZone)
            {
                //debug
                //printf("Boid too close! Distance: %f\n", dist);

                mBoidSpeedX *= -1;
				mBoidSpeedY *= -1;

                /*if (mBoidPosition.x - mBoidDetectionZone < pBoids[i]->mBoidPosition.x + mBoidDetectionZone)
                {
                    mBoidSpeedX *= -1;
                    mBoidPosition.x = pBoids[i]->mBoidPosition.x + mBoidDetectionZone;
                }
                else if (mBoidPosition.x + mBoidDetectionZone < pBoids[i]->mBoidPosition.x - mBoidRadius)
                {
                    mBoidSpeedX *= -1;
                    mBoidPosition.x = pBoids[i]->mBoidPosition.x - mBoidDetectionZone;
                }
                if (mBoidPosition.y - mBoidDetectionZone < pBoids[i]->mBoidPosition.y + mBoidDetectionZone)
                {
                    mBoidSpeedY*= -1;
                    mBoidPosition.y = pBoids[i]->mBoidPosition.y + mBoidDetectionZone;
                }
                else if (mBoidPosition.y + mBoidDetectionZone < pBoids[i]->mBoidPosition.y - mBoidRadius)
                {
                    mBoidSpeedY *= -1;
                    mBoidPosition.y = pBoids[i]->mBoidPosition.y - mBoidDetectionZone;
                }*/


                mBoidPosition.x += mBoidSpeedX;
                mBoidPosition.y += mBoidSpeedY;
            }
        }
    }
}
