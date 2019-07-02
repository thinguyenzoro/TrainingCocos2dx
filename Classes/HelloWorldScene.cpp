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

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
Sprite *batman1;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
	// create a sprite
	auto mySprite = Sprite::create("batman.jpg");
	mySprite->setScale(0.5, 0.5);
	mySprite->setPosition(Vec2(visibleSize.width *0.5, visibleSize.height *0.5));


	// create a MoveBy Action to where we want the sprite to drop from.
	auto move = MoveBy::create(2, Vec2(200,200));

	// create a BounceIn Ease Action
	auto move_ease_in = EaseBounceInOut::create(move->clone());
	auto move_ease_in_back = move_ease_in->reverse();

	// create a delay that is run in between sequence events
	auto delay = DelayTime::create(0.25f);

	// create the sequence of actions, in the order we want to run them
	auto seq1 = Sequence::create(move_ease_in, delay, move_ease_in_back,delay->clone(), nullptr);

	// run the sequence and repeat forever.
	mySprite->runAction(RepeatForever::create(seq1));
	this->addChild(mySprite);
	////////////////////////////////animation///////////////////////////////
	//SpriteBatchNode* grossiniNode = SpriteBatchNode::create("grossini.png");
	//SpriteFrameCache::getInstance()->addSpriteFramesWithFile("grossini.plist");

	//auto mole1 = Sprite::createWithSpriteFrameName("grossini_dance_01.png");
	//mole1->setPosition(Vec2(visibleSize.width*0.2, visibleSize.height*0.5));
	//grossiniNode->addChild(mole1);
	//this->addChild(grossiniNode);
	//auto animate = Animate::create(HelloWorld::createAnimation("grossini_dance_", 14, 0.5));
	//animate->retain();
	//mole1->runAction(RepeatForever::create(animate));


	////////////////////////////////////////////////////touch/////////////////////////////////////////////////
	//batman = Sprite::create("batman.jpg");
	//batman->setScale(0.3, 0.3);
	//batman->setPosition(Vec2(0, 0));

	//this->addChild(batman);
	/////////////////////////////////////////////////////////////////////////////edit/////////////////////////////////////
	//// On Touch
	//auto touchListner = EventListenerTouchOneByOne::create();
	//touchListner->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	//touchListner->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	//touchListner->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	//this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListner, this);

	//auto batman = Sprite::create("batman.jpg");
	//batman->setPosition(Vec2(0, 0));
	//batman->setAnchorPoint(Vec2(0,0));
	//batman->setScale(0.5, 0.5);

	////batman->setRotation(45);
	////batman->setSkewX(20);
	////batman->setSkewY(20);

	//// Action
	//auto actionMoveTo = MoveTo::create(2, Vec2(400, 300));
	//auto actionMoveBy = MoveBy::create(2, Vec2(-400, -300));
	//auto actionRotateBy = RotateBy::create(3, 180);

	//auto actionSpawn1 = Spawn::create(actionMoveTo, actionRotateBy,nullptr);
	//auto actionSpawn2 = Spawn::create(actionMoveBy, actionRotateBy, nullptr);

	//auto actionSequence = Sequence::create(actionSpawn1, actionSpawn2, nullptr);
	//auto actionRepeat = RepeatForever::create(actionSequence);
	//
	////batman->runAction(actionMoveTo);
	////batman->runAction(actionMoveBy);
	////batman->runAction(actionRotateTo);
	//batman->runAction(actionRepeat);


	//addChild(batman);
	///////////////////////////////////////////////////////////////////////////edit/////////////////////////////////////

    ///////////////////////////////
    //// 2. add a menu item with "X" image, which is clicked to quit the program
    ////    you may modify it.

    //// add a "close" icon to exit the progress. it's an autorelease object
    //auto closeItem = MenuItemImage::create(
    //                                       "CloseNormal.png",
    //                                       "CloseSelected.png",
    //                                       CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    //if (closeItem == nullptr ||
    //    closeItem->getContentSize().width <= 0 ||
    //    closeItem->getContentSize().height <= 0)
    //{
    //    problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    //}
    //else
    //{
    //    float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
    //    float y = origin.y + closeItem->getContentSize().height/2;
    //    closeItem->setPosition(Vec2(x,y));
    //}

    //// create menu, it's an autorelease object
    //auto menu = Menu::create(closeItem, NULL);
    //menu->setPosition(Vec2::ZERO);
    //this->addChild(menu, 1);

    ///////////////////////////////
    //// 3. add your codes below...

    //// add a label shows "Hello World"
    //// create and initialize a label

    //auto label = Label::createWithTTF("Thi Nguyen", "fonts/Marker Felt.ttf", 24);
    //if (label == nullptr)
    //{
    //    problemLoading("'fonts/Marker Felt.ttf'");
    //}
    //else
    //{
    //    // position the label on the center of the screen
    //    label->setPosition(Vec2(origin.x + visibleSize.width/2,
    //                            origin.y + visibleSize.height - label->getContentSize().height));

    //    // add the label as a child to this layer
    //    this->addChild(label, 1);
    //}

    //// add "HelloWorld" splash screen"
    //auto sprite = Sprite::create("HelloWorld.png");
    //if (sprite == nullptr)
    //{
    //    problemLoading("'HelloWorld.png'");
    //}
    //else
    //{
    //    // position the sprite on the center of the screen
    //    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    //    // add the sprite as a child to this layer
    //    this->addChild(sprite, 0);
    //}
    return true;
}
cocos2d::Animation *HelloWorld::createAnimation(std::string prefixName, int pFramesOrder, float delay) {
	Vector<SpriteFrame*> animFrames;
	//Make frame
	for (int i = 1; i <= pFramesOrder; i++) {
		//std::string str = prefixName +std::to_string(i) + ".png";
		char buffer[20] = { 0 };
		sprintf(buffer, "%0.2d.png", i);
		std::string str = prefixName + buffer;
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	animation = Animation::createWithSpriteFrames(animFrames, delay);
	return animation;
}
bool HelloWorld::onTouchBegan(Touch *touch, Event *unused_event) {
	Vec2 touchLocation = touch->getLocation();
	
	auto actionMoveTo = MoveTo::create(0.5, touchLocation);
	batman1->runAction(actionMoveTo);
	return true;
}
bool HelloWorld::onTouchMoved(Touch *touch, Event *unused_event) {
	Vec2 touchLocation = touch->getLocation();
	batman1->setPosition(touchLocation);
	return true;
}
bool HelloWorld::onTouchEnded(Touch *touch, Event *unused_event) {
	auto actionMoveTo = MoveTo::create(1, Vec2(0, 0));
	batman1->runAction(actionMoveTo);

	return true;
}
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
