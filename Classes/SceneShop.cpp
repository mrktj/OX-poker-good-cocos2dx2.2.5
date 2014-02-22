#include "SceneShop.h"
#include "SceneWelcome.h"
#include "SceneHall.h"
#include "Managers.h"
#include "Tools.h"
#include "VisibleRect.h"

#include "AdManager.h"

using namespace cocos2d;

int manager_x = 512; //右侧管理器坐标
int manager_y = 300;

CCScene* LayerShop::scene(short fromWhichScene)
{
	CCScene * scene = NULL;
	do 
	{
		scene = CCScene::create();
		CC_BREAK_IF(! scene);

		LayerShop *layer = LayerShop::create();
		CC_BREAK_IF(! layer);
		layer->setFromWhichScene(fromWhichScene);
		scene->addChild(layer);

        if(Tools::sharedTools()->getShouldAd()){
            AdManager::sharedAdManager()->showAd();
        }

        
	} while (0);
	return scene;
}


bool LayerShop::init()
{
	CCSize size = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint point = CCDirector::sharedDirector()->getVisibleOrigin();

	bool bRet = false;
	do 
	{
		m_current_actived=0;
		m_next_actived=0;

		CCSprite* pSprite = CCSprite::create("scene_welcome_bg.png");
		CC_BREAK_IF(! pSprite);
		pSprite->setPosition(ccp(size.width/2, size.height/2));
//		pSprite->setScaleX(size.width / pSprite->getContentSize().width);
//		pSprite->setScaleY(size.height/ pSprite->getContentSize().height);
		this->addChild(pSprite);

		//back
		CCMenuItemImage *pItem3 = CCMenuItemImage::create(
			"scene_game_btn_back0.png",
			"scene_game_btn_back1.png",
			this,
			menu_selector(LayerShop::menuCloseCallback));
		CC_BREAK_IF(! pItem3);
		pItem3->setPosition(point);
		pItem3->setAnchorPoint(ccp(0,0));
		CCMenu* pMenu3 = CCMenu::create(pItem3, NULL);
		pMenu3->setPosition(CCPointZero);
		CC_BREAK_IF(! pMenu3);
		this->addChild(pMenu3);
		pItem3->setRotation(-180);
		CCActionInterval*  act1 = CCRotateTo::create(0.15f, 0);
		CCActionInterval* seqBack = CCSequence::createWithTwoActions(CCDelayTime::create(0.3),act1);
		pItem3->runAction(seqBack);

		//个人资产框
		MyMoneyInfoBoard* moneyBoard = new MyMoneyInfoBoard();
		addChild(moneyBoard);
		//moneyBoard->setScale(0.85f);
		moneyBoard->setPosition(ccp(VisibleRect::leftTop().x+111*moneyBoard->getScale() - 500,VisibleRect::leftTop().y - 34*moneyBoard->getScale()));
		CCActionInterval*  actionForMoneyBoard = CCMoveBy::create(0.2f, ccp(500,0));
		CCActionInterval* seqMoney = CCSequence::createWithTwoActions(CCDelayTime::create(0.5),actionForMoneyBoard);
		moneyBoard->runAction(seqMoney);

		for(short i =0;i<3;i++){
			ShopItemsManager * manager = new ShopItemsManager(i,moneyBoard);
			addChild(manager);
			manager->setPosition(ccp(manager_x,-manager_y));
			m_manangers[i]=manager;
			manager->autorelease();
		}

		initMenus();
		show();
	
		MACRO_CREATE_GOLDBOX_TIMER
		MACRO_GOLDBOX_TIMER_WITH_MONEYBOARD

		//MACRO_CREATE_GOLDBOX_AMOUNT
		//MACRO_GOLDBOX_AMOUNT_WITH_MONEYBOARD

		bRet = true;

	} while (0);
	return bRet;
}


void LayerShop::initMenus()
{
	CCActionInterval*  actionTopToDown = CCMoveBy::create(0.24f, ccp(0, -300));
	CCActionInterval* seq = CCSequence::createWithTwoActions(CCDelayTime::create(0.75f),actionTopToDown);

	CCSprite* btn00 = CCSprite::create("scene_shop_btn_charge0.png");
	btn00->setPosition(ccp(VisibleRect::center().x-172,VisibleRect::top().y-38+300));
	this->addChild(btn00,TAG_BTN0_NORMAL,TAG_BTN0_NORMAL);
	btn00->setVisible(false);

	CCSprite* btn01 = CCSprite::create("scene_shop_btn_charge1.png");
	btn01->setPosition(ccp(VisibleRect::center().x-172,VisibleRect::top().y-38+300));
	this->addChild(btn01,TAG_BTN0_FOCUSED,TAG_BTN0_FOCUSED);

	CCSprite* btn10 = CCSprite::create("scene_shop_btn_charge2.png");
	btn10->setPosition(ccp(VisibleRect::center().x,VisibleRect::top().y-38+300));
	this->addChild(btn10,TAG_BTN1_NORMAL,TAG_BTN1_NORMAL);

	CCSprite* btn11 = CCSprite::create("scene_shop_btn_charge3.png");
	btn11->setPosition(ccp(VisibleRect::center().x,VisibleRect::top().y-38+300));
	this->addChild(btn11,TAG_BTN1_FOCUSED,TAG_BTN1_FOCUSED);
	btn11->setVisible(false);

	CCSprite* btn20 = CCSprite::create("scene_shop_btn_exchange0.png");
	btn20->setPosition(ccp(VisibleRect::center().x+172,VisibleRect::top().y-38+300));
	this->addChild(btn20,TAG_BTN2_NORMAL,TAG_BTN2_NORMAL);

	CCSprite* btn21 = CCSprite::create("scene_shop_btn_exchange1.png");
	btn21->setPosition(ccp(VisibleRect::center().x+172,VisibleRect::top().y-38+300));
	this->addChild(btn21,TAG_BTN2_FOCUSED,TAG_BTN2_FOCUSED);
	btn21->setVisible(false);

	btn00->runAction((CCActionInterval*)seq->copy());
	btn01->runAction((CCActionInterval*)seq->copy());
	btn10->runAction((CCActionInterval*)seq->copy());
	btn11->runAction((CCActionInterval*)seq->copy());
	btn20->runAction((CCActionInterval*)seq->copy());
	btn21->runAction((CCActionInterval*)seq->copy());

	//	CCSprite* btn30 = CCSprite::create("scene_shop_btn_coin20.png");
	//	btn30->setPosition(ccp(200,140));
	//	this->addChild(btn30,TAG_BTN3_NORMAL,TAG_BTN3_NORMAL);

	//	CCSprite* btn31 = CCSprite::create("scene_shop_btn_coin21.png");
	//	btn31->setPosition(ccp(200,140));
	//	this->addChild(btn31,TAG_BTN3_FOCUSED,TAG_BTN3_FOCUSED);
	//	btn31->setVisible(false);

	m_current_actived_normal_tag = TAG_BTN0_NORMAL;
}

void LayerShop::setFromWhichScene(short isfrom){
	this->m_fromWhichScene = isfrom;
}

void LayerShop::menuCloseCallback(CCObject* pSender)
{
    AdManager::sharedAdManager()->hideAd();
	Tools::sharedTools()->saveAllData();
	this->unscheduleAllSelectors();
	this->removeAllChildrenWithCleanup(true);
//	CCScene *scene =LayerWelcome::scene();
//	CCDirector::sharedDirector()->replaceScene(CCTransitionFadeUp::create(0.5f,scene ) );
//	CCDirector::sharedDirector()->replaceScene(scene);
	if(m_fromWhichScene==2){
		CCDirector::sharedDirector()->popScene();
	}else if(m_fromWhichScene==1){
		CCScene *scene =LayerHall::scene();
		CCDirector::sharedDirector()->replaceScene(scene);
	}else if(m_fromWhichScene==0){
		//welcome
		CCScene *scene =LayerWelcome::scene();
		CCDirector::sharedDirector()->replaceScene(scene);
	}		
}

void LayerShop::show(){
	m_manangers[m_next_actived]->stopAllActions();
	m_manangers[m_next_actived]->setPosition(ccp(manager_x,-manager_y));
	
	CCActionInterval*  action = CCMoveTo::create(20/60.0f,ccp(manager_x,manager_y));
	m_manangers[m_next_actived]->runAction(action);
}

void LayerShop::hide(){
	m_manangers[m_current_actived]->stopAllActions();
	m_manangers[m_current_actived]->setPosition(ccp(manager_x,manager_y));
	CCActionInterval*  action = CCMoveTo::create(20/60.0f,ccp(manager_x+1000,manager_y));
	m_manangers[m_current_actived]->runAction(action);
}

bool LayerShop::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{	
	CCSize s =getChildByTag(TAG_BTN0_NORMAL)->getContentSize();
	CCRect s2 = CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
	CCPoint pp0  =getChildByTag(TAG_BTN0_NORMAL)->convertTouchToNodeSpaceAR(pTouches);
	CCPoint pp1  =getChildByTag(TAG_BTN1_NORMAL)->convertTouchToNodeSpaceAR(pTouches);
	CCPoint pp2  =getChildByTag(TAG_BTN2_NORMAL)->convertTouchToNodeSpaceAR(pTouches);
//	CCPoint pp3  =getChildByTag(TAG_BTN3_NORMAL)->convertTouchToNodeSpaceAR(pTouches);
	if(s2.containsPoint(pp0)){
		if(m_current_actived!=0){		
			getChildByTag(TAG_BTN0_FOCUSED)->setVisible(true);
			getChildByTag(TAG_BTN0_NORMAL)->setVisible(false);
			getChildByTag(m_current_actived_normal_tag)->setVisible(true);
			getChildByTag(m_current_actived_normal_tag+1)->setVisible(false);
			m_current_actived_normal_tag = TAG_BTN0_NORMAL;
			m_next_actived=0;
			hide();
			show();
			m_current_actived=0;
		}
		return false;
	}else if(s2.containsPoint(pp1)){
		if(m_current_actived!=1){
			getChildByTag(TAG_BTN1_FOCUSED)->setVisible(true);
			getChildByTag(TAG_BTN1_NORMAL)->setVisible(false);
			getChildByTag(m_current_actived_normal_tag)->setVisible(true);
			getChildByTag(m_current_actived_normal_tag+1)->setVisible(false);
			m_current_actived_normal_tag = TAG_BTN1_NORMAL;
			m_next_actived=1;
			hide();
			show();
			m_current_actived=1;
		}
		return false;		
	}
	else if(s2.containsPoint(pp2)){
		if(m_current_actived!=2){
			getChildByTag(TAG_BTN2_FOCUSED)->setVisible(true);
			getChildByTag(TAG_BTN2_NORMAL)->setVisible(false);
			getChildByTag(m_current_actived_normal_tag)->setVisible(true);
			getChildByTag(m_current_actived_normal_tag+1)->setVisible(false);
			m_current_actived_normal_tag = TAG_BTN2_NORMAL;
			m_next_actived=2;
			hide();
			show();
			m_current_actived=2;
		}
		return false;		
	}
/*
	else if(s2.containsPoint(pp3)){
		if(m_current_actived!=3){
			getChildByTag(TAG_BTN3_FOCUSED)->setVisible(true);
			getChildByTag(TAG_BTN3_NORMAL)->setVisible(false);
			getChildByTag(m_current_actived_normal_tag)->setVisible(true);
			getChildByTag(m_current_actived_normal_tag+1)->setVisible(false);
			m_current_actived_normal_tag = TAG_BTN3_NORMAL;
			m_next_actived=3;
			hide();
			show();
			m_current_actived=3;
		}
		return false;		
	}
	*/
	return false;
}

void LayerShop::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCLayer::onEnter();
}

void LayerShop::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}   