#include "BBCommonResources.h"
#include <fstream>
#include <iostream>
using namespace std;

USING_NS_CC;


float BB_getMillis() {
	timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long millisecs = (tv.tv_sec * 1000) + (tv.tv_usec/1000);
    return (float) millisecs;
}

CCPoint BBconvert_AngleAndMagnitudeToVelocityXY(float degrees, float velocityMagnitude){
	float radians = degrees * 0.0174532925;
	return ccp(velocityMagnitude * cos(radians),
			velocityMagnitude * sin(radians) );
}

bool isMovingOffScreen(CCPoint pos, CCPoint vel, CCSize size){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	if(vel.x > 0.0f){//moving right
		if(pos.x + size.width/2 > winSize.width) return true;
	}else if(vel.x < 0.0f){//moving left
		if(pos.x + size.width/2 < 0.0f) return true;
	}

	if(vel.y > 0.0f){//moving up
		if(pos.y + size.height/2 > winSize.height) return true;
	}else if(vel.y < 0.0f){//moving down
		if(pos.x + size.height/2 < 0.0f) return true;
	}
	return false;
}