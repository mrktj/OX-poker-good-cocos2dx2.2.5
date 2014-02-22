#include "LayerGameDesk.h"
#include "Controls.h"
#include "GameLogic.h"
#include "Tools.h"
#include "SceneWelcome.h"
#include "SceneHall.h"
#include <string>
#include "MyAudioEngine.h"
#include "SceneShop.h"
#include "VisibleRect.h"

using namespace cocos2d;

MyGoldBoxByAmount* box_amount;

// on "init" you need to initialize your instance
bool LayerGameDesk::init()
{
	bool bRet = false;
	do 
	{
		m_GameStat = eGameStatStandBy;
		m_whoIsBoss = -1;

		size = CCDirector::sharedDirector()->getVisibleSize();
		point = CCDirector::sharedDirector()->getVisibleOrigin();

		logic = GameLogic::sharedGameLogic();


		CCSprite* pSprite = CCSprite::create("scene_game_bg_desk.png");
		CC_BREAK_IF(! pSprite);
		pSprite->setPosition(ccp(VisibleRect::center().x, VisibleRect::center().y));
		this->addChild(pSprite);

		MyGameTopBar* bar = new MyGameTopBar();
		CC_BREAK_IF(! bar);
	//	bar->setPositionY(point.y+size.height - 35 + 200);
		bar->setPositionY(VisibleRect::top().y - 35 + 200);
		bar->setAnchorPoint(ccp(0.5,0.5));
		this->addChild(bar,500);
		bar->autorelease();
		CCActionInterval*  actionBar = CCMoveBy::create(0.6f, ccp(0, -200));
		//	CCActionInterval*  actionTo = CCMoveTo::create(1.0f, ccp(size.width /2, point.y+size.height - 35));
		bar->runAction(actionBar);

		//back
		CCMenuItemImage *pItem3 = CCMenuItemImage::create(
			"scene_game_btn_back0.png",
			"scene_game_btn_back1.png",
			this,
			menu_selector(LayerGameDesk::menuCloseCallback));
		CC_BREAK_IF(! pItem3);
		pItem3->setPosition(point);
		pItem3->setAnchorPoint(ccp(0,0));
		CCMenu* pMenu3 = CCMenu::create(pItem3, NULL);
		pMenu3->setPosition(CCPointZero);
		pMenu3->setAnchorPoint(CCPointZero);
		CC_BREAK_IF(! pMenu3);
		addChild(pMenu3);
		pItem3->setRotation(-180);
		CCActionInterval*  act1 = CCRotateTo::create(0.2f, 0);
		pItem3->runAction(act1);

		//shop
//		CCMenuItemImage *pItem_chat = CCMenuItemImage::create(
//			"scene_game_btn_shop0.png",
//			"scene_game_btn_shop1.png",
//			this,
//			menu_selector(LayerGameDesk::menuShopCallback));
//		CC_BREAK_IF(! pItem_chat);
//		pItem_chat->setPosition(ccp(point.x+size.width,point.y));
//		pItem_chat->setAnchorPoint(ccp(1,0));
//		CCMenu* pMenu3_chat = CCMenu::create(pItem_chat, NULL);
//		pMenu3_chat->setPosition(CCPointZero);
//		pMenu3_chat->setAnchorPoint(CCPointZero);
//		CC_BREAK_IF(! pMenu3_chat);
//		this->addChild(pMenu3_chat);
//		pItem_chat->setRotation(180);
//		CCActionInterval*  act2 = CCRotateTo::create(0.2f, 0);
//		pItem_chat->runAction(act2);

		//牌堆
		CCSprite *sprite_heap = CCSprite::create("scene_game_cards_heap.png");
		addChild(sprite_heap,TAG_CARD_HEAP,TAG_CARD_HEAP);
		sprite_heap->setPosition(ccp(size.width/2,size.height*0.85f));
		sprite_heap->setVisible(false);

		//Boss图标icon
		CCSprite *sprite_rob_icon = CCSprite::create("scene_game_boss.png");
		addChild(sprite_rob_icon,TAG_BOSS_ICON,TAG_BOSS_ICON);
		sprite_rob_icon->setPosition(ccp(VisibleRect::center().x,size.height*0.65f));
		sprite_rob_icon->setVisible(false);   

		//schedule(schedule_selector(LayerGameDesk::updateGameStat),0.5);
		schedule(schedule_selector(LayerGameDesk::updateGameStat), 1.0f);

		m_flag_for_start=0;
		m_delay_set_stat=2;

		//延时进入下一阶段，保证流程连贯性
		schedule(schedule_selector(LayerGameDesk::startGameDelayed),10 / 60.0f);

		setTouchEnabled(true);

		bRet = true;
	} while (0);

	return bRet;
}

void LayerGameDesk::setRoomKind(short kind_id){
	m_room_kind=kind_id;
}

void LayerGameDesk::startGameDelayed(float dt){
	m_flag_for_start++;
	if(m_flag_for_start==2){
		initPlayer(0);
	}else if(m_flag_for_start ==3){
		initPlayer(1);
	}else if(m_flag_for_start ==4){
		initPlayer(2);
	}else if(m_flag_for_start ==5){
		initPlayer(3);	
	}else if(m_flag_for_start ==6){
		initPlayer(4);
	}else if(m_flag_for_start ==7){
		setGameStat(eGameStatStandBy);
		m_flag_for_start=0;
		unschedule(schedule_selector(LayerGameDesk::startGameDelayed));
	}
}

void LayerGameDesk::updateGameStat(float dt){
//	CCLog("LayerGameDesk::updateGameStat");
	GameStat stat = couldChangeGameStat();
	if(m_GameStat != stat){
		//延时调用setGameStat，为了优化操作体验，-1表示还未达到set条件，倒计时到0说明该执行了，正值为倒计时		
		if(m_delay_set_stat>0){
			m_delay_set_stat--;
		}
		if(m_delay_set_stat==0){
			setGameStat(stat);
			m_delay_set_stat=2;
		}		
	}
}

void LayerGameDesk::toStandBy(float dt){
	setGameStat(eGameStatStandBy);
}


void LayerGameDesk::setRobotsStat(GameStat gameStat){
	for(int i = 0 ;i<4;i++){
		if(m_robot_alive[i]!=-1){
			m_managersRobot[i]->setGameStat(gameStat);
		}
	}
}

void LayerGameDesk::setPlayerStat(GameStat gameStat){
	m_managerPlayer->setGameStat(gameStat);
}

GameStat LayerGameDesk::whoIsBoss(){
	//特殊情况：都不抢庄时
	bool noneRob=true;
	for(short i =0;i< 5;i++){
		if(i==0){
			if(m_managerPlayer->m_isRobed==1){
				noneRob =false;
				break;
			}
		}else{
			if(m_managersRobot[i-1]->m_isRobed==1){
				noneRob =false;
				break;
			}
		}
	}

	//有人抢庄时
	if(!noneRob){
		//抢庄时读取玩家银币信息，不抢时赋值0
		unsigned int moneys[5];

		if(m_managerPlayer->m_isRobed==1){
			 //m_isRobed是否抢庄,-1未表态，0不抢，1抢
			moneys[0] = m_managerPlayer->m_board->m_money;
		}else{
			moneys[0] = 0;
		}

		//读取机器人信息
		for(short i =1;i<5;i++){
			if(m_managersRobot[i-1]->m_isRobed==1){
				moneys[i] = m_managersRobot[i-1]->m_board->m_money;
			}else{
				moneys[i] = 0;
			}		
		}

		short whoIsBoss = 0;
		for(int i=1;i<5;i++){
			if(moneys[i] > moneys[whoIsBoss]){
				whoIsBoss  = i;
			}
		}
		m_boss_flag = whoIsBoss;
		
	}else{
		//无人抢庄时,随机抢庄
		Tools::sharedTools()->showToast(eToastNoneRobBoss,this);
		short temp = rand() % 5;
		m_boss_flag = temp;
	}

	//赋值
	if(m_boss_flag == 0){
		m_managerPlayer->m_isBoss=true;
	}else{
		m_managersRobot[m_boss_flag-1]->m_isBoss =true;
	}

	CCPoint point ;
	if(m_boss_flag==0){
		point = m_managerPlayer->getPosition();
		point = ccp(point.x-45,point.y+65);
	}else{
		point = m_managersRobot[m_boss_flag-1]->getPosition();
		point = ccp(point.x-38,point.y+50);
	}
	CCActionInterval*  leftToRight = CCMoveTo::create(2.0f, point);
	CCActionInterval*  rotate = CCRotateBy::create(2.0f,-360*4);
	CCActionInterval* spawn = CCSpawn::createWithTwoActions(leftToRight,rotate);
	CCActionInterval* ease = CCEaseBackOut::create(spawn);

	getChildByTag(TAG_BOSS_ICON)->runAction(ease);
//	getChildByTag(TAG_BOSS_ICON)->runAction(rotate);

	//把庄家的钱赋给闲家，用于决定押注金额
	//1.若庄是自己，就把自己的钱赋给机器人们
	if(m_boss_flag==0){
		for(short i=0;i<4;i++){
			m_managersRobot[i]->m_bossMoney =m_managerPlayer->m_board->m_money;
		}
		m_managerPlayer->m_bossMoney =0;
	}else{
		//2.若庄是机器人，就把庄的钱赋给自己和其他机器人
		m_managerPlayer->m_bossMoney = m_managersRobot[m_boss_flag-1]->m_board->m_money;
		for(short i=0;i<4;i++){
			if(i!=m_boss_flag-1){
				m_managersRobot[i]->m_bossMoney =m_managersRobot[m_boss_flag-1]->m_board->m_money;
			}else{
				m_managersRobot[i]->m_bossMoney =0;
			}		
		}
	}

	return eGameStatShowMoney;
}

GameStat LayerGameDesk::couldChangeGameStat(){
	switch(m_GameStat){
	case eGameStatStandBy:			
		if(!m_managerPlayer->m_isStandBy){
			return m_GameStat;
		}
		for(short i=0;i<4;i++){
			if(!m_managersRobot[i]->m_isStandBy){
				return m_GameStat;
			}
		}
		return eGameStatRobBoss;
	case eGameStatRobBoss:
		if(m_managerPlayer->m_isRobed==-1){
			return m_GameStat;
		}
		for(short i=0;i<4;i++){
			if(m_managersRobot[i]->m_isRobed==-1){
				return m_GameStat;
			}
		}
		if(m_boss_flag==-1){
			return whoIsBoss();
		}else{
			return eGameStatShowMoney;
		}	
	case eGameStatShowMoney:
		if(!m_managerPlayer->m_isShowMoney){
			return m_GameStat;
		}
		for(short i=0;i<4;i++){
			if(!m_managersRobot[i]->m_isShowMoney){
				return m_GameStat;
			}
		}
		return eGameStatGaming;
	case eGameStatGaming:	
		if(!m_managerPlayer->m_isShowOx){
			return m_GameStat;
		}
		for(short i=0;i<4;i++){
			if(!m_managersRobot[i]->m_isShowOx){
				return m_GameStat;
			}
		}
		return eGameStatCalcResult;
	case eGameStatCalcResult:
		if(!m_managerPlayer->m_isCalcResult){
			return m_GameStat;
		}
		for(short i=0;i<4;i++){
			if(!m_managersRobot[i]->m_isCalcResult){
				return m_GameStat;
			}
		}
		return eGameStatStandBy;
	}
	return m_GameStat;
}

void LayerGameDesk::setGameStat(GameStat gameState){
	std::string s1_before,s1_after;
	std::string s2_before,s2_after;

	bool shouldCreate = true;

	m_GameStat = gameState;
	setRobotsStat(gameState);
	
/*
	if(m_managerPlayer->m_gameStat != gameState){
		setPlayerStat(gameState);
	}*/
	switch(gameState){

	case eGameStatStandBy:				
		GameLogic::sharedGameLogic()->refreshAllCards();
		s1_before="scene_game_btn_start0.png";
		s1_after="scene_game_btn_start1.png";
		s2_before="scene_game_btn_change0.png";
		s2_after ="scene_game_btn_change1.png";		
		getChildByTag(TAG_BOSS_ICON)->setVisible(false);
		m_isOxShowed=false;
		m_boss_flag=-1;
		m_isHandOx=false;
		break;
	case eGameStatRobBoss:
		s1_before="scene_game_btn_rob0.png";
		s1_after="scene_game_btn_rob1.png";
		s2_before="scene_game_btn_unrob0.png";
		s2_after ="scene_game_btn_unrob1.png";	
		m_managerPlayer -> removeStandBy();
		getChildByTag(TAG_BOSS_ICON)->setVisible(true);
		getChildByTag(TAG_BOSS_ICON)->setPosition(ccp(VisibleRect::center().x,size.height * 0.65f));
		break;
	case eGameStatShowMoney:
		shouldCreate=false;
		m_managerPlayer -> removeRobChat();
		setPlayerStat(m_GameStat);
		break;
	case eGameStatGaming:
		s1_before="scene_game_btn_tip0.png";
		s1_after="scene_game_btn_tip1.png";
		s2_before="scene_game_btn_open0.png";
		s2_after ="scene_game_btn_open1.png";	
		setPlayerStat(m_GameStat);
		break;
	case eGameStatCalcResult:
		setPlayerStat(m_GameStat);
		shouldCreate=false;
		showCalcBoard();
		scheduleOnce(schedule_selector(LayerGameDesk::toStandBy),2.0);
//		setGameStat(eGameStatStandBy);
		break;
	}

	if(shouldCreate){
		removeChildByTag(TAG_BTN_UP);
		removeChildByTag(TAG_BTN_DOWN);


		// 上方按钮 --  开始，抢庄,自动摊牌
		CCMenuItemImage *pItem_up = CCMenuItemImage::create(
			s1_before.c_str(),
			s1_after.c_str(),
			this,
			menu_selector(LayerGameDesk::menuUpCallback));
		pItem_up->setAnchorPoint(ccp(0,0));
		pItem_up->setPosition(ccp(750+482,175));
		CCMenu* pMenu3_up = CCMenu::create(pItem_up, NULL);
		pMenu3_up->setAnchorPoint(CCPointZero);
		pMenu3_up->setPosition(CCPointZero);
		this->addChild(pMenu3_up,TAG_BTN_UP,TAG_BTN_UP);

		// 下方按钮 --  换桌，不抢，提示
		CCMenuItemImage *pItem_down = CCMenuItemImage::create(
			s2_before.c_str(),
			s2_after.c_str(),
			this,
			menu_selector(LayerGameDesk::menuDownCallback));
		pItem_down->setAnchorPoint(ccp(0,0));
		pItem_down->setPosition(ccp(750+450,105));
		CCMenu* pMenu_down = CCMenu::create(pItem_down, NULL);
		pMenu_down->setAnchorPoint(CCPointZero);
		pMenu_down->setPosition(CCPointZero);
		this->addChild(pMenu_down,TAG_BTN_DOWN,TAG_BTN_DOWN);

		moveIn(pMenu3_up,0.1);
		moveIn(pMenu_down,0.15);
	}else{
		if(m_GameStat!=eGameStatCalcResult && m_GameStat!=eGameStatShowMoney){
			moveOutMenus();
		}
	}
}

void LayerGameDesk::initPlayer(int flag){
	float x1,y1; //玩家信息和牌组管理器控件
	//尚未添加是否存在，若存在还得销毁
	switch(flag){
	case 0:
		x1=220+VisibleRect::leftBottom().x;
		y1=170+VisibleRect::leftBottom().y;
		break;
	case 1:
		x1=113+VisibleRect::leftBottom().x;
		y1=310+VisibleRect::leftBottom().y;
		break;
	case 2:
		x1=113+VisibleRect::leftBottom().x;
		y1=445+VisibleRect::leftBottom().y;
		break;
	case 3:
		x1=VisibleRect::right().x-115;
		y1=445+VisibleRect::leftBottom().y;
		break;
	case 4:
		x1=VisibleRect::right().x-115;
		y1=310+VisibleRect::leftBottom().y;
		break;
	}

	if(flag!=0){		
		RobotManager* robot = new RobotManager(flag,m_room_kind);
		addChild(robot,TAG_PLAYERS+flag,TAG_PLAYERS+flag);
		robot->setPosition(ccp(x1,y1));
		robot->setScale(0.8);
		robot->autorelease();
		m_managersRobot[flag-1]=robot;
		m_robot_alive[flag-1]=1;


		short offset1 = 0;
		short offset2 = 0;
		if(flag>=3){
			robot->setPosition(ccp(x1+300,y1));
			offset1 = -315;
			offset2 = 15;
		}else{
			robot->setPosition(ccp(x1-300,y1));
			offset1 = 315;
			offset2 = -15;
		}
		//CCActionInterval* action1 = CCMoveTo::create(0.1f,ccp(point.x+x1+45+offset,point.y+y1+64));
		//CCActionInterval* action2 = CCMoveTo::create(0.05f,ccp(point.x+x1+45,point.y+y1+64));
		
		CCActionInterval* action1 = CCMoveBy::create(0.1f,ccp(offset1,0));
		CCActionInterval* action2 = CCMoveBy::create(0.05,ccp(offset2,0));
		CCActionInterval* seq = CCSequence::create(action1,action2,NULL);
		getChildByTag(TAG_PLAYERS+flag)->runAction(seq);
	}else{
		m_managerPlayer=new PlayerManager(m_room_kind);
		addChild(m_managerPlayer,TAG_PLAYERS+flag,TAG_PLAYERS+flag);
		m_managerPlayer->setPosition(ccp(x1,y1));
		m_managerPlayer->autorelease();
		m_managerPlayer->setScale(0.1);
		CCActionInterval* action1 = CCScaleTo::create(0.1f,1.25f,1.25f);
		CCActionInterval* action2 = CCScaleTo::create(0.05f,1,1);
		CCActionInterval* seq = CCSequence::create(action1,action2,NULL);
		m_managerPlayer->runAction(seq);

		MACRO_CREATE_GOLDBOX_TIMER

		//局数宝箱
		if(Tools::sharedTools()->couldCreateGoldBoxOfAmount()){
			box_amount = new MyGoldBoxByAmount();
			addChild(box_amount,999,998);
			box_amount->setPosition(ccpAdd(VisibleRect::rightTop(), ccp(-30+200,-100)));
			box_amount->autorelease();
			CCActionInterval*  actionForGoldBoxAmount = CCMoveBy::create(1.0f, ccp(-200,0));
			box_amount->runAction(actionForGoldBoxAmount);
		}

		if(m_room_kind!=4){
			//除了银锭场，其他的都要更新宝箱送的银币的信息
			MACRO_GOLDBOX_TIMER_IN_DESK
				if(Tools::sharedTools()->couldCreateGoldBoxOfAmount()){
					box_amount->setUserBoard(m_managerPlayer->m_board);
				}
		}

	}

}

void LayerGameDesk::moveOutMenus(){
	moveOut(getChildByTag(TAG_BTN_UP),0.1);
	moveOut(getChildByTag(TAG_BTN_DOWN),0.2);
}

void LayerGameDesk::changeTable(){
	for(short i = 1;i<5;i++){
		removeChildByTag(TAG_PLAYERS+i);
		initPlayer(i);
	}
}

void LayerGameDesk::showCalcBoard(){	
	//处理局数宝箱
	if(Tools::sharedTools()->couldCreateGoldBoxOfAmount()){
		box_amount->updateData();
	}

	//结算框
	MyCalcBoard* board;
	if(m_room_kind==4){
		//银锭场
		board = new MyCalcBoard(false);
	}else{
		//银币场
		board = new MyCalcBoard(true);
	}
	addChild(board,TAG_CALC_BOARD,TAG_CALC_BOARD);
	board->autorelease();
	board->setPosition(ccp(512,350+576));
	board->setBoss(m_boss_flag);
	board->runAction(Tools::sharedTools()->getActionMoveDown());

	//处理本局输赢数据
	GameLogic* logic = GameLogic::sharedGameLogic();
	bool bossWinOrNot[5]={true,true,true,true,true};
	short ox[5]={-1,-1,-1,-1,-1};
	unsigned moneys_chip[5]={0,0,0,0,0};
	unsigned moneys_total[5]={0,0,0,0,0};

	for(short i = 0 ; i < 5 ; i++){
		if(i==0){
			ox[i]=m_managerPlayer->m_cards->m_ox;
			if(m_boss_flag!=i){
				moneys_chip[i]=m_managerPlayer->m_money->m_money;
			}	
			moneys_total[i]=m_managerPlayer->m_board->m_money;
		}else{
			ox[i]=m_managersRobot[i-1]->m_cards->m_ox;
			if(m_boss_flag!=i){
				moneys_chip[i]=m_managersRobot[i-1]->m_money->m_money;
			}
			moneys_total[i]=m_managersRobot[i-1]->m_board->m_money;
		}
		bossWinOrNot[i]=getCompareResultBossWith(i);
	}

	moneys_chip[m_boss_flag]=0;

	int moneys[5]={-1,-1,-1,-1,-1}; //此数组表示各个玩家金钱改变量，不是总量哦
	logic->getChipsResult(moneys,m_boss_flag,bossWinOrNot,ox,moneys_chip,moneys_total);
	
	for(short i = 0 ; i < 5 ; i++){
		if(i!=m_boss_flag){
			if(i==0){
				board->setInfo(i,m_managerPlayer->m_board->m_pLabel_name->getString(),ox[i],moneys_chip[i],!bossWinOrNot[i],abs(moneys[i]));
			}else{
				board->setInfo(i,m_managersRobot[i-1]->m_board->m_pLabel_name->getString(),ox[i],moneys_chip[i],!bossWinOrNot[i],abs(moneys[i]));
			}	
		}else{
			if(i==0){
				board->setInfo(i,m_managerPlayer->m_board->m_pLabel_name->getString(),ox[i],moneys_chip[i],moneys[i]>=0,abs(moneys[i]));
			}else{
				board->setInfo(i,m_managersRobot[i-1]->m_board->m_pLabel_name->getString(),ox[i],moneys_chip[i],moneys[i]>=0,abs(moneys[i]));
			}	
		}	
	}

	//播放音效
	if(moneys[0]>=0){
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioWin);
	}else{
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioLose);
	}

	//基础经验，手动摆牛，vip，房间
	short exps[4] = {10,0,0,0};
	short exp_total=10;

	if(m_isHandOx){
		exps[1]=rand()%3+1;
		exp_total+=exps[1];
	}

	short higsetVip =Tools::sharedTools()->getVipHighest();
	if(higsetVip!=-1){
		if(higsetVip==0){
			exps[2]=3;
		}else if(higsetVip==1){
			exps[2]=5;
		}else if(higsetVip==2){
			exps[2]=10;
		}
		exp_total+=exps[2];
	}

	if(m_room_kind==5){
		exps[3]=exps[2];
		exp_total+=exps[3];
	}
	board->setExperience(exps);

	//更新所有数据库数据
	m_managerPlayer->updateGameData(moneys[0],exp_total,this);


	//检测输家
	for(short i = 0 ; i < 5 ; i++){
		if(i==0){
			if(m_room_kind!=4){
				//银币场
				m_managerPlayer->m_board->updateMoney(Tools::sharedTools()->getCoin());
			}else{
				m_managerPlayer->m_board->updateMoney(Tools::sharedTools()->getIngot());
			}	
			unsigned min = logic->getMinMoneyToEnterRoom(m_room_kind);
			if((moneys_total[i]+moneys[i])<min){
				//输到下限了,出场
				Tools::sharedTools()->showToast(eToastMoneyNotEnough,this);
				scheduleOnce(schedule_selector(LayerGameDesk::exitGameDelay),2.5f);
				break;
			}
			//VIP场时效用光
			if(m_room_kind>=4){
				if(Tools::sharedTools()->getVipHighest()==-1){
					//VIP场时效用光,出场
					Tools::sharedTools()->showToast(eToastNeedVip,this);
					scheduleOnce(schedule_selector(LayerGameDesk::exitGameDelay),2.5f);
					break;
				}
			}
		}else{
			//机器人输光
			m_managersRobot[i-1]->m_board->updateMoney(moneys_total[i]+moneys[i]);
			unsigned min = logic->getMinMoneyToEnterRoom(m_room_kind);
			if((moneys_total[i]+moneys[i])<min){
				removeChildByTag(TAG_PLAYERS+i);
				initPlayer(i);
			}
		}
	}
}

void LayerGameDesk::exitGameDelay(float dt){
	exitGame();
}

void LayerGameDesk::removeCalcBoard(){
	removeChildByTag(TAG_CALC_BOARD);
}

bool LayerGameDesk::getCompareResultBossWith(short flag){
	// return true:boss win  
	// return false:boss lose

	//庄家自己跟自己比较
	if(flag==m_boss_flag){
		return true;
	}

	short ox_boss;
	short ox_player;
	//虽然下面这些值可能不用，但是先取出来，不然再取就很麻烦，因为每次都要判断flag是0还是非0
	short value_boss;
	short value_player;
	short kind_boss;
	short kind_player;

	//得到庄家的数据
	if(m_boss_flag==0){
		ox_boss=m_managerPlayer->m_cards->m_ox; //得到boss牛
		value_boss=m_managerPlayer->m_cards->getBigestValue();
		kind_boss=m_managerPlayer->m_cards->getBigestKind();
	}else{
		ox_boss=m_managersRobot[m_boss_flag-1]->m_cards->m_ox;
		value_boss=m_managersRobot[m_boss_flag-1]->m_cards->getBigestValue();
		kind_boss=m_managersRobot[m_boss_flag-1]->m_cards->getBigestKind();
	}

	//得到要和庄家比的玩家的数据
	if(flag==0){
		ox_player=m_managerPlayer->m_cards->m_ox;
		value_player=m_managerPlayer->m_cards->getBigestValue();
		kind_player=m_managerPlayer->m_cards->getBigestKind();
	}else{
		ox_player=m_managersRobot[flag-1]->m_cards->m_ox;
		value_player=m_managersRobot[flag-1]->m_cards->getBigestValue();
		kind_player=m_managersRobot[flag-1]->m_cards->getBigestKind();
	}

	//第一次比较：比牛
	if(ox_boss>ox_player){
		//boss win
		return true;
	}else if(ox_boss<ox_player){
		return false;
	}

	//第二次比较：比单牌的数值
	//若value为1，则把它变为14，因为A比K大
	value_boss= value_boss==1?14:value_boss;
	value_player= value_player==1?14:value_player;
	if(value_boss>value_player){
		return true;
	}else if(value_boss<value_player){
		return false;
	}

	//第三次比较：比单牌的花色
	//花色值越小，代表越大。 0：黑桃
	if(kind_boss<kind_player){
		return true;
	}else{
		return false;
	}

	return true;
}

unsigned int LayerGameDesk::getCalcResultBossWith(short flag,bool isBossWin){
	return 0;
}


void LayerGameDesk::menuUpCallback(CCObject* pSender)
{
	// 上方按钮 --  开始，抢庄,自动摊牌
	switch(m_GameStat){
	case eGameStatStandBy:	
		//		m_GameStat = eGameStatRobBoss;
		//		setGameStat(m_GameStat);
		removeCalcBoard();
		setPlayerStat(m_GameStat);
		moveOutMenus();
		break;
	case eGameStatRobBoss:			
		//		m_GameStat = eGameStatShowMoney; 
		//		setGameStat(m_GameStat);
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioRob);
		m_managerPlayer->m_isRobed=1;
		setPlayerStat(m_GameStat);
		moveOutMenus();
		break;
	case eGameStatShowMoney:
		//		m_GameStat = eGameStatGaming;
		//		setGameStat(m_GameStat);
		break;
	case eGameStatGaming:
		if(m_managerPlayer->m_cards->m_isWashingDone){
			m_managerPlayer ->showOxAuto();
			m_isOxShowed=true;
			moveOutMenus();
		}	
		break;
	case eGameStatCalcResult:
		//		m_GameStat = eGameStatStandBy;
		//		setGameStat(m_GameStat);
		m_managerPlayer->m_isCalcResult=true;
		break;
	}
}

void LayerGameDesk::menuDownCallback(CCObject* pSender)
{
	// 下方按钮 --  换桌，不抢，手动摆牛
	switch(m_GameStat){
	case eGameStatStandBy:	
		changeTable();
		removeCalcBoard();
		setGameStat(eGameStatStandBy);
		break;
	case eGameStatRobBoss:			
		//		m_GameStat = eGameStatShowMoney; 
		//		setGameStat(m_GameStat);
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioUnrob);
		m_managerPlayer->m_isRobed=0;
		setPlayerStat(m_GameStat);
		moveOutMenus();
		break;
	case eGameStatShowMoney:
		//		m_GameStat = eGameStatGaming;
		//		setGameStat(m_GameStat);
		//		m_managerPlayer->m_isRobed=false;
		break;
	case eGameStatGaming:
		if(m_managerPlayer->m_cards->m_isWashingDone){
			if(!m_isOxShowed){
				if(m_managerPlayer ->showOxHand()){
					m_isOxShowed=true;
					m_isHandOx=true;
					moveOutMenus();
				}else{
					Tools::sharedTools()->showToast(eToastOxError,this);
				}
			}	
		}
		break;
	case eGameStatCalcResult:
		//		m_GameStat = eGameStatStandBy;
		//		setGameStat(m_GameStat);
		break;
	}
}


void LayerGameDesk::moveIn(CCNode* node,float time){
	if(node!=NULL){
		//	CCActionInterval*  rightToLeft = CCMoveTo::create(0.05, ccp(Tools::sharedTools()->X(580),node->getPositionY()));
		CCActionInterval*  rightToLeft = CCMoveBy::create(time, ccp(-500,0));
		node->runAction(rightToLeft);
	}
}

void LayerGameDesk::moveOut(CCNode* node,float time){
	if(node!=NULL){
		CCActionInterval*  leftToRight = CCMoveBy::create(time, ccp(500,0));
		node->runAction(leftToRight);
	}
}


bool LayerGameDesk::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{

	return false;
}

void LayerGameDesk::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCLayer::onEnter();
}

void LayerGameDesk::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}    

void LayerGameDesk::menuShopCallback(CCObject* pSender)
{
	CCScene *scene =LayerShop::scene(2);
	//CCDirector::sharedDirector()->replaceScene(CCTransitionFadeDown::create(0.5f,scene ) );
	//CCDirector::sharedDirector()->replaceScene(scene);
	CCDirector::sharedDirector()->pushScene(scene);
}

void LayerGameDesk::menuCloseCallback(CCObject* pSender)
{
	if(m_GameStat==eGameStatGaming || m_GameStat == eGameStatCalcResult){
		//游戏中禁止退出toast		
		Tools::sharedTools()->showToast(eToastCantExitGaming,this);
	}else{
		exitGame();
	}
}

void LayerGameDesk::exitGame(){
	this->unscheduleAllSelectors();
	this->removeAllChildrenWithCleanup(true);
	GameLogic::deleteSharedGameLogic();
	Tools::sharedTools()->saveAllData();
	CCScene *scene =LayerHall::scene();
	//	CCDirector::sharedDirector()->replaceScene(CCTransitionFadeUp::create(0.5f,scene ) );
	CCDirector::sharedDirector()->replaceScene(scene);
}