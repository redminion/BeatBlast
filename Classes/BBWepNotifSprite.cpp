#include "BBWepNotifSprite.h" 
 

void BBWepNotifSprite::updateTransform(void){
	//CCLog("CALLED");
	CCSprite::updateTransform();
}

void BBWepNotifSprite::draw(){
	//CCSprite::draw(); 

	//glColor4ub(255, 0, 0, 255);
	//glLineWidth(10);
	//ccDrawLine(this->getPosition(), ccpAdd(this->getPosition(), ccp(0,40))  );
	

}

void BBWepNotifSprite::update(float dt){
	this->setPositionX( this->getPositionX() + BBK_WepNotifSprite_xVelocity * dt );
}





BBWepNotifSprite::BBWepNotifSprite(){
	CCSprite();
}

BBWepNotifSprite* BBWepNotifSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame) {
	BBWepNotifSprite *newSprite = new BBWepNotifSprite();
	if (newSprite && newSprite->initWithSpriteFrame(pSpriteFrame)) {
		newSprite->autorelease();
		return newSprite;
	}
	CC_SAFE_DELETE(newSprite);
	return NULL;
}

BBWepNotifSprite* BBWepNotifSprite::createWithSpriteFrameName(const char *pszSpriteFrameName){
	BBWepNotifSprite *newSprite = new BBWepNotifSprite();
	if (newSprite && newSprite->initWithSpriteFrameName(pszSpriteFrameName )) {
		newSprite->autorelease();
		return newSprite;
	}
	CC_SAFE_DELETE(newSprite);
	return NULL;
}