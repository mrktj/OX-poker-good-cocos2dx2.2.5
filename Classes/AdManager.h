#ifndef __AD_MANAGER_H__
#define __AD_MANAGER_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    #include "ProtocolAds.h"
#endif

/************************************
*								    *
*            AdManager              *
*							        *
*************************************/
class AdManager
{
public:
	static AdManager *sharedAdManager();
	static void deleteSharedAdManager();	

	void showAd();
    void hideAd();
private:
	AdManager();
	~AdManager();
	bool init();

	static AdManager *mSharedAdManager;
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
        cocos2d::plugin::ProtocolAds* m_pAdmob;
    #endif 
};


#endif

