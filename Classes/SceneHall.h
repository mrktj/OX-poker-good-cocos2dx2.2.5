#ifndef __SCENE_HALL_H__
#define __SCENE_HALL_H__

#include "cocos2d.h"

#include "SimpleAudioEngine.h"

class LayerHall: public cocos2d::CCLayer
{
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();  
	void initRooms();

	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();

	// selector callback
	void menuCloseCallback(CCObject* pSender);
	void menuShopCallback(CCObject* pSender);

	void menuRoom0Callback(CCObject* pSender);
	void menuRoom1Callback(CCObject* pSender);
	void menuRoom2Callback(CCObject* pSender);
	void menuRoom3Callback(CCObject* pSender);
	void menuRoom4Callback(CCObject* pSender);
	void menuRoom5Callback(CCObject* pSender);


	void enterRoom(short flag);
	// implement the "static node()" method manually
	CREATE_FUNC(LayerHall);
};

#endif 