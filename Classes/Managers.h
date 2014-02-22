#ifndef __MANAGERS_H__
#define __MANAGERS_H__

#include "Controls.h"
#include "cocos2d.h"
#include "GameLogic.h"
#include <list>

using namespace cocos2d;

/************************************
*								    *
*            CardManager            *
*							        *
*************************************/
class CardManager: public cocos2d::CCNode
{
public:	
	CardManager(float x,float y); 
	~CardManager();
	short getResultsAuto();  //返回牛牛结果 -1为error
	short getResultsHand();  //返回牛牛结果 -1为error
	bool m_isWashingDone; //是否发牌结束。发牌结束方可摊牌
	short getBigestValue(); //得到最大单牌的值
	short getBigestKind();  //得到最大单牌的花色
	short m_ox; //这个值一定要有，因为不管手动摆牛还是自动摆牛，都把结果赋给这个值，作为最后结算的时候所用
private:
	const static int TAG_CARDS=10;  //卡牌ID，用于逐秒显示 [10,14]
	const static int TAG_TOAST_SENDING_CARDS=20;  //发牌中toast
	float m_x;
	float m_y;
	short m_offset;//牌间距
	short m_cards_values[5]; //数组
	short m_cards_kinds[5]; //数组
	MyCardSelf *m_cards[5]; //数组
	bool isFiveFlowerOx();
	void refreshCards(); //得到5张牌

	short m_showed_flag; //已显示的牌的id序号[-1,4]
	void showOneCardScheduler(float dt);
};

/************************************
*								    *
*            CardManagerRobot       *
*							        *
*************************************/
class CardManagerRobot: public cocos2d::CCNode
{
public:	
	CardManagerRobot();
	CardManagerRobot(float x,float y); 
	~CardManagerRobot();
	void showAll();

	short m_ox; //摆牛把结果赋给这个值，作为最后结算的时候所用
	short getResults();  //返回牛牛结果
	short getBigestValue(); //得到最大单牌的值
	short getBigestKind();  //得到最大单牌的花色
private:
	const static int TAG_CARDS = 20;
	const static int TAG_COVERS = 40;
	float m_x;
	float m_y;
	short m_offset;//牌间距
	short m_cards_values[5]; //数组
	short m_cards_kinds[5]; //数组
	MyCardBase *m_cards[5]; //数组
	bool m_isWashingDone; //是否发牌结束。发牌结束方可摊牌
	short m_showed_flag;
	bool isFiveFlowerOx();
	void refreshCards(); //得到5张牌
	void hideAll();
	void showOneCardScheduler(float dt);
};

/************************************
*								    *
*            RobotManager           *
*							        *
*************************************/
class RobotManager: public cocos2d::CCNode
{
public:	
	const static int TAG_STANDBY=10;
	const static int TAG_CARDS = 20;
	const static int TAG_MONEY_BAR = 30;   //四个闲家的条
	const static int TAG_USERBOARD = 40;
	const static int TAG_OX = 50; 
	const static int TAG_ROB = 60;

	RobotManager(short flag,short room_kind); 
	~RobotManager();

	void setGameStat(GameStat stat);

	short m_room_kind;
	bool m_isStandBy;
	short m_isRobed;  //是否抢庄,-1未表态，0不抢，1抢
	bool m_isBoss;  //是否庄家
	bool m_isShowMoney;
	bool m_isShowCards;
	bool m_isShowOx;
	bool m_isCalcResult;
	
	unsigned m_bossMoney;//庄家的钱。用来和机器人的钱比较，计算押注金额

	MyUserBoard* m_board;
	CardManagerRobot* m_cards;
	MyMoneyBarRobot* m_money;
private:
	GameStat m_gameStat;
	
	short m_flag;
	bool m_isScheduleCalling;

	void updateCallback(CCObject* pSender);

	void initRobot(); 

	void showStandBy();
	void removeStandBy();

	void showRobChat();
	void removeRobChat();
	
	void showMoneyBar();
	void removeMoneyBar();

	void showCards();	
	void removeCards();
	void refreshCards();

	void showOx();
	void removeOx();

	void refreshData();  //清空坐标数据等

};

/************************************
*								    *
*           PlayerManager           *
*							        *
*************************************/
class PlayerManager: public cocos2d::CCNode
{
public:	
	const static short TAG_STANDBY=10;
	const static short TAG_CARDS = 20;
	const static short TAG_USERBOARD = 30;
	static const short TAG_FOUR_BARS =40; //每次递增1 [40,43]
	short TAG_MONEY_BAR; //选择好四个条中的一个的时候，就把TAG_FOUR_BARS+i的值赋给它
	const static short TAG_OX = 50; 
	const static short TAG_ROB = 60;
	

	PlayerManager(short room_kind); //是否银币场 
	~PlayerManager();

	GameStat m_gameStat;
	void setGameStat(GameStat stat);

	short m_room_kind;
	bool m_isStandBy;
	short m_isRobed;  //是否抢庄,-1未表态，0不抢，1抢
	bool m_isBoss;  //是否庄家
	bool m_isShowMoney;
	bool m_isShowCards;
	bool m_isShowOx;
	bool m_isCalcResult;

	unsigned m_bossMoney;//庄家的钱。用来和闲的钱比较，计算押注金额

	void removeStandBy();
	void removeRobChat();

	MyUserBoard* m_board;
	CardManager* m_cards;
	MyMoneyBar* m_money;

	void showOxAuto();
	bool showOxHand();

	void updateGameData(int currentWinOrLoseMoney,short exp,CCLayer* layer);
private:	
	MyMoneyBar* m_moneys_temp[4];
		
	void initPlayer(); 

	void showStandBy();

	void showRobChat();
	
	void showMoneyBar(); //显示四条哦
	void removeMoneyBar();
	void checkMoneyBarsStat(float dt);	 //监听是否有bar被选中

	void showCards();	
	void removeCards();
	void refreshCards();

	
	void removeOx();

	void refreshData();  //清空坐标数据等
};

/************************************
*								    *
*            ShopItemsManager       *
*							        *
*************************************/
class ShopItemsManager: public cocos2d::CCNode
{
public:	
	ShopItemsManager(short flag,MyMoneyInfoBoard* moneyBoard); 
private:
	short m_flag;//四大分类
	MyMoneyInfoBoard* m_moneyBoard;
	void initByFlag();	
	cocos2d::CCPoint getPointAtLocation(short location);//九宫格的坐标位置 [0,8]
};

#endif