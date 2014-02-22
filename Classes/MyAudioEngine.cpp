#include "cocos2d.h"
#include "MyAudioEngine.h"
#include "SimpleAudioEngine.h"
#include "Tools.h"

using namespace CocosDenshion;

MyAudioEngine *MyAudioEngine::mSharedMyAudioEngine = NULL;

MyAudioEngine *MyAudioEngine::sharedMyAudioEngine()
{
	if (mSharedMyAudioEngine == NULL)
	{
		mSharedMyAudioEngine = new MyAudioEngine();
		if (!mSharedMyAudioEngine)
		{
			CC_SAFE_DELETE(mSharedMyAudioEngine);
		}
	}
	return mSharedMyAudioEngine;
}


void MyAudioEngine::deleteSharedMyAudioEngine()
{
	CC_SAFE_DELETE(mSharedMyAudioEngine);
}


MyAudioEngine::MyAudioEngine()
{
	//	SimpleAudioEngine::sharedEngine()->preloadEffect("ogg_btn_click.mp3");
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1.0f);
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.0f);
}

void MyAudioEngine::playEffect(enumAudioFile audio){
	if(Tools::sharedTools()->m_isSoundsOn){
		switch(audio){
		case eAudioClick:
			SimpleAudioEngine::sharedEngine()->playEffect("ogg_btn_click.mp3");
			break;
		case eAudioSkew:
			SimpleAudioEngine::sharedEngine()->playEffect("ogg_icon_skew.mp3");
			break;
		case eAudioShowChipsAll:
			SimpleAudioEngine::sharedEngine()->playEffect("ogg_show_chips_all.mp3");
			break;
		case eAudioShowChipsOne:
			SimpleAudioEngine::sharedEngine()->playEffect("ogg_show_chips_one.mp3");
			break;
		case eAudioSendOneCard:
			SimpleAudioEngine::sharedEngine()->playEffect("ogg_deal_cards.mp3");
			break;
		case eAudioLose:
			SimpleAudioEngine::sharedEngine()->playEffect("ogg_lose.mp3");
			break;
		case eAudioWin:
			SimpleAudioEngine::sharedEngine()->playEffect("ogg_win.mp3");
			break;
		case eAudioUnrob:
			SimpleAudioEngine::sharedEngine()->playEffect("ogg_unrob.mp3");
			break;
		case eAudioRob:
			SimpleAudioEngine::sharedEngine()->playEffect("ogg_rob.mp3");
			break;
		case eAudioShopSuccess:
			SimpleAudioEngine::sharedEngine()->playEffect("ogg_shop_success.mp3");
			break;		
		case eAudioAddCoin:
			SimpleAudioEngine::sharedEngine()->playEffect("ogg_add_coin.wav");
			break;	
		}
	}
}

void MyAudioEngine::playBackgrondMusic(){
	if(Tools::sharedTools()->m_isSoundsOn){
		srand(time(NULL));
		if(rand()%100>=50){
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("ogg_bg_gaming0.mp3",true);
		}else{
			SimpleAudioEngine::sharedEngine()->playBackgroundMusic("ogg_bg_gaming1.mp3",true);
		}
	}
}

void MyAudioEngine::stopBackgrondMusic(){
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void MyAudioEngine::playEffectShowOx(short ox ,bool isBoy){
	if(Tools::sharedTools()->m_isSoundsOn){
		char temp[32];
		if(isBoy){
			sprintf(temp,"ogg_ox_boy_%d.mp3",ox);
		}else{
			sprintf(temp,"ogg_ox_girl_%d.mp3",ox);
		}
		SimpleAudioEngine::sharedEngine()->playEffect(temp);
	}
}


