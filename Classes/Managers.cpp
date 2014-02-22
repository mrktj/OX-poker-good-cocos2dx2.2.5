#include "Managers.h"
#include "Controls.h"
#include "Tools.h"
#include <list>
#include "MyAudioEngine.h"
#include "VisibleRect.h"

using namespace std;

/****************************************************************************************************
*								   								   								    *
*                                    CardManager实现                                                *
*							        							        							*
*****************************************************************************************************/
CardManager::~CardManager(){
	removeAllChildren();
}

CardManager::CardManager(float x,float y){
	m_x=x;
	m_y=y;
	m_offset=63;
	m_ox=-1;
	m_isWashingDone=false;
	refreshCards();
}

//返回牛牛结果
short CardManager::getResultsHand(){
	if(isFiveFlowerOx()){
		m_ox=11;
		return m_ox;
	}

	list<short> list_choosed_flag;
	list<short> list_unchoosed_flag;

	//整理
	for(short i =0;i<5;i++){
		if(m_cards[i]->m_isChoosed){
			list_choosed_flag.push_back(i);
		}else{
			list_unchoosed_flag.push_back(i);
		}
	}

	//判断升起牌数
	if(list_choosed_flag.size()!=3){
		return -1;  //不是3+2 结构
	}

	//判断三张
	list<short>::iterator iter= list_choosed_flag.begin();
	short sum =0;
	short temp=0;

	for(;iter!=list_choosed_flag.end();iter++){
		temp = 	m_cards_values[*iter];
		//value是1-13
		if(temp>10){
			temp=10;
		}
		sum+=temp;
	}
	if(sum % 10 !=0){
		m_ox=0;
		return m_ox;
	}

	//判断剩余两张
	sum =0;
	temp =0;
	for(iter = list_unchoosed_flag.begin(); iter != list_unchoosed_flag.end() ;iter++){
		temp = 	m_cards_values[*iter];
		if(temp>10){
			temp=10;
		}
		sum+=temp;
	}

	if(sum % 10 ==0){
		m_ox=10;
		return m_ox;
	}else{
		m_ox=sum % 10;
		return m_ox;
	}
	return -1;
}

//返回牛牛结果
short CardManager::getResultsAuto(){
	if(isFiveFlowerOx()){
		m_ox=11;
		return m_ox;
	}
	short temp1=0;
	short temp2=0;
	short temp3=0;
	short temp_sum;
	//找到三张
	for (short i=0; i < 3; i++)
	{
		for (short j=i+1; j < 4; j++)
		{
			for (short k = j+1; k < 5; k++)
			{

				temp1 =  m_cards_values[i];
				temp2 =  m_cards_values[j];
				temp3 =  m_cards_values[k];

				if(temp1 >10)
					temp1=10;
				if(temp2 >10)
					temp2=10;
				if(temp3 >10)
					temp3=10;

				temp_sum = temp1+temp2+temp3;

				if (temp_sum%10 ==0)
				{
					m_cards[i]->setFocused(true);
					m_cards[j]->setFocused(true);
					m_cards[k]->setFocused(true);
					temp_sum = 0;
					int temp4 =0;
					for(short m = 0;m<5;m++){
						if(m!=i && m!=j && m!=k){
							temp4 = m_cards_values[m];	
							if(temp4 >10)
								temp4=10;
							temp_sum+=temp4;					
						}
					}

					if(temp_sum % 10 ==0){
						m_ox=10;
						return m_ox;
					}else{
						m_ox=temp_sum % 10;
						return m_ox;
					}
				}
			}
		}
	}
	m_ox=0;
	return 0;
}


//得到5张牌
void CardManager::refreshCards(){
	m_showed_flag=-1;
	m_ox=-1;
	m_isWashingDone=false;
	GameLogic *logic = GameLogic::sharedGameLogic();
	int temp =0;
	for(int i =0;i<5;i++){
		temp = logic->getCard();	
		MyCardSelf* card = new MyCardSelf(temp/13,temp%13+1,m_x+m_offset*i,m_y);
		card->setPosition(ccp(m_x+m_offset*i,m_y));
		card->autorelease();
		CC_BREAK_IF(! card);
		this->addChild(card,TAG_CARDS+i,TAG_CARDS+i);
		m_cards_values[i]=card->m_value;
		m_cards_kinds[i]=temp/13;
		m_cards[i]=card;
		card->setVisible(false);
	}
	CCSprite* toast = CCSprite::create("toast10.png");
	toast->setPosition(ccp(250,100));
	addChild(toast,TAG_TOAST_SENDING_CARDS,TAG_TOAST_SENDING_CARDS);
	CCActionInterval* fade = CCFadeIn::create(0.25f);
	toast->runAction(fade);

	schedule(schedule_selector(CardManager::showOneCardScheduler),0.5f);
}

void CardManager::showOneCardScheduler(float dt){
	MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioSendOneCard);
	if(m_showed_flag<=3){
		m_showed_flag++;
		getChildByTag(TAG_CARDS+m_showed_flag)->setVisible(true);
	}
	if(m_showed_flag==4){
		m_isWashingDone=true;
		removeChildByTag(TAG_TOAST_SENDING_CARDS);
		unschedule(schedule_selector(CardManager::showOneCardScheduler));
	}	
}


//判断五花牛
bool CardManager::isFiveFlowerOx(){
	for(int i = 0 ; i < 5 ; i++){
		if(m_cards_values[i]<=10){
			return false;
		}
	}
	return true;
}

//得到最大单牌的值
short CardManager::getBigestValue(){
	short bigest = m_cards_values[0];
	bigest = bigest == 1 ? 14 : bigest;

	for(int i = 0 ; i < 4 ; i++){
		short next =m_cards_values[i+1];
		next = next == 1 ? 14 : next;
		if(bigest<next){
			bigest=next;
		}
	}
	bigest = bigest == 14 ? 1 : bigest;
	return bigest;
}

//得到最大单牌的花色
short CardManager::getBigestKind(){
	// [0,3]黑桃、红桃，梅花，方片。 值越小越大
	short smallest = 3;
	short bigest_value = getBigestValue();
	for(int i = 0 ; i < 5 ; i++){
		if(bigest_value == m_cards_values[i]){
			smallest = smallest > m_cards_kinds[i] ? m_cards_kinds[i]:smallest;
		}
	}
	return smallest;
}

/****************************************************************************************************
*								   								   								    *
*                                    CardManagerRobot实现                                           *
*							        							        							*
*****************************************************************************************************/

CardManagerRobot::~CardManagerRobot(){

}

CardManagerRobot::CardManagerRobot(float x,float y){
	m_x=x;
	m_y=y;
	m_offset=23;
	m_ox=-1;
	refreshCards();

}

//返回牛牛结果
short CardManagerRobot::getResults(){
	if(isFiveFlowerOx()){
		m_ox=11;
		return 11;
	}
	short temp1=0;
	short temp2=0;
	short temp3=0;
	short temp_sum;
	//找到三张
	for (short i=0; i < 3; i++)
	{
		for (short j=i+1; j < 4; j++)
		{
			for (short k = j+1; k < 5; k++)
			{

				temp1 =  m_cards_values[i];
				temp2 =  m_cards_values[j];
				temp3 =  m_cards_values[k];

				if(temp1 >10)
					temp1=10;
				if(temp2 >10)
					temp2=10;
				if(temp3 >10)
					temp3=10;

				temp_sum = temp1+temp2+temp3;

				if (temp_sum%10 ==0)
				{
					m_cards[i]->setFocused();
					m_cards[j]->setFocused();
					m_cards[k]->setFocused();
					temp_sum = 0;
					int temp4 =0;
					for(int m = 0;m<5;m++){
						if(m!=i && m!=j && m!=k){
							temp4 = m_cards_values[m];	
							if(temp4 >10)
								temp4=10;
							temp_sum+=temp4;					
						}
					}

					if(temp_sum % 10 ==0){
						m_ox=10;
						return 10;
					}else{
						m_ox=temp_sum % 10;
						return m_ox;
					}
				}
			}
		}
	}
	m_ox=0;
	return 0;
}

//得到5张牌
void CardManagerRobot::refreshCards(){
	m_ox=-1;
	GameLogic *logic = GameLogic::sharedGameLogic();
	short temp =0;
	for(short i =0;i<5;i++){
		temp = logic->getCard();	
		MyCardBase* card = new MyCardBase(temp/13,temp%13+1,m_x+m_offset*i,m_y);
		card->setPosition(ccp(m_x+m_offset*i,m_y));
		card->autorelease();
		CC_BREAK_IF(! card);
		this->addChild(card,TAG_CARDS+i,TAG_CARDS+i);
		card->setVisible(false);
		m_cards_values[i]=card->m_value;
		m_cards_kinds[i]=temp / 13 ;
		m_cards[i]=card;
	}
	hideAll();
	m_isWashingDone=false;
	m_showed_flag=-1;
	schedule(schedule_selector(CardManagerRobot::showOneCardScheduler),0.5f);
}

void CardManagerRobot::showOneCardScheduler(float dt){
	if(m_showed_flag<=3){	
		m_showed_flag++;
		int s =TAG_COVERS+m_showed_flag;
		getChildByTag(TAG_CARDS+m_showed_flag)->setVisible(true);		
		getChildByTag(TAG_COVERS+m_showed_flag)->setVisible(true);					
	}
	if(m_showed_flag==4){
		m_isWashingDone=true;
		unschedule(schedule_selector(CardManagerRobot::showOneCardScheduler));
	}	
}

//显示牌背面
void CardManagerRobot::hideAll(){
	for(int i =0;i<5;i++){
		cocos2d::CCSprite * card = cocos2d::CCSprite::create("card_back.png");
		card->setPosition(ccp(m_x+m_offset*i,m_y));
		CC_BREAK_IF(! card);
		this->addChild(card,TAG_COVERS+i,TAG_COVERS+i);
		card->setVisible(false);
	}
}


//显示牌正面
void CardManagerRobot::showAll(){
	for(int i =0;i<5;i++){
		removeChildByTag(TAG_COVERS+i);
	}
}

//判断五花牛
bool CardManagerRobot::isFiveFlowerOx(){
	for(int i = 0 ; i < 5 ; i++){
		if(m_cards_values[i]<=10){
			return false;
		}
	}
	return true;
}

//得到最大单牌的值
short CardManagerRobot::getBigestValue(){
	short bigest = m_cards_values[0];
	bigest = bigest == 1 ? 14 : bigest;

	for(int i = 0 ; i < 4 ; i++){
		short next =m_cards_values[i+1];
		next = next == 1 ? 14 : next;
		if(bigest<next){
			bigest=next;
		}
	}
	bigest = bigest == 14 ? 1 : bigest;
	return bigest;
}

//得到最大单牌的花色
short CardManagerRobot::getBigestKind(){
	// [0,3]黑桃、红桃，梅花，方片。 值越小越大
	short smallest = 3;
	short bigest_value = getBigestValue();
	for(int i = 0 ; i < 5 ; i++){
		if(bigest_value == m_cards_values[i]){
			smallest = smallest > m_cards_kinds[i] ? m_cards_kinds[i]:smallest;
		}
	}
	return smallest;
}

/****************************************************************************************************
*								   								   								    *
*                                    RobotManager实现                                               *
*							        							        							*
*****************************************************************************************************/

RobotManager::~RobotManager(){

}

RobotManager::RobotManager(short flag,short room_kind){
	m_gameStat = eGameStatStandBy;
	m_isRobed =-1;
	m_flag=flag;
	m_isBoss=false;  //是否庄家
	m_isScheduleCalling=false;
	m_room_kind = room_kind;

	initRobot(); 
	refreshData();

	/*
	showRobChat();
	showMoneyBar();
	refreshCards();
	showOx();
	showCards();*/

	//	setGameStat(eGameStatStandBy);

}

void RobotManager::refreshData(){
	m_gameStat = eGameStatStandBy;	
	m_isScheduleCalling=false;

	m_isStandBy = false;
	m_isRobed =-1;
	m_isBoss=false;  //是否庄家
	m_isShowMoney=false;
	m_isShowCards=false;
	m_isShowOx=false;
	m_isCalcResult=false;
	m_bossMoney = 0;
}

void RobotManager::setGameStat(GameStat stat){	
	m_gameStat = stat;
	m_isScheduleCalling=true;
	float time =(rand()%400)*1.0f /100 + 0.1;
	
	if(stat == eGameStatGaming){
		 time =(rand()%700)*1.0f /100 + 3.0; //注意别小于发牌的时间了，不然会崩溃
	}

	CCLog("setGameStat%d",time);
	scheduleOnce(schedule_selector(RobotManager::updateCallback),time);	
	m_board->stopShowTime();

	if(stat!=eGameStatShowMoney && stat!=eGameStatGaming){
		m_board->showTime(10);
	}else{
		if(!m_isBoss){
			m_board->showTime(10);
		}
	}

	//remove的操作都放在这,以及刚切换状态时就需要执行的操作，延时调用的放在下个函数
	switch(m_gameStat){
	case eGameStatStandBy:	
		removeCards();
		removeOx();
		removeMoneyBar();
		break;
	case eGameStatRobBoss:
		removeStandBy();
		break;
	case eGameStatShowMoney:
		removeRobChat();
		break;
	case eGameStatGaming:
		refreshCards();
		break;
	case eGameStatCalcResult:		
		break;
	}

}

//延时调用
void RobotManager::updateCallback(CCObject* pSender){
	if(m_isScheduleCalling){
		m_board->stopShowTime();
		switch(m_gameStat){
		case eGameStatStandBy:	
			refreshData();
			showStandBy();
			break;
		case eGameStatRobBoss:
			showRobChat();
			break;
		case eGameStatShowMoney:
			showMoneyBar();
			break;
		case eGameStatGaming:
			showOx();
			showCards();
			break;
		case eGameStatCalcResult:		
			break;
		}

		m_isScheduleCalling=false;
	}	
}

void RobotManager::initRobot(){
	m_board = new MyUserBoard(true,true);	
	addChild(m_board,TAG_USERBOARD,TAG_USERBOARD);
	m_board->autorelease();

	unsigned int money =1000; 
	unsigned int min =1000; //入场限制
	unsigned int max=9000; //最多拥有

	switch(m_room_kind){
	case 0:
		min = 1000;
		max = 49000;
		money=min+(rand()*10)%max;
		break;
	case 1:
		min = 50000;
		if(Tools::sharedTools()->getCoin()<=100000){
			max = 50000;
		}else if(Tools::sharedTools()->getCoin()<=250000){
			max = 200000;
		}else{
			max = 450000;
		}	
		money=min+(rand()*100)%max;
		break;
	case 2:
		min = 500000;
		if(Tools::sharedTools()->getCoin()<=1500000){
			max = 1000000;
		}else if(Tools::sharedTools()->getCoin()<=3000000){
			max = 2500000;
		}else{
			max = 4500000;
		}	
		money=min+(rand()*2000)%max;
		break;
	case 3:
		if(Tools::sharedTools()->getCoin()<1000*10000){
			min = 5000000;
			max = 5000000;
		}else{
			min = 5000000;
			max = Tools::sharedTools()->getCoin() * 1.5f;  
		}
		money=min+(rand()*10000)%max;
		break;
	case 4:
		if(Tools::sharedTools()->getIngot()<200){
			min = 5;
			max = 150;
		}else{
			min = 5;
			max = Tools::sharedTools()->getIngot() * 1.5f;  
		}
		money=min+rand()%max;
		break;
	case 5:
		if(Tools::sharedTools()->getCoin()<100000){
			min = 1000;
			max = 100000;
		}else{
			min = 100000;
			max = Tools::sharedTools()->getCoin() * 1.5f;  
		}
		money=min+(rand()*10000)%max;
		break;
	}

	money= money>0 ? money:money*-1;

	m_board->updateMoney(money);
}

void RobotManager::showStandBy(){
	m_isStandBy = true;
	cocos2d::CCSprite *sprite2 = cocos2d::CCSprite::create("scene_game_standby.png");		
	if(m_flag>=3){
		sprite2->setPosition(ccp(-90,-20));
	}else{
		sprite2->setPosition(ccp(100,-20));
	}
	addChild(sprite2,TAG_STANDBY,TAG_STANDBY);
}

void RobotManager::removeStandBy(){
	removeChildByTag(TAG_STANDBY);
	m_isStandBy = false;
}

void RobotManager::showRobChat(){
	m_isRobed = 0;
	short temp = rand() % 5;
	if(temp<=2){
		//60%的概率抢庄
		m_isRobed=1;
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioRob);
	}else{
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioUnrob);
	}
	string name;
	if(m_isRobed==1){
		name="scene_game_rob_chat_";
	}else{
		name="scene_game_unrob_chat_";
	}
	string kind;
	if(m_flag >=3){
		kind = "right.png";
	}else{
		kind = "left.png";
	}
	string ss =  name+kind;


	cocos2d::CCPoint point;
	/*
	if(flag<=2){			
	point=ccp();
	}else{
	point=ccp(300,0);
	}	*/		

	cocos2d::CCSprite *sprite2 = cocos2d::CCSprite::create(ss.c_str());	
	sprite2->setScale(0.05);
	addChild(sprite2,TAG_ROB,TAG_ROB);

	if(m_flag>=3){
		point=ccp(-70,10);
		sprite2->setAnchorPoint(ccp(1,0));
	}else{
		point=ccp(70,10);
		sprite2->setAnchorPoint(ccp(0,0));
	}
	sprite2->setPosition(point);
	sprite2->runAction(Tools::sharedTools()->getActionScaleXYBiuBiu());
}

void RobotManager::removeRobChat(){
	removeChildByTag(TAG_ROB);
	m_isRobed=-1;
}

void RobotManager::showMoneyBar(){
	m_isShowMoney=true;
	if(!m_isBoss){
		unsigned smaller = m_bossMoney > m_board->m_money ? m_board->m_money: m_bossMoney;	
		short numbers[]={1,2,4,8};	
		smaller/=5;
		short which = rand()%100;
		which = rand()%100;
		short flag = 0; //4个押注选一个
		if(which>=40){
			flag=0;
		}else if(which>=20&&which<=39){
			flag=1;
		}else if(which>=7&&which<=19){
			flag=2;
		}else{
			flag=3;
		}
		if(m_room_kind==4){
			//银锭场 [4,7]
			m_money = new MyMoneyBarRobot(7-flag,smaller/numbers[flag]);
		}else{
			//银币场 [0,3]
			m_money = new MyMoneyBarRobot(3-flag,smaller/numbers[flag]);
		}
		
		if(m_flag>=3){
			m_money->setPosition(ccp(0,-15));
			CCActionInterval*  action = CCMoveBy::create(0.15f, ccp(-140,0));
			m_money->runAction(action);
		}else{
			m_money->setPosition(ccp(0,-15));
			CCActionInterval*  action = CCMoveBy::create(0.15f, ccp(140,0));
			m_money->runAction(action);
		}
		addChild(m_money,TAG_MONEY_BAR,TAG_MONEY_BAR);
		m_money->autorelease();

		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioShowChipsOne);
	}
}

void RobotManager::removeMoneyBar(){
	removeChildByTag(TAG_MONEY_BAR);
	m_isShowMoney = false;
}

void RobotManager::showCards(){
	m_isShowCards=true;
	m_cards->showAll();
}

void RobotManager::removeCards(){
	removeChildByTag(TAG_CARDS);
	m_isShowCards = false;
}

void RobotManager::refreshCards(){
	if(m_flag>=3){
		m_cards = new CardManagerRobot(-190,-20);
	}else{
		m_cards = new CardManagerRobot(100,-20);
	}	
	addChild(m_cards,TAG_CARDS,TAG_CARDS);
	m_cards->autorelease();
}

void RobotManager::showOx(){
	m_isShowOx=true;
	short ox;
	ox = m_cards->getResults();
	char temp[32]; //注意不要越界
	sprintf(temp,"ox_%d.png",ox);	
	cocos2d::CCSprite *sprite2 = cocos2d::CCSprite::create(temp);	
	sprite2->setScale(2.5);
	addChild(sprite2,TAG_OX,TAG_OX);
	if(m_flag>=3){
		sprite2->setPosition(ccp(-290,-20));
	}else{
		sprite2->setPosition(ccp(300,-20));
	}


	cocos2d::CCActionInterval *actionScaleToBack = cocos2d::CCScaleTo::create(0.15f, 1.0f);
	sprite2->runAction(actionScaleToBack);

	MyAudioEngine::sharedMyAudioEngine()->playEffectShowOx(ox,m_board->m_sex_isBoy);
}

void RobotManager::removeOx(){
	removeChildByTag(TAG_OX);
	m_isShowOx =false;
}



/****************************************************************************************************
*								   								   								    *
*                                    PlayerManager实现                                              *
*							        							        							*
*****************************************************************************************************/

PlayerManager::~PlayerManager(){

}

PlayerManager::PlayerManager(short room_kind){
	m_gameStat = eGameStatStandBy;
	m_isRobed =-1;
	m_isBoss=false;  //是否庄家
	m_room_kind=room_kind;
	initPlayer(); 
	refreshData();
}

void PlayerManager::refreshData(){
	m_gameStat = eGameStatStandBy;

	m_isStandBy = false;
	m_isRobed =-1;
	m_isBoss=false;  //是否庄家
	m_isShowMoney=false;
	m_isShowCards=false;
	m_isShowOx=false;
	m_isCalcResult=false;
	m_bossMoney = 0;
}

void PlayerManager::setGameStat(GameStat stat){	
	m_gameStat = stat;
	switch(m_gameStat){
	case eGameStatStandBy:	
		if(!m_isStandBy){
			refreshData();
			removeCards();
			removeOx();
			removeMoneyBar();
			removeMoneyBar();
			showStandBy();
		}
		break;
	case eGameStatRobBoss:
		removeStandBy();
		showRobChat();
		break;
	case eGameStatShowMoney:
		if(!m_isShowMoney){
			removeRobChat();
			showMoneyBar();
		}
		break;
	case eGameStatGaming:
		if(!m_isShowOx){
			refreshCards();
			showCards();
		}
		break;
	case eGameStatCalcResult:
		if(!m_isCalcResult){
			
		}
		break;
	}	
}

void PlayerManager::initPlayer(){
	m_board = new MyUserBoard(false,m_room_kind!=4);		
	addChild(m_board,TAG_USERBOARD,TAG_USERBOARD);
	m_board->autorelease();
}

void PlayerManager::showStandBy(){
	m_isStandBy=true;
	cocos2d::CCSprite *sprite2 = cocos2d::CCSprite::create("scene_game_standby.png");		
	sprite2->setPosition(ccp(250,70));
	addChild(sprite2,TAG_STANDBY,TAG_STANDBY);
}

void PlayerManager::removeStandBy(){
	removeChildByTag(TAG_STANDBY);
	m_isStandBy=false;
}

void PlayerManager::showRobChat(){
	string name;
	if(m_isRobed==1){
		name="scene_game_rob_chat_left.png";
	}else{
		name="scene_game_unrob_chat_left.png";
	}
	cocos2d::CCSprite *sprite2 = cocos2d::CCSprite::create(name.c_str());	
	sprite2->setScale(0.05f);
	addChild(sprite2,TAG_ROB,TAG_ROB);
	sprite2->setPosition(ccp(70,10));
	sprite2->setAnchorPoint(ccp(0,0));
	sprite2->runAction(Tools::sharedTools()->getActionScaleXYBiuBiu());
}

void PlayerManager::removeRobChat(){
	removeChildByTag(TAG_ROB);
	m_isRobed=-1;
}

void PlayerManager::showMoneyBar(){	
	if(!m_isBoss){
		short numbers[]={8,4,2,1}; //倍数除数
		unsigned smaller = m_bossMoney > m_board->m_money ? m_board->m_money: m_bossMoney;	
		smaller/=5;
		for(short i=0;i<4;i++){
			removeChildByTag(TAG_FOUR_BARS+i);
			if(m_room_kind!=4){
				m_moneys_temp[i] = new MyMoneyBar(i,smaller/numbers[i]);
			}else{
				m_moneys_temp[i] = new MyMoneyBar(i+4,smaller/numbers[i]);
			}		
			//	m_money->setPosition(ccp(100,-85));	
			addChild(m_moneys_temp[i],TAG_FOUR_BARS+i,TAG_FOUR_BARS+i);
			m_moneys_temp[i]->autorelease();
		}
		m_isShowMoney=false; //4选1结束时才算完毕
		schedule(schedule_selector(PlayerManager::checkMoneyBarsStat),30/60);
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioShowChipsAll);
	}else{
		m_isShowMoney=true;
	}	
}

void PlayerManager::checkMoneyBarsStat(float dt){
	bool isChoosed = false;
	for(short i = 0 ;i<4;i++){
		if(m_moneys_temp[i]->m_isMoved){			
			m_money = m_moneys_temp[i];
			for(short j = 0 ;j<4;j++){
				if(i!=j){
					removeChildByTag(TAG_FOUR_BARS+j);
				}else{
					//getChildByTag(TAG_FOUR_BARS+j)->setTag(TAG_MONEY_BAR);  
					TAG_MONEY_BAR=TAG_FOUR_BARS+j;
					m_money->moveIn();
				}		
			}
			m_isShowMoney=true;
			unschedule(schedule_selector(PlayerManager::checkMoneyBarsStat));
			break;
		}
	}
}

void PlayerManager::removeMoneyBar(){
	removeChildByTag(TAG_MONEY_BAR);
	m_isShowMoney = false;
}

void PlayerManager::showCards(){
	m_isShowCards=true;
	//	m_cards->showAll();
}

void PlayerManager::removeCards(){
	removeChildByTag(TAG_CARDS);
	m_isShowCards = false;
}

void PlayerManager::refreshCards(){
	m_cards = new CardManager(100,-20);
	addChild(m_cards,TAG_CARDS,TAG_CARDS);
	m_cards->autorelease();
}

void PlayerManager::showOxAuto(){
	m_isShowOx=true;
	short ox;
	ox = m_cards->getResultsAuto();
	char temp[32]; //注意不要越界
	sprintf(temp,"ox_%d.png",ox);	
	cocos2d::CCSprite *sprite2 = cocos2d::CCSprite::create(temp);	
	sprite2->setScale(2.5);
	addChild(sprite2,TAG_OX,TAG_OX);
	sprite2->setPosition(ccp(280,57));
	cocos2d::CCActionInterval *actionScaleToBack = cocos2d::CCScaleTo::create(0.15f, 1.0f);
	sprite2->runAction(actionScaleToBack);
	MyAudioEngine::sharedMyAudioEngine()->playEffectShowOx(ox,m_board->m_sex_isBoy);
}

bool PlayerManager::showOxHand(){
	short ox=m_cards->getResultsHand();
	if(ox!=-1){
		m_isShowOx=true;
		//拼牛正确
		char temp[32]; //注意不要越界
		sprintf(temp,"ox_%d.png",ox);	
		cocos2d::CCSprite *sprite2 = cocos2d::CCSprite::create(temp);	
		sprite2->setScale(2.5);
		addChild(sprite2,TAG_OX,TAG_OX);
		sprite2->setPosition(ccp(280,57));
		cocos2d::CCActionInterval *actionScaleToBack = cocos2d::CCScaleTo::create(0.15f, 1.0f);
		sprite2->runAction(actionScaleToBack);		

		MyAudioEngine::sharedMyAudioEngine()->playEffectShowOx(ox,m_board->m_sex_isBoy);
		return true;
	}
	return false;
}

void PlayerManager::removeOx(){
	removeChildByTag(TAG_OX);
	m_isShowOx = false;
}

//更新所有数据
void PlayerManager:: updateGameData(int currentWinOrLoseMoney,short exp,CCLayer* layer){
	Tools* tool =Tools::sharedTools();

	//更新银币或银锭
	if(m_room_kind!=4){
		tool->updateCoin(currentWinOrLoseMoney);
	}else{
		tool->updateIngot(currentWinOrLoseMoney);
	}

	//最大赢取
	tool->updateMostWon(currentWinOrLoseMoney);

	//输赢局数
	if(currentWinOrLoseMoney>=0){
		tool->addWin();
	}else{
		tool->addLost();
	}

	//基础经验，手动摆牛，vip，房间
	tool->useVipOnce();
	tool->addExperience(exp,layer);
	
}


/****************************************************************************************************
*								   								   								    *
*                                    ShopItemsManager实现                                           *
*							        							        							*
*****************************************************************************************************/
ShopItemsManager::ShopItemsManager(short flag,MyMoneyInfoBoard* moneyBoard){
	m_flag=flag;
	m_moneyBoard=moneyBoard;
	initByFlag();
}

void ShopItemsManager::initByFlag(){
	CCSprite* bg = CCSprite::create("scene_shop_item_bg.png");
	addChild(bg);

	switch(m_flag){
	case 0:
		for(short i=0;i<5;i++){
			MyShopItem* item = new MyShopItem(i);
			item->setMoneyBoard(m_moneyBoard);
			addChild(item);
			item->setPosition(getPointAtLocation(i));
			item->autorelease();
		}
		break;
	case 1:
		for(short i=0;i<6;i++){
			MyShopItem* item = new MyShopItem(i+6);
			item->setMoneyBoard(m_moneyBoard);
			addChild(item);
			item->setPosition(getPointAtLocation(i));
			item->autorelease();
		}
		break;
	case 2:
		for(short i=0;i<6;i++){
			MyShopItem* item = new MyShopItem(12+i);
			item->setMoneyBoard(m_moneyBoard);
			addChild(item);
			item->setPosition(getPointAtLocation(i));
			item->autorelease();
		}
		break;
	}
}

//九宫格的坐标位置 [0,8]
cocos2d::CCPoint ShopItemsManager::getPointAtLocation(short location){
	switch (location){
	case 0:return ccp(-360,98);
	case 1:return ccp(-55,98);
	case 2:return ccp(250,98);
	case 3:return ccp(-360,-95);
	case 4:return ccp(-55,-95);
	case 5:return ccp(250,-95);
	}
	return ccp(0,0);
}

