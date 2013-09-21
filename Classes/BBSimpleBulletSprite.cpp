#include "BBSimpleBulletSprite.h" 
 



void BBSimpleBulletSprite::draw(){
	CCSprite::draw(); 
}

void BBSimpleBulletSprite::update(float dt){
	//increment position by velocity
	this->setPositionX( this->getPositionX() + m_xVel*dt);
	this->setPositionY( this->getPositionY() + m_yVel*dt);
}




// --- GETTER/SETTER
void BBSimpleBulletSprite::setVelocity(CCPoint vel){
	this->m_xVel = vel.x;
	this->m_yVel = vel.y;
}
void BBSimpleBulletSprite::addVelocity(CCPoint vel){
	this->m_xVel += vel.x;
	this->m_yVel += vel.y;
}
CCPoint BBSimpleBulletSprite::getVelocity(){
	return ccp( m_xVel, m_yVel);
}



// --- CONSTRUCTORS
BBSimpleBulletSprite::BBSimpleBulletSprite(){
	CCSprite();

	this->m_xVel = 0.0f;
	this->m_yVel = 0.0f;
}

BBSimpleBulletSprite* BBSimpleBulletSprite::createWithSpriteFrame(CCSpriteFrame *pSpriteFrame) {
	BBSimpleBulletSprite *newSprite = new BBSimpleBulletSprite();
	if (newSprite && newSprite->initWithSpriteFrame(pSpriteFrame)) {
		newSprite->autorelease();
		return newSprite;
	}
	CC_SAFE_DELETE(newSprite);
	return NULL;
}

BBSimpleBulletSprite* BBSimpleBulletSprite::createWithSpriteFrameName(const char *pszSpriteFrameName){
	BBSimpleBulletSprite *newSprite = new BBSimpleBulletSprite();
	if (newSprite && newSprite->initWithSpriteFrameName(pszSpriteFrameName )) {
		newSprite->autorelease();
		return newSprite;
	}
	CC_SAFE_DELETE(newSprite);
	return NULL;
}