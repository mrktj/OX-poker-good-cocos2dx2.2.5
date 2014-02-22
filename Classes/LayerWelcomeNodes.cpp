#include "LayerWelcomeNodes.h"
#include "SceneGame.h"
#include "SceneShop.h"
#include "SceneHall.h"
#include "Controls.h"
#include "Tools.h"
#include "GameLogic.h"
#include "MyAudioEngine.h"
#include "VisibleRect.h"
//#include "cocos-ext.h"

using namespace cocos2d;
//using namespace cocos2d::extension;


// on "init" you need to initialize your instance
bool LayerWelcomeNodes::init()
{
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();

	bool bRet = false;
	do 
	{
		MyAudioEngine::sharedMyAudioEngine()->playBackgrondMusic();

		CCSprite* pSprite = CCSprite::create("scene_welcome_logo.png");
		CC_BREAK_IF(! pSprite);
		pSprite->setPosition(ccp(size.width * 0.25f - 512, size.height * 0.58f));
		pSprite->setScale(0.85f);
		this->addChild(pSprite);
		CCActionInterval*  actionTo_ = CCMoveTo::create(0.15, ccp(size.width * 0.25f, size.height * 0.58f+20));
		pSprite->runAction(actionTo_);
		
		//scene_welcome_btn_enter1
		CCMenuItemImage *pItem1 = CCMenuItemImage::create(
			"scene_welcome_btn_enter0.png",
			"scene_welcome_btn_enter1.png",
			this,
			menu_selector(LayerWelcomeNodes::menuBeginCallback));
		CC_BREAK_IF(! pItem1);
		pItem1->setAnchorPoint(ccp(0.5,0));
		pItem1->setPosition(ccp(point.x - size.width/2,  point.y + 130));
		CCMenu* pMenu1 = CCMenu::create(pItem1, NULL);
		pMenu1->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu1);
		this->addChild(pMenu1);
		CCActionInterval*  actionTo3 = CCMoveTo::create(0.4f, ccp(point.x + size.width/2 -250,  point.y + 130));
		pItem1->runAction(actionTo3);
		//pItem1->setScale(0.85f);

//		CCMenuItemImage *pItemShop = CCMenuItemImage::create(
//			"scene_game_btn_shop0.png",
//			"scene_game_btn_shop1.png",
//			this,
//			menu_selector(LayerWelcomeNodes::menuShopCallback));
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


		// Create menu 
		CCMenuItemImage *pItem3 = CCMenuItemImage::create(
			"scene_welcome_btn_exit0.png",
			"scene_welcome_btn_exit1.png",
			this,
			menu_selector(LayerWelcomeNodes::menuCloseCallback));
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


		MyInfoBoard* info = new MyInfoBoard();
		info->setPosition(ccpAdd(VisibleRect::center(),ccp(200+1024,25)));
		addChild(info);
		info->setAnchorPoint(ccp(0,0));
		info->autorelease();
		//info->setScale(0.8f);
		//info->runAction(Tools::sharedTools()->getActionScaleYBiuBiuDelay(0.1f,0.25f));
		CCActionInterval*  actionBoard = CCMoveBy::create(0.3f,ccp(-1024,0));
		CCActionInterval*  seq = CCSequence::create(CCDelayTime::create(0.5f),actionBoard,NULL);
		info->runAction(seq);

		MyGameTopBar* bar = new MyGameTopBar();
		CC_BREAK_IF(! bar);
		bar->setPositionY(point.y+size.height - 35 + 200);
		bar->setAnchorPoint(ccp(0.5,0.5));
		this->addChild(bar);
		bar->autorelease();
		CCActionInterval*  actionBar = CCMoveBy::create(1.0f, ccp(0, -200));
		CCActionInterval*  actionTo = CCMoveTo::create(1.0f, ccp(size.width /2, point.y+size.height - 35));
		bar->runAction(actionBar);


		MACRO_CREATE_GOLDBOX_TIMER
		MACRO_GOLDBOX_TIMER_IN_WELCOME

		MACRO_CREATE_GOLDBOX_AMOUNT
		MACRO_GOLDBOX_AMOUNT_IN_WELCOME

                
		bRet = true;
	} while (0);

	return bRet;
}

/*

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 


void MyAdsListener::onAdsResult(AdsResultCode code, const char* msg)
{
	CCLog("OnAdsResult, code : %d, msg : %s", code, msg);
}

void MyAdsListener::onPlayerGetPoints(cocos2d::plugin::ProtocolAds* pAdsPlugin, int points)
{
	CCLog("Player get points : %d", points);

	// @warning should add code to give game-money to player here

	// spend the points of player
	if (pAdsPlugin != NULL) {
		pAdsPlugin->spendPoints(points);
	}
}


#endif 
*/

void LayerWelcomeNodes::menuCloseCallback(CCObject* pSender)
{
	Tools::sharedTools()->saveAllData();
	GameLogic::deleteSharedGameLogic();
	Tools::deleteSharedTools();
	removeAllChildren();
	CCTextureCache::sharedTextureCache()->removeAllTextures();
    CCTextureCache::sharedTextureCache()->purgeSharedTextureCache();
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void LayerWelcomeNodes::menuBeginCallback(CCObject* pSender)
{
	MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioClick);
	CCScene *scene =LayerHall::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFadeUp::create(0.5f,scene ) );
	CCDirector::sharedDirector()->replaceScene(scene );
}


void LayerWelcomeNodes::menuShopCallback(CCObject* pSender)
{    
	MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioClick);
	CCScene *scene =LayerShop::scene(0);
	//CCDirector::sharedDirector()->replaceScene(CCTransitionFadeDown::create(0.5f,scene ) );
	CCDirector::sharedDirector()->replaceScene(scene);
//	CCDirector::sharedDirector()->pushScene(scene);
     
}

