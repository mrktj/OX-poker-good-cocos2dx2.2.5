#ifndef __LAYER_WELCOME_NODES_H__
#define __LAYER_WELCOME_NODES_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	//#include "AdsAdmob.h"

//class MyAdsListener : public cocos2d::plugin::AdsListener
//{
//public:
//	virtual void onAdsResult(cocos2d::plugin::AdsResultCode code, const char* msg);
//	virtual void onPlayerGetPoints(cocos2d::plugin::ProtocolAds* pAdsPlugin, int points);
//};

#endif 

class LayerWelcomeNodes: public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// selector callback
	void menuCloseCallback(CCObject* pSender);
	void menuBeginCallback(CCObject* pSender);
	void menuShopCallback(CCObject* pSender);

	// implement the "static node()" method manually
	CREATE_FUNC(LayerWelcomeNodes);
    
    /*
private:
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 

	cocos2d::plugin::AdsAdmob* m_pAdmob;
	MyAdsListener* m_pListener;

	cocos2d::CCMenuItemToggle* m_pCaseItem;
	cocos2d::CCMenuItemToggle* m_pTypeItem;
	cocos2d::CCMenuItemToggle* m_pPosItem;

	cocos2d::plugin::ProtocolAds* m_pAds;
	cocos2d::plugin::ProtocolAds::AdsPos m_ePos;
	cocos2d::plugin::ProtocolAds::AdsType m_eType;
	#endif 
*/
};

#endif 