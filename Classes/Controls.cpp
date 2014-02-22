#include "Controls.h"
#include "Tools.h"
#include "MyAudioEngine.h"
#include <string>
using namespace cocos2d;

//#define YY(__Y__) return y/480 * (CCDirector::sharedDirector()->getWinSize().height);


/************************************
*								    *
*            MyClock实现            *
*							        *
*************************************/
bool MyClock::init()
{
	CCSize size = CCDirector::sharedDirector()->getWinSize();

	bool bRet = false;
	do 
	{
		CCSprite* pSprite = CCSprite::create("clock.png");
		CC_BREAK_IF(! pSprite);
		addChild(pSprite,100,100);

		char temp[8];
		sprintf(temp,"");
		pLabel = CCLabelTTF::create(temp, "", 20);
		CC_BREAK_IF(! pLabel);
		pLabel->setColor(ccBLACK);
		this->addChild(pLabel,101,101);

		bRet = true;

		schedule(schedule_selector(MyClock::clockCallback),1.0f);
	} while (0);

	return bRet;
}

//正常倒计时
void MyClock::clockCallback(CCObject* pSender)
{	
	if(currentSeconds>0){
		char temp[8];
		sprintf(temp,"%d",currentSeconds);

		this->currentSeconds --;
		pLabel->setString(temp);
		if(currentSeconds==0){
			scheduleOnce(schedule_selector(MyClock::timesUpCallback),0.0f);
		}
	}
}

//时间到
void MyClock::timesUpCallback(CCObject* pSender)
{	
	char temp[8];
	sprintf(temp,"%d",currentSeconds);
	this->currentSeconds =0;
	pLabel->setString(temp);
	//unschedule
	CCActionInterval*  action = CCFadeOut::create(0.75f);
	getChildByTag(100)->runAction(action);
	CCActionInterval*  action2 = CCFadeOut::create(0.75f);
	getChildByTag(101)->runAction(action2);

	unschedule(schedule_selector(MyClock::clockCallback));

	//	scheduleOnce(schedule_selector(MyClock::removeClock),1.0f);
}

//未测试
void MyClock::removeClock()
{	
	//this->setVisible(false);
	//this->release();
	//this->removeAllChildren();
}


/************************************************************************
*								    								    *
*							  MyUserBoard实现					        *
*																	    *
************************************************************************/
MyUserBoard::MyUserBoard(bool isRobot,bool isCoin){
	this->m_totalSeconds=10;
	this->m_currentSeconds=0;
	this->m_isRobot = isRobot;
	this->m_isCoin =isCoin;
	initSelf();
}

void MyUserBoard::showTime(short seconds){
	m_isShowTime=true;
	this->m_currentSeconds=0;
	this->m_totalSeconds=seconds;
	m_pTo= CCProgressFromTo::create(m_totalSeconds, 100.0,0.0);
	m_pTimer->getSprite()->setColor(ccc3(0,255,0));
	m_pTimer->runAction( m_pTo);
	schedule(schedule_selector(MyUserBoard::changeColorCallback),1.0f/10);
}
void MyUserBoard::stopShowTime(){
	m_isShowTime=false;
	this->m_currentSeconds=0;
	unschedule(schedule_selector(MyUserBoard::changeColorCallback));
	m_pTimer->stopAllActions();
	m_pTimer->setPercentage(0);
}

void MyUserBoard::changeColorCallback(CCObject* pSender)
{	
	m_currentSeconds+=1.0f/10;
	float progress = m_currentSeconds/m_totalSeconds *100.0;

	if(progress>=100.0f){
		scheduleOnce(schedule_selector(MyUserBoard::stopCallback),0.0f);	
	}else if(progress>=50.0){
		float shit = 255-((progress-50)/50*255);
		m_pTimer->getSprite()->setColor(ccc3(255,shit,0));
	}
	else{
		float shit =(progress/50*255);
		m_pTimer->getSprite()->setColor(ccc3(shit,255,0));
	}

}

void MyUserBoard::stopCallback(CCObject* pSender){
	stopShowTime();
}

void MyUserBoard::initSelf(){

	//std::string ddd="Adan";
	char names[21][10]={"Adam","Ben","Chris","David","Eva","Frank","Gary","Jason","Ken","Leo",
		"Mark","Nick","Oscar","Paul","Ray","Sam","Tony","Vin","Will","Yoyo","Zack"};

//	char names[5][256]={"飒沓","倒萨","为钱而","请问","尺寸"};

	CCSprite *sprite1 = CCSprite::create("scene_game_head_bg.png");
	sprite1->setAnchorPoint(ccp(0.5,0.5));
	addChild(sprite1);

	//初始化头像
	short head;
	unsigned int money;
	char temp[32]; //注意不要越界
	if(m_isRobot){
		head=rand();
		head%=16;
		head=head<0 ? -head : head;	
		money=0;
		short name_flag=rand()%21; //随机name
		sprintf(temp,"%s",names[name_flag]);	
		m_pLabel_name = CCLabelTTF::create(temp, "Arial", 22);		
	}else{
		Tools* tools=Tools::sharedTools();
		head = tools->getHead();
		if(m_isCoin){
			money= tools->getCoin();
		}else{
			money= tools->getIngot();
		}
		m_pLabel_name = CCLabelTTF::create(tools->getNickName().c_str(), "Arial", 22);
	}

	//性别
	if(head>=8){
		m_sex_isBoy=true;
	}else{
		m_sex_isBoy=false;
	}

	//设置昵称
	m_pLabel_name->setColor(ccWHITE);
	this->addChild(m_pLabel_name);
	m_pLabel_name->setPositionY(57);

	sprintf(temp,"head_%d.png",head);	
	CCSprite *sprite2 = CCSprite::create(temp);
	addChild(sprite2);
	sprite2->setAnchorPoint(ccp(0.5,0.5));

	m_pLabel_money = CCLabelTTF::create("", "Arial", 22);
	m_pLabel_money->setColor(ccWHITE);
	this->addChild(m_pLabel_money);
	m_pLabel_money->setPositionY(-58);
	updateMoney(money);

	m_pTo= CCProgressFromTo::create(m_totalSeconds, 100.0,0.0);
	m_pTimer = CCProgressTimer::create(CCSprite::create("scene_game_head_bar.png"));
	m_pTimer->setType( kCCProgressTimerTypeRadial );
	//	m_pTimer->getSprite()->setColor(ccBLUE);
	m_pTimer->setReverseProgress(true);
	addChild(m_pTimer);
	m_pTimer->setAnchorPoint(ccp(0.5,0.5));
	//	m_pTimer->runAction( CCRepeatForever::create(m_pTo));
	//	m_pTimer->runAction( m_pTo);
}

void MyUserBoard::updateMoney(unsigned int money){
	char mmm[16];
	sprintf(mmm,"%d",money);
	m_pLabel_money->setString(mmm);
	this->m_money = money;
}

/************************************
*								    *
*            MyCardBase实现         *
*							        *
*************************************/
MyCardBase::MyCardBase(CardKind kind ,int value,float x ,float y){
	this->m_kind = kind;
	this->m_value = value;
	this->m_default_x=x;
	this->m_default_y =y;
	this->m_isChoosed = false;
	this->m_currentChanged = false;
	this->m_offset_y = 10.0f;

	initSelf();
	setPosition( ccp(m_default_x,m_default_y) );
}

MyCardBase::MyCardBase(int kind ,int value,float x ,float y){
	this->m_value = value;
	this->m_default_x=x;
	this->m_default_y =y;
	this->m_isChoosed = false;
	this->m_currentChanged = false;
	this->m_offset_y =10.0f;

	switch(kind){
	case 0:
		this->m_kind = eCardKindSpades;
		break;
	case 1:
		this->m_kind = eCardKindHearts;
		break;
	case 2:
		this->m_kind = eCardKindClubs;
		break;
	case 3:
		this->m_kind = eCardKindDiamonds;
		break;
	}

	initSelf();
	setPosition( ccp(m_default_x,m_default_y) );
}

//注意，这里是没有考虑大小王的情况的
void MyCardBase::initSelf(){
	//kind:0,1,2,3 value:1-13
	if(m_value<=13 && m_value>=1 && m_kind>=0 && m_kind<=3){
		char temp[32];
		switch(m_kind){
		case eCardKindSpades:
			sprintf(temp,"card_spades_%d.png",m_value);
			break;
		case eCardKindHearts:
			sprintf(temp,"card_hearts_%d.png",m_value);
			break;
		case eCardKindClubs:
			sprintf(temp,"card_clubs_%d.png",m_value);
			break;
		case eCardKindDiamonds:
			sprintf(temp,"card_diamonds_%d.png",m_value);
			break;
		}
		this->initWithFile(temp);
	}			
}

void MyCardBase::setFocused(){
	setColor(ccGRAY);	
}

/************************************
*								    *
*            MyCardSelf实现         *
*							        *
*************************************/

/*
bool MyCard::init(){

CCSize size = CCDirector::sharedDirector()->getWinSize();

bool bRet = false;
do 
{	
//initSelf(1,1);
bRet = true;
} while (0);

return bRet;
}*/


MyCardSelf::MyCardSelf(CardKind kind ,int value,float x ,float y){
	this->m_kind = kind;
	this->m_value = value;
	this->m_default_x=x;
	this->m_default_y =y;
	this->m_isChoosed = false;
	this->m_currentChanged = false;
	this->m_offset_y = 20.0f;

	initSelf();
	setPosition( ccp(m_default_x,m_default_y) );
}

MyCardSelf::MyCardSelf(int kind ,int value,float x ,float y){
	this->m_value = value;
	this->m_default_x=x;
	this->m_default_y =y;
	this->m_isChoosed = false;
	this->m_currentChanged = false;
	this->m_offset_y =20.0f;

	switch(kind){
	case 0:
		this->m_kind = eCardKindSpades;
		break;
	case 1:
		this->m_kind = eCardKindHearts;
		break;
	case 2:
		this->m_kind = eCardKindClubs;
		break;
	case 3:
		this->m_kind = eCardKindDiamonds;
		break;
	}

	initSelf();
	setPosition( ccp(m_default_x,m_default_y) );
}

//注意，这里是没有考虑大小王的情况的
void MyCardSelf::initSelf(){
	//kind:0,1,2,3 value:0-12
	if(m_value<=13 && m_value>=1 && m_kind>=0 && m_kind<=3){
		char temp[32];
		switch(m_kind){
		case eCardKindSpades:
			sprintf(temp,"card_spades_%d.png",m_value);
			break;
		case eCardKindHearts:
			sprintf(temp,"card_hearts_%d.png",m_value);
			break;
		case eCardKindClubs:
			sprintf(temp,"card_clubs_%d.png",m_value);
			break;
		case eCardKindDiamonds:
			sprintf(temp,"card_diamonds_%d.png",m_value);
			break;
		}
		this->initWithFile(temp);
	}		

}

void MyCardSelf::setChoosed(bool choosed){
	if(choosed){
		//从未选中状态改为选中状态
		setPositionY(m_default_y+m_offset_y);
		setPositionX(m_default_x);
	}else{
		//从选中状态改为初始状态
		setPositionY(m_default_y);
		setPositionX(m_default_x);
	}
}

void MyCardSelf::setFocused(bool focus){

	if(focus){
		setColor(ccGRAY);
	}else{
		setColor(ccWHITE);
	}
}

/*
void MyCard::shoot(int flag,int sum)){
CCSize size = CCDirector::sharedDirector()->getWinSize();
CCPoint finalPoint;
float y = size.height*0.65f;
if(sum==1){
finalPoint=ccp(size.width/2,y);
}
CCActionInterval*  action = CCMoveTo::create(0.5f,finalPoint);
runAction(action);
}*/


bool MyCardSelf::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{
	if (m_currentChanged == true) 
		return false;
	if ( !containsTouchLocation(pTouches) ) 
		return false;

	m_currentChanged=true;
	setFocused(true);

	return true;
}

/*
void MyCard::ccTouchMoved(CCTouch* touch, CCEvent* event)
{	
CCPoint point = touch->getLocation();
char temp[128];
sprintf(temp,"TouchMove:(%f,%f)",point.x,point.y);
CCLOG(temp);

if (containsTouchLocation(touch) ) {
m_currentChanged=true;
setFocused(true);
}
}*/


void MyCardSelf::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint point = touch->getLocation();
	/*
	char temp[128];
	sprintf(temp,"TouchEnd:(%f,%f)",point.x,point.y);
	CCLOG(temp);*/

	if(m_currentChanged){
		setFocused(false);
	}

	m_currentChanged=false;

	//如果落点还在本张牌
	if ( containsTouchLocation(touch) ) {	
		this->m_isChoosed=!m_isChoosed;
		setChoosed(m_isChoosed);
	}

	//	shoot();

} 

CCRect MyCardSelf::rect()
{
	CCSize s = getTexture()->getContentSize();
	return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}


void MyCardSelf::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCSprite::onEnter();
}

void MyCardSelf::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCSprite::onExit();
}   

bool MyCardSelf::containsTouchLocation(CCTouch* touch)
{
	return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

void MyCardSelf::touchDelegateRetain()
{
	this->retain();
}

void MyCardSelf::touchDelegateRelease()
{
	this->release();
}

/*
CCObject* MyCard::copyWithZone(CCZone *pZone)
{
this->retain();
return this;
}*/

/************************************
*								    *
*            MyGameTopBar实现       *
*							        *
*************************************/
MyGameTopBar::MyGameTopBar(){
	this->m_isTouched = false;
	initSelf();
	setPosition(ccp(cocos2d::CCDirector::sharedDirector()->getWinSize().width/2,cocos2d::CCDirector::sharedDirector()->getWinSize().height/2));
}

void MyGameTopBar::initSelf(){

	CCSprite* pSpriteBg = CCSprite::create("scene_game_btn_top_bg.png");
	addChild(pSpriteBg,TAG_BG,TAG_BG);

	/*
	CCSprite* pSpriteLeft = CCSprite::create("scene_game_btn_top_light.png");
	//	CC_BREAK_IF(! pSprite);
	addChild(pSpriteLeft,11,11);
	pSpriteLeft->setPositionX(-170);

	CCSprite* pSpriteRight = CCSprite::create("scene_game_btn_top_light.png");
	//	CC_BREAK_IF(! pSprite);
	addChild(pSpriteRight,12,12);
	pSpriteRight->setPositionX(170);*/

	//	CCSprite* pSprite2 = CCSprite::create("scene_game_btn_top_light.png");
	//CC_BREAK_IF(! pSprite2);
	//	addChild(pSprite2);


	//	CCSprite* pSprite3 = CCSprite::create("scene_game_btn_top_mask.png");
	//	CC_BREAK_IF(! pSprite3);
	//	addChild(pSprite3,13,13);
	//	pSprite3->setScaleX(CCDirector::sharedDirector()->getWinSize().width /pSprite3->getContentSize().width);

	CCSprite* pSpritePoker = CCSprite::create("scene_game_btn_top_poker.png");
	pSpritePoker->setPositionX(-150);
	addChild(pSpritePoker,TAG_POKER,TAG_POKER);

	CCSprite* pSpriteSoundON = CCSprite::create("scene_game_btn_top_sound.png");
	addChild(pSpriteSoundON,TAG_SOUND_ON,TAG_SOUND_ON);
	pSpriteSoundON->setPositionX(-50);

	CCSprite* pSpriteSoundOff = CCSprite::create("scene_game_btn_top_soundoff.png");
	addChild(pSpriteSoundOff,TAG_SOUND_OFF,TAG_SOUND_OFF);
	pSpriteSoundOff->setPositionX(-50);

	if(Tools::sharedTools()->m_isSoundsOn){
		//声音为开启状态
		pSpriteSoundOff->setVisible(false);
	}else{
		pSpriteSoundON->setVisible(false);
	}

	CCSprite* pSpriteHelp = CCSprite::create("scene_game_btn_top_prize.png");
	pSpriteHelp->setPositionX(50);
	addChild(pSpriteHelp,TAG_PRIZE,TAG_PRIZE);

	CCSprite* pAbout = CCSprite::create("scene_game_btn_top_help.png");
	pAbout->setPositionX(150);
	addChild(pAbout,TAG_HELP,TAG_HELP);

	this->setAnchorPoint(ccp(0.5,1));

}


CCRect MyGameTopBar::rect()
{	
	CCSize s = getChildByTag(TAG_BG)->getContentSize();
	return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}


bool MyGameTopBar::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{
	if ( !containsTouchLocation(pTouches) ) 
		return false;
	return true;
}


/*
void MyGameTopBar::ccTouchMoved(CCTouch* touch, CCEvent* event)
{	

if (containsTouchLocation(touch) ) {
CCPoint point =  this->convertTouchToNodeSpace(touch);
getChildByTag(11)->setPosition(ccp(point.x,0));
getChildByTag(12)->setPosition(ccp(point.x,0));
}
}*/

void MyGameTopBar::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	removeChildByTag(TAG_TIP);

	CCPoint point =  this->convertTouchToNodeSpace(touch);
	CCActionInterval*  actionOrbit = CCOrbitCamera::create(0.2f,0.5f, 0.5f, 0, 360, 0, 0);

	if(getChildByTag(TAG_POKER)->boundingBox().containsPoint(point)){
		//poker	
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioSkew);
		getChildByTag(TAG_POKER)->runAction(actionOrbit);

		MyBaseNodeTouchToRemove* poker = new MyBaseNodeTouchToRemove("common_poker_tip_board.png");
		addChild(poker,TAG_TIP,TAG_TIP);
		poker->setPosition(ccp(0,-230+576));
		CCActionInterval* move = CCMoveBy::create(0.5f,ccp(0,-576));
		poker->runAction(Tools::sharedTools()->getActionMoveDown());

	}else if(getChildByTag(TAG_SOUND_ON)->boundingBox().containsPoint(point)){
		if(getChildByTag(TAG_SOUND_ON)->isVisible()){
			//sound off
			Tools::sharedTools()->m_isSoundsOn = false;
			MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioSkew);
			getChildByTag(TAG_SOUND_ON)->setVisible(false);
			getChildByTag(TAG_SOUND_OFF)->setVisible(true);
			getChildByTag(TAG_SOUND_OFF)->runAction(actionOrbit);
			MyAudioEngine::sharedMyAudioEngine()->stopBackgrondMusic();
		}else if(getChildByTag(TAG_SOUND_OFF)->isVisible()){
			//sound on
			Tools::sharedTools()->m_isSoundsOn = true;
			MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioSkew);
			getChildByTag(TAG_SOUND_OFF)->setVisible(false);
			getChildByTag(TAG_SOUND_ON)->setVisible(true);
			getChildByTag(TAG_SOUND_ON)->runAction(actionOrbit);
			MyAudioEngine::sharedMyAudioEngine()->playBackgrondMusic();
		}	
	}else if(getChildByTag(TAG_HELP)->boundingBox().containsPoint(point)){
		//help
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioSkew);
		getChildByTag(TAG_HELP)->runAction(actionOrbit);

		MyBaseNodeTouchToRemove* tip = new MyBaseNodeTouchToRemove("common_help_tip_board.png");
		addChild(tip,TAG_TIP,TAG_TIP);
		tip->setPosition(ccp(0,-230+576));
		CCActionInterval* move = CCMoveBy::create(0.5f,ccp(0,-576));
		tip->runAction(Tools::sharedTools()->getActionMoveDown());

	}else if(getChildByTag(TAG_PRIZE)->boundingBox().containsPoint(point)){
		//about
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioSkew);
		getChildByTag(TAG_PRIZE)->runAction(actionOrbit);

		MyBaseNodeTouchToRemove* tip = new MyBaseNodeTouchToRemove("common_upgrade_tip_board.png");
		addChild(tip,TAG_TIP,TAG_TIP);
		tip->setPosition(ccp(0,-230+576));
		CCActionInterval* move = CCMoveBy::create(0.5f,ccp(0,-576));
		tip->runAction(Tools::sharedTools()->getActionMoveDown());
	}

} 

void MyGameTopBar::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCNode::onEnter();
}

void MyGameTopBar::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCNode::onExit();
}    

bool MyGameTopBar::containsTouchLocation(CCTouch* touch)
{
	return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

void MyGameTopBar::touchDelegateRetain()
{
	this->retain();
}

void MyGameTopBar::touchDelegateRelease()
{
	this->release();
}



/************************************
*								    *
*            MyMoneyBarTouchable实现         *
*							        *
*************************************/

MyMoneyBarTouchable::MyMoneyBarTouchable(int flag,long money){
	this->m_flag = flag;
	this->m_money = money;
	this->m_isMoved = false;

	initSelf();

}

cocos2d::CCPoint MyMoneyBarTouchable::getDefaultPoint(int flag){
	float x1,y1;
	switch(m_flag){
	case 0:
		x1=151.0;
		y1=49.0;
		break;
	case 1:
		x1=21.0;
		y1=195.0;
		break;
	case 2:
		x1=21.0;
		y1=338.0;
		break;
	case 3:
		x1=693.0;
		y1=339.0;
		break;
	case 4:
		x1=692.0;
		y1=195.0;
		break;
	}
	return ccp(x1,y1);
}

void MyMoneyBarTouchable::initSelf(){

	CCPoint point = getDefaultPoint(m_flag);

	CCSprite* m_sprite_bg = CCSprite::create("scene_game_money_bar.png");
	addChild(m_sprite_bg,100,100);
	m_sprite_bg->setPosition(point);

	char temp[32]="";
	sprintf(temp,"%d",m_money);
	m_label_money = CCLabelTTF::create(temp, "Arail", 22);
	m_label_money->setColor(ccWHITE);
	m_label_money->setPosition(ccp(point.x+8,point.y));
	this->addChild(m_label_money,111,111);
}


void MyMoneyBarTouchable::setFocused(bool focus){
	if(!m_isMoved){
		if(focus){
			getChildByTag(100)->setScale(1.2f);
			getChildByTag(111)->setScale(1.2f);
		}else{
			getChildByTag(100)->setScale(1.0f);
			getChildByTag(111)->setScale(1.0f);
		}
	}	
}

//进场
void MyMoneyBarTouchable::moveIn(){
	if(!m_isMoved){
		int offset_y = 24,offset_x = 112 ,offset_x2 = 8;

		float x1,y1;
		switch(m_flag){
		case 0:
			x1=420-offset_x /2;
			y1=230+offset_y;
			break;
		case 1:
			x1=420-offset_x /2-offset_x2;
			y1=230+offset_y*2;
			break;
		case 2:
			x1=420-offset_x /2-offset_x2*2;
			y1=230+offset_y*3;
			break;
		case 3:
			x1=420+offset_x /2-offset_x2*2;
			y1=230+offset_y*3;
			break;
		case 4:
			x1=420+offset_x /2-offset_x2;
			y1=230+offset_y*2;
			break;
		}

		CCActionInterval*  action1 = CCMoveTo::create(0.1f, ccp(x1,y1));
		CCActionInterval*  action2 = CCMoveTo::create(0.1f, ccp(x1+8,y1));

		getChildByTag(100)->runAction(action1);
		getChildByTag(111)->runAction(action2);
	}
}

void MyMoneyBarTouchable::moveTo(int flag){
	float x1,y1;
	switch(m_flag){
	case 0:
		x1=151.0;
		y1=49.0;
		break;
	case 1:
		x1=21.0;
		y1=195.0;
		break;
	case 2:
		x1=21.0;
		y1=338.0;
		break;
	case 3:
		x1=693.0;
		y1=339.0;
		break;
	case 4:
		x1=692.0;
		y1=195.0;
		break;
	}
	CCActionInterval*  action1 = CCMoveTo::create(0.1f, ccp(x1,y1));
	CCActionInterval*  action2 = CCMoveTo::create(0.1f, ccp(x1+8,y1));
	CCActionInterval*  action3 = CCFadeOut::create(0.1f);
	CCActionInterval*  action4 = CCFadeOut::create(0.1f);
	getChildByTag(100)->runAction(action1);
	getChildByTag(111)->runAction(action2);
	getChildByTag(100)->runAction(action3);
	getChildByTag(111)->runAction(action4);

}

void MyMoneyBarTouchable::refreshWithMoney(long money){
	m_money = money;
	m_isMoved = false;
	getChildByTag(100)->setVisible(true);
	getChildByTag(111)->setVisible(true);

	char temp[32]="";
	sprintf(temp,"%d",m_money);
	m_label_money->setString(temp);

	CCPoint point = getDefaultPoint(m_flag);
	getChildByTag(100)->setPosition(point);
	getChildByTag(111)->setPosition(point);
}

bool MyMoneyBarTouchable::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{
	if ( !containsTouchLocation(pTouches) ) 
		return false;

	setFocused(true);

	return true;
}


void MyMoneyBarTouchable::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint point = touch->getLocation();
	setFocused(false);

	//如果落点还在本张牌
	if ( containsTouchLocation(touch) ) {		
		moveIn();
		m_isMoved=true;
	}

} 

CCRect MyMoneyBarTouchable::rect()
{
	CCSize s =getChildByTag(100)->getContentSize();
	return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}


void MyMoneyBarTouchable::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCNode::onEnter();
}

void MyMoneyBarTouchable::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCNode::onExit();
}   

bool MyMoneyBarTouchable::containsTouchLocation(CCTouch* touch)
{
	CCPoint pp  =getChildByTag(100)->convertTouchToNodeSpaceAR(touch);
	return rect().containsPoint(pp);
}

void MyMoneyBarTouchable::touchDelegateRetain()
{
	this->retain();
}

void MyMoneyBarTouchable::touchDelegateRelease()
{
	this->release();
}



/************************************
*								    *
*            MyMoneyBarRobot实现         *
*							        *
*************************************/

MyMoneyBarRobot::MyMoneyBarRobot(short flag,unsigned int money){
	this->m_flag = flag;
	this->m_money = money;
	this->m_isMoved = false;

	initSelf();
}

cocos2d::CCPoint MyMoneyBarRobot::getDefaultPoint(short flag){
	return ccp(0,60);
}

void MyMoneyBarRobot::initSelf(){

	CCPoint point = getDefaultPoint(m_flag);

	CCSprite* m_sprite_bg;
	switch(m_flag){
	case 0:
		m_sprite_bg = CCSprite::create("scene_game_coin_bar_0.png");
		break;
	case 1:
		m_sprite_bg = CCSprite::create("scene_game_coin_bar_1.png");
		break;
	case 2:
		m_sprite_bg = CCSprite::create("scene_game_coin_bar_2.png");
		break;
	case 3:
		m_sprite_bg = CCSprite::create("scene_game_coin_bar_3.png");
		break;
	case 4:
		m_sprite_bg = CCSprite::create("scene_game_ingot_bar_0.png");
		break;
	case 5:
		m_sprite_bg = CCSprite::create("scene_game_ingot_bar_1.png");
		break;
	case 6:
		m_sprite_bg = CCSprite::create("scene_game_ingot_bar_2.png");
		break;
	case 7:
		m_sprite_bg = CCSprite::create("scene_game_ingot_bar_3.png");
		break;
	}

	addChild(m_sprite_bg,100,100);
	m_sprite_bg->setPosition(point);

	char temp[32]="";
	sprintf(temp,"%d",m_money);
	m_label_money = CCLabelTTF::create(temp, "Arial", 22);
	m_label_money->setColor(ccWHITE);
	m_label_money->setPosition(ccp(point.x+15,point.y));
	this->addChild(m_label_money,111,111);
}


void MyMoneyBarRobot::setFocused(bool focus){
	if(!m_isMoved){
		if(focus){
			getChildByTag(100)->setScale(1.2f);
			getChildByTag(111)->setScale(1.2f);
		}else{
			getChildByTag(100)->setScale(1.0f);
			getChildByTag(111)->setScale(1.0f);
		}
	}	
}

//进场
void MyMoneyBarRobot::moveIn(){
	if(!m_isMoved){
		int offset_y = 24,offset_x = 112 ,offset_x2 = 8;

		float x1,y1;
		switch(m_flag){
		case 0:
			x1=420-offset_x /2;
			y1=260+offset_y;
			break;
		case 1:
			x1=420-offset_x /2-offset_x2;
			y1=260+offset_y*2;
			break;
		case 2:
			x1=420-offset_x /2-offset_x2*2;
			y1=260+offset_y*3;
			break;
		case 3:
			x1=420+offset_x /2-offset_x2*2;
			y1=260+offset_y*3;
			break;
		case 4:
			x1=420+offset_x /2-offset_x2;
			y1=260+offset_y*2;
			break;
		}

		CCActionInterval*  action1 = CCMoveTo::create(0.2f, ccp(x1,y1));
		CCActionInterval*  action2 = CCMoveTo::create(0.2f, ccp(x1+8,y1));

		getChildByTag(100)->runAction(action1);
		getChildByTag(111)->runAction(action2);
	}

}

void MyMoneyBarRobot::moveTo(short flag){
	float x1,y1;
	switch(m_flag){
	case 0:
		x1=151.0;
		y1=49.0;
		break;
	case 1:
		x1=21.0;
		y1=195.0;
		break;
	case 2:
		x1=21.0;
		y1=338.0;
		break;
	case 3:
		x1=693.0;
		y1=339.0;
		break;
	case 4:
		x1=692.0;
		y1=195.0;
		break;
	}
	CCActionInterval*  action1 = CCMoveTo::create(0.1f, ccp(x1,y1));
	CCActionInterval*  action2 = CCMoveTo::create(0.1f, ccp(x1+8,y1));
	CCActionInterval*  action3 = CCFadeOut::create(0.1f);
	CCActionInterval*  action4 = CCFadeOut::create(0.1f);
	getChildByTag(100)->runAction(action1);
	getChildByTag(111)->runAction(action2);
	//getChildByTag(100)->runAction(action3);
	//getChildByTag(111)->runAction(action4);

	scheduleOnce(schedule_selector(MyMoneyBarRobot::removeCallback),0.3f);

}

void MyMoneyBarRobot::refreshWithMoney(unsigned int money){
	m_money = money;
	m_isMoved = false;
	getChildByTag(100)->setVisible(true);
	getChildByTag(111)->setVisible(true);

	char temp[32]="";
	sprintf(temp,"%d",m_money);
	m_label_money->setString(temp);

	CCPoint point = getDefaultPoint(m_flag);
	getChildByTag(100)->setPosition(point);
	getChildByTag(111)->setPosition(point);
}


CCRect MyMoneyBarRobot::rect()
{
	CCSize s =getChildByTag(100)->getContentSize();
	return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

bool MyMoneyBarRobot::containsTouchLocation(CCTouch* touch)
{
	CCPoint pp  =getChildByTag(100)->convertTouchToNodeSpaceAR(touch);
	return rect().containsPoint(pp);
}

//时间到
void MyMoneyBarRobot::removeCallback(CCObject* pSender)
{	
	removeAllChildren();
}



/************************************
*								    *
*            MyMoneyBar实现         *
*							        *
*************************************/

MyMoneyBar::MyMoneyBar(short flag,long money){
	this->m_flag = flag;
	this->m_money = money;
	this->m_isMoved = false;

	initSelf();
}

cocos2d::CCPoint MyMoneyBar::getDefaultPoint(int flag){
	float x1,y1;
	if(flag>3){
		flag-=4;
	}
	switch(flag){
	case 0:
		x1=130;
		y1=57.0;
		break;
	case 1:
		x1=290.0;
		y1=57.0;
		break;
	case 2:
		x1=450.0;
		y1=57.0;
		break;
	case 3:
		x1=610.0;
		y1=57.0;
		break;
	}
	return ccp(x1,y1);
}

void MyMoneyBar::initSelf(){

	CCPoint point = getDefaultPoint(m_flag);
	CCSprite* m_sprite_bg;
	switch(m_flag){
	case 0:
		m_sprite_bg = CCSprite::create("scene_game_coin_bar_0.png");
		break;
	case 1:
		m_sprite_bg = CCSprite::create("scene_game_coin_bar_1.png");
		break;
	case 2:
		m_sprite_bg = CCSprite::create("scene_game_coin_bar_2.png");
		break;
	case 3:
		m_sprite_bg = CCSprite::create("scene_game_coin_bar_3.png");
		break;
	case 4:
		m_sprite_bg = CCSprite::create("scene_game_ingot_bar_0.png");
		break;
	case 5:
		m_sprite_bg = CCSprite::create("scene_game_ingot_bar_1.png");
		break;
	case 6:
		m_sprite_bg = CCSprite::create("scene_game_ingot_bar_2.png");
		break;
	case 7:
		m_sprite_bg = CCSprite::create("scene_game_ingot_bar_3.png");
		break;
	}

	addChild(m_sprite_bg,90,90);
	m_sprite_bg->setPositionY(point.y);

	char temp[32]="";
	sprintf(temp,"%d",m_money);
	m_label_money = CCLabelTTF::create(temp, "Arial", 22);
	m_label_money->setColor(ccWHITE);
	m_label_money->setPosition(ccp(8,0));
	addChild(m_label_money,91,91);

	CCActionInterval*  action1 = CCMoveTo::create(0.15f,point);
	CCActionInterval*  action2 = CCMoveTo::create(0.15f,ccp(point.x+15,point.y));

	m_sprite_bg->runAction(action1);
	m_label_money->runAction(action2);
}


void MyMoneyBar::setFocused(bool focus){
	if(!m_isMoved){
		if(focus){
			getChildByTag(90)->setScale(1.2f);
			getChildByTag(91)->setScale(1.2f);
		}else{
			getChildByTag(90)->setScale(1.0f);
			getChildByTag(91)->setScale(1.0f);
		}
	}	
}

//进场
void MyMoneyBar::moveIn(){
	if(!m_isMoved){
		CCPoint point = getDefaultPoint(m_flag);
		CCActionInterval*  action1 = CCMoveBy::create(0.15f,ccp(140 - point.x,0));
		CCActionInterval*  action2 = CCMoveBy::create(0.15f,ccp(140 - point.x,0));

		getChildByTag(90)->runAction(action1);
		getChildByTag(91)->runAction(action2);
	}
}

void MyMoneyBar::moveTo(int flag){
	float x1,y1;
	switch(m_flag){
	case 0:
		x1=151.0;
		y1=49.0;
		break;
	case 1:
		x1=21.0;
		y1=195.0;
		break;
	case 2:
		x1=21.0;
		y1=338.0;
		break;
	case 3:
		x1=693.0;
		y1=339.0;
		break;
	case 4:
		x1=692.0;
		y1=195.0;
		break;
	}
	CCActionInterval*  action1 = CCMoveTo::create(0.1f, ccp(x1,y1));
	CCActionInterval*  action2 = CCMoveTo::create(0.1f, ccp(x1+8,y1));
	CCActionInterval*  action3 = CCFadeOut::create(0.1f);
	CCActionInterval*  action4 = CCFadeOut::create(0.1f);
	getChildByTag(90)->runAction(action1);
	getChildByTag(91)->runAction(action2);
	//getChildByTag(100)->runAction(action3);
	//getChildByTag(111)->runAction(action4);

}

void MyMoneyBar::refreshWithMoney(long money){
	m_money = money;
	m_isMoved = false;
	getChildByTag(90)->setVisible(true);
	getChildByTag(91)->setVisible(true);

	char temp[32]="";
	sprintf(temp,"%d",m_money);
	m_label_money->setString(temp);

	CCPoint point = getDefaultPoint(m_flag);
	getChildByTag(90)->setPosition(point);
	getChildByTag(91)->setPosition(point);
}


CCRect MyMoneyBar::rect()
{
	CCSize s =getChildByTag(90)->getContentSize();
	return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

bool MyMoneyBar::containsTouchLocation(CCTouch* touch)
{
	CCPoint pp  =getChildByTag(90)->convertTouchToNodeSpaceAR(touch);
	return rect().containsPoint(pp);
}

//时间到
void MyMoneyBar::removeCallback(CCObject* pSender)
{	
	removeAllChildren();
}


bool MyMoneyBar::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{
	if ( !containsTouchLocation(pTouches) ) 
		return false;

	setFocused(true);

	return true;
}


void MyMoneyBar::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	CCPoint point = touch->getLocation();
	setFocused(false);

	//如果落点还在本张牌
	if ( containsTouchLocation(touch) ) {	
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioShowChipsOne);
		moveIn();
		m_isMoved=true;
	}

} 


void MyMoneyBar::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCNode::onEnter();
}

void MyMoneyBar::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCNode::onExit();
}   


void MyMoneyBar::touchDelegateRetain()
{
	this->retain();
}

void MyMoneyBar::touchDelegateRelease()
{
	this->release();
}

/************************************
*								    *
*            MyClock2实现           *
*							        *
*************************************/
MyClock2::MyClock2(int seconds)
{
	if(seconds>=1){
		this->currentSeconds = seconds;
	}else{
		this->currentSeconds = 1;
	}

	CCSize size = CCDirector::sharedDirector()->getWinSize();

	pSprite = CCSprite::create("clock.png");
	addChild(pSprite,100,100);

	char temp[8]="";
	sprintf(temp,"%d",currentSeconds);
	pLabel = CCLabelTTF::create(temp, "", 20);
	pLabel->setColor(ccBLACK);
	this->addChild(pLabel,101,101);

	schedule(schedule_selector(MyClock2::clockCallback),1.0f);

}

//正常倒计时
void MyClock2::clockCallback(CCObject* pSender)
{	
	if(currentSeconds>0){
		this->currentSeconds --;
		char temp[8];
		sprintf(temp,"%d",currentSeconds);
		pLabel->setString(temp);
		if(currentSeconds==0){ 
			scheduleOnce(schedule_selector(MyClock2::timesUpCallback),1.0f);
		}
	}
}

//时间到
void MyClock2::timesUpCallback(CCObject* pSender)
{	
	char temp[8];
	sprintf(temp,"%d",currentSeconds);
	this->currentSeconds =0;
	pLabel->setString(temp);


	CCActionInterval*  action = CCFadeOut::create(0.75f);
	pLabel->runAction(action);
	CCActionInterval*  action2 = CCFadeOut::create(0.75f);
	pSprite->runAction(action2);

	unschedule(schedule_selector(MyClock2::clockCallback));

}

//未测试
void MyClock2::removeClock()
{	
	unschedule(schedule_selector(MyClock2::clockCallback));
	removeAllChildren();
}



/************************************
*								    *
*            MyInfoBoard实现         *
*							        *
*************************************/

MyInfoBoard::MyInfoBoard(){
	m_isHeadChooseBarShowed=false;
	initSelf();
}

void MyInfoBoard::initSelf(){
	CCSprite* m_sprite_bg = CCSprite::create("scene_welcome_info_bg.png");
	addChild(m_sprite_bg);

	float highest_y = 55;
	float offset_y = 42;
	float center_x = 60;

	Tools* tool = Tools::sharedTools();

	tool->updateRooted();

	std::string nick = tool->getNickName();
	unsigned short lv =  tool->getLevel();
	unsigned int coin =  tool->getCoin();
	unsigned int mess = tool->getIngot();
	unsigned int most_won =  tool->getMostWon();
	unsigned int win = tool->getWin();
	unsigned int lost = tool->getLost();

	char temp[64]="";	
	sprintf(temp,"%s",nick.c_str());

	m_label_name = CCLabelTTF::create(temp, "", 24);
	//	l_lv->setFontName("fonts\felt.ttf");
//	m_label_name->setColor(ccWHITE);
	m_label_name->setPosition(ccp(-50,130));
	this->addChild(m_label_name);

	//vip剩余有效局数
	sprintf(temp,"%d",tool->getVipAmount());
	addLabel(temp,ccp(45,75));

	sprintf(temp,"%d",lv);
	addLabel(temp,ccp(-135,highest_y - offset_y));

	sprintf(temp,"%d",coin);
	pLabelCoin=addLabel(temp,ccp(center_x,highest_y - offset_y * 2));

	sprintf(temp,"%d",mess);
	addLabel(temp,ccp(center_x,highest_y - offset_y* 3));

	sprintf(temp,"%d",most_won);
	addLabel(temp,ccp(center_x,highest_y - offset_y* 4));

	sprintf(temp,"%d",win);
	addLabel(temp,ccp(center_x,highest_y - offset_y* 5));

/*  //用户名编辑按钮，暂时取消
	CCSprite* m_sprite_set = CCSprite::create("scene_welcome_btn_set0.png");
	addChild(m_sprite_set,TAG_EDIT_BTN,TAG_EDIT_BTN);
	m_sprite_set->setPosition(ccp(190,130));*/

	setHead(tool->getHead());
	//	setNickName("Lucy Ismygirl");

	//经验条背景
	CCSprite* bar_bg = CCSprite::create("scene_welcome_lv_bar0.png");
	addChild(bar_bg);
	bar_bg->setAnchorPoint(ccp(0.5f,0.5f));
	bar_bg->setPosition(ccp(center_x - 50, highest_y - offset_y + 2));

	unsigned short s_level = lv;
	unsigned short s_experience =tool->getExperience();
	unsigned short s_max = tool->getMaxExperience();

	//经验条
	m_loading_bar = CCProgressTimer::create(CCSprite::create("scene_welcome_lv_bar1.png"));
	m_loading_bar->setType(kCCProgressTimerTypeBar);
	m_loading_bar->setMidpoint(ccp(0,0));
	m_loading_bar->setBarChangeRate(ccp(1, 0));
	m_loading_bar->setAnchorPoint(ccp(0.5f,0.5f));
	m_loading_bar->setPosition(ccp(center_x-50, highest_y - offset_y + 2));
	m_loading_bar->setPercentage(s_experience * 100.0f / s_max);
	addChild(m_loading_bar);

	//经验条数字
	//short award = Tools::sharedTools()->getAwardFromGoldBox();
	sprintf(temp,"%d/%d",s_experience,s_max);
	pLabelBar= CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
	//pLabel->setScale(0.75f);
	pLabelBar->setAnchorPoint(ccp(0.5f,0.5f));
	pLabelBar->setPosition(ccp(center_x, highest_y - offset_y - 2));
	pLabelBar->setScale(0.6f);
	addChild(pLabelBar);

	MyNextPrizeButton* pPrizeBtn = new MyNextPrizeButton();
	//	CCSprite* pPrizeBtn = CCSprite::create("scene_welcome_info_btn_prize.png");

	addChild(pPrizeBtn,TAG_PRIZE_BTN,TAG_PRIZE_BTN);
	pPrizeBtn->setAnchorPoint(ccp(0.5f,0.5f));
	pPrizeBtn->setPosition(ccp(center_x + 115, highest_y - offset_y + 10));

	CCActionInterval* moveUp = CCMoveBy::create(1.5f,ccp(0,10));
	CCActionInterval* moveDown = moveUp->reverse();
	CCActionInterval* seq = CCSequence::create(moveUp,moveDown,NULL);
	CCActionInterval* rep = CCRepeatForever::create(seq);

	pPrizeBtn->runAction(rep);

}

void MyInfoBoard::setLabelCoin(unsigned int money){
	char temp[32];	
	sprintf(temp,"%d",money);
	pLabelCoin->setString(temp);
}


CCLabelBMFont* MyInfoBoard::addLabel(char* str,cocos2d::CCPoint point){
	CCLabelBMFont *label1 = CCLabelBMFont::create(str, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
	label1->setPosition(point);
	//label1->setScale(0.75f);
	this->addChild(label1);
	return label1;
}

CCRect MyInfoBoard::rect()
{
	CCSize s =getChildByTag(100)->getContentSize();
	return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
}

bool MyInfoBoard::containsTouchLocation(CCTouch* touch)
{
	CCPoint pp  =getChildByTag(100)->convertTouchToNodeSpaceAR(touch);
	return rect().containsPoint(pp);
}

void MyInfoBoard::setNickName(std::string nickname){
	m_label_name->setString(nickname.c_str());
}

void MyInfoBoard::setHead(short flag){
	removeChildByTag(TAG_HEAD);
	removeChildByTag(TAG_HEAD_MASK);
	char temp[32];
	sprintf(temp,"head_%d.png",flag);
	CCSprite* m_sprite= CCSprite::create(temp);
	addChild(m_sprite,TAG_HEAD,TAG_HEAD);
	m_sprite->setPosition(ccp(-160,110));

	sprintf(temp,"scene_welcome_head_mask_%d.png",Tools::sharedTools()->getVipHighest());
	CCSprite* m_sprite_mask= CCSprite::create(temp);
	addChild(m_sprite_mask,TAG_HEAD_MASK,TAG_HEAD_MASK);
	m_sprite_mask->setPosition(ccp(-160,110));

	if(flag==-1){
		CCLabelTTF* temp_tips = CCLabelTTF::create("点击可换", "", 16);
		temp_tips->setPosition(ccp(-160,110));
		this->addChild(temp_tips,TAG_HEAD);

		CCActionInterval* moveUp = CCScaleTo::create(0.25f,1.5f,1.5f);
		CCActionInterval* moveDown = CCScaleTo::create(0.25f,1.0f,1.0f);
		CCActionInterval* seq = CCSequence::create(CCDelayTime::create(1.25f),moveUp,moveDown,moveUp,moveDown,NULL);
		CCActionInterval* rep = CCRepeatForever::create(seq);
		temp_tips->runAction(seq);
	}
}


bool MyInfoBoard::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{
	CCSize s =getChildByTag(TAG_HEAD)->getContentSize();
	CCRect s2 = CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
	CCPoint pp  =getChildByTag(TAG_HEAD)->convertTouchToNodeSpaceAR(pTouches);
	if(s2.containsPoint(pp)){
		getParent()->removeChildByTag(TAG_HEAD_CHOOSE_BAR);	
		m_isHeadChooseBarShowed=true;
		MyHeadChooseBar* head_bar = new MyHeadChooseBar(this);
		getParent()->addChild(head_bar,TAG_HEAD_CHOOSE_BAR,TAG_HEAD_CHOOSE_BAR);
		head_bar->setPosition(ccp(512,288));
		head_bar->autorelease();
		return false;
	}
	return false;
}

void MyInfoBoard::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCNode::onEnter();
}

void MyInfoBoard::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCNode::onExit();
}    

void MyInfoBoard::touchDelegateRetain()
{
	this->retain();
}

void MyInfoBoard::touchDelegateRelease()
{
	this->release();
}
/************************************
*								    *
*            MyShopItem实现           *
*							        *
*************************************/

MyShopItem::MyShopItem(short flag){
	this->m_flag = flag;
	this->m_isOpened = false;
	this->m_isBuyClicked =false;

	initByFlag();
}

void MyShopItem::setMoneyBoard(MyMoneyInfoBoard* board){
	this->m_moneyBoard = board;
}

void MyShopItem::initByFlag(){
	char board_up_string[32];
	char board_down_string[32];

	sprintf(board_up_string,"scene_shop_item_%d_%d_0.png",m_flag/6,m_flag%6);
	sprintf(board_down_string,"scene_shop_item_%d_%d_1.png",m_flag/6,m_flag%6);

	CCSprite* board_up = CCSprite::create(board_up_string);                                                
	addChild(board_up,TAG_BOARD_UP,TAG_BOARD_UP);

	CCSprite* board_down = CCSprite::create(board_down_string);
	addChild(board_down,TAG_BOARD_DOWN,TAG_BOARD_DOWN);
	board_down->setPositionX(14);

	CCSprite* arrow = CCSprite::create("scene_shop_item_arrow.png");
	addChild(arrow,TAG_ARROW,TAG_ARROW);
	arrow->setPositionX(78);
	arrow->setAnchorPoint(ccp(0.5f,0.5f));

	if(m_flag<=11){
		CCSprite* btn_up = CCSprite::create("scene_shop_item_btn_buy0.png");
		addChild(btn_up,TAG_BTN_UP,TAG_BTN_UP);
		btn_up->setPosition(ccp(26,-60));

		CCSprite* btn_down = CCSprite::create("scene_shop_item_btn_buy1.png");
		addChild(btn_down,TAG_BTN_DOWN,TAG_BTN_DOWN);
		btn_down->setPosition(ccp(26,-60));
		btn_down->setVisible(false);
	}else{
		CCSprite* btn_up = CCSprite::create("scene_shop_item_btn_exchange0.png");
		addChild(btn_up,TAG_BTN_UP,TAG_BTN_UP);
		btn_up->setPosition(ccp(26,-60));

		CCSprite* btn_down = CCSprite::create("scene_shop_item_btn_exchange1.png");
		addChild(btn_down,TAG_BTN_DOWN,TAG_BTN_DOWN);
		btn_down->setPosition(ccp(26,-60));
		btn_down->setVisible(false);
	}

}

//进场
void MyShopItem::openDetail(){
	m_isOpened = true;

	CCActionInterval*  action1 = CCMoveBy::create(10/60.0f,ccp(120,0));
	//	CCActionInterval*  action2 = CCMoveBy::create(10/60.0f,ccp(80,0));
	CCActionInterval*  action3 = CCMoveBy::create(10/60.0f,ccp(120,0));
	CCActionInterval*  action4 = CCMoveBy::create(10/60.0f,ccp(120,0));
	CCActionInterval*  action5 = CCRotateBy::create(20/60.0f,180,180);

	getChildByTag(TAG_BOARD_DOWN)->runAction(action1);
	//	getChildByTag(TAG_ARROW)->runAction(action2);
	getChildByTag(TAG_BTN_UP)->runAction(action3);
	getChildByTag(TAG_BTN_DOWN)->runAction(action4);
	getChildByTag(TAG_ARROW)->setRotation(0);
	getChildByTag(TAG_ARROW)->runAction(action5);
}

void MyShopItem::closeDetail(){
	m_isOpened = false;
	CCActionInterval*  action1 = CCMoveBy::create(10/60.0f,ccp(-120,0));
	//	CCActionInterval*  action2 = CCMoveBy::create(10/60.0f,ccp(-140,0));
	CCActionInterval*  action3 = CCMoveBy::create(10/60.0f,ccp(-120,0));
	CCActionInterval*  action4 = CCMoveBy::create(10/60.0f,ccp(-120,0));
	CCActionInterval*  action5 = CCRotateBy::create(20/60.0f,180,180);

	getChildByTag(TAG_BOARD_DOWN)->runAction(action1);
	//	getChildByTag(TAG_ARROW)->runAction(action2);
	getChildByTag(TAG_BTN_UP)->runAction(action3);
	getChildByTag(TAG_BTN_DOWN)->runAction(action4);
	getChildByTag(TAG_ARROW)->setRotation(180);
	getChildByTag(TAG_ARROW)->runAction(action5);

}


bool MyShopItem::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{
	m_isBuyClicked=false;

	if(!m_isOpened){
		CCSize s =getChildByTag(TAG_BOARD_UP)->getContentSize();
		CCRect s2 = CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
		CCPoint pp  =getChildByTag(TAG_BOARD_UP)->convertTouchToNodeSpaceAR(pTouches);

		if(!s2.containsPoint(pp)){
			return false;
		}
	}else{
		//先判断是否点中购买按钮
		CCSize s =getChildByTag(TAG_BTN_UP)->getContentSize();
		CCRect s2 = CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
		CCPoint pp  =getChildByTag(TAG_BTN_UP)->convertTouchToNodeSpaceAR(pTouches);
		if(s2.containsPoint(pp)){
			//点中购买按钮
			m_isBuyClicked =true;
			getChildByTag(TAG_BTN_DOWN)->setVisible(true);
			getChildByTag(TAG_BTN_UP)->setVisible(false);
			return true;
		}else{
			s =getChildByTag(TAG_BOARD_UP)->getContentSize();
			s2 = CCRectMake(-s.width / 2, -s.height / 2, s.width+140, s.height);
			pp  =getChildByTag(TAG_BOARD_UP)->convertTouchToNodeSpaceAR(pTouches);
			if(!s2.containsPoint(pp)){
				//没有点中close区域
				return false;
			}
		}
	}
	return true;
}

void MyShopItem::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
	if(!m_isOpened){
		openDetail();
	}else{
		if(m_isBuyClicked){
			getChildByTag(TAG_BTN_DOWN)->setVisible(false);
			getChildByTag(TAG_BTN_UP)->setVisible(true);
			buy();
		}else{
			closeDetail();
		}	
	}

} 

void MyShopItem::buy(){
//	Tools* tools = Tools::sharedTools();
//	bool isSuccess = false;
//	if(m_flag / 6 ==0){
//		//充值专区一 [0,5]
//        IapManager::sharedIapManager()->pay(m_flag % 6);		
//	}else if(m_flag / 6 ==1){
//		//充值专区二 [6,11]
//		 IapManager::sharedIapManager()->pay(6+ m_flag % 6);
//	}else if(m_flag / 6 ==2){
//		//兑换专区[12,17]
//		switch (m_flag % 6){
//		case 0:
//			if(tools->getIngot()>=1000){
//				tools->updateIngot(-1000);
//				tools->updateCoin(1000*10000);
//				isSuccess=true;
//			}
//			break;
//		case 1:
//			if(tools->getIngot()>=100){
//				tools->updateIngot(-100);
//				tools->updateCoin(100*10000);
//				isSuccess=true;
//			}
//			break;
//		case 2:
//			if(tools->getIngot()>=10){
//				tools->updateIngot(-10);
//				tools->updateCoin(10*10000);
//				isSuccess=true;
//			}
//			break;
//		case 3:
//			if(tools->getIngot()>=1){
//				tools->updateIngot(-1);
//				tools->updateCoin(1*10000);
//				isSuccess=true;
//			}
//			break;
//		case 4:
//			if(tools->getIngot()>=5){
//				tools->updateIngot(-5);
//				tools->addVipAmount(1,20);
//				isSuccess=true;
//			}
//			break;
//		case 5:
//			if(tools->getCoin()>=20000){
//				tools->updateCoin(-20000);
//				tools->addVipAmount(0,10);
//				isSuccess=true;
//			}
//			break;
//		}
//        showToast(isSuccess);
//	}
//	
//	MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioShopSuccess);
//	m_moneyBoard -> updateLabelCoin();
//	m_moneyBoard -> updateLabelIngot();
}

void MyShopItem::showToast(bool isSuccess){
	removeToast(0);
	CCSprite* toast;
	if(isSuccess){
		toast = CCSprite::create("scene_shop_toast_success.png");	
	}else{
		toast = CCSprite::create("scene_shop_toast_fail.png");
	}
	addChild(toast,TAG_TOAST,TAG_TOAST);
	toast->setPositionX(50);

	CCActionInterval*  action1 = CCFadeIn::create(0.75f);
	CCDelayTime *delay = CCDelayTime::create(1.75f);
	CCActionInterval*  action2 = CCFadeOut::create(0.5f);
	CCSequence*  seq = CCSequence::create(action1,delay,action2, NULL);

	toast->runAction(seq);

	scheduleOnce(schedule_selector(MyShopItem::removeToast),3.0f);
}

void MyShopItem::removeToast(float dt){
	removeChildByTag(TAG_TOAST);
}

void MyShopItem::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCNode::onEnter();
}

void MyShopItem::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCNode::onExit();
}   

void MyShopItem::touchDelegateRetain()
{
	this->retain();
}

void MyShopItem::touchDelegateRelease()
{
	this->release();
}



/************************************
*								    *
*            MyCalcBoard实现        *
*							        *
*************************************/

MyCalcBoard::MyCalcBoard(bool isCoinRoom){
	m_isCoinRoom = isCoinRoom;
	initSelf();
}

void MyCalcBoard::initSelf(){
	CCSprite* m_sprite_bg = CCSprite::create("scene_game_calc_bg.png");
	addChild(m_sprite_bg);

	//测试代码
	/*
	setInfo(0,"asdff",1,2222,true,1000);
	setInfo(1,"asdfasd",2,232,false,222);
	setInfo(2,"asdf",3,3212,false,123121);
	setInfo(3,"asdfas",4,122,true,1232);
	setInfo(4,"adsfadsf",10,22222,true,123456789);
	setBoss(0);
	setBoss(1);
	setBoss(2);
	setBoss(3);
	setBoss(4);
	short exps[4] = {5,5,5,5};
	setExperience(exps);*/
}

void MyCalcBoard::setExperience(short* exps){
	char temp[32];
	for(short i=0;i<4;i++){
		sprintf(temp,"%d",exps[i]);
		addLabel(0,temp,ccp(-115+95*i,-125));	
	}
}

CCLabelBMFont* MyCalcBoard::addLabel(short font_kind,char* str,cocos2d::CCPoint point){
	CCLabelBMFont *label1;
	switch (font_kind)
	{
	case 0 :label1 = CCLabelBMFont::create(str, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentCenter, CCPointZero);break;
	case 1 :label1 = CCLabelBMFont::create(str, "font_o.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentCenter, CCPointZero);break;
	case 2 :label1 = CCLabelBMFont::create(str, "font_g.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentCenter, CCPointZero);break;
	}
	label1->setPosition(point);
	label1->setScale(0.75f);
	this->addChild(label1);
	return label1;
}

void MyCalcBoard::setBoss(short flag){
	float y;
	float x;
	switch (flag)
	{
	case 0:
		x=-65;
		y=-25;
		break;
	case 1:
		x=-174;
		y=52;
		break;
	case 2:
		x=-174;
		y=114;
		break;
	case 3:
		x=18;
		y=114;
		break;
	case 4:
		x=18;
		y=52;
		break;
	}

	CCSprite* m_sprite = CCSprite::create("scene_game_boss.png");
	addChild(m_sprite);
	m_sprite->setScale(0.6f);
	m_sprite->setPosition(ccp(x,y));
}

void MyCalcBoard::setInfo(short flag,const char* name,short ox,unsigned int money,bool win,unsigned int result){
	float x1,x2,x3;
	float y1,y2;
	switch (flag)
	{
	case 0:
		//name
		x1=30;
		//ox
		x2=125;	
		//result
		x3=125;
		y1=-80;
		y2=-25;
		break;
	case 1:
		//name
		x1=-85;
		//ox
		x2=-25;	
		//result
		x3=-100;
		y1=20;
		y2=55;
		break;
	case 2:
		//name
		x1=-85;
		//ox
		x2=-25;	
		//result
		x3=-100;
		y1=80;
		y2=115;
		break;
	case 3:
		//name
		x1=115;
		//ox
		x2=165;	
		//result
		x3=100;
		y1=80;
		y2=115;
		break;
	case 4:
		//name
		x1=115;
		//ox
		x2=165;	
		//result
		x3=100;
		y1=20;
		y2=55;
		break;
	}

	//昵称
	CCLabelTTF* m_label_name = CCLabelTTF::create(name, "Arial", 22);
	m_label_name->setColor(ccWHITE);
	m_label_name->setPosition(ccp(x1-25,y2-2));
	this->addChild(m_label_name);
	m_label_name->setAnchorPoint(ccp(0.5f,0.5f));

	/*  //押注金额就暂时不要了
	if(money>0){
	sprintf(temp,"%d",money);	
	addLabel(0,temp,ccp(x3,y));
	}*/

	//本局输赢
	char temp[32]; //注意不要越界
	CCLabelBMFont* label_change_temp;
	if(win){
		sprintf(temp,"+%d",result);	
		label_change_temp=addLabel(1,temp,ccp(x3,y1));
	}else{
		sprintf(temp,"-%d",result);	
		label_change_temp=addLabel(2,temp,ccp(x3,y1));
	}

	//自己的特殊处理
	if(flag==0){
		Tools* tools =Tools::sharedTools();
		m_current_change_times=0;
		m_label_money_change =label_change_temp;
		label_change_temp=NULL;
		short controler=win?1:-1; //赢钱为1，输钱为-1

		m_money_change = result * controler;
		if(m_isCoinRoom){
			m_money_total=tools->getCoin(); //还原之前数字,银币场
		}else{
			m_money_total=tools->getIngot(); //还原之前数字,银锭场
		}


		char temp[32];
		sprintf(temp,"head_%d.png",tools->getHead());
		CCSprite* m_sprite= CCSprite::create(temp);
		addChild(m_sprite);
		m_sprite->setPosition(ccp(-134,-49));

		sprintf(temp,"scene_welcome_head_mask_%d.png",tools->getVipHighest());
		CCSprite* m_sprite_mask= CCSprite::create(temp);
		addChild(m_sprite_mask);
		m_sprite_mask->setPosition(ccp(-134,-49));

		sprintf(temp,"%d",m_money_total);	
		m_label_money_total = addLabel(0,temp,ccp(x1-25,y1));

		schedule(schedule_selector(MyCalcBoard::changeLabel),0.03f); 

		if(win){
			cocos2d::CCSprite *p_ox = cocos2d::CCSprite::create("scene_game_calc_mask.png");	
			addChild(p_ox);
		}
	}

	//牛数
	sprintf(temp,"ox_%d.png",ox);	
	cocos2d::CCSprite *p_ox = cocos2d::CCSprite::create(temp);	
	addChild(p_ox);
	p_ox->setPosition(ccp(x2,y2-3));
	p_ox->setScale(0.5f);
	//	p_ox->setRotation(-20);

}	

void MyCalcBoard::changeLabel(float dt){
	m_current_change_times++;

	char temp[32];

	if(m_current_change_times<=40){
		int total_change = 0 + m_money_change /40.0f * m_current_change_times;
		if(total_change>=0){
			sprintf(temp,"+%d",total_change);
		}else{
			sprintf(temp,"%d",total_change);
		}
		m_label_money_change->setString(temp);
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioAddCoin);
	}

	if(m_current_change_times>=60){
		int total = m_money_total + (m_current_change_times-60) /40.0f * m_money_change;
		sprintf(temp,"%d",total);
		m_label_money_total->setString(temp);
		MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioAddCoin);
	}

	if(m_current_change_times==100){
		unscheduleAllSelectors();
	}
}

/************************************
*								    *
*            MyHeadChooseBar实现    *
*							        *
*************************************/
MyHeadChooseBar::MyHeadChooseBar(MyInfoBoard* info){
	m_info =info;
	m_flag_focused=-1;

	CCSprite* pSprite = CCSprite::create("scene_welcome_head_choose_bg.png");
	addChild(pSprite);

	showAll();
}

void MyHeadChooseBar::showAll(){
	char temp[32];

	for(short i =3 ; i >=0; i--){
		sprintf(temp,"head_%d.png",i);
		CCSprite* pSprite = CCSprite::create(temp);
		addChild(pSprite,TAG_HEAD+i,TAG_HEAD+i);
		pSprite->setPosition(ccp(-700-64*2*i,58));

		CCSprite* pSprite2 = CCSprite::create("scene_welcome_head_mask_-1.png");
		addChild(pSprite2,TAG_MASK+i,TAG_MASK+i);
		pSprite2->setPosition(ccp(-700-64*2*i+2,58));

		CCActionInterval*  action1 = CCMoveTo::create(0.12f,ccp(-512+64+(i*2*64),58));
		CCActionInterval*  action2 = CCMoveTo::create(0.12f,ccp(-512+64+(i*2*64)+2,58));

		CCActionInterval*  action3 = CCMoveBy::create(0.03,ccp(30,0));
		CCActionInterval*  action4 = CCMoveBy::create(0.06,ccp(-30,0));

		CCActionInterval*  action5 = CCMoveBy::create(0.03,ccp(30,0));
		CCActionInterval*  action6 = CCMoveBy::create(0.06,ccp(-30,0));

		CCActionInterval*  seq1 = CCSequence::create(CCDelayTime::create(0.12f*(3-i)),action1,action3,action4,NULL);
		CCActionInterval*  seq2 = CCSequence::create(CCDelayTime::create(0.12f*(3-i)),action2,action5,action6,NULL);

		pSprite->runAction(seq1);
		pSprite2->runAction(seq2);
	}

	for(short i =3 ; i >=0; i--){
		sprintf(temp,"head_%d.png",i+8);
		CCSprite* pSprite = CCSprite::create(temp);
		addChild(pSprite,TAG_HEAD+i+8,TAG_HEAD+i+8);
		pSprite->setPosition(ccp(-700-64*2*i,-58));

		CCSprite* pSprite2 = CCSprite::create("scene_welcome_head_mask_-1.png");
		addChild(pSprite2,TAG_MASK+i+8,TAG_MASK+i+8);
		pSprite2->setPosition(ccp(-700-64*2*i+2,-58));

		CCActionInterval*  action1 = CCMoveTo::create(0.12f,ccp(-512+64+(i*2*64),-58));
		CCActionInterval*  action2 = CCMoveTo::create(0.12f,ccp(-512+64+(i*2*64)+2,-58));

		CCActionInterval*  action3 = CCMoveBy::create(0.03,ccp(30,0));
		CCActionInterval*  action4 = CCMoveBy::create(0.06,ccp(-30,0));

		CCActionInterval*  action5 = CCMoveBy::create(0.03,ccp(30,0));
		CCActionInterval*  action6 = CCMoveBy::create(0.06,ccp(-30,0));

		CCActionInterval*  seq1 = CCSequence::create(CCDelayTime::create(0.12f*(3-i)),action1,action3,action4,NULL);
		CCActionInterval*  seq2 = CCSequence::create(CCDelayTime::create(0.12f*(3-i)),action2,action5,action6,NULL);

		pSprite->runAction(seq1);
		pSprite2->runAction(seq2);
	}

	for(short i =0 ; i <4; i++){
		sprintf(temp,"head_%d.png",i+4);
		CCSprite* pSprite = CCSprite::create(temp);
		addChild(pSprite,TAG_HEAD+i+4,TAG_HEAD+i+4);
		pSprite->setPosition(ccp(700+64*2*i,58));

		CCSprite* pSprite2 = CCSprite::create("scene_welcome_head_mask_-1.png");
		addChild(pSprite2,TAG_MASK+i+4,TAG_MASK+i+4);
		pSprite2->setPosition(ccp(700+64*2*i+2,58));

		CCActionInterval*  action1 = CCMoveTo::create(0.12f,ccp(-512+64+(4+i)*2*64,58));
		CCActionInterval*  action2 = CCMoveTo::create(0.12f,ccp(-512+64+(4+i)*2*64+2,58));

		CCActionInterval*  action3 = CCMoveBy::create(0.03,ccp(-30,0));
		CCActionInterval*  action4 = CCMoveBy::create(0.06,ccp(30,0));

		CCActionInterval*  action5 = CCMoveBy::create(0.03,ccp(-30,0));
		CCActionInterval*  action6 = CCMoveBy::create(0.06,ccp(30,0));

		CCActionInterval*  seq1 = CCSequence::create(CCDelayTime::create(0.12f*i),action1,action3,action4,NULL);
		CCActionInterval*  seq2 = CCSequence::create(CCDelayTime::create(0.12f*i),action2,action5,action6,NULL);

		pSprite->runAction(seq1);
		pSprite2->runAction(seq2);
	}

	for(short i =0 ; i <4; i++){
		sprintf(temp,"head_%d.png",i+12);
		CCSprite* pSprite = CCSprite::create(temp);
		addChild(pSprite,TAG_HEAD+i+12,TAG_HEAD+i+12);
		pSprite->setPosition(ccp(700+64*2*i,-58));

		CCSprite* pSprite2 = CCSprite::create("scene_welcome_head_mask_-1.png");
		addChild(pSprite2,TAG_MASK+i+12,TAG_MASK+i+12);
		pSprite2->setPosition(ccp(700+64*2*i+2,-58));

		CCActionInterval*  action1 = CCMoveTo::create(0.12f,ccp(-512+64+(4+i)*2*64,-58));
		CCActionInterval*  action2 = CCMoveTo::create(0.12f,ccp(-512+64+(4+i)*2*64+2,-58));

		CCActionInterval*  action3 = CCMoveBy::create(0.03,ccp(-30,0));
		CCActionInterval*  action4 = CCMoveBy::create(0.06,ccp(30,0));

		CCActionInterval*  action5 = CCMoveBy::create(0.03,ccp(-30,0));
		CCActionInterval*  action6 = CCMoveBy::create(0.06,ccp(30,0));

		CCActionInterval*  seq1 = CCSequence::create(CCDelayTime::create(0.12f*i),action1,action3,action4,NULL);
		CCActionInterval*  seq2 = CCSequence::create(CCDelayTime::create(0.12f*i),action2,action5,action6,NULL);

		pSprite->runAction(seq1);
		pSprite2->runAction(seq2);
	}
}

bool MyHeadChooseBar::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{
	if(m_flag_focused==-1){
		CCSize s =getChildByTag(TAG_HEAD)->getContentSize();
		CCRect s2 = CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);

		for(short i = 0;i<16;i++){
			CCPoint pp  =getChildByTag(TAG_HEAD+i)->convertTouchToNodeSpaceAR(pTouches);
			if(s2.containsPoint(pp)){
				m_flag_focused = i;

				Tools::sharedTools()->setHead(m_flag_focused);
				m_info->setHead(m_flag_focused);
				m_info->m_isHeadChooseBarShowed=false;
				//	removeAllChildrenWithCleanup(true);
				this->setVisible(false);
				return true;
			}
		}	
	}	
	return false;
}

void MyHeadChooseBar::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this,0, true);
	CCNode::onEnter();
}

void MyHeadChooseBar::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCNode::onExit();
}    

void MyHeadChooseBar::touchDelegateRetain()
{
	this->retain();
}

void MyHeadChooseBar::touchDelegateRelease()
{
	this->release();
}


/************************************
*								    *
*            MyToast实现           *
*							        *
*************************************/
MyToast::MyToast(Toast tagToast)
{
	CCSprite* toast;
	float delayTime = 1.0f;

	switch(tagToast){
	case eToastAntiHacker:
		toast = CCSprite::create("toast0.png");
		break;
	case eToastMoneyNotEnough:
		toast = CCSprite::create("toast1.png");
		break;
	case eToastNeedVip:
		toast = CCSprite::create("toast3.png");
		break;
	case eToastCantExitGaming:
		toast = CCSprite::create("toast2.png");
		break;
	case eToastNoneRobBoss:
		toast = CCSprite::create("scene_game_toast_unrob.png");
		break;	
	case eToastOxError:
		toast = CCSprite::create("toast8.png");
		break;
	}
	toast->setOpacity(0);
	addChild(toast,TAG_TOAST,TAG_TOAST);

	CCActionInterval*  action1 = CCFadeIn::create(0.5f);
	CCDelayTime *delay = CCDelayTime::create(delayTime);
	CCActionInterval*  action2 = CCFadeOut::create(0.5f);
	CCSequence*  seq = CCSequence::create(action1,delay,action2, NULL);

	toast->runAction(seq);

	//	schedule(schedule_selector(MyClock2::clockCallback),1.0f);
	scheduleOnce(schedule_selector(MyToast::removeToast),4.0f);
}

void MyToast::removeToast(float dt)
{	
	removeAllChildren();
}


/************************************
*									*
*            MyGoldBoxByAmount实现          *
*									*
*************************************/
void MyGoldBoxByAmount::showShiningBox(){
	if(Tools::sharedTools()->couldCreateGoldBoxOfAmount()){
		m_isShiningBoxShowed=true;

		removeChildByTag(TAG_BOX);

		CCSprite* pSprite2 = CCSprite::create("goldbox_spec0.png");
		addChild(pSprite2,TAG_SPEC0,TAG_SPEC0);

		CCSprite* pSprite3 = CCSprite::create("goldbox_spec1.png");
		addChild(pSprite3,TAG_SPEC1,TAG_SPEC1);

		CCSprite* pSprite = CCSprite::create("goldbox_open.png");
		addChild(pSprite,TAG_BOX,TAG_BOX);

		CCActionInterval*  act1 = CCRotateTo::create(0.5f, 360);
		CCActionInterval*  act2 = CCRotateTo::create(1, 0);
		CCActionInterval*  seq = CCSequence::create(act1, act2, NULL);

		CCAction*  rep1 = CCRepeatForever::create(act1);
		CCActionInterval*  rep2 = CCRepeat::create((CCFiniteTimeAction*)(seq->copy()->autorelease()), 10);

		CCRepeatForever *repeat = CCRepeatForever::create( CCRotateBy::create(2.0f, 360) );
		CCRepeatForever *repeat2 = CCRepeatForever::create( CCRotateBy::create(2.0f, -360) );

		pSprite2->runAction(repeat2);
		pSprite3->runAction(repeat);

	}
}

void MyGoldBoxByAmount::openBox(){
	m_isShiningBoxShowed=false;
	removeChildByTag(TAG_BOX);
	removeChildByTag(TAG_SPEC0);
	removeChildByTag(TAG_SPEC1);
	CCSprite* pSprite2 = CCSprite::create("goldbox_close.png");
	addChild(pSprite2,TAG_BOX,TAG_BOX);

	if(!Tools::sharedTools()->couldCreateGoldBoxOfAmount()){
		removeAllChildren();
	}	
}

void MyGoldBoxByAmount::updateData(){
	Tools* tools =Tools::sharedTools();
	tools->updateGoldBoxOfAmount();
	updateLabel();
	if(!m_isShiningBoxShowed&&tools->couldOpenGoldBoxOfAmount()){
		if(tools->couldCreateGoldBoxOfAmount()){
			showShiningBox();
		}
	}
}

void MyGoldBoxByAmount::updateLabel(){
	short seconds = Tools::sharedTools()->getCurrentAmountOfGoldBox();
	char temp[32];
	sprintf(temp,"%d",seconds);

	pLabel->setString(temp);
}

MyGoldBoxByAmount::~MyGoldBoxByAmount(){

}

MyGoldBoxByAmount::MyGoldBoxByAmount()
{
	m_isUserBoardSetted = false;
	m_isInfoBoardSetted = false;
	m_isMoneyBoardSetted = false;
	m_isShiningBoxShowed=false;
	if(Tools::sharedTools()->couldCreateGoldBoxOfAmount()){

		CCSprite* box_bg1 = CCSprite::create("goldbox_bg.png");
		addChild(box_bg1);
		box_bg1->setPosition(ccp(5,-10));

		if(Tools::sharedTools()->couldOpenGoldBoxOfAmount()){
			showShiningBox();
		}else{
			CCSprite* pSprite2 = CCSprite::create("goldbox_close.png");
			addChild(pSprite2,TAG_BOX,TAG_BOX);
		}
		short seconds = Tools::sharedTools()->getCurrentAmountOfGoldBox();
		char temp[32];
		sprintf(temp,"%d",seconds);

		pLabel= CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
		addChild(pLabel);
		pLabel->setScale(0.6f);
		pLabel->setPositionY(-35);
	}
}

void MyGoldBoxByAmount::showToast(bool isSuccess){
	removeChildByTag(TAG_TOAST);
	removeChildByTag(TAG_TOAST_LABEL);
	removeChildByTag(TAG_TOAST_LABEL2);

	if(isSuccess){
		CCSprite* pSprite = CCSprite::create("toast6.png");
		pSprite->setPosition(ccp(200,-10));
		addChild(pSprite,TAG_TOAST,TAG_TOAST);
		CCActionInterval* action1 = CCMoveBy::create(0.15f,ccp(-370,0));
		CCActionInterval* action3 = CCFadeOut::create(2.0f);
		pSprite->runAction(action1);
		pSprite->runAction(action3);
	}else{
		CCSprite* pSprite = CCSprite::create("toast4.png");
		pSprite->setPosition(ccp(200,-10));
		addChild(pSprite,TAG_TOAST,TAG_TOAST);
		CCActionInterval* action1 = CCMoveBy::create(0.15f,ccp(-410,0));
		CCActionInterval* action3 = CCFadeOut::create(2.0f);
		pSprite->runAction(action1);
		pSprite->runAction(action3);

		short amount = Tools::sharedTools()->getCurrentAmountOfGoldBox();
		char temp[32];
		sprintf(temp,"%d",amount);
		CCLabelBMFont* pLabel= CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
		pLabel->setPosition(ccp(120,-14));
		addChild(pLabel,TAG_TOAST_LABEL2,TAG_TOAST_LABEL2);

		CCActionInterval* action2 = CCMoveBy::create(0.15f,ccp(-400,0));
		CCActionInterval* action4 = CCFadeOut::create(2.0f);
		pLabel->runAction(action2);
		pLabel->runAction(action4);

	}

	short award = Tools::sharedTools()->getAwardFromGoldBoxByAmount();
	char temp[32];
	sprintf(temp,"%d",award);
	CCLabelBMFont* pLabel= CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
	//pLabel->setScale(0.75f);
	pLabel->setPosition(ccp(320,-14));
	addChild(pLabel,TAG_TOAST_LABEL,TAG_TOAST_LABEL);

	CCActionInterval* action2 = CCMoveBy::create(0.15f,ccp(-400,0));
	CCActionInterval* action4 = CCFadeOut::create(2.0f);
	pLabel->runAction(action2);
	pLabel->runAction(action4);
}

void MyGoldBoxByAmount::setUserBoard(MyUserBoard* board){
	this->m_isUserBoardSetted = true;
	this->m_userBoard=board;
}

void MyGoldBoxByAmount::setInfoBoard(MyInfoBoard* board){
	this->m_isInfoBoardSetted = true;
	this->m_infoBoard=board;
}

void MyGoldBoxByAmount::setMoneyBoard(MyMoneyInfoBoard* board){
	this->m_isMoneyBoardSetted = true;
	this->m_moneyBoard=board;
}

bool MyGoldBoxByAmount::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{
	if(!getChildByTag(TAG_BOX)==NULL){
		Tools* tools = Tools::sharedTools();

		CCSize s =getChildByTag(TAG_BOX)->getContentSize();
		CCRect s2 = CCRectMake(-s.width / 4, -s.height / 4, s.width/2, s.height/2);
		CCPoint pp  =getChildByTag(TAG_BOX)->convertTouchToNodeSpaceAR(pTouches);
		if(s2.containsPoint(pp)){
			if(tools->couldOpenGoldBoxOfAmount()){
				MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioShowChipsAll);
				showToast(true);	
				openBox();		
				tools->openGoldBoxofAmount();	
				updateLabel();
				if(m_isUserBoardSetted){
					m_userBoard->updateMoney(Tools::sharedTools()->getCoin());
				}
				if(m_isInfoBoardSetted){
					m_infoBoard->setLabelCoin(Tools::sharedTools()->getCoin());
				}	
				if(m_isMoneyBoardSetted){
					m_moneyBoard->updateLabelCoin();
				}	
			}else{
				if(tools->couldCreateGoldBoxOfAmount()){
					showToast(false);
				}else{
					removeAllChildren();
				}		
			}
		}
	}
	return false;
}


void MyGoldBoxByAmount::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this,0, true);
	CCNode::onEnter();
}

void MyGoldBoxByAmount::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCNode::onExit();
}    

void MyGoldBoxByAmount::touchDelegateRetain()
{
	this->retain();
}

void MyGoldBoxByAmount::touchDelegateRelease()
{
	this->release();
}


/************************************
*									*
*            MyGoldBox实现          *
*									*
*************************************/
void MyGoldBox::showShiningBox(){
	if(Tools::sharedTools()->couldCreateGoldBox()){
		m_isShiningBoxShowed=true;

		removeChildByTag(TAG_BOX);

		CCSprite* pSprite2 = CCSprite::create("goldbox_spec0.png");
		addChild(pSprite2,TAG_SPEC0,TAG_SPEC0);

		CCSprite* pSprite3 = CCSprite::create("goldbox_spec1.png");
		addChild(pSprite3,TAG_SPEC1,TAG_SPEC1);

		CCSprite* pSprite = CCSprite::create("goldbox_open.png");
		addChild(pSprite,TAG_BOX,TAG_BOX);

		CCActionInterval*  act1 = CCRotateTo::create(0.5f, 360);
		CCActionInterval*  act2 = CCRotateTo::create(1, 0);
		CCActionInterval*  seq = CCSequence::create(act1, act2, NULL);

		CCAction*  rep1 = CCRepeatForever::create(act1);
		CCActionInterval*  rep2 = CCRepeat::create((CCFiniteTimeAction*)(seq->copy()->autorelease()), 10);

		CCRepeatForever *repeat = CCRepeatForever::create( CCRotateBy::create(2.0f, 360) );
		CCRepeatForever *repeat2 = CCRepeatForever::create( CCRotateBy::create(2.0f, -360) );

		pSprite2->runAction(repeat2);
		pSprite3->runAction(repeat);

		unschedule(schedule_selector(MyGoldBox::updateCallBack));
	}
}

void MyGoldBox::openBox(){
	m_isShiningBoxShowed=false;
	removeChildByTag(TAG_BOX);
	removeChildByTag(TAG_SPEC0);
	removeChildByTag(TAG_SPEC1);
	CCSprite* pSprite2 = CCSprite::create("goldbox_close.png");
	addChild(pSprite2,TAG_BOX,TAG_BOX);

	schedule(schedule_selector(MyGoldBox::updateCallBack),1.0f);

	if(!Tools::sharedTools()->couldCreateGoldBox()){
		unscheduleAllSelectors();
		removeAllChildren();
	}
}

void MyGoldBox::updateCallBack(float dt){
	Tools* tools =Tools::sharedTools();
	tools->updateGoldBox();
	updateLabel();
	if(!m_isShiningBoxShowed&&tools->couldOpenGoldBox()){
		if(tools->couldCreateGoldBox()){
			showShiningBox();
		}
	}
}

void MyGoldBox::updateLabel(){
	short seconds = Tools::sharedTools()->getSecondsOfGoldBox();
	char temp[32];
	sprintf(temp,"%d:%d",seconds/60,seconds%60);

	pLabel->setString(temp);
}

MyGoldBox::~MyGoldBox(){

}

MyGoldBox::MyGoldBox()
{
	if(Tools::sharedTools()->couldCreateGoldBox()){

		m_isUserBoardSetted = false;
		m_isInfoBoardSetted = false;
		m_isMoneyBoardSetted = false;

		CCSprite* box_bg1 = CCSprite::create("goldbox_bg.png");
		addChild(box_bg1);
		box_bg1->setPosition(ccp(5,-10));
		CCSprite* pSprite2 = CCSprite::create("goldbox_close.png");
		addChild(pSprite2,TAG_BOX,TAG_BOX);

		short seconds = Tools::sharedTools()->getSecondsOfGoldBox();
		char temp[32];
		sprintf(temp,"%d:%d",seconds/60,seconds%60);

		pLabel= CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
		addChild(pLabel);
		pLabel->setScale(0.6f);
		pLabel->setPositionY(-35);

		schedule(schedule_selector(MyGoldBox::updateCallBack),1.0f);

		m_isShiningBoxShowed=false;
	}
}

void MyGoldBox::showToast(bool isSuccess){
	removeChildByTag(TAG_TOAST);
	removeChildByTag(TAG_TOAST_LABEL);

	if(isSuccess){
		CCSprite* pSprite = CCSprite::create("toast6.png");
		pSprite->setPosition(ccp(200,-10));
		addChild(pSprite,TAG_TOAST,TAG_TOAST);
		CCActionInterval* action1 = CCMoveBy::create(0.15f,ccp(-370,0));
		CCActionInterval* action3 = CCFadeOut::create(2.0f);
		pSprite->runAction(action1);
		pSprite->runAction(action3);
	}else{
		CCSprite* pSprite = CCSprite::create("toast7.png");
		pSprite->setPosition(ccp(200,-10));
		addChild(pSprite,TAG_TOAST,TAG_TOAST);
		CCActionInterval* action1 = CCMoveBy::create(0.15f,ccp(-370,0));
		CCActionInterval* action3 = CCFadeOut::create(2.0f);
		pSprite->runAction(action1);
		pSprite->runAction(action3);
	}

	short award = Tools::sharedTools()->getAwardFromGoldBox();
	char temp[32];
	sprintf(temp,"%d",award);
	CCLabelBMFont* pLabel= CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
	//pLabel->setScale(0.75f);
	pLabel->setPosition(ccp(265,-4));
	addChild(pLabel,TAG_TOAST_LABEL,TAG_TOAST_LABEL);

	CCActionInterval* action2 = CCMoveBy::create(0.15f,ccp(-350,-10));
	CCActionInterval* action4 = CCFadeOut::create(2.0f);
	pLabel->runAction(action2);
	pLabel->runAction(action4);
}

void MyGoldBox::setUserBoard(MyUserBoard* board){
	this->m_isUserBoardSetted = true;
	this->m_userBoard=board;
}

void MyGoldBox::setInfoBoard(MyInfoBoard* board){
	this->m_isInfoBoardSetted = true;
	this->m_infoBoard=board;
}

void MyGoldBox::setMoneyBoard(MyMoneyInfoBoard* board){
	this->m_isMoneyBoardSetted = true;
	this->m_moneyBoard=board;
}

bool MyGoldBox::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{
	if(!getChildByTag(TAG_BOX)==NULL){


		Tools* tools = Tools::sharedTools();

		CCSize s =getChildByTag(TAG_BOX)->getContentSize();
		CCRect s2 = CCRectMake(-s.width / 4, -s.height / 4, s.width/2, s.height/2);
		CCPoint pp  =getChildByTag(TAG_BOX)->convertTouchToNodeSpaceAR(pTouches);
		if(s2.containsPoint(pp)){
			if(tools->couldOpenGoldBox()){
				MyAudioEngine::sharedMyAudioEngine()->playEffect(eAudioShowChipsAll);
				showToast(true);	
				openBox();		
				tools->openGoldBox();			
				if(m_isUserBoardSetted){
					m_userBoard->updateMoney(Tools::sharedTools()->getCoin());
				}
				if(m_isInfoBoardSetted){
					m_infoBoard->setLabelCoin(Tools::sharedTools()->getCoin());
				}	
				if(m_isMoneyBoardSetted){
					m_moneyBoard->updateLabelCoin();
				}	
			}else{
				if(tools->couldCreateGoldBox()){
					showToast(false);
				}		
			}
		}
	}
	return false;
}


void MyGoldBox::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this,0, true);
	CCNode::onEnter();
}

void MyGoldBox::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCNode::onExit();
}    

void MyGoldBox::touchDelegateRetain()
{
	this->retain();
}

void MyGoldBox::touchDelegateRelease()
{
	this->release();
}


/************************************
*								    *
*            MyMoneyInfoBoard实现   *
*							        *
*************************************/

MyMoneyInfoBoard::MyMoneyInfoBoard(){
	CCSprite* bg = CCSprite::create("money_info_board.png");
	addChild(bg);

	pLabelCoin = CCLabelBMFont::create("", "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
	pLabelCoin->setPosition(ccp(20,12));
	this->addChild(pLabelCoin);

	pLabelIngot = CCLabelBMFont::create("", "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
	pLabelIngot->setPosition(ccp(0,-24));
	this->addChild(pLabelIngot);

	updateLabelCoin();
	updateLabelIngot();
}

void MyMoneyInfoBoard::updateLabelCoin(){
	unsigned money = Tools::sharedTools()->getCoin();
	char temp[32];
	sprintf(temp,"%d",money);
	pLabelCoin->setString(temp);
}

void MyMoneyInfoBoard::updateLabelIngot(){
	unsigned money = Tools::sharedTools()->getIngot();
	char temp[32];
	sprintf(temp,"%d",money);
	pLabelIngot->setString(temp);
}

/****************************************************************************************************
*								   								   								    *
*                                    MyBaseNodeTouchToRemove		                                *
*							        							        							*
*****************************************************************************************************/

MyBaseNodeTouchToRemove::MyBaseNodeTouchToRemove(){
	//	CCSprite* bg = CCSprite::create("scene_game_toast_upgrade.png");
	//	addChild(bg,TAG_BASE,TAG_BASE);
}

MyBaseNodeTouchToRemove::MyBaseNodeTouchToRemove(char* picName){
	CCSprite* bg = CCSprite::create(picName);
	addChild(bg,TAG_BASE,TAG_BASE);
}

bool MyBaseNodeTouchToRemove::ccTouchBegan(CCTouch *pTouches,CCEvent *pEvent)
{
	if(!getChildByTag(TAG_BASE)==NULL){
		CCSize s =getChildByTag(TAG_BASE)->getContentSize();
		CCRect s2 = CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
		CCPoint pp  =getChildByTag(TAG_BASE)->convertTouchToNodeSpaceAR(pTouches);
		if(s2.containsPoint(pp)){
			removeAllChildren();
		}
	}
	return false;
}


void MyBaseNodeTouchToRemove::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this,-10, true);
	CCNode::onEnter();
}

void MyBaseNodeTouchToRemove::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCNode::onExit();
}    

void MyBaseNodeTouchToRemove::touchDelegateRetain()
{
	this->retain();
}

void MyBaseNodeTouchToRemove::touchDelegateRelease()
{
	this->release();
}

/****************************************************************************************************
*								   								   								    *
*                                    MyUpgradeBoard		                                *
*							        							        							*
*****************************************************************************************************/

MyUpgradeBoard::MyUpgradeBoard(unsigned int* current ,unsigned int* next){

	CCSprite* pSprite2 = CCSprite::create("goldbox_spec0.png");
	addChild(pSprite2);
	pSprite2->setScale(6.0f);

	CCSprite* pSprite3 = CCSprite::create("goldbox_spec1.png");
	addChild(pSprite3);
	pSprite3->setScale(6.0f);

	CCActionInterval*  act1 = CCRotateTo::create(0.5f, 360);
	CCActionInterval*  act2 = CCRotateTo::create(1, 0);
	CCActionInterval*  seq = CCSequence::create(act1, act2, NULL);

	CCAction*  rep1 = CCRepeatForever::create(act1);
	CCActionInterval*  rep2 = CCRepeat::create((CCFiniteTimeAction*)(seq->copy()->autorelease()), 10);

	CCRepeatForever *repeat = CCRepeatForever::create( CCRotateBy::create(2.0f, 360) );
	CCRepeatForever *repeat2 = CCRepeatForever::create( CCRotateBy::create(2.0f, -360) );

	pSprite2->runAction(repeat2);
	pSprite3->runAction(repeat);


	CCSprite* bg = CCSprite::create("scene_game_toast_upgrade.png");
	addChild(bg,TAG_BASE,TAG_BASE);

	Tools* tools =Tools::sharedTools();
	int number = tools->getLevel();
	char temp[64];
	sprintf(temp,"%d",number);

	CCLabelBMFont* pLabelLevel = CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
	pLabelLevel->setPosition(ccp(50,95));
	this->addChild(pLabelLevel,20);

	//当前送银币或银锭
	if(current[0]!=0){
		//银币
		CCSprite* icon = CCSprite::create("scene_game_toast_upgrade_coin.png");
		addChild(icon,TAG_BASE+2);
		icon->setPosition(ccp(-120,10));

		sprintf(temp,"%d",current[0]);
		CCLabelBMFont* pLabelGain = CCLabelBMFont::create(temp, "font_o.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
		pLabelGain->setPosition(ccp(-50,0));
		this->addChild(pLabelGain,21);
	}else{
		//银锭
		CCSprite* icon = CCSprite::create("scene_game_toast_upgrade_ingot.png");
		addChild(icon,TAG_BASE+2);
		icon->setPosition(ccp(-120,10));

		sprintf(temp,"%d",current[1]);
		CCLabelBMFont* pLabelGain = CCLabelBMFont::create(temp, "font_o.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
		pLabelGain->setPosition(ccp(-50,0));
		this->addChild(pLabelGain,21);
	}


	//即将送的银币或银锭
	if(next[0]!=0){
		//银币
		CCSprite* icon = CCSprite::create("scene_game_toast_upgrade_coin.png");
		addChild(icon,TAG_BASE+4);
		icon->setPosition(ccp(-120,-90));

		sprintf(temp,"%d",next[0]);
		CCLabelBMFont* pLabelFuture = CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
		pLabelFuture->setPosition(ccp(-50,-100));
		this->addChild(pLabelFuture,22);
	}else{
		//银锭
		CCSprite* icon = CCSprite::create("scene_game_toast_upgrade_ingot.png");
		addChild(icon,TAG_BASE+4);
		icon->setPosition(ccp(-120,-90));

		sprintf(temp,"%d",next[1]);
		CCLabelBMFont* pLabelFuture = CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
		pLabelFuture->setPosition(ccp(-50,-100));
		this->addChild(pLabelFuture,22);
	}

	//当前送vip
	if(current[2]!=0){
		sprintf(temp,"scene_game_toast_upgrade_vip_%d.png",current[2]/100-1);
		CCSprite* icon = CCSprite::create(temp);
		addChild(icon,TAG_BASE+5);
		icon->setPosition(ccp(50,10));

		sprintf(temp,"%d",current[2]%100);
		CCLabelBMFont* pLabelFuture = CCLabelBMFont::create(temp, "font_o.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
		pLabelFuture->setPosition(ccp(100,0));
		this->addChild(pLabelFuture,22);
	}

	//即将送vip
	if(next[2]!=0){
		sprintf(temp,"scene_game_toast_upgrade_vip_%d.png",next[2]/100-1);
		CCSprite* icon = CCSprite::create(temp);
		addChild(icon,TAG_BASE+6);
		icon->setPosition(ccp(50,-90));

		sprintf(temp,"%d",next[2]%100);
		CCLabelBMFont* pLabelFuture = CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
		pLabelFuture->setPosition(ccp(100,-100));
		this->addChild(pLabelFuture,22);
	}

}

/****************************************************************************************************
*								   								   								    *
*                                    MyNextPrizeButton       		                                *
*							        							        							*
*****************************************************************************************************/

MyNextPrizeButton::MyNextPrizeButton(){
	CCSprite* pPrizeBtn = CCSprite::create("scene_welcome_info_btn_prize.png");
	addChild(pPrizeBtn,TAG_BTN,TAG_BTN);
}

bool MyNextPrizeButton::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
	CCSize s =getChildByTag(TAG_BTN)->getContentSize();
	CCRect s2 = CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
	CCPoint pp  =getChildByTag(TAG_BTN)->convertTouchToNodeSpaceAR(pTouch);
	if(s2.containsPoint(pp))
	{
		showToast();
	}
	return false;
}

void MyNextPrizeButton::showToast(){
	removeChildByTag(TAG_TOAST);
	removeChildByTag(TAG_TOAST_CHILD1);
	removeChildByTag(TAG_TOAST_CHILD2);
	removeChildByTag(TAG_TOAST_CHILD3);
	removeChildByTag(TAG_TOAST_CHILD4);

	CCSprite* pSprite = CCSprite::create("toast9.png");
	pSprite->setPosition(ccp(0,0));
	addChild(pSprite,TAG_TOAST,TAG_TOAST);

	CCActionInterval* actionMoveBy = CCMoveBy::create(0.15f,ccp(-200,0));
	CCActionInterval* actionFadeOut = CCFadeOut::create(2.0f);
	pSprite->runAction(actionMoveBy);
	pSprite->runAction(actionFadeOut);

	Tools* tools =Tools::sharedTools();
	int number = tools->getLevel();
	unsigned int next[3] ;
	tools->getAwardByUpgrade(number+1,next);
	char temp[64];

	CCSprite* icon ;
	CCLabelBMFont* pLabelFuture;

	//即将送的银币或银锭
	if(next[0]!=0){
		//银币
		icon = CCSprite::create("scene_game_toast_upgrade_coin.png");
		addChild(icon,TAG_TOAST_CHILD1,TAG_TOAST_CHILD1);
		icon->setPosition(ccp(-120,-20));

		sprintf(temp,"%d",next[0]);
		pLabelFuture = CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
		pLabelFuture->setPosition(ccp(-50,-30));
		this->addChild(pLabelFuture,TAG_TOAST_CHILD2,TAG_TOAST_CHILD2);

	}else{
		//银锭
		icon = CCSprite::create("scene_game_toast_upgrade_ingot.png");
		addChild(icon,TAG_TOAST_CHILD1,TAG_TOAST_CHILD1);
		icon->setPosition(ccp(-120,-20));

		sprintf(temp,"%d",next[1]);
		pLabelFuture = CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
		pLabelFuture->setPosition(ccp(-50,-30));
		this->addChild(pLabelFuture,TAG_TOAST_CHILD2,TAG_TOAST_CHILD2);
	}

	CCActionInterval* actionFadeOut2 = (CCActionInterval*)(actionFadeOut->copy());
	icon->runAction(actionFadeOut2);
	pLabelFuture->runAction((CCActionInterval*)(actionFadeOut->copy()));

	CCActionInterval* actionMoveBy2 = (CCActionInterval*)(actionMoveBy->copy());
	icon->runAction(actionMoveBy2);
	pLabelFuture->runAction((CCActionInterval*)(actionMoveBy2->copy()));


	//即将送vip
	if(next[2]!=0){
		sprintf(temp,"scene_game_toast_upgrade_vip_%d.png",next[2]/100-1);
		CCSprite* icon = CCSprite::create(temp);
		addChild(icon,TAG_TOAST_CHILD3,TAG_TOAST_CHILD3);
		icon->setPosition(ccp(50,-20));

		sprintf(temp,"%d",next[2]%100);
		CCLabelBMFont* pLabelFuture = CCLabelBMFont::create(temp, "font_w.fnt", kCCLabelAutomaticWidth, kCCTextAlignmentLeft, CCPointZero);
		pLabelFuture->setPosition(ccp(100,-30));
		this->addChild(pLabelFuture,TAG_TOAST_CHILD4,TAG_TOAST_CHILD4);

		CCActionInterval* actionFadeOut2 = (CCActionInterval*)(actionFadeOut->copy());
		icon->runAction(actionFadeOut2);
		pLabelFuture->runAction((CCActionInterval*)(actionFadeOut->copy()));

		CCActionInterval* actionMoveBy2 = (CCActionInterval*)(actionMoveBy->copy());
		icon->runAction(actionMoveBy2);
		pLabelFuture->runAction((CCActionInterval*)(actionMoveBy2->copy()));
	}

}

void MyNextPrizeButton::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this,-10, true);
	CCNode::onEnter();
}

void MyNextPrizeButton::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCNode::onExit();
}    

void MyNextPrizeButton::touchDelegateRetain()
{
	this->retain();
}

void MyNextPrizeButton::touchDelegateRelease()
{
	this->release();
}