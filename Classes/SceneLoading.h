#ifndef __SCENE_LOADING_H__
#define __SCENE_LOADING_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
#include "Managers.h"
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1

class LayerLoading : public cocos2d::CCLayer
{
		
private:
	cocos2d::CCProgressTimer *m_loading_bar;
	int aaaa;
public:
	int m_nNumberOfLoadedSprites;

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    cocos2d::CCLabelTTF* pLabel;

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();

	void enterSceneWelcome();

	void loadingCallBack(cocos2d::CCObject *obj);


    // implement the "static node()" method manually
    CREATE_FUNC(LayerLoading);
};

#endif  