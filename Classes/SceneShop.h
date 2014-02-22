#ifndef __SCENE_SHOP_H__
#define __SCENE_SHOP_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Controls.h"
#include "Managers.h"

class LayerShop : public cocos2d::CCLayer
{
private:
	const static short TAG_BTN0_NORMAL = 10;
	const static short TAG_BTN0_FOCUSED = 11;
	const static short TAG_BTN1_NORMAL = 12;
	const static short TAG_BTN1_FOCUSED = 13;
	const static short TAG_BTN2_NORMAL = 14;
	const static short TAG_BTN2_FOCUSED = 15;
//	const static short TAG_BTN3_NORMAL = 16;
//	const static short TAG_BTN3_FOCUSED = 17;

	short m_current_actived_normal_tag; //focused 的值+1
	short m_next_actived_normal_tag;

	short m_current_actived;
	short m_next_actived;
	short m_fromWhichScene;

	ShopItemsManager* m_manangers[3];

	void initMenus();

	void show();
	void hide();
	void showManager(short flag);

	void menuCloseCallback(CCObject* pSender);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
	virtual void onEnter();
	virtual void onExit();
public:
	virtual bool init();  
	static cocos2d::CCScene* scene(short fromWhichScene);
	CREATE_FUNC(LayerShop);
	void setFromWhichScene(short isfrom);
};

#endif  