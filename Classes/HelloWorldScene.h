#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"
#include "CCParallaxNodeExtras.h"
#include "BBCommonResources.h"
#include "BBWepNotifSprite.h"
#include "BBSimpleBulletSprite.h"

//TEMPORARY
#define KSIZE_WEPNOTIFICATIONheight 40
#define KSIZE_SimpleBulletWidth 61


USING_NS_CC;

class HelloWorld : public cocos2d::CCLayer
{
private:
	CCSpriteBatchNode * _batchNode;
	CCSprite * _ship;

	//PARALLAX AND BG STUFF
	CCParallaxNodeExtras *_backgroundNode;
	CCSprite *_spacedust1;
	CCSprite *_spacedust2;
	CCSprite *_planetsunrise;
	CCSprite *_galaxy;
	CCSprite *_spacialanomaly;
	CCSprite *_spacialanomaly2;

	// General Scene Info
	float m_startMillis;
	float m_elapsedSeconds;

	// Touch Info
	CCPoint m_mostRecentTouchLocation;
	bool m_isBeingTouched;

	// Update Functions
	void update_ParralaxNodeExtra();
	void update_Asteroids();
	void doSimpleBulletHitDetect();
	void doSimpleBulletSpawning();
	void doWeaponNotificationSpawning();
	///
	void update_WepNotifs(float dt);
	void update_SimpleBullets(float dt);


	// Helper Functions
	CCSprite* helper_spawnSimpleBullet(CCPoint startPos, float velocity, float degrees, float secondsOverdue);
	CCSprite* helper_spawnWeaponNotification(float DURATION, CCPoint startPos, float rotation);

	void freeSimpleBullet(CCNode * node);
	void freeWeaponNotification(CCNode * node);
	void freeAsteroid(CCNode * node);

	// SIMPLE BULLETS
	CCArray* m_simpleBullets;//SPRITES!
	int m_nextSimpleBullet; //index of next ship projectile sprite from m_shipProjectiles to be used
	int m_numActiveSimpleBullets; //
	float* m_simpleBulletFireTimes; //time that a new SP needs to start (for now just BEAT TIMES, in seconds)
	int m_simpleBulletFireTimeNext; //waiting for time at this index before FIRING next bullet (make visible at front of ship)

	// Weapon Notification
	CCPoint m_weaponNotificationStartPoint;
	int m_weaponNotificationHeight;
	CCLayerColor* m_weaponNotificationLayer;//LAYER!
	CCArray* m_wepNotifs;//SPRITES!
	int m_nextWepNotif;//index of next weapon notification sprite from m_wepNotifs to be used
	int m_numActiveWeaponNotifications; //
	float* m_wepNotifFireTimes; //time that a new WN needs to finish (for now just BEAT TIMES, in seconds)
	int m_wepNotifFireTimeNext; //waiting for time at this index before STARTING next weapon (making visible on right side)


	// ASTEROID STUFF
	int m_numActiveAsteroids; //
	CCArray * _asteroids ;
	int _nextAsteroid ;
	float _nextAsteroidSpawn ;
	//CCArray * _shipLasers ; 
	//int _nextShipLaser ;
	int _lives ;

	void update(float dt);

public:
	CCLabelTTF* m_LABEL1;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);


	virtual void didAccelerate(CCAcceleration* pAccelerationValue);
	virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
	virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
	virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);


	float randomValueBetween( float low , float high );
	void setInvisible(CCNode * node);
	float getTimeTick();
};

#endif  // __HELLOWORLD_SCENE_H__