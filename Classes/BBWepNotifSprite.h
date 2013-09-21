#ifndef BeatBlaster_BBWepNotifSprite_h
#define BeatBlaster_BBWepNotifSprite_h
 
#define BBK_WepNotifSprite_xVelocity (-800 / 6.0f)

#include "cocos2d.h"
 
USING_NS_CC;
 
class BBWepNotifSprite : public CCSprite {
	virtual void draw(void);
	virtual void updateTransform(void);
	public :
		void update(float dt);
		
		

	BBWepNotifSprite();
	static BBWepNotifSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
	static BBWepNotifSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);

};
 
#endif