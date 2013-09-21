#ifndef BeatBlaster_BBSimpleBulletSprite_h
#define BeatBlaster_BBSimpleBulletSprite_h
 
#define BBK_BBSimpleBulletSprite_xVelocity (-800 / 6.0f)

#include "cocos2d.h"

USING_NS_CC;
 
class BBSimpleBulletSprite : public CCSprite {
	virtual void draw(void);
public :
		void update(float dt);
		void setVelocity(CCPoint vel);
		void addVelocity(CCPoint vel);
		CCPoint getVelocity();



	BBSimpleBulletSprite();
	static BBSimpleBulletSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
	static BBSimpleBulletSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);


private :
	float m_xVel; //xVelocity
	float m_yVel; //yVelocity
};
 
#endif