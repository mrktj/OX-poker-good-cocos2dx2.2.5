#include "cocos2d.h"

#include "AdManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
//    #include "AdsAdmob.h"
    #include "PluginManager.h"
    using namespace cocos2d::plugin;
#endif

using namespace cocos2d;



AdManager *AdManager::mSharedAdManager = NULL;


AdManager *AdManager::sharedAdManager()
{
	if (mSharedAdManager == NULL){
	
		mSharedAdManager = new AdManager();
		if (!mSharedAdManager || !mSharedAdManager->init())
		{
			CC_SAFE_DELETE(mSharedAdManager);
		}
	}
	return mSharedAdManager;
}


void AdManager::deleteSharedAdManager()
{
    CC_SAFE_DELETE(mSharedAdManager);
}



bool AdManager::init()
{
	bool bRet = false;
    do 
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        
            m_pAdmob = dynamic_cast<ProtocolAds*>(PluginManager::getInstance()->loadPlugin("AdsAdmob"));
            TAdsDeveloperInfo devInfo;
            devInfo["AdmobID"] = "a151d3de9b7717c";
            m_pAdmob->configDeveloperInfo(devInfo);
            //m_pListener = new MyAdsListener();
            //m_pAdmob->setAdsListener(m_pListener);
            m_pAdmob->setDebugMode(false);
        
#endif
        bRet = true;
    } while (0);
    return bRet;
}

void AdManager::showAd(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    
        m_pAdmob->showAds(ProtocolAds::kBannerAd, 0, ProtocolAds::kPosBottom);
    
#endif
}

void AdManager::hideAd(){
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        m_pAdmob->hideAds(ProtocolAds::kBannerAd);
    
#endif
}

AdManager::AdManager()
{
}


AdManager::~AdManager()
{
}