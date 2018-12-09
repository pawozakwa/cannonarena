#include "Utils.h"
#include <math.h>
#include <iostream>

#define PI 3.14159

Utils::Utils(void)
{
}


Utils::~Utils(void)
{
}


float Utils::getVectorAngle(float x, float y){
	float angle = atan2(x,y);
	angle = PI - angle;
	
	return angle;
	
	//return PI - atan2(x,y);
	
	
}