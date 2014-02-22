#include "cocos2d.h"

#include "GameLogic.h"

using namespace cocos2d;


GameLogic *GameLogic::mSharedGameLogic = NULL;


GameLogic *GameLogic::sharedGameLogic()
{
	if (mSharedGameLogic == NULL)
	{
		mSharedGameLogic = new GameLogic();
		if (!mSharedGameLogic || !mSharedGameLogic->init())
		{
			CC_SAFE_DELETE(mSharedGameLogic);
		}
	}
	return mSharedGameLogic;
}


void GameLogic::deleteSharedGameLogic()
{
    CC_SAFE_DELETE(mSharedGameLogic);
}

 //得到一张牌
int GameLogic::getCard(){
	if(m_alreadyGeted<51){
		m_alreadyGeted++;
		return m_totalCards[m_alreadyGeted];
	}else{
	 	refreshAllCards();
	}
		return getCard();
}

bool GameLogic::init()
{
	bool bRet = false;
    do 
    {
		this->m_alreadyGeted=-1;
		for(int i=0;i<52;i++)//用for循环实现洗牌
		{   
			m_totalCards[i]=i;
		}
		refreshAllCards();
        bRet = true;
    } while (0);
    return bRet;
}

void GameLogic::refreshAllCards(){
	m_alreadyGeted=-1;
	int temp;
//	cout<<"现在开始洗牌"<<endl;
	srand(time(NULL));//生成一个真随机数
	for(int i=0;i<52;i++)//用for循环实现洗牌
	{   
		int c;
		c=rand()%52;
		temp=m_totalCards[i];
		m_totalCards[i]=m_totalCards[c];
		m_totalCards[c]=temp;
	};
}

//得到入场限制
unsigned int GameLogic::getMinMoneyToEnterRoom(short room_kind){
	switch(room_kind){
	case 0:return 5;
	case 1:return 50000;
	case 2:return 500000;
	case 3:return 5000000;
	case 4:return 5;
	case 5:return 5;
	}
}

//转换牛牛结果为应该赔偿的倍数
short GameLogic::calcOxToResult(short ox){
	if(ox>=0 && ox<=6){
		return 1;
	}else if(ox==7 || ox ==8){
		return 2;
	}else if(ox==9){
		return 3;
	}else if(ox==10 || ox ==11){
		return 5;
	}else{
		return 0;
	}
}

//庄家先干输家
unsigned int GameLogic::fuckLoser(short ox_boss,unsigned money_chip,unsigned money_player){
	//得到翻倍数
	short ox_times = calcOxToResult(ox_boss);
	//得到赔偿总数
	unsigned temp = money_chip * ox_times;
	if(temp<=money_player){
		return temp;
	}else{
		return money_player;
	}
}

//庄家先干输家,然后闲家干庄家
int* GameLogic::getChipsResult(int *result,short boss_flag,bool* bossWinOrNot,short* ox,unsigned* moneys_chip,unsigned* moneys_total){
	unsigned money_boss = moneys_total[boss_flag];
	short ox_boss = ox[boss_flag];

	int* moneys_change = result;
	short fucked = 0;
	//第一步:干闲家
	for(short i = 0;i<5;i++){
		if(boss_flag!=i && bossWinOrNot[i]){
			fucked++;
			unsigned player_lost = fuckLoser(ox_boss,moneys_chip[i],moneys_total[i]);
			money_boss+=player_lost;
			moneys_change[i]=player_lost*-1;
		}
	}

	//判断是否被庄家全干死
	if(fucked == 4){
		moneys_change[boss_flag] = money_boss - moneys_total[boss_flag];
		return moneys_change;
	}

	//第二步：庄家被干，因为要加上庄家赢的钱，然后计算庄家的最终金钱，所以再循环N次来处理被干的流程
	//首先，看够不够被闲家轮干
	unsigned money_total_player_win=0;
	for(short i = 0;i<5;i++){
		if(boss_flag!=i && !bossWinOrNot[i]){
			unsigned player_win =calcOxToResult(ox[i])*moneys_chip[i];
			money_total_player_win+=player_win;	
			moneys_change[i]=player_win;		
		}
	}
	if(money_total_player_win<=money_boss){
		//庄家够被轮
		money_boss-=money_total_player_win;
		moneys_change[boss_flag]=(money_boss - moneys_total[boss_flag]);
		return moneys_change;
	}else{
		//庄家不够被轮
		float percentage = money_boss*1.0f / money_total_player_win; //得到赔偿比例
		unsigned beFucked = 0;//庄家被干的数值
		for(short i = 0;i<5;i++){
			if(boss_flag!=i && !bossWinOrNot[i]){		
				moneys_change[i]=percentage*calcOxToResult(ox[i])*moneys_chip[i];
				beFucked+=moneys_change[i];
			}	
		}
		money_boss -= beFucked;
		moneys_change[boss_flag] = money_boss - moneys_total[boss_flag];
	}
	return moneys_change;
}


GameLogic::GameLogic()
{
}


GameLogic::~GameLogic()
{
}




