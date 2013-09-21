#include "HelloWorldScene.h"


using namespace CocosDenshion;
using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::create();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}
void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}
bool HelloWorld::init()
{

    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
        this->addChild(pMenu, 1);

        // 2. Add a label
		m_LABEL1 = CCLabelTTF::create("m_LABEL1", "Arial", 24);
		CC_BREAK_IF(! m_LABEL1);
        m_LABEL1->setPosition(ccp(winSize.width / 4, winSize.height - 50));
        this->addChild(m_LABEL1, 9000);



		// --- ADD BATCHNODE  and ship ---------------------------------------------------------------
		_batchNode = CCSpriteBatchNode::create("Sprites.pvr.ccz");
		this->addChild(_batchNode, 9999);
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Sprites.plist");
		_ship = CCSprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
		_ship->setPosition(ccp(winSize.width * 0.1, winSize.height * 0.5));
		_batchNode->addChild(_ship, 1);

		// ---  PARALAX ---------------------------------------------------------------
		// 1) Create the CCParallaxNode
		_backgroundNode = CCParallaxNodeExtras::create(); //1 
		this->addChild(_backgroundNode,-1) ;
		// 2) Create the sprites we'll add to the CCParallaxNode
		_spacedust1 = CCSprite::create("bg_front_spacedust.png");
		_spacedust2 = CCSprite::create("bg_front_spacedust.png");
		_planetsunrise = CCSprite::create("bg_planetsunrise.png");
		_galaxy = CCSprite::create("bg_galaxy.png");
		_spacialanomaly = CCSprite::create("bg_spacialanomaly.png");
		_spacialanomaly2 = CCSprite::create("bg_spacialanomaly2.png"); 
		// 3) Determine relative movement speeds for space dust and background
		CCPoint dustSpeed = ccp(0.1, 0.1);
		CCPoint bgSpeed = ccp(0.05, 0.05);
		// 4) Add children to CCParallaxNode
		_backgroundNode->addChild(_spacedust1, 0 , dustSpeed , ccp(0,winSize.height/2) ); // 2
		_backgroundNode->addChild(_spacedust2, 0 , dustSpeed , ccp( _spacedust1->getContentSize().width,winSize.height/2)); 
		_backgroundNode->addChild(_galaxy,-1, bgSpeed , ccp(0,winSize.height * 0.7));
		_backgroundNode->addChild(_planetsunrise,-1 , bgSpeed,ccp(600,winSize.height * 0));   
		_backgroundNode->addChild(_spacialanomaly,-1, bgSpeed,ccp(900,winSize.height * 0.3));        
		_backgroundNode->addChild(_spacialanomaly2,-1, bgSpeed,ccp(1500,winSize.height * 0.9));

		// --- STARS ---------------------------------------------------------------------
		HelloWorld::addChild(CCParticleSystemQuad::create("Stars1.plist")) ;
		HelloWorld::addChild(CCParticleSystemQuad::create("Stars2.plist")) ;
		HelloWorld::addChild(CCParticleSystemQuad::create("Stars3.plist")) ;

		// --- ASTEROIDS -----------------------------------------------------------------
		#define KNUMASTEROIDS 100
		_asteroids = CCArray::create();
		_asteroids->retain(); // NO AUTO-RELEASE
		for(int i = 0; i < KNUMASTEROIDS; ++i) {
			CCSprite *asteroid = CCSprite::createWithSpriteFrameName("asteroid.png");
			asteroid->setVisible(false);
			_batchNode->addChild(asteroid);
			_asteroids->addObject(asteroid);
		}
		m_numActiveAsteroids =0;
		_nextAsteroid = 0;

		
#define KNUM_TimeBetweenBullets 0.15f
#define KNUM_TimeBetweenBulletBursts 1.50f
#define KNUM_BulletsPerBurst 5

		// --- SHIP PROJECTILE FIRE TIMES ----------------------------------------------------------
		m_simpleBulletFireTimes = new float[9999];
		for(int i=KNUM_BulletsPerBurst; i<9000; i+=KNUM_BulletsPerBurst){
			for(int j=0; j<KNUM_BulletsPerBurst; j++){
				m_simpleBulletFireTimes[i+j-KNUM_BulletsPerBurst] = (i*KNUM_TimeBetweenBulletBursts)/KNUM_BulletsPerBurst  +  j*KNUM_TimeBetweenBullets;
			}
		}
		m_simpleBulletFireTimeNext = 0;

		// --- SHIP PROJECTILES --------------------------------------------------------------------
		#define KMAX_SimpleBullets 750
		m_simpleBullets = CCArray::create();
		m_simpleBullets->retain();
		for(int i = 0; i < KMAX_SimpleBullets; ++i) {
			BBSimpleBulletSprite *simpleBullet = BBSimpleBulletSprite::createWithSpriteFrameName("laserbeam_blue.png");
			simpleBullet->setVisible(false) ;
			_batchNode->addChild(simpleBullet);
			m_simpleBullets->addObject(simpleBullet);
		}
		m_nextSimpleBullet = 0;
		m_numActiveSimpleBullets = 0;


		#define KORD_WEAPONNOTIFICATION 10
		// --- WEAPON NOTIFICATION FIRE TIMES ----------------------------------------------------------
		m_wepNotifFireTimes = new float[9999];
		for(int i=KNUM_BulletsPerBurst; i<9000; i+=KNUM_BulletsPerBurst){
			for(int j=0; j<KNUM_BulletsPerBurst; j++){
				m_wepNotifFireTimes[i+j-KNUM_BulletsPerBurst] = (i*KNUM_TimeBetweenBulletBursts)/KNUM_BulletsPerBurst  +  j*KNUM_TimeBetweenBullets;
			}
		}
		//for(int i=0; i<100; i++) CCLog("weptime[%i]=%f", i, m_wepNotifFireTimes[i]);
		m_wepNotifFireTimeNext = 0;
		// --- WEAPON NOTIFICATION BAR -------------------------------------------------------------
		m_weaponNotificationStartPoint = ccp( winSize.width, winSize.height - KSIZE_WEPNOTIFICATIONheight/2);
		m_weaponNotificationLayer = CCLayerColor::create(ccc4(100,0,100,100));
		m_weaponNotificationLayer->setContentSize( CCSizeMake(winSize.width, KSIZE_WEPNOTIFICATIONheight) );
		m_weaponNotificationLayer->setPosition( ccp(0, winSize.height - (KSIZE_WEPNOTIFICATIONheight) ) );
		addChild(m_weaponNotificationLayer, KORD_WEAPONNOTIFICATION-5);
		//m_weaponNotificationLayer->setVisible(false);///////////////////////////////////////////////////////////////////////////

		#define KMAX_WEAPONNOTIFICATIONS 250
		m_wepNotifs = CCArray::create();
		m_wepNotifs->retain();
		for(int i = 0; i < KMAX_WEAPONNOTIFICATIONS; ++i) {
			BBWepNotifSprite *wepNotif = BBWepNotifSprite::createWithSpriteFrameName("laserbeam_blue.png");
			wepNotif->setVisible(false) ;
			_batchNode->addChild(wepNotif, KORD_WEAPONNOTIFICATION+5);
			m_wepNotifs->addObject(wepNotif);
			wepNotif->setScaleX(0.1f);
		}
		m_nextWepNotif = 0;
		m_numActiveWeaponNotifications = 0;
		// setup notifications already on screen
		#define KNUM_WEPNOTIFDURATION 6.0f
		for(int i=0; i<KMAX_WEAPONNOTIFICATIONS; i++){
			float nextWepNotifTime = m_wepNotifFireTimes[m_wepNotifFireTimeNext];
			float fractionOfDurationRemaining = nextWepNotifTime / KNUM_WEPNOTIFDURATION;

			if(fractionOfDurationRemaining < 1.00f){
				float xOffset = (1 - fractionOfDurationRemaining) * -winSize.width; //offset from default position
				helper_spawnWeaponNotification(KNUM_WEPNOTIFDURATION, ccpAdd( m_weaponNotificationStartPoint,
																	  ccp(xOffset, 0.0f) ),  90.0f);
				m_wepNotifFireTimeNext++;
			}else{
				//no need to go through all WEPNOTIF sprites (slight loadtime speedup)
				break;
			}
		}


		//--- Initialize Misc. ---------------------------------------------------------------------
		this->m_startMillis = -1.0f;
		this->m_elapsedSeconds = 0.0f;
		this->setTouchEnabled(true) ;
		this->scheduleUpdate();
		this->setAccelerometerEnabled(true);
        bRet = true;
    } while (0);

	this->setAccelerometerEnabled(false);

    return bRet;
}// INIT()

float lastWepNotifOverdueSec = 0.0f;
float lastWepNotifStartX = 0.0f;
CCSprite * lastWepNotif= NULL;

float xPosAt(float elapsedSinceStart){
	float frac = elapsedSinceStart / KNUM_WEPNOTIFDURATION;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	return frac * winSize.width;
}


void HelloWorld::update_ParralaxNodeExtra(){
	 CCArray *spaceDusts = CCArray::createWithCapacity(2) ;
	spaceDusts->addObject(_spacedust1) ;
	spaceDusts->addObject(_spacedust2) ;
	for ( int ii = 0  ; ii <spaceDusts->count() ; ii++ ) {
		CCSprite * spaceDust = (CCSprite *)(spaceDusts->objectAtIndex(ii)) ;
		float xPosition = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).x  ;
		float size = spaceDust->getContentSize().width ;
		if ( xPosition < -size ) {
			//_backgroundNode->incrementOffset(ccp(spaceDust->getContentSize().width*2,0),spaceDust) ; 
		}                                   
	}
 
	CCArray *backGrounds = CCArray::createWithCapacity(4) ;
	backGrounds->addObject(_galaxy) ;
	backGrounds->addObject(_planetsunrise) ;
	backGrounds->addObject(_spacialanomaly) ;
	backGrounds->addObject(_spacialanomaly2) ;
	for ( int ii = 0 ; ii <backGrounds->count() ; ii++ ) {
		CCSprite * background = (CCSprite *)(backGrounds->objectAtIndex(ii)) ;
		float xPosition = _backgroundNode->convertToWorldSpace(background->getPosition()).x ;
		float size = background->getContentSize().width ;
		if ( xPosition < -size ) {
			//_backgroundNode->incrementOffset(ccp(2000,0),background) ; 
		}
	}

}
void HelloWorld::update_Asteroids(){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	float curTimeMillis = getTimeTick();
	if (curTimeMillis > _nextAsteroidSpawn) {
		m_numActiveAsteroids++;

		float randMillisecs = randomValueBetween(0.03,0.10) * 1000;
		_nextAsteroidSpawn = randMillisecs + curTimeMillis;
 
		float randY = randomValueBetween(0.0,winSize.height);
		float randDuration = randomValueBetween(2.0,10.0);
 
		CCObject *obj = _asteroids->objectAtIndex(_nextAsteroid);
		CCSprite *asteroid = dynamic_cast<CCSprite*>( obj );
		_nextAsteroid++;
 
		if (_nextAsteroid >= _asteroids->count())
			_nextAsteroid = 0;

		asteroid->stopAllActions();
		asteroid->setPosition( ccp(winSize.width+asteroid->getContentSize().width/2, randY));
		asteroid->setVisible(true) ;
		asteroid->runAction ( CCSequence::create( 
			CCMoveBy::create(randDuration,ccp(-winSize.width-asteroid->getContentSize().width,0)) , 
			CCCallFuncN::create(this,callfuncN_selector(HelloWorld::freeAsteroid)) ,
			NULL // DO NOT FORGET TO TERMINATE WITH NULL (unexpected in C++)
			) ) ;        
	}
}
void HelloWorld::doSimpleBulletHitDetect(){
		// Asteroids
		CCArray *simpleBulletsToDelete = new CCArray;
		CCObject *isAsteroid = NULL, *isSimpleBullet = NULL;
		CCARRAY_FOREACH( _asteroids, isAsteroid ){
			CCSprite * asteroid = dynamic_cast<CCSprite*>(isAsteroid);
			if( asteroid->isVisible() ){
						CCRect asteroidRect = CCRectMake(
							asteroid->getPosition().x - (asteroid->getContentSize().width/2),
							asteroid->getPosition().y - (asteroid->getContentSize().height/2),
							asteroid->getContentSize().width,
							asteroid->getContentSize().height);

						//CCArray* lasersToDelete = new CCArray;
						CCARRAY_FOREACH( m_simpleBullets, isSimpleBullet) {
							CCSprite * simpleBullet = dynamic_cast<CCSprite*>(isSimpleBullet);
							if( simpleBullet->isVisible() ){
								CCRect laserRect = CCRectMake(
									simpleBullet->getPosition().x - (simpleBullet->getContentSize().width/2),
									simpleBullet->getPosition().y - (simpleBullet->getContentSize().height/2),
									simpleBullet->getContentSize().width,
									simpleBullet->getContentSize().height);

								if( asteroidRect.intersectsRect(laserRect)){
									simpleBulletsToDelete->addObject(simpleBullet);
									freeAsteroid(asteroid);
									freeSimpleBullet(simpleBullet);
								}
							}
						}//done going through lasers, delete lasersToDelete
			}
		}//done going through asteroids, delete asteroidsToDelete
		simpleBulletsToDelete->release();
}
void HelloWorld::doSimpleBulletSpawning(){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	#define KNUM_bulletDurationMillis 1750.0f
	#define KNUM_bulletDuration 1.750f
	#define KNUM_bulletVelocityMag (800.0/ KNUM_bulletDuration)

	float millisOverdue = m_elapsedSeconds - m_simpleBulletFireTimes[m_simpleBulletFireTimeNext];

	while(millisOverdue >= 0.0f){
		//CCLog("SP overdue by %f   -  %f,   %f,  %i", millisOverdue, m_elapsedSeconds, m_simpleBulletFireTimes[m_simpleBulletFireTimeNext],  m_simpleBulletFireTimeNext);
		float fractionOverdue = (millisOverdue / KNUM_bulletDurationMillis);
		m_simpleBulletFireTimeNext++;

		//--- FIRE ANGLE --------------------------------------------
		/*** spawn 1 bullet for each angle in degreeSet ***/
		float degreeSet[] = {40,20,10,0,-10,-20,-40};
		for(int i=0; i< sizeof(degreeSet)/sizeof(degreeSet[0]); i++){
				helper_spawnSimpleBullet( _ship->getPosition(),
										KNUM_bulletVelocityMag,
										degreeSet[i],
										millisOverdue/1000.0f);
		}




		//--- SETUP CHECK FOR NEXT OVERDUE,  LAG SPIKE might cause us to miss two
		millisOverdue = m_elapsedSeconds - m_simpleBulletFireTimes[m_simpleBulletFireTimeNext];
	}
	
}
void HelloWorld::doWeaponNotificationSpawning(){
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	float secondsOverdue = m_elapsedSeconds - (m_wepNotifFireTimes[m_wepNotifFireTimeNext] - KNUM_WEPNOTIFDURATION);
	
	while(secondsOverdue >= -1.0f ){
		float fractionOverdue = (secondsOverdue / KNUM_WEPNOTIFDURATION);
		
		//--- SPAWN NEW WEAPON NOTIFICATION
		float xOverdue = fractionOverdue * -winSize.width;
		float rotation = 90.0f;//make laser vertical...

		/*
		CCSprite* newWepNotif =	helper_spawnWeaponNotification( KNUM_WEPNOTIFDURATION,   ccpAdd(m_weaponNotificationStartPoint, ccp(xOverdue,0.0f)) ,
													ccp(-winSize.width, 0),  rotation);
		*/

		float timeDiff = m_wepNotifFireTimes[m_wepNotifFireTimeNext] - m_wepNotifFireTimes[m_wepNotifFireTimeNext-1];
		float distancePerSec = winSize.width/ KNUM_WEPNOTIFDURATION;
		float distToAdd  = distancePerSec * timeDiff;
		/*
		CCLog("distTOAdd=%f,  timeDiff=%f", distToAdd, timeDiff);
				CCSprite* newWepNotif =	helper_spawnWeaponNotification( KNUM_WEPNOTIFDURATION,   ccpAdd(lastWepNotif->getPosition(),
																									ccp(distToAdd,0.0f)) , rotation);
	*/




		if(m_nextWepNotif == 18){
			CCLog("ooolala");
		}
		//--- SETUP CHECK FOR NEXT OVERDUE,  LAG SPIKE might cause us to miss two
		m_wepNotifFireTimeNext++;
		secondsOverdue = m_elapsedSeconds - (m_wepNotifFireTimes[m_wepNotifFireTimeNext] - KNUM_WEPNOTIFDURATION);
	}
}

void HelloWorld::update_WepNotifs(float dt){
	for(int i=0; i<KMAX_WEAPONNOTIFICATIONS; i++){
		CCObject *obj = m_wepNotifs->objectAtIndex(i);
		BBWepNotifSprite *wepNotif = dynamic_cast<BBWepNotifSprite*>( obj );

		if(wepNotif->isVisible()){
			wepNotif->update(dt);

			if( wepNotif->getPosition().x + wepNotif->getContentSize().width/2 < 0.0f){
				freeWeaponNotification(wepNotif);
			}
		}
	}
}
void HelloWorld::update_SimpleBullets(float dt){
	for(int i=0; i<KMAX_SimpleBullets; i++){
		CCObject *obj = m_simpleBullets->objectAtIndex(i);
		BBSimpleBulletSprite *simpleBullet = dynamic_cast<BBSimpleBulletSprite*>( obj );

		if(simpleBullet->isVisible()){
			simpleBullet->update(dt);

			if( isMovingOffScreen( simpleBullet->getPosition(), simpleBullet->getVelocity(), simpleBullet->getContentSize() )){
				freeSimpleBullet(simpleBullet);
			}
		}
	}
}




//Helper Function
// Overdue = distance from default position (front of ship), to start animation
// DisplacementFromOverdue = distance from default position (front of ship), to end animation
CCSprite* HelloWorld::helper_spawnSimpleBullet(CCPoint startPos, float velocity, float degrees, float secondsOverdue){

	BBSimpleBulletSprite *newSimpleBullet = dynamic_cast<BBSimpleBulletSprite*>(m_simpleBullets->objectAtIndex(m_nextSimpleBullet++));
	if ( m_nextSimpleBullet >= m_simpleBullets->count() )
		m_nextSimpleBullet = 0;
	newSimpleBullet->setVisible(true) ;
	newSimpleBullet->stopAllActions() ;


	CCPoint vel = BBconvert_AngleAndMagnitudeToVelocityXY(degrees, velocity);
	newSimpleBullet->setPosition(  ccpAdd(startPos, ccp(vel.x*secondsOverdue + KSIZE_SimpleBulletWidth/2, vel.y*secondsOverdue))  );
	newSimpleBullet->setVelocity( vel );
	newSimpleBullet->setRotation(-degrees);
		m_numActiveSimpleBullets++; //keep count for debug purposes

	return newSimpleBullet;
}
CCSprite* HelloWorld::helper_spawnWeaponNotification(float DURATION, CCPoint startPos, float rotation){
	m_numActiveWeaponNotifications++; //keep count for debug purposes
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	CCSprite *newWepNotif = dynamic_cast<CCSprite*>(m_wepNotifs->objectAtIndex(m_nextWepNotif++));
	if ( m_nextWepNotif >= m_wepNotifs->count() )
		m_nextWepNotif = 0;

	newWepNotif->setRotation( rotation );
	newWepNotif->setPosition( startPos );
	newWepNotif->setVisible(true) ;
	newWepNotif->stopAllActions() ;



	// TESTING
	/*
if(lastWepNotif != NULL){
	float diffX = lastWepNotif->getPosition().x   -  newWepNotif->getPosition().x;
	if(diffX < -100) CCLog("BREAK\n");


		CCLog("DIFFX=%f  curX(new:%f,last:%f) elapsed:%f,  ind:%i",
			20 + diffX,
			newWepNotif->getPosition().x,
			lastWepNotif->getPosition().x,
			m_elapsedSeconds,
			m_nextWepNotif-1
			);
	
}
lastWepNotif = newWepNotif;
lastWepNotifStartX = newWepNotif->getPosition().x;
*/











	return newWepNotif;
}

void HelloWorld::update(float dt) {

	if(m_startMillis < 0.0f){
		m_startMillis = BB_getMillis();
		m_elapsedSeconds = 0.0f;
		//CCLog("DELTA=======================%f", dt);
	}else{
		m_elapsedSeconds = (BB_getMillis() - m_startMillis) /1000.0f;
	}


	/*
	float r =randomValueBetween(0 ,100);
	if( r > 90){
		Sleep(randomValueBetween(90,900) );
	}
	*/


	update_WepNotifs(dt);
	update_SimpleBullets(dt);


  //Scroll background
	CCPoint backgroundScrollVert = ccp(-1000,0);
	_backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(),ccpMult(backgroundScrollVert, dt)));
  //ParallaxNodeExtra
  update_ParralaxNodeExtra();


 

	// --- SHIP MOVEMENT -------------------------------------------------------------------------------------
	#define KNUM_THUMBSIZE 40
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	/// THESE MIN/MAX ARE CONSTANT (based on ship and screen size),  move somewhere else
	float maxX = winSize.width - _ship->getContentSize().width/3;
	float minX = _ship->getContentSize().width/4;
	float maxY = winSize.height - KSIZE_WEPNOTIFICATIONheight - _ship->getContentSize().height/3;
	float minY = _ship->getContentSize().height/3;

	float newX = m_mostRecentTouchLocation.x + KNUM_THUMBSIZE;
	float newY = m_mostRecentTouchLocation.y;
	newX = MIN(MAX(newX, minX), maxX);
	newY = MIN(MAX(newY, minY), maxY);
	_ship->setPosition(ccp(newX, newY));



	// --- ASTEROIDS ------------------------------------------------------------------------------------
	update_Asteroids();
	
	

	// --- SHIP PROJECTILE HIT_DETECT ------------------------------------------------------------------------------
	doSimpleBulletHitDetect();

		//--- PLAYER SHIP HIT DETECTION (with asteroid)
		/* 
			if ( CCRect::CCRectIntersectsRect(_ship->boundingBox(), asteroid->boundingBox()) ) {
				asteroid->setIsVisible(false) ;
				_ship->runAction( CCBlink::actionWithDuration(1.0, 9)) ;
				_lives-- ;
			}
		*/







	//--- SHIP PROJECTILE SPAWN
	doSimpleBulletSpawning();


	//--- WEAPON NOTIFICATION SPAWN
	doWeaponNotificationSpawning();




	//--- SET LABEL TEXT
	char text[256];
	sprintf(text,"lasers=%i  asteroids=%i    +others=%i", m_numActiveSimpleBullets, m_numActiveAsteroids,
					m_numActiveSimpleBullets+ m_numActiveAsteroids + m_numActiveWeaponNotifications);
	m_LABEL1->setString(text);
}




void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue) {
	/*
#define KFILTERINGFACTOR 0.1
#define KRESTACCELX -0.6
#define KSHIPMAXPOINTSPERSEC (winSize.height*0.5)        
#define KMAXDIFFX 0.2
 
  double rollingX ;
 
  // Cocos2DX inverts X and Y accelerometer depending on device orientation
  // in landscape mode right x=-y and y=x !!! (Strange and confusing choice)
  pAccelerationValue->x = pAccelerationValue->y ;
  rollingX = (pAccelerationValue->x * KFILTERINGFACTOR) + (rollingX * (1.0 - KFILTERINGFACTOR));    
  float accelX = pAccelerationValue->x - rollingX ;
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  float accelDiff = accelX - KRESTACCELX;
  float accelFraction = accelDiff / KMAXDIFFX;
  _shipPointsPerSecY = KSHIPMAXPOINTSPERSEC * accelFraction;   
  */
}
float HelloWorld::randomValueBetween( float low , float high ) {
  return (((float) std::rand() / RAND_MAX) * (high-low)) + low;
}
float HelloWorld::getTimeTick() {
  timeval time;
  gettimeofday(&time, NULL);
  unsigned long millisecs = (time.tv_sec * 1000) + (time.tv_usec / 1000);

  return (float) millisecs;
}
void HelloWorld::setInvisible(CCNode * node) {
  node->setVisible(false) ;
}
void HelloWorld::freeSimpleBullet(CCNode * node){
	if(node->isVisible()){
		m_numActiveSimpleBullets--;
		node->setVisible(false) ;
	}
}
void HelloWorld::freeWeaponNotification(CCNode * node){
	if(node->isVisible()){
		m_numActiveWeaponNotifications--;
		node->setVisible(false) ;
	}
}
void HelloWorld::freeAsteroid(CCNode * node){
	if(node->isVisible()){
		m_numActiveAsteroids--;
		node->setVisible(false) ;
	}
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize() ;
	m_isBeingTouched = true;
	
	CCSetIterator iter=touches->begin();
	for(;iter!=touches->end();iter++){
		CCTouch* touch=(CCTouch*)(*iter);
		CCPoint location = touch->getLocation();
		//CCLog("ccTouch id=%d  (x=%f, y=%f)", touch->getID(), location.x, location.y );
		m_mostRecentTouchLocation.x = location.x;
		m_mostRecentTouchLocation.y = location.y;
	}
}
void HelloWorld::ccTouchesMoved(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize() ;
	m_isBeingTouched = true;
	
	CCSetIterator iter=touches->begin();
	for(;iter!=touches->end();iter++){
		CCTouch* touch=(CCTouch*)(*iter);
		CCPoint location = touch->getLocation();
		//CCLog("ccTouch id=%d  (x=%f, y=%f)", touch->getID(), location.x, location.y );
		m_mostRecentTouchLocation.x = location.x;
		m_mostRecentTouchLocation.y = location.y;
	}
}
void HelloWorld::ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event)
{
	m_isBeingTouched = false;
}