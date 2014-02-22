#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__


/************************************
*								    *
*            ResultKind             *
*							        *
*************************************/
typedef enum tagResultKind
{
	eResultOxError,
	eResultOx0,
	eResultOx1,
	eResultOx2,
	eResultOx3,
	eResultOx4,
	eResultOx5,
	eResultOx6,
	eResultOx7,
	eResultOx8,
	eResultOx9
}oxResultKind; 

/************************************
*								    *
*            GameStat               *
*							        *
*************************************/
typedef enum tagGameStat
{
	eGameStatStandBy,
	eGameStatRobBoss,
	eGameStatShowMoney,
	eGameStatGaming,
	eGameStatCalcResult
}GameStat; 


/************************************
*								    *
*            CardKind               *
*							        *
*************************************/
typedef enum tagCardKind 
{
	eCardKindSpades,
	eCardKindHearts,
	eCardKindClubs,
	eCardKindDiamonds

}CardKind; 

/************************************
*								    *
*            GameLogic              *
*							        *
*************************************/
class GameLogic
{
public:
	static GameLogic *sharedGameLogic();
	static void deleteSharedGameLogic();	

	unsigned int getMinMoneyToEnterRoom(short room_kind); //得到入场限制

	void refreshAllCards();  //给cards打乱序
	int getCard();  //得到一张牌

	short calcOxToResult(short ox); //牛牛的倍数，如牛牛，返回5倍

	int* getChipsResult(int* result,short boss_flag,bool* bossWinOrNot,short* ox,unsigned* moneys_chip,unsigned* moneys_total); //庄家先干输家,然后闲家干庄家
private:
	GameLogic();
	~GameLogic();
	bool init();

	int m_totalCards[52]; //所有的牌，已经为乱序，0-12为黑桃,用N/13得到花色，N%13得到数字
	int m_alreadyGeted; //已经取出的扑克的数组下标，默认为-1
	static GameLogic *mSharedGameLogic;

	unsigned int fuckLoser(short ox_boss,unsigned money_chip,unsigned money_player); //庄家先干输家

};


#endif

