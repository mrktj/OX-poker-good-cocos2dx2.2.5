#include "SceneWelcome.h"
#include "LayerWelcomeNodes.h"
#include "AdManager.h"
#include "Tools.h"

using namespace cocos2d;

CCScene* LayerWelcome::scene()
{
	CCScene * scene = NULL;
	do 
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		// 'layer' is an autorelease object
		LayerWelcome *layer = LayerWelcome::create();
		CC_BREAK_IF(! layer);

		LayerWelcomeNodes *layer2 = LayerWelcomeNodes::create();
		CC_BREAK_IF(! layer2);

		// add layer as a child to scene
		scene->addChild(layer);
		scene->addChild(layer2);
        
        if(Tools::sharedTools()->getShouldAd()){
             AdManager::sharedAdManager()->showAd();
        }
       
        
	} while (0);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LayerWelcome::init()
{
	bool bRet = false;
	do 
	{
		//////////////////////////////////////////////////////////////////////////
		// super init first
		//////////////////////////////////////////////////////////////////////////
		CC_BREAK_IF(! CCLayer::init());

		CCSize size = CCDirector::sharedDirector()->getWinSize();

		CCSprite* pSprite = CCSprite::create("scene_welcome_bg.png");
		CC_BREAK_IF(! pSprite);
		pSprite->setPosition(ccp(size.width/2, size.height/2));
//		pSprite->setScaleX(size.width / pSprite->getContentSize().width);
//		pSprite->setScaleY(size.height/ pSprite->getContentSize().height);
		this->addChild(pSprite);


		bRet = true;
	} while (0);

	return bRet;
}



