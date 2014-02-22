#include "cocos2d.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
#include "SceneLoading.h"
#include "SimpleAudioEngine.h"
#include "SceneWelcome.h"
#include "Tools.h"

using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

//ß	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(1024, 576, kResolutionExactFit);

    // turn on display FPS
  //   pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
     pDirector->setAnimationInterval(1.0/60);

    // create a scene. it's an autorelease object

    CCScene *pScene = LayerLoading::scene();
//	CCScene *pScene = LayerWelcome::scene();

    // run
    pDirector->runWithScene(pScene);

	//srand((unsigned)time(0));

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    Tools::sharedTools()->saveAllData();
	CCDirector::sharedDirector()->pause();
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();

}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
	CCDirector::sharedDirector()->resume();
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
