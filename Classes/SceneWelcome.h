#ifndef __SCENE_WELCOME_H__
#define __SCENE_WELCOME_H__

#include "cocos2d.h"
#include "LayerWelcomeNodes.h"
#include "SimpleAudioEngine.h"

class LayerWelcome : public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(LayerWelcome);
};

#endif  