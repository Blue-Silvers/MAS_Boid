#include "Obstacle.h"


Obstacle::Obstacle(Vector2 pObstaclePosition, Vector2 pObstacleSize)
{
	mObstaclePosition = pObstaclePosition;
	mObstacleSize = pObstacleSize;
}

void Obstacle::Draw()
{
    //Draw boid
    DrawRectanglePro(Rectangle{ mObstaclePosition.x, mObstaclePosition.y, (float)mObstacleSize.x*2, (float)mObstacleSize.y*2}, mObstacleSize, mObstacleAngle, mObstacleColor);
}