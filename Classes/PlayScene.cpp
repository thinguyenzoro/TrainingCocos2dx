/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "PlayScene.h"
#include "SimpleAudioEngine.h"
#include <ctime>


USING_NS_CC;
#define MAX(a,b) (a>b)?a:b
#define TYPE_ROCKET 1
#define TYPE_BULLET 2
#define TYPE_DANDRUFF 3
Vec2 fromLocation;
Vec2 toLocation;
float distance;
float distanceX;
float distanceY;
bool isTouch = false;
float xMin, xMax, yMin, yMax;
float screenWidth, screenHeight;
float rocketWidth, rocketHeight;
bool firstClick = false;

bool isEnemyMove[32] = { false };
Vec2 enemyMoveFrom[32];
Vec2 enemyMoveTo[32];
int gameScore = 0;
double gameTime = 30;
Label* scoreLabel;
Label* timeLabel;

Scene* GamePlay::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); // 2
	Vect gravity(0.0f, 10.0f);                                               // 3
	scene->getPhysicsWorld()->setGravity(gravity);

	auto layer = GamePlay::create();
	scene->addChild(layer);
	return scene;
}

bool GamePlay::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	xMin = visibleSize.width *0.14;
	xMax = visibleSize.width * 0.77;
	yMin = 0;
	yMax = visibleSize.height * 0.28;
	screenWidth = visibleSize.width;
	screenHeight = visibleSize.height;

	CCString *tempTime = CCString::createWithFormat("%i", (int)gameTime);
	timeLabel = Label::createWithTTF(tempTime->getCString(), "fonts/Marker Felt.ttf",visibleSize.height*0.07);
	timeLabel->setPosition(Point(visibleSize.width / 4, visibleSize.height*0.92));

	CCString *tempScore = CCString::createWithFormat("%i", (int)gameScore);
	scoreLabel = Label::createWithTTF(tempScore->getCString(), "fonts/Marker Felt.ttf", visibleSize.height*0.07);
	scoreLabel->setPosition(Point(visibleSize.width / 2, visibleSize.height*0.92));

	this->addChild(timeLabel, 10000);
	this->addChild(scoreLabel, 10000);

	// set background
	auto bg = Sprite::create("bg.jpg");
	bg->setScale(MAX((visibleSize.width/bg->boundingBox().size.width), (visibleSize.height / bg->boundingBox().size.height)));
	bg->setAnchorPoint(Vec2(0, 0));
	this->addChild(bg, 0);

	// create a rocket
	sprite = Sprite::create("images/1.f0012.png");
	sprite->setScale(visibleSize.width/8/sprite->boundingBox().size.width);
	sprite->setPosition(this->getBoundingBox().getMidX(), this->getBoundingBox().getMidX()*0.15);
	auto spriteBody = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0, 1, 0));
	sprite->setPhysicsBody(spriteBody);
	sprite->setTag(TYPE_ROCKET);
	spriteBody->setContactTestBitmask(true);
	spriteBody->setDynamic(false);
	this->addChild(sprite,1 );
	rocketWidth = sprite->boundingBox().getMaxX();
	rocketHeight = sprite->boundingBox().getMaxY();

	// create bullet
	bullet = Sprite::create("images/1.f0025.png");
	bullet->setScale(visibleSize.width*0.025 / bullet->boundingBox().size.width);
	bullet->setPosition(sprite->getPosition().x , sprite->getPosition().y+sprite->boundingBox().getMidY());
	bullet->setAnchorPoint(Vec2(0.5,0));
	bullet->setVisible(false);
	auto bulletBody = PhysicsBody::createBox(bullet->getContentSize(), PhysicsMaterial(0, 1, 0));
	bullet->setPhysicsBody(bulletBody);
	bulletBody->setContactTestBitmask(true);
	bulletBody->setDynamic(false);
	bullet->setTag(TYPE_BULLET);
	this->addChild(bullet, 1);
	
	// create dandruff list
	dandruff = makeRandomDandruff();
	//auto cloud = Sprite::create("images/1.f0026.png");
	//cloud->setPosition(Vec2(screenWidth*0.14, screenHeight*0.8));
	//this->addChild(cloud, 1);
	for (int i = 0; i < 32; i++) {
		this->addChild(dandruff[i], 1);
	}



	// On Touch
	auto touchListner = EventListenerTouchOneByOne::create();
	touchListner->onTouchBegan = CC_CALLBACK_2(GamePlay::onTouchBegan, this);
	touchListner->onTouchMoved = CC_CALLBACK_2(GamePlay::onTouchMove, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListner, this);
	
	// Collision
	auto contactListner = EventListenerPhysicsContact::create();
	contactListner->onContactBegin = CC_CALLBACK_1(GamePlay::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListner, this);


	return true;
}

Vec2 limitedPosition(Vec2 currentLocation) {
	if (currentLocation.x < xMin)
		currentLocation.x = xMin;
	if (currentLocation.x > xMax)
		currentLocation.x = xMax;
	if (currentLocation.y < yMin)
		currentLocation.y = yMin;
	if (currentLocation.y > yMax)
		currentLocation.y = yMax;
	return currentLocation;
}
void shuffleArray(Sprite** enemy);
void GamePlay::update(float delta) {
	if (gameTime <= 0)
		return;
	if (gameTime>27 && gameTime - delta<=27) {
		shuffleArray(dandruff);
	}
	if (gameTime>20 && gameTime - delta <= 20) {
		shuffleArray(dandruff);
	}
	gameTime -= delta;
	for (int i = 0; i < 32; i++) {
		if (isEnemyMove[i] == true) {
			Vec2 dandruffPosition = dandruff[i]->getPosition();
			dandruffPosition.x += 400 * delta*(enemyMoveTo[i].x - enemyMoveFrom[i].x)/ enemyMoveFrom[i].getDistance(enemyMoveTo[i]);
			dandruffPosition.y += 400 * delta*(enemyMoveTo[i].y - enemyMoveFrom[i].y) / enemyMoveFrom[i].getDistance(enemyMoveTo[i]);
			if (((enemyMoveTo[i].x < dandruffPosition.x ) && (dandruffPosition.x < enemyMoveFrom[i].x)) || ((enemyMoveFrom[i].x <  dandruffPosition.x)&&(dandruffPosition.x < enemyMoveTo[i].x ))) {
				dandruff[i]->setPosition(dandruffPosition);
			}
			else {
				dandruff[i]->setPosition(enemyMoveTo[i]);
				isEnemyMove[i] = false;
			}
		}
		else {

		}
	}
	CCString *tempTime = CCString::createWithFormat("%i", (int)gameTime);
	timeLabel->setString(tempTime->getCString());
	CCString *tempScore = CCString::createWithFormat("%i", (int)gameScore);
	scoreLabel->setString(tempScore->getCString());

	auto spritePosition = sprite->getPosition();
	auto bulletPosition = bullet->getPosition();
	if (isTouch) {
		Vec2 currentLocation = spritePosition;
		spritePosition.x += 800 * delta*distanceX/distance;
		spritePosition.y += 800 * delta*distanceY / distance;

		if (((toLocation.x < spritePosition.x) && (spritePosition.x < currentLocation.x)) || ((toLocation.x > spritePosition.x) && (spritePosition.x > currentLocation.x))) {
			spritePosition = limitedPosition(spritePosition);
			sprite->setPosition(spritePosition);
		}
		else
			isTouch = false;
	}
	if (bulletPosition.y > screenHeight) {
		bulletPosition.x = spritePosition.x;
		bulletPosition.y = spritePosition.y + rocketHeight/2;
		bullet->getPhysicsBody()->setContactTestBitmask(true);
		bullet->setVisible(true);
	}
	bulletPosition.y += 1000 * delta;
	bullet->setPosition(bulletPosition);

}

bool GamePlay::onTouchBegan(Touch *touch, Event *unused_event) {
	if (!firstClick) {
		bullet->setVisible(true);
		this->scheduleUpdate();
		firstClick = true;
	}
	fromLocation = sprite->getPosition();
	toLocation = limitedPosition(touch->getLocation());
	distance = fromLocation.getDistance(toLocation);
	distanceX = toLocation.x - fromLocation.x;
	distanceY = toLocation.y - fromLocation.y;
	isTouch = true;
	return true;
}
bool GamePlay::onTouchMove(Touch *touch, Event *unused_event) {
	fromLocation = sprite->getPosition();
	toLocation = limitedPosition(touch->getLocation());
	distance = fromLocation.getDistance(toLocation);
	distanceX = toLocation.x - fromLocation.x;
	distanceY = toLocation.y - fromLocation.y;
	isTouch = true;
	return true;
}
Sprite ** GamePlay::makeRandomDandruff() {
	Sprite** dandruff = new Sprite*[32];
	srand(time(NULL));
	for (int i = 0; i < 32; i++) {
		int a = rand()%3;
		if (a == 0) {
			dandruff[i] = Sprite::create("images/1.f0026.png");
		}
		else if (a == 1) {
			dandruff[i] = Sprite::create("images/1.f0038.png");
		}
		else {
			dandruff[i] = Sprite::create("images/1.f0050.png");
		}
		dandruff[i]->setPosition(screenWidth*0.14 + (i % 8)*0.09*screenWidth, 0.8*screenHeight - (i / 8)*0.1*screenHeight);
		dandruff[i]->setScale(screenWidth/dandruff[i]->boundingBox().size.width*0.07);
		auto dandruffBody = PhysicsBody::createBox(dandruff[i]->getContentSize(), PhysicsMaterial(0, 1, 0));
		dandruff[i]->setPhysicsBody(dandruffBody);
		dandruffBody->setContactTestBitmask(true);
		dandruffBody->setDynamic(false);
		dandruff[i]->setTag(TYPE_DANDRUFF);

	}
	return dandruff;
}	
bool GamePlay::onContactBegin(PhysicsContact &contact) {
	auto spriteA = (Sprite *)contact.getShapeA()->getBody()->getNode();
	auto spriteB = (Sprite *)contact.getShapeB()->getBody()->getNode();
	int tagA = spriteA->getTag();
	int tagB = spriteB->getTag();
	if ((tagA == TYPE_BULLET && tagB == TYPE_DANDRUFF)||(tagB == TYPE_BULLET && tagA == TYPE_DANDRUFF)) {
		if (tagA == TYPE_DANDRUFF) {
			spriteA->setVisible(false);
			spriteA->getPhysicsBody()->setContactTestBitmask(false);
			gameScore += 100;
		}
		else {
			spriteB->setVisible(false);
			spriteB->getPhysicsBody()->setContactTestBitmask(false);
			gameScore += 100;
		}
		bullet->setVisible(false);
		bullet->getPhysicsBody()->setContactTestBitmask(false);
	}
	return true;
}
void shuffleArray(Sprite** enemy) {
	int Arr1[32];
	int Arr2[32];
	int temp = 0;
	for (int i = 0; i < 32; i++) {
		if (enemy[i]->isVisible()==true) {
			Arr1[temp] = i;
			Arr2[temp] = i;
			temp++;
			isEnemyMove[i] = true;
		}
	}
	srand(time(NULL));
	for (int i = 0; i < temp; i++) {
		int randomNumber = rand() % temp;
		int x = Arr2[i];
		Arr2[i] = Arr2[randomNumber];
		Arr2[randomNumber] = x;
	}
	for (int i = 0; i < temp; i++) {
		enemyMoveFrom[Arr1[i]] = enemy[Arr1[i]]->getPosition();
		enemyMoveTo[Arr1[i]] = enemy[Arr2[i]]->getPosition();
		//CCLOG("%d %d %f %f\n", Arr1[i], Arr2[i], enemyMoveFrom[Arr1[i]], enemyMoveTo[Arr1[i]]);
	}
}