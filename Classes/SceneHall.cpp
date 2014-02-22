#include "SceneGame.h"
#include "SceneShop.h"
#include "SceneHall.h"
#include "SceneWelcome.h"
#include "Controls.h"
#include "Tools.h"
#include "GameLogic.h"
#include "LayerGameDesk.h"
#include "MyAudioEngine.h"
#include "VisibleRect.h"

#include "AdManager.h"

using namespace cocos2d;

CCScene* LayerHall::scene()
{
	CCScene * scene = NULL;
	do 
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		LayerHall *layer = LayerHall::create();
		CC_BREAK_IF(! layer);

		scene->addChild(layer);
        
        if(Tools::sharedTools()->getShouldAd()){
            AdManager::sharedAdManager()->showAd();
        }
        
	} while (0);
	return scene;
}


// on "init" you need to initialize your instance
bool LayerHall::init()
{
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();

	bool bRet = false;
	do 
	{
		//bg
		CCSprite* pSprite = CCSprite::create("scene_welcome_bg.png");
		CC_BREAK_IF(! pSprite);
		pSprite->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(pSprite);
	
		//back 
		CCMenuItemImage *pItem3 = CCMenuItemImage::create(
			"scene_game_btn_back0.png",
			"scene_game_btn_back1.png",
			this,
			menu_selector(LayerHall::menuCloseCallback));
		CC_BREAK_IF(! pItem3);
		pItem3->setPosition(point);
		pItem3->setAnchorPoint(ccp(0,0));
		CCMenu* pMenu3 = CCMenu::create(pItem3, NULL);
		pMenu3->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu3);
		this->addChild(pMenu3);
		pItem3->setRotation(-180);
		CCActionInterval*  act1 = CCRotateTo::create(0.2f, 0);
		pItem3->runAction(act1);


		//shop
//		CCMenuItemImage *pItemShop = CCMenuItemImage::create(
//			"scene_game_btn_shop0.png",
//			"scene_game_btn_shop1.png",
//			this,
//			menu_selector(LayerHall::menuShopCallback));
//		CC_BREAK_IF(! pItemShop);
//		pItemShop->setPosition(ccp(point.x+size.width,point.y));
//		pItemShop->setAnchorPoint(ccp(1,0));
//		CCMenu* pMenuShop = CCMenu::create(pItemShop, NULL);
//		pMenuShop->setPosition(CCPointZero);
//		CC_BREAK_IF(! pMenuShop);
//		this->addChild(pMenuShop);
//		pItemShop->setRotation(180);
//		CCActionInterval*  act2 = CCRotateTo::create(0.2f, 0);
//		pItemShop->runAction(act2);



		initRooms();

		MACRO_CREATE_GOLDBOX_TIMER
		MACRO_CREATE_MONEY_BOARD
		MACRO_GOLDBOX_TIMER_WITH_MONEYBOARD

		MACRO_CREATE_GOLDBOX_AMOUNT
		MACRO_GOLDBOX_AMOUNT_WITH_MONEYBOARD

		MyGameTopBar* bar = new MyGameTopBar();
		CC_BREAK_IF(! bar);
		bar->setPositionY(point.y+size.height - 35 + 200);
		bar->setAnchorPoint(ccp(0.5,0.5));
		this->addChild(bar);
		bar->autorelease();
		CCActionInterval*  actionBar = CCMoveBy::create(0.8f, ccp(0, -200));
		//	CCActionInterval*  actionTo = CCMoveTo::create(1.0f, ccp(size.width /2, point.y+size.height - 35));
		bar->runAction(actionBar);

		bRet = true;
	} while (0);

	return bRet;
}

void LayerHall::initRooms(){
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();

	short offset = 750;

	//0
	CCMenuItemImage *pItem0 = CCMenuItemImage::create(
		"scene_hall_room0_0.png",
		"scene_hall_room0_1.png",
		this,
		menu_selector(LayerHall::menuRoom0Callback));
	pItem0->setAnchorPoint(ccp(0.5,0.5));
	pItem0->setPosition(ccp(246 ,370 +offset));
	CCActionInterval*  action6= CCMoveBy::create(25 / 60.0f, ccp(0,offset * -1));
	pItem0->runAction(action6);

	//room1
	CCMenuItemImage *pItem1 = CCMenuItemImage::create(
		"scene_hall_room1_0.png",
		"scene_hall_room1_1.png",
		this,
		menu_selector(LayerHall::menuRoom1Callback));
	pItem1->setAnchorPoint(ccp(0.5,0.5));
	pItem1->setPosition(ccp(512,370  +offset));
	CCActionInterval*  action1 = CCMoveBy::create(25 / 60.0f, ccp(0,offset * -1));
	pItem1->runAction(action1);

	//2
	CCMenuItemImage *pItem2 = CCMenuItemImage::create(
		"scene_hall_room2_0.png",
		"scene_hall_room2_1.png",
		this,
		menu_selector(LayerHall::menuRoom2Callback));
	pItem2->setAnchorPoint(ccp(0.5,0.5));
	pItem2->setPosition(ccp(246,200 -offset));
	CCActionInterval*  action2 =  CCMoveBy::create(25 / 60.0f, ccp(0,offset));
	pItem2->runAction(action2);

	//3
	CCMenuItemImage *pItem3 = CCMenuItemImage::create(
		"scene_hall_room3_0.png",
		"scene_hall_room3_1.png",
		this,
		menu_selector(LayerHall::menuRoom3Callback));
	pItem3->setAnchorPoint(ccp(0.5,0.5));
	pItem3->setPosition(ccp(512,200 -offset));
	CCActionInterval*  action3 =  CCMoveBy::create(25 / 60.0f, ccp(0,offset));
	pItem3->runAction(action3);

	//4
	CCMenuItemImage *pItem4 = CCMenuItemImage::create(
		"scene_hall_room4_0.png",
		"scene_hall_room4_1.png",
		this,
		menu_selector(LayerHall::menuRoom4Callback));
	pItem4->setAnchorPoint(ccp(0.5,0.5));
	pItem4->setPosition(ccp(778,200-offset));
	CCActionInterval*  action4 =  CCMoveBy::create(25 / 60.0f, ccp(0,offset));
	pItem4->runAction(action4);

	//5
	CCMenuItemImage *pItem5 = CCMenuItemImage::create(
		"scene_hall_room5_0.png",
		"scene_hall_room5_1.png",
		this,
		menu_selector(LayerHall::menuRoom5Callback));
	pItem5->setAnchorPoint(ccp(0.5,0.5));
	pItem5->setPosition(ccp(778,370+offset));
	CCActionInterval*  action5 =  CCMoveBy::create(25 / 60.0f, ccp(0,offset * -1));
	pItem5->runAction(action5);



	//menu
	CCMenu* pMenu1 = CCMenu::create(pItem0,pItem1,pItem2, pItem3, pItem4, pItem5,NULL);
	pMenu1->setPosition(CCPointZero);
	this->addChild(pMenu1);
}

void LayerHall::menuRoom0Callback(CCObject* pSender)
{
	enterRoom(0);
}

void LayerHall::menuRoom1Callback(CCObject* pSender)
{
	enterRoom(1);
}

void LayerHall::menuRoom2Callback(CCObject* pSender)
{
	enterRoom(2);
}

void LayerHall::menuRoom3Callback(CCObject* pSender)
{
	enterRoom(3);
}

void LayerHall::menuRoom4Callback(CCObject* pSender)
{
	enterRoom(4);
}

void LayerHall::menuRoom5Callback(CCObject* pSender)
{
	enterRoom(5);
}



void LayerHall::enterRoom(short flag){
	MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioClick);

	unsigned int min;//入场限制
	unsigned int money;//玩家拥有金钱

	min = GameLogic::sharedGameLogic()->getMinMoneyToEnterRoom(flag);

	if(flag != 4){
		//银币场
		money = Tools::sharedTools()->getCoin();
	}else{
		//银锭场
		money = Tools::sharedTools()->getIngot();
	}

	if(money>=min){
		//vip场
		if(flag>=4){
			if(Tools::sharedTools()->getVipHighest()>=0){
				CCScene *scene =LayerGame::scene();
				LayerGameDesk *layer2 = LayerGameDesk::create();
				layer2->setRoomKind(flag);
				scene->addChild(layer2);
				CCDirector::sharedDirector()->replaceScene(scene);
			}else{
				Tools::sharedTools()->showToast(eToastNeedVip,this);
			}
		}else{
			CCScene *scene =LayerGame::scene();
			LayerGameDesk *layer2 = LayerGameDesk::create();
			layer2->setRoomKind(flag);
			scene->addChild(layer2);
			CCDirector::sharedDirector()->replaceScene(scene);
		}
	}else{
		Tools::sharedTools()->showToast(eToastMoneyNotEnough,this);
	}
	
}

void LayerHall::menuCloseCallback(CCObject* pSender)
{
	Tools::sharedTools()->saveAllData();
	CCScene *scene =LayerWelcome::scene();
//	CCDirector::sharedDirector()->replaceScene(CCTransitionFadeUp::create(0.5f,scene ) );
	CCDirector::sharedDirector()->replaceScene(scene);
}

void LayerHall::menuShopCallback(CCObject* pSender)
{
	CCScene *scene =LayerShop::scene(1);
	//CCDirector::sharedDirector()->replaceScene(CCTransitionFadeDown::create(0.5f,scene ) );
	CCDirector::sharedDirector()->replaceScene(scene);
	//CCDirector::sharedDirector()->pushScene(scene);
}

