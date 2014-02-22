#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

class LayerGame : public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// implement the "static node()" method manually
	CREATE_FUNC(LayerGame);
};

#endif  