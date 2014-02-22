#include "Tools.h"
#include "cocos2d.h"
#include <string>
#include "Controls.h"
#include "md5.h"

using namespace cocos2d;

Tools *Tools::mSharedTools = NULL;

int key = rand()%5000+1000; //内存实时监测key

Tools *Tools::sharedTools()
{
	if (mSharedTools == NULL)
	{
		mSharedTools = new Tools();
		if (!mSharedTools)
		{
			CC_SAFE_DELETE(mSharedTools);
		}
	}
	return mSharedTools;
}

void Tools::deleteSharedTools()
{
	CC_SAFE_DELETE(mSharedTools);
}

Tools::Tools()
{
	userDB =  CCUserDefault::sharedUserDefault();
	initVariables();
}

void Tools::initVariables(){
	m_isSoundsOn = true;

	m_isRootedUpdated = false;//保证每次登录只加一次rooted的值
	m_level =  userDB->getIntegerForKey("level");
	m_experience =  userDB->getIntegerForKey("experience");
	m_coin  =  userDB->getIntegerForKey("coin");
	m_ingot =  userDB->getIntegerForKey("ingot");
	m_most_won =  userDB->getIntegerForKey("most_won");
	m_win =  userDB->getIntegerForKey("win");
	m_lost =  userDB->getIntegerForKey("lost");
	m_vips[0] =  userDB->getIntegerForKey("v0");
	m_vips[1] =  userDB->getIntegerForKey("v1");
	m_vips[2] =  userDB->getIntegerForKey("v2");

    m_shouldAd = userDB->getIntegerForKey("ad");
    
	//内存检测
	m_level+=key;
	m_experience+=key;
	m_coin+=key;
	m_ingot+=key;
	m_vips[0]+=key;
	m_vips[1]+=key;
	m_vips[2]+=key;


	//宝箱数据
	m_stage=0;
	m_seconds_alive=getSecondsOfGoldBoxByStage();
	m_stage_amount=0;   //连续玩宝箱的stage
	m_goldbox_amount=getAmountOfGoldBoxByAmount(); //连续玩了N局送奖励
}

bool Tools::getShouldAd(){
    //是否显示id,0为显示，514为不显示
    if(m_shouldAd==514){
        return false;
    }
    return true;
}

void Tools::setShouldAd(bool shouldAd){
    if(!shouldAd){
        m_shouldAd=514;
        userDB -> setIntegerForKey("ad",m_shouldAd);
    }
}

void Tools::updateGoldBox(){
	//保证m_seconds_alive最小为0
	if(m_seconds_alive>0){
		m_seconds_alive--;
	}else{
		m_seconds_alive=0;
	}
}

void Tools::openGoldBox(){
	if(m_stage>=0 && m_stage<=30){
		m_coin+=getAwardFromGoldBox();
		m_stage++;
		if(m_stage<=30){
			m_seconds_alive=getSecondsOfGoldBoxByStage();
		}	
	}else{
		m_stage=31;
	}
}

bool Tools::couldCreateGoldBox(){
	if(m_stage>=0 && m_stage<=30){
		return true;
	}else{
		return false;
	}	
}

bool Tools::couldOpenGoldBox(){
	if(m_seconds_alive==0 && couldCreateGoldBox()){
		return true;
	}else{
		return false;
	}	
}

unsigned int Tools::getSecondsOfGoldBox(){
	return m_seconds_alive;
}

unsigned short Tools::getSecondsOfGoldBoxByStage(){
	if(m_stage>=0 && m_stage<=30){
		return 60+60*m_stage;
	}
	return 1;
}

void Tools::openGoldBoxofAmount(){
	if(couldOpenGoldBoxOfAmount()){
			m_coin+=getAwardFromGoldBoxByAmount();
			m_stage_amount++;
			if(m_stage_amount<=10){
				m_goldbox_amount=getAmountOfGoldBoxByAmount();
			}	
		}else{
			m_stage=11;
		}
}

unsigned int Tools::getAwardFromGoldBox(){
	if(m_stage>=0 && m_stage<=30){
		return 1000+1000*(m_stage+0.1*m_stage);
	}
	return 0;
}

unsigned short Tools::getAwardFromGoldBoxByAmount(){
	//第一次是送3000，以后每次送5000+stage*1000
	if(m_stage_amount>0){
		return 5000+m_stage_amount*1000;
	}else{
		return 2000;
	}
}

unsigned short Tools::getAmountOfGoldBoxByAmount(){
	if(!couldCreateGoldBoxOfAmount()){
		return 0;
	}
	//第一次是5局送，以后都是10局送
	if(m_stage_amount>0){
		return 10;
	}else{
		return 5;
	}
}

unsigned short Tools::getCurrentAmountOfGoldBox(){
	return m_goldbox_amount;
}

void Tools::updateGoldBoxOfAmount(){
	if(m_goldbox_amount>0){
		m_goldbox_amount--;
	}
}

bool Tools::couldCreateGoldBoxOfAmount(){
	if(m_stage_amount>=0 && m_stage_amount<=10){
		return true;
	}else{
		return false;
	}	
}

bool Tools::couldOpenGoldBoxOfAmount(){
	if(m_goldbox_amount==0 && couldCreateGoldBoxOfAmount()){
		return true;
	}else{
		return false;
	}	
}

void Tools::saveAllData(){
	saveLevelAndExperience();
	saveWinAndLost();
	saveCoin();
	saveIngot();
	saveVip();
	userDB->flush();
}

void Tools::showToast(Toast tagToast,CCLayer* layer){
	MyToast* toast;
	float y=250;
	toast =new MyToast(tagToast);
	toast->setPosition(ccp(512,y));
	layer->addChild(toast,1000);
	toast->autorelease();
}

//登录次数
void Tools::updateRooted(){
	if(!userDB->getBoolForKey("isExisted")) {     
		initUserDefault();
		initVariables();
	} 
	if(!m_isRootedUpdated){	
		unsigned root = userDB->getIntegerForKey("rooted");
		root++;
		userDB->setIntegerForKey("rooted",root);
		m_isRootedUpdated=true;
	}

}

//数据合法性检查，旧
bool Tools::checkEncryptionAll(){
	for(short i = 0 ; i< 7 ;i++){
		if(!checkEncryption(i)){
			initUserDefault();
			initVariables();
			saveAllData();
			return false;
		}
	}
	return true;
}

//数据合法性检查,新
bool Tools::checkEncryptionAll(CCLayer* layer){
	if(!checkEncryptionAll()){
		showToast(eToastAntiHacker,layer);
		return false;
	}
	return true;
}

std::string Tools::encrypt(unsigned int number){
	char temp[32];
	sprintf(temp,"%u",number);
	std::string string_number = temp;
	MD5 md5(string_number);
	string_number = md5.md5();
//	CCLOG(string_number.c_str());

	return string_number;
}

std::string Tools::encrypt(std::string encryption0){
	MD5 md5(encryption0);
	encryption0 = md5.md5();

	return encryption0;
}

//检查加密数字和字符串的匹配型
bool Tools::checkEncryption(short flag){
	if(flag>=0 && flag<=6){
		//得到加密字符串
		char temp[32];
		if(flag<=3){
			sprintf(temp,"string%d",flag+4); //读取双加密后的字符串
		}else{
			//vip系统
			sprintf(temp,"string%d",flag+7); //读取双加密后的字符串
		}
		
		std::string encryption = userDB->getStringForKey(temp);

		//得到数值
		unsigned int number;
		switch(flag){
		case 0:
			number=m_level - key;
			break;
		case 1:
			number=m_experience - key;
			break;
		case 2:
			number=m_coin - key;
			break;
		case 3:	
			number=m_ingot - key;
			break;
		case 4:	
			number=m_vips[0] - key;
			break;
		case 5:	
			number=m_vips[1] - key;
			break;
		case 6:	
			number=m_vips[2] - key;
			break;
		}
		if(encrypt(encrypt(number)) == encryption){
			return true;
		}else{
			return false;
		}
	}else{
		return false;
	}
}

//昵称
std::string Tools::getNickName(){
	return  userDB->getStringForKey("nick");
}

void Tools::setNickName(std::string name){
	userDB->setStringForKey("nick",name);
}

//等级
unsigned short Tools::getLevel(){
	return m_level - key;
}

//经验
unsigned short Tools::getExperience(){
	return m_experience - key;
}

unsigned short Tools::getMaxExperience(){
	if((m_level-key)<=10){
		return 20+ 30*(m_level-key);
	}else{
		return 50+ 50*(m_level-key);
	}
//	return 40;
} 

void Tools::getAwardByUpgrade(short m_level,unsigned int* resultArrayPoint){
	//银币,银锭,vip
	//vip： 1xx铜 2xx银 3xx金
	unsigned int awards[3]= {0,0,0};
	//vip赠送策略,2送5，4送10
	if(	m_level%5==2 ||m_level%5==4	){
		if(m_level >=20){
			awards[2]=300+(5*(m_level%5))/2;
		}else if(m_level>=10){
			awards[2]=200+(5*(m_level%5))/2;
		}else{
			awards[2]=100+(5*(m_level%5))/2;
		}
	}

	//银锭赠送策略,3送level/2+1，5送,level数量的锭
	if(	m_level%5==3 ){
		awards[1] = m_level/2+1;
	}else if(m_level%5==0){
		awards[1] = m_level;
	}

	//银币赠送策略：level*5K
	if(	m_level%5==1 ||m_level%5==2	||m_level%5==4){
		awards[0] = m_level*5000;
	}

	//特殊：30级以后每次都送30锭
	if(m_level>=30){
		awards[1] = 30;
		awards[0] = 0;
	}

	resultArrayPoint[0] = awards[0];
	resultArrayPoint[1] = awards[1];
	resultArrayPoint[2] = awards[2];
}

void Tools::addExperience(short exp,CCLayer* layer){
	//根据vip和经验场的逻辑
	m_experience+=exp;
	if(m_experience-key>=getMaxExperience()){
		m_experience-=getMaxExperience();
		m_level++;

		unsigned int current[3];
		getAwardByUpgrade(m_level-key,current);
		unsigned int next[3];
		getAwardByUpgrade(m_level+1-key,next);

		//升级提示
		MyUpgradeBoard* board = new MyUpgradeBoard(current,next);
		board->setPosition(ccp(512,350));
		layer->addChild(board,999);
		//CCActionInterval* actionn1 = CCMoveBy::create(1.0f,ccp(0,576+350));
		board->runAction(Tools::sharedTools()->getActionScaleYBiuBiu());

		m_coin+=current[0];
		m_ingot+=current[1];
		if(current[2]!=0){
			m_vips[current[2]/100-1]+=current[2]%100;
		}

	}
}

void Tools::saveLevelAndExperience(){
	userDB -> setIntegerForKey("level",m_level-key);
	userDB -> setIntegerForKey("experience",m_experience-key);

	userDB->setStringForKey("string0",encrypt(m_level-key));	
	userDB->setStringForKey("string4",encrypt(encrypt(m_level-key)));	
	userDB->setStringForKey("string1",encrypt(m_experience-key));	
	userDB->setStringForKey("string5",encrypt(encrypt(m_experience-key)));	
}

unsigned int Tools::getMostWon(){
	return m_most_won;
}

void Tools::updateMostWon(int currentWon){
	if(currentWon>0 && currentWon>m_most_won){
		m_most_won = currentWon;
	}
}

unsigned int Tools::getWin(){
	return m_win;
}

void Tools::addWin(){
	m_win++;
}

unsigned int Tools::getLost(){
	return m_lost;
}

void Tools::addLost(){
	m_lost++;
}

void Tools::saveWinAndLost(){
	userDB -> setIntegerForKey("most_won",m_most_won);
	userDB -> setIntegerForKey("win",m_win);
	userDB -> setIntegerForKey("lost",m_lost);
}

short Tools::getHead(){
	return userDB->getIntegerForKey("head");
}
void Tools::setHead(short head){
	userDB->setIntegerForKey("head",head);
}

unsigned int Tools::getCoin(){
	return m_coin-key;
}

unsigned int Tools::getIngot(){
	return m_ingot-key;
}

void Tools::updateCoin(int change){
	if(change<0){
		if(abs(change) > m_coin){
			change=-m_coin;
		}
	}
	m_coin+=change;
}

//正数增加，负数减少
void Tools::updateIngot(int change){
	if(change<0){
		if(abs(change) > m_ingot){
			change=m_ingot * -1;
		}
	}
	m_ingot+=change;
}

//保存数据到持久化文件中
void Tools::saveCoin(){
	userDB->setIntegerForKey("coin",m_coin-key);
	userDB->setStringForKey("string2",encrypt(m_coin-key));	
	userDB->setStringForKey("string6",encrypt(encrypt(m_coin-key)));	
}

void Tools::saveIngot(){
	userDB->setIntegerForKey("ingot",m_ingot-key);
	userDB->setStringForKey("string3",encrypt(m_ingot-key));	
	userDB->setStringForKey("string7",encrypt(encrypt(m_ingot-key)));	
}

//得到最大的vip等级
short Tools::getVipHighest(){
	for(short i =0 ;i<3;i++){
		if(m_vips[2-i]-key>0){
			return 2-i;
		}
	}
	return -1;
}

//得到最大vip等级的剩余局数
short Tools::getVipAmount(){
	short highest = getVipHighest();
	if(highest!=-1){
		return m_vips[highest]-key;
	}else{
		return 0;
	}
}

void Tools::addVipAmount(short flag , short amount){
	m_vips[flag]+=amount;
}

void Tools::useVipOnce(){
	//判断哦
	if(getVipHighest()>-1){
		m_vips[getVipHighest()]--;
	}
}

void Tools::saveVip(){
	userDB->setIntegerForKey("v0",m_vips[0]-key);	
	userDB->setIntegerForKey("v1",m_vips[1]-key);	
	userDB->setIntegerForKey("v2",m_vips[2]-key);	

	//vip1
	userDB->setStringForKey("string8",encrypt(m_vips[0]-key));
	userDB->setStringForKey("string11",encrypt(encrypt(m_vips[0]-key)));	
	//vip2
	userDB->setStringForKey("string9",encrypt(m_vips[1]-key));
	userDB->setStringForKey("string12",encrypt(encrypt(m_vips[1]-key)));	
	//vip金
	userDB->setStringForKey("string10",encrypt(m_vips[2]-key));
	userDB->setStringForKey("string13",encrypt(encrypt(m_vips[2]-key)));	
}

CCActionInterval* Tools::getActionScaleYBiuBiuDelay(float time , float delay){
	CCActionInterval* action1 = CCScaleTo::create(time,1.0f,1.5f);
	CCActionInterval* action2 = CCScaleTo::create(time,1.0f,0.8f);
	CCActionInterval* action3 = CCScaleTo::create(time,1.0f,1.2f);
	CCActionInterval* action4 = CCScaleTo::create(time,1.0f,0.9f);
	CCActionInterval* action5 = CCScaleTo::create(time,1.0f,1.1f);
	CCActionInterval* action6 = CCScaleTo::create(time,1.0f,1.0f);
	CCActionInterval* seq = CCSequence::create(CCDelayTime::create(delay),action1,action2,action3,action4,action5,action6,NULL);
	return seq;
}

CCActionInterval* Tools::getActionScaleYBiuBiu(){
	CCActionInterval* action1 = CCScaleTo::create(0.1f,1.0f,1.5f);
	CCActionInterval* action2 = CCScaleTo::create(0.1f,1.0f,0.8f);
	CCActionInterval* action3 = CCScaleTo::create(0.1f,1.0f,1.2f);
	CCActionInterval* action4 = CCScaleTo::create(0.1f,1.0f,0.9f);
	CCActionInterval* action5 = CCScaleTo::create(0.1f,1.0f,1.1f);
	CCActionInterval* action6 = CCScaleTo::create(0.1f,1.0f,1.0f);
	CCActionInterval* seq = CCSequence::create(action1,action2,action3,action4,action5,action6,NULL);
	return seq;
}

CCActionInterval* Tools::getActionScaleXYBiuBiu(){
	CCActionInterval* action1 = CCScaleTo::create(0.1f,1.5f,1.5f);
	CCActionInterval* action2 = CCScaleTo::create(0.1f,0.8f,0.8f);
	CCActionInterval* action3 = CCScaleTo::create(0.1f,1.2f,1.2f);
	CCActionInterval* action4 = CCScaleTo::create(0.1f,0.9f,0.9f);
	CCActionInterval* action5 = CCScaleTo::create(0.1f,1.1f,1.1f);
	CCActionInterval* action6 = CCScaleTo::create(0.1f,1.0f,1.0f);
	CCActionInterval* seq = CCSequence::create(action1,action2,action3,action4,action5,action6,NULL);
	return seq;
}

CCActionInterval* Tools::getActionMoveDown(){
	CCActionInterval* actionn1 = CCMoveBy::create(0.1f,ccp(0,-576));
	CCActionInterval* actionn2 = CCMoveBy::create(0.075f,ccp(0,80));
	CCActionInterval* actionn3 = CCMoveBy::create(0.075f,ccp(0,-80));
	CCActionInterval* actionn4 = CCMoveBy::create(0.05f,ccp(0,40));
	CCActionInterval* actionn5 = CCMoveBy::create(0.05f,ccp(0,-40));
	CCActionInterval* actionn6 = CCMoveBy::create(0.025f,ccp(0,20));
	CCActionInterval* actionn7 = CCMoveBy::create(0.025f,ccp(0,-20));
	CCActionInterval* seq = CCSequence::create(actionn1,actionn2,actionn3,actionn4,actionn5,actionn6,actionn7,NULL);
	return seq;
}


void Tools::initUserDefault(){
	//第一次启动时初始化数据存储   
	userDB->setBoolForKey("isExisted",true); 
	userDB->setIntegerForKey("rooted",0);
	userDB->setStringForKey("nick","玩家");
	userDB->setIntegerForKey("level",0);
	userDB->setIntegerForKey("experience",0);
	m_coin=5000;
	userDB->setIntegerForKey("coin",m_coin);
	userDB->setIntegerForKey("ingot",0);
	userDB->setIntegerForKey("most_won",0);
	userDB->setIntegerForKey("win",0);
	userDB->setIntegerForKey("lost",0);	
	userDB->setIntegerForKey("head",-1);	
	userDB->setIntegerForKey("v0",0);	
	userDB->setIntegerForKey("v1",0);	
	userDB->setIntegerForKey("v2",0);	
    userDB->setIntegerForKey("ad",0);
    
	//level
	userDB->setStringForKey("string0",encrypt(0));	
	//经验
	userDB->setStringForKey("string1",encrypt(0));	
	//银币
	userDB->setStringForKey("string2",encrypt(m_coin));	
	//银锭
	userDB->setStringForKey("string3",encrypt(0));

	//level
	userDB->setStringForKey("string4",encrypt(encrypt(0)));	
	//经验
	userDB->setStringForKey("string5",encrypt(encrypt(0)));	
	//银币
	userDB->setStringForKey("string6",encrypt(encrypt(m_coin)));	
	//银锭
	userDB->setStringForKey("string7",encrypt(encrypt(0)));

	//vip1
	userDB->setStringForKey("string8",encrypt(0));
	//vip2
	userDB->setStringForKey("string9",encrypt(0));
	//vip金
	userDB->setStringForKey("string10",encrypt(0));

	userDB->setStringForKey("string11",encrypt(encrypt(0)));	
	userDB->setStringForKey("string12",encrypt(encrypt(0)));	
	userDB->setStringForKey("string13",encrypt(encrypt(0)));	

	userDB->flush();

}
