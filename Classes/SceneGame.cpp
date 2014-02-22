#include "SceneGame.h"
#include "LayerGameDesk.h"

#include "AdManager.h"

using namespace cocos2d;

CCScene* LayerGame::scene()
{
	CCScene * scene = NULL;
	do 
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		LayerGame *layer = LayerGame::create();
		CC_BREAK_IF(! layer);

		scene->addChild(layer);

        AdManager::sharedAdManager()->hideAd();

        //	scene->addChild(layer3,3,3); //tag值LayerGameScheduler中会用到

	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LayerGame::init()
{
	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first														//    
		//////////////////////////////////////////////////////////////////////////
		CC_BREAK_IF(! CCLayer::init());

		CCSize size = CCDirector::sharedDirector()->getVisibleSize();

		CCSprite* pSprite = CCSprite::create("scene_welcome_bg.png");
		CC_BREAK_IF(! pSprite);
		pSprite->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(pSprite);

	//	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(800, 480, kResolutionNoBorder);

	//	pSprite->setScaleX(size.width / pSprite->getContentSize().width);
	//	pSprite->setScaleY(size.height/ pSprite->getContentSize().height);

		bRet = true;
	} while (0);

	return bRet;
}



