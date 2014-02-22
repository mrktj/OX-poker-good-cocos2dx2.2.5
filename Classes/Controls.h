#ifndef __CONTROLS_H__
#define __CONTROLS_H__

#include "cocos2d.h"
#include "GameLogic.h"
#include "SimpleAudioEngine.h"
#include <string>

#define MACRO_CREATE_GOLDBOX_TIMER 		MyGoldBox* box = new MyGoldBox();\
	addChild(box,999,999);\
	box->setPosition(ccpAdd(VisibleRect::rightTop(), ccp(-30+200,-30)));\
	box->autorelease();\
	CCActionInterval*  actionForGoldBox = CCMoveBy::create(0.5f, ccp(-200,0));\
	box->runAction(actionForGoldBox);


#define MACRO_GOLDBOX_TIMER_IN_DESK box->setUserBoard(m_managerPlayer->m_board);
#define MACRO_GOLDBOX_TIMER_IN_WELCOME box->setInfoBoard(info);

#define MACRO_CREATE_MONEY_BOARD 		MyMoneyInfoBoard* moneyBoard = new MyMoneyInfoBoard();\
moneyBoard->setPosition(ccp(VisibleRect::leftTop().x+111 - 500,VisibleRect::leftTop().y - 34));\
addChild(moneyBoard);\
CCActionInterval*  actionForMoneyBoard = CCMoveBy::create(0.5f, ccp(500,0));\
moneyBoard->runAction(actionForMoneyBoard);



#define MACRO_GOLDBOX_TIMER_WITH_MONEYBOARD box->setMoneyBoard(moneyBoard);

#define MACRO_CREATE_GOLDBOX_AMOUNT 	MyGoldBoxByAmount* box_amount = new MyGoldBoxByAmount();\
	addChild(box_amount,999,998);\
	box_amount->setPosition(ccpAdd(VisibleRect::rightTop(), ccp(-30+200,-100)));\
	box_amount->autorelease();\
	CCActionInterval*  actionForGoldBoxAmount = CCMoveBy::create(1.0f, ccp(-200,0));\
	box_amount->runAction(actionForGoldBoxAmount);
#define MACRO_GOLDBOX_AMOUNT_IN_WELCOME box_amount->setInfoBoard(info);
#define MACRO_GOLDBOX_AMOUNT_WITH_MONEYBOARD box_amount->setMoneyBoard(moneyBoard);



/****************************************************************************************************
*								   								   								    *
*                                    MyClock实现                                                    *
*							        							        							*
*****************************************************************************************************/
class MyClock: public cocos2d::CCNode
{
public:
    int currentSeconds;
	cocos2d::CCLabelTTF* pLabel;
public:
	virtual bool init();  
	void clockCallback(CCObject* pSender);
	void timesUpCallback(CCObject* pSender);
	void removeClock();
	CREATE_FUNC(MyClock);
};

/****************************************************************************************************
*								   								   								    *
*                                   MyMoneyInfoBoard												*
*							        							        							*
*****************************************************************************************************/
class MyMoneyInfoBoard: public cocos2d::CCNode
{
public:
	MyMoneyInfoBoard();

	void updateLabelCoin();
	void updateLabelIngot(); 
private:	
	cocos2d::CCLabelBMFont* pLabelIngot;
	cocos2d::CCLabelBMFont* pLabelCoin;

};

/****************************************************************************************************
*								   								   								    *
*                                    MyUserBoard实现                                                *
*							        							        							*
*****************************************************************************************************/
class MyUserBoard: public cocos2d::CCNode
{
public:
//	char[] m_name;
	MyUserBoard(bool isRobot,bool isCoin);
	void showTime(short seconds);	
	void stopShowTime();
	unsigned int m_money;
	cocos2d::CCLabelTTF* m_pLabel_money;
	cocos2d::CCLabelTTF* m_pLabel_name;
	void updateMoney(unsigned int money);
	bool m_sex_isBoy;
private:
	bool m_isShowTime; //是否是当前玩家操作时间
	bool m_isRobot; //是否是机器人，若是的话就用随机头像和名字
	bool m_isCoin; //是否银币场，否则为银锭
	short m_totalSeconds; //可操作时间
	float m_currentSeconds; //当为showtime时，已过时间
	cocos2d::CCProgressFromTo *m_pTo ;
	cocos2d::CCProgressTimer *m_pTimer;
	void initSelf();
	void changeColorCallback(CCObject* pSender);
	void stopCallback(CCObject* pSender);
};

/****************************************************************************************************
*								   								   								    *
*                                    MyCardBase实现                                                 *
*							        							        							*
*****************************************************************************************************/
class MyCardBase : public cocos2d::CCSprite
{
public:
	MyCardBase(CardKind kind ,int value,float x,float y);
	MyCardBase(int kind ,int value,float x,float y);
	int m_value;  //面值
	void setFocused(); //当前是否focus
private:
	bool m_isChoosed; //状态变量，是否选中，需要初始化
	bool m_currentChanged; //状态变量，在当前点击流程中是否改变，避免在touchmove的时候不停改变状态，需要初始化
	CardKind m_kind; //花色	
	float m_default_x, m_default_y; //未选中时的初始位置
	float m_offset_y; //选中时上移距离，需要初始化
	
	void initSelf(); //根据kind和value来初始化扑克图片
	//void shoot(int flag,int sum); //出牌动画。flag为序号，第几张，sum表示一共几张~~~~已取消，斗地主才用
};


/****************************************************************************************************
*								   								   								    *
*                                    MyCardSelf实现                                                 *
*							        							        							*
*****************************************************************************************************/
class MyCardSelf :  public cocos2d::CCSprite, public cocos2d::CCTargetedTouchDelegate
{
public:
	bool m_isChoosed; //状态变量，是否选中，需要初始化
	int m_value;  //面值
	MyCardSelf(CardKind kind ,int value,float x,float y);
	MyCardSelf(int kind ,int value,float x,float y);
		void setFocused(bool focus); //当前是否focus
private:	
	bool m_currentChanged; //状态变量，在当前点击流程中是否改变，避免在touchmove的时候不停改变状态，需要初始化
	CardKind m_kind; //花色
	float m_default_x, m_default_y; //未选中时的初始位置
	float m_offset_y; //选中时上移距离，需要初始化

	void setChoosed(bool choosed); // true 为选中操作，false时为正常状态

	void initSelf(); //根据kind和value来初始化扑克图片
	//void shoot(int flag,int sum); //出牌动画。flag为序号，第几张，sum表示一共几张~~~~已取消，斗地主才用

	cocos2d::CCRect rect();

	virtual void onEnter();
	virtual void onExit();
	bool containsTouchLocation(cocos2d::CCTouch* touch);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
	//virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ; //已取消
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent) ;

	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();
};

/****************************************************************************************************
*								   								   								    *
*                                    MyGameTopBar实现                                               *
*							        							        							*
*****************************************************************************************************/
class MyGameTopBar :  public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
	MyGameTopBar();
private:	
	bool m_isTouched; //状态变量，是否按下

	const static short TAG_BG = 5;
	const static short TAG_POKER = 10;
	const static short TAG_SOUND_ON = 11;
	const static short TAG_SOUND_OFF = 12;
	const static short TAG_PRIZE = 13;
	const static short TAG_HELP = 14;

	const static short TAG_TIP = 1;

	void initSelf(); 
	virtual void onEnter();
	virtual void onExit();
	bool containsTouchLocation(cocos2d::CCTouch* touch);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
//	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ; 
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent) ;
	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();
	cocos2d::CCRect rect();
};


/****************************************************************************************************
*								   								   								    *
*                                   MyMoneyBarTouchable												        *
*							        							        							*
*****************************************************************************************************/
class MyMoneyBarTouchable: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
	MyMoneyBarTouchable(int flag, long money);
	void moveIn();
	void moveTo(int flag);
	void refreshWithMoney(long money);
private:	
	bool m_isMoved; //在四个押注中选出的一个，当为false时才能setFocus
	short m_flag;
	unsigned int m_money;
	float m_default_x, m_default_y; //初始位置
	cocos2d::CCLabelTTF* m_label_money;
	cocos2d::CCPoint getDefaultPoint(int flag);
	void setFocused(bool focus); //当前是否focus
	void initSelf(); //根据kind和value来初始化扑克图片
	virtual void onEnter();
	virtual void onExit();
	bool containsTouchLocation(cocos2d::CCTouch* touch);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent) ;
	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();
	cocos2d::CCRect rect();
};


/****************************************************************************************************
*								   								   								    *
*                                   MyMoneyBar												        *
*							        							        							*
*****************************************************************************************************/
class MyMoneyBar: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
	unsigned int m_money;
	MyMoneyBar(short flag, long money);
	void moveIn();//进场
	void moveTo(int flag);//给赢家
	void refreshWithMoney(long money);
	bool m_isMoved; //在四个押注中选出的一个，当为false时才能setFocus
private:	
	short m_flag;
	float m_default_x, m_default_y; //初始位置
	cocos2d::CCLabelTTF* m_label_money;
	cocos2d::CCPoint getDefaultPoint(int flag);
	void setFocused(bool focus); //当前是否focus
	void initSelf(); //根据kind和value来初始化扑克图片
	void removeCallback(CCObject* pSender);

	virtual void onEnter();
	virtual void onExit();
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent) ;
	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();
	cocos2d::CCRect rect();
	bool containsTouchLocation(cocos2d::CCTouch* touch);	
};



/****************************************************************************************************
*								   								   								    *
*                                   MyMoneyBarRobot												    *
*							        							        							*
*****************************************************************************************************/
class MyMoneyBarRobot: public cocos2d::CCNode
{
public:
	unsigned int m_money;
	MyMoneyBarRobot(short flag, unsigned int money);
	void moveIn();
	void moveTo(short flag);
	void refreshWithMoney(unsigned int money);
	bool containsTouchLocation(cocos2d::CCTouch* touch);	
private:	
	bool m_isMoved; //在四个押注中选出的一个，当为false时才能setFocus
	short m_flag;
	float m_default_x, m_default_y; //初始位置
	cocos2d::CCLabelTTF* m_label_money;
	cocos2d::CCPoint getDefaultPoint(short flag);
	void setFocused(bool focus); //当前是否focus
	void initSelf(); //根据kind和value来初始化扑克图片
	cocos2d::CCRect rect();
	void removeCallback(CCObject* pSender);
};


/****************************************************************************************************
*								   								   								    *
*                                   MyInfoBoard												        *
*							        							        							*
*****************************************************************************************************/
class MyInfoBoard: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:
	MyInfoBoard();
	void setNickName(std::string nickname);
	void setHead(short flag);
	bool containsTouchLocation(cocos2d::CCTouch* touch);	

	bool m_isHeadChooseBarShowed;//这个变量很重要，也很危险，当headChoosedBar指针的对象有头像被点时，这个值才被false
	void setLabelCoin(unsigned int money);
private:	
	const static  short  TAG_HEAD = 100;
	const static  short  TAG_HEAD_MASK = 110;
	const static  short  TAG_HEAD_CHOOSE_BAR = 120;
	const static  short  TAG_PRIZE_BTN = 130;
	const static  short  TAG_PRIZE_BTN_TOAST = 131;
	const static  short  TAG_EDIT_BTN = 132;

	cocos2d::CCLabelTTF* m_label_name;

	cocos2d::CCProgressTimer* m_loading_bar;
	cocos2d::CCLabelBMFont* pLabelBar;

	cocos2d::CCLabelBMFont* pLabelCoin;

	void initSelf();
	cocos2d::CCRect rect();
	cocos2d::CCLabelBMFont* addLabel(char* str,cocos2d::CCPoint point);
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;

	virtual void onEnter();
	virtual void onExit();

	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();
};

/****************************************************************************************************
*								   								   								    *
*                                    MyClock2  new 方式实现                                         *
*							        							        							*
*****************************************************************************************************/
class MyClock2: public cocos2d::CCNode
{
public:	
	MyClock2(int seconds);
private:
	int currentSeconds;
	cocos2d::CCLabelTTF* pLabel;
	cocos2d::CCSprite* pSprite;
	short m_value;
	void clockCallback(CCObject* pSender);
	void timesUpCallback(CCObject* pSender);
	void removeClock();
};

/****************************************************************************************************
*								   								   								    *
*                                   ShopScene -- MyShopItem                                           *
*							        							        							*
*****************************************************************************************************/
class MyShopItem: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:	
	MyShopItem(short flag);
	void setMoneyBoard(MyMoneyInfoBoard* board);
private:
	short m_flag;
	bool  m_isOpened;
	bool  m_isBuyClicked;

	MyMoneyInfoBoard* m_moneyBoard;

	const static  short  TAG_BOARD_DOWN = 10;
	const static  short  TAG_BTN_UP=12;
	const static  short  TAG_BTN_DOWN=13;
	const static  short  TAG_BOARD_UP = 14;
	const static  short  TAG_ARROW=15;
	const static  short  TAG_TOAST=16;

	void initByFlag();
	void openDetail();
	void closeDetail();
	void buy();
	void showToast(bool isSuccess);
	void removeToast(float dt);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent) ;

	virtual void onEnter();
	virtual void onExit();
	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();
};

/****************************************************************************************************
*								   								   								    *
*                                   MyCalcBoard  											        *
*							        							        							*
*****************************************************************************************************/
class MyCalcBoard: public cocos2d::CCNode
{
public:
	MyCalcBoard(bool isCoinRoom);
	void setInfo(short flag,const char* name,short ox,unsigned int money,bool win,unsigned int result);
	void setBoss(short flag);
	void setExperience(short* exps);
private:	
	bool m_isCoinRoom;
	void initSelf();
	cocos2d::CCLabelBMFont* addLabel(short font_kind,char* str,cocos2d::CCPoint point); //font_kind [0,2]

	cocos2d::CCLabelBMFont* m_label_money_total;
	cocos2d::CCLabelBMFont* m_label_money_change;

	short m_current_change_times;
	unsigned int m_money_total;
	int m_money_change;
	void changeLabel(float dt);
};



/****************************************************************************************************
*								   								   								    *
*                                   WelcomeScene -- MyHeadChooseBar                                 *
*							        							        							*
*****************************************************************************************************/
class MyHeadChooseBar: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:	
	MyHeadChooseBar(MyInfoBoard* info);
	void showAll();
private:
	const static short TAG_HEAD = 10;
	const static short TAG_MASK=100;

	MyInfoBoard* m_info;

	short m_flag_focused;//按下状态和move时用


	void hideAll();

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;

	virtual void onEnter();
	virtual void onExit();

	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();
};

/****************************************************************************************************
*								   								   								    *
*                                    MyToast		                                                *
*							        							        							*
*****************************************************************************************************/
typedef enum tagToast
{
	eToastAntiHacker,
	eToastMoneyNotEnough,
	eToastCantExitGaming,
	eToastNeedVip,	
	eToastNoneRobBoss,
	eToastWillGetMoney,
	eToastAlreadyGetMoney,
	eToastOxError
}Toast; 

class MyToast: public cocos2d::CCNode
{
public:	

	const static short TAG_TOAST = 100;
	MyToast(Toast tagToast);

private:
	void removeToast(float dt);
};




/****************************************************************************************************
*								   								   								    *
*                                    MyGoldBox		                                                *
*							        							        							*
*****************************************************************************************************/
class MyGoldBox: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:	
	MyGoldBox();
	void setUserBoard(MyUserBoard* board);
	void setInfoBoard(MyInfoBoard* board);
	void setMoneyBoard(MyMoneyInfoBoard* board);
private:
	const static short TAG_BOX = 12;
	const static short TAG_SPEC0=10;
	const static short TAG_SPEC1 = 11;
	const static short TAG_TOAST = 20;
	const static short TAG_TOAST_LABEL = 21;
	
	cocos2d::CCLabelBMFont* pLabel;

	//实时更新
	MyUserBoard* m_userBoard; 
	bool m_isUserBoardSetted;
	MyInfoBoard* m_infoBoard;
	bool m_isInfoBoardSetted;
	MyMoneyInfoBoard* m_moneyBoard;
	bool m_isMoneyBoardSetted;

	bool m_isShiningBoxShowed;

	void updateCallBack(float dt);

	void updateLabel();
	void showShiningBox();
	void openBox();
	void showToast(bool isSuccess);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
	virtual void onEnter();
	virtual void onExit();
	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();

	~MyGoldBox();
};

/****************************************************************************************************
*								   								   								    *
*                                    MyGoldBoxByAmount		                                        *
*							        							        							*
*****************************************************************************************************/
class MyGoldBoxByAmount: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:	
	MyGoldBoxByAmount();
	void setUserBoard(MyUserBoard* board);
	void setInfoBoard(MyInfoBoard* board);
	void setMoneyBoard(MyMoneyInfoBoard* board);
	void updateData();

private:
	const static short TAG_BOX = 12;
	const static short TAG_SPEC0=10;
	const static short TAG_SPEC1 = 11;
	const static short TAG_TOAST = 20;
	const static short TAG_TOAST_LABEL = 21;
	const static short TAG_TOAST_LABEL2 = 22;

	cocos2d::CCLabelBMFont* pLabel;

	//实时更新
	MyUserBoard* m_userBoard; 
	bool m_isUserBoardSetted;
	MyInfoBoard* m_infoBoard;
	bool m_isInfoBoardSetted;
	MyMoneyInfoBoard* m_moneyBoard;
	bool m_isMoneyBoardSetted;

	bool m_isShiningBoxShowed;

	void updateLabel();
	void showShiningBox();
	void openBox();
	void showToast(bool isSuccess);

	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
	virtual void onEnter();
	virtual void onExit();
	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();

	~MyGoldBoxByAmount();
};



/****************************************************************************************************
*								   								   								    *
*                                    MyBaseNodeTouchToRemove		                                *
*							        							        							*
*****************************************************************************************************/
class MyBaseNodeTouchToRemove: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:	
	MyBaseNodeTouchToRemove();
	MyBaseNodeTouchToRemove(char* picName);
	const static short TAG_BASE = 10;
private:
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
	virtual void onEnter();
	virtual void onExit();
	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();

};

/****************************************************************************************************
*								   								   								    *
*                                    MyUpgradeBoard		                                *
*							        							        							*
*****************************************************************************************************/
class MyUpgradeBoard: public MyBaseNodeTouchToRemove
{
public:	
	MyUpgradeBoard(unsigned int* current ,unsigned int* next);
private:
};

/****************************************************************************************************
*								   								   								    *
*                                    MyNextPrizeButton       		                                *
*							        							        							*
*****************************************************************************************************/
class MyNextPrizeButton: public cocos2d::CCNode, public cocos2d::CCTargetedTouchDelegate
{
public:	
	MyNextPrizeButton();
	const static short TAG_BTN = 10;
	const static short TAG_TOAST = 20;
	const static short TAG_TOAST_CHILD1 = 30;
	const static short TAG_TOAST_CHILD2 = 31;
	const static short TAG_TOAST_CHILD3 = 32;
	const static short TAG_TOAST_CHILD4 = 33;
private:
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
	virtual void onEnter();
	virtual void onExit();
	virtual void touchDelegateRetain();
	virtual void touchDelegateRelease();
	void showToast();
};

#endif 