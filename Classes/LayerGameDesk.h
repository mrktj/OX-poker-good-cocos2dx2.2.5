#ifndef __LAYER_GAME_DESK_H__
#define __LAYER_GAME_DESK_H__

#include "cocos2d.h"
#include "Controls.h"
#include "Managers.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

class LayerGameDesk: public cocos2d::CCLayer
{
public:
	const static int TAG_BTN_UP = 50;
	const static int TAG_BTN_DOWN = 70;
	const static int TAG_CARD_HEAP = 120;
	const static int TAG_CARD_FLY = 121;	
	const static int TAG_PLAYERS=300; //【300，304】
	const static int TAG_BOSS_ICON = 350;
	const static int TAG_CALC_BOARD = 370;
	const static int TAG_TOAST = 380;
private:
	CCSize size;
	CCPoint point;
	GameStat m_GameStat;
	short m_boss_flag;
	short m_robot_alive[4]; //-1为没钱挂了，挂了就该换机器人了
	RobotManager* m_managersRobot[4];
	PlayerManager* m_managerPlayer;
	short m_whoIsBoss;
	short m_delay_set_stat;//延时调用setGameStat，为了优化操作体验，-1表示还未达到set条件，倒计时到0说明该执行了，正值为倒计时
	bool m_isHandOx; //是否手动摆牛，手动的话有经验加成
private:
	GameLogic* logic;
	short m_room_kind;
	short m_flag_for_start;
	bool m_isOxShowed;//showOxAuto后置true，保证只能show一次。而且当在游戏中退出时，会强制showOX
	void startGameDelayed(float dt);
	void initPlayer(int flag); 
	void setRobotsStat(GameStat gameStat);
	void setPlayerStat(GameStat gameStat);
	void setGameStat(GameStat gameStat);
	void moveIn(CCNode* node,float time);
	void moveOut(CCNode* node,float time);	
	void moveOutMenus();
	GameStat couldChangeGameStat();
	GameStat whoIsBoss();

	void changeTable();

	void showCalcBoard();
	void removeCalcBoard();
	bool getCompareResultBossWith(short flag); // true:boss win   false:boss lose
	unsigned int getCalcResultBossWith(short flag,bool isBossWin);// return 应该赔偿的数额

private:
	virtual bool init();  
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
	//	virtual void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) ;
	//	virtual void ccTouchEnded(cocos2d::CCTouch *pTouch,cocos2d::CCEvent *pEvent) ;
	virtual void onEnter();
	virtual void onExit();
	// menu selector callback
	void menuCloseCallback(CCObject* pSender);
	void menuShopCallback(CCObject* pSender);
	void menuUpCallback(CCObject* pSender);
	void menuDownCallback(CCObject* pSender);	
	void exitGameDelay(float dt);
	void exitGame();

public:		
	static cocos2d::CCScene* scene();
	CREATE_FUNC(LayerGameDesk);
	void updateGameStat(float dt);	
	void toStandBy(float dt);
	void setRoomKind(short kind_id); //房间[0,5]
};

#endif 