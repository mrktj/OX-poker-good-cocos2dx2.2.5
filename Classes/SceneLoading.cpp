#include "SceneLoading.h"
#include "SceneWelcome.h"
#include "Tools.h"
#include "Controls.h"
#include "LayerGameDesk.h"
#include "MyAudioEngine.h"
using namespace cocos2d;

CCScene* LayerLoading::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::create();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        LayerLoading *layer = LayerLoading::create();
        CC_BREAK_IF(! layer);

		///////////!!!!!!!!!!!!!!!!!!!!!!!!!!!!1//////////
		//fucking here!!!!!!!!!!!!
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111
	//	LayerGameDesk *layer = LayerGameDesk::create();
	 //   CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);


    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LayerLoading::init()
{
    bool bRet = false;
    do 
    {
		CCEGLView::sharedOpenGLView()->setDesignResolutionSize(1024, 576, kResolutionShowAll);

		m_nNumberOfLoadedSprites=0;

        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////
        CC_BREAK_IF(! CCLayer::init());

		CCSize s = CCDirector::sharedDirector()->getWinSize();

        // Add a label
		pLabel = CCLabelTTF::create("Loading...", "Arial", 24);
        CC_BREAK_IF(! pLabel);
        pLabel->setPosition(ccp(s.width / 2, s.height * 0.15f));
        this->addChild(pLabel, 1);

        // 3. Add add a splash screen, show the cocos2d splash image.
        CCSprite* pSprite = CCSprite::create("scene_loading_bg.png");
        CC_BREAK_IF(! pSprite);
        pSprite->setPosition(ccp(s.width/2, s.height/2));
        this->addChild(pSprite, 0);
		pSprite->setScaleX(s.width / pSprite->getContentSize().width);
		pSprite->setScaleY(s.height/ pSprite->getContentSize().height);

        
		char temp[32]; //注意不要越界
		for(int i =1;i<14;i++){
			sprintf(temp,"card_clubs_%d.png",i);
			CCTextureCache::sharedTextureCache()->addImageAsync(temp, this, callfuncO_selector(LayerLoading::loadingCallBack));
		}
		for(int i =1;i<14;i++){
			sprintf(temp,"card_diamonds_%d.png",i);
			CCTextureCache::sharedTextureCache()->addImageAsync(temp, this, callfuncO_selector(LayerLoading::loadingCallBack));
		}
		for(int i =1;i<14;i++){
			sprintf(temp,"card_hearts_%d.png",i);
			CCTextureCache::sharedTextureCache()->addImageAsync(temp, this, callfuncO_selector(LayerLoading::loadingCallBack));
		}
		for(int i =1;i<14;i++){
			sprintf(temp,"card_spades_%d.png",i);
			CCTextureCache::sharedTextureCache()->addImageAsync(temp, this, callfuncO_selector(LayerLoading::loadingCallBack));
		}
		
        CCTextureCache::sharedTextureCache()->addImageAsync("scene_game_bg_desk.png", this, callfuncO_selector(LayerLoading::loadingCallBack));
        CCTextureCache::sharedTextureCache()->addImageAsync("scene_welcome_logo.png", this, callfuncO_selector(LayerLoading::loadingCallBack));
        CCTextureCache::sharedTextureCache()->addImageAsync("scene_welcome_info_bg.png", this, callfuncO_selector(LayerLoading::loadingCallBack));
        CCTextureCache::sharedTextureCache()->addImageAsync("scene_welcome_bg.png", this, callfuncO_selector(LayerLoading::loadingCallBack));
        
		CCSprite *m_loading_0 =CCSprite::create("scene_loading_0.png");
		addChild(m_loading_0);
		m_loading_0->setAnchorPoint(ccp(0.5f,0.5f));
		m_loading_0->setPosition(ccp(s.width/2, s.height/2));

		m_loading_bar = CCProgressTimer::create(CCSprite::create("scene_loading_100.png"));
		m_loading_bar->setType(kCCProgressTimerTypeBar);
		m_loading_bar->setMidpoint(ccp(0,0));
		m_loading_bar->setBarChangeRate(ccp(0, 1));
		addChild(m_loading_bar);
		m_loading_bar->setAnchorPoint(ccp(0.5f,0.5f));
		m_loading_bar->setPosition(ccp(s.width/2, s.height/2));

//        CCProgressTo *to = CCProgressTo::create(2,100);
//        CCFiniteTimeAction* fn = CCCallFunc::create(this,callfunc_selector(LayerLoading::enterSceneWelcome));
//        CCActionInterval* seq =  CCSequence::create(to,fn,NULL);
//        
//        m_loading_bar->runAction(seq);
        
		//合法性检查，注意调用顺序
		Tools* tools =Tools::sharedTools();	
		tools->updateRooted();
		tools->checkEncryptionAll(this);

       bRet = true;
    } while (0);

    return bRet;
}

void LayerLoading::enterSceneWelcome(){
	MyAudioEngine::sharedMyAudioEngine()->playBackgrondMusic();
	CCScene *scene =LayerWelcome::scene();
	CCDirector::sharedDirector()->pushScene(scene);
	//	CCDirector::sharedDirector()->pushScene(CCTransitionSlideInB::create(0.5f,scene ));
	//CCDirector::sharedDirector()->pushScene(CCTransitionFadeDown::create(0.5f,scene ));
	//CCDirector::sharedDirector()->replaceScene(CCTransitionFadeDown::create(0.5f,scene ) );
	
}

void LayerLoading::loadingCallBack(cocos2d::CCObject *obj){
	++m_nNumberOfLoadedSprites;

	char temp[32];
	float f = m_nNumberOfLoadedSprites /56.0f *100.0f;
//	int i = f*100;
	sprintf(temp,"%.2f %%",f);
//	sprintf(temp,"%d",m_nNumberOfLoadedSprites);
	pLabel->setString(temp);
	m_loading_bar->setPercentage(f);
	if (m_nNumberOfLoadedSprites == 56)
	{
		scheduleOnce(schedule_selector(LayerLoading::enterSceneWelcome),0.1f);
	}


}

