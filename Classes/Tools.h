#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "cocos2d.h"
#include <string>
#include "Controls.h"
using namespace cocos2d;

//第一次进入就初始化
//退出游戏才销毁
class Tools
{
public:
	static Tools *sharedTools();
	static void deleteSharedTools();

	bool checkEncryptionAll(CCLayer* layer); //数据合法性检查

	void showToast(Toast tagToast ,CCLayer* layer);

	void saveAllData();
	//是否开启声音
	bool m_isSoundsOn;

	//登录次数
	void updateRooted();

	//昵称
	std::string getNickName();
	void setNickName(std::string name);

	//等级
	unsigned short getLevel();
	//经验
	unsigned short getExperience();
	void addExperience(short exp , CCLayer* layer);
	unsigned short getMaxExperience();
	void saveLevelAndExperience();
	void getAwardByUpgrade(short level,unsigned int* resultArrayPoint);

	//银币
	unsigned int getCoin(); //有时间了记得加上加密措施
	void updateCoin(int change); //正数增加，负数减少
	void saveCoin();  //保存数据到持久化文件中

	//银锭
	unsigned int getIngot();
	void updateIngot(int change); //正数增加，负数减
	void saveIngot();

	//最大赢取
	unsigned int getMostWon();
	void updateMostWon(int currentWon);
	//胜利场次
	unsigned int getWin();
	void addWin();
	//输场次
	unsigned int getLost();
	void addLost();
	void saveWinAndLost();

	//头像
	short getHead();
	void setHead(short head);

	//VIP
	short getVipHighest(); //得到最大的vip等级
	short getVipAmount();  //得到最大vip等级的剩余局数
	void addVipAmount(short flag , short amount);
	void useVipOnce();
	void saveVip();
    
    //是否广告
    bool getShouldAd();
    void setShouldAd(bool shouldAd);

	CCActionInterval* getActionScaleXYBiuBiu();
	CCActionInterval* getActionScaleYBiuBiuDelay(float time , float delay);
	CCActionInterval* getActionScaleYBiuBiu();
	CCActionInterval* getActionMoveDown();

	//更新宝箱控件的数据
	void updateGoldBox();
	void openGoldBox();
	bool couldCreateGoldBox();
	bool couldOpenGoldBox();
	unsigned int getAwardFromGoldBox();
	unsigned short getSecondsOfGoldBoxByStage();
	unsigned int getSecondsOfGoldBox();

	void openGoldBoxofAmount();
	void updateGoldBoxOfAmount();
	bool couldCreateGoldBoxOfAmount();
	bool couldOpenGoldBoxOfAmount();
	unsigned short getAwardFromGoldBoxByAmount();
	unsigned short getAmountOfGoldBoxByAmount();
	unsigned short getCurrentAmountOfGoldBox();
protected:
	Tools();
	bool init();
private:
	static Tools *mSharedTools;
	CCUserDefault* userDB;

    int m_shouldAd; //是否显示id,0为显示，514为不显示
	bool m_isRootedUpdated;  //保证每次登录只加一次rooted的值

	unsigned short m_level; //等级
	unsigned short m_experience;//经验

	unsigned int m_coin;  //银币
	unsigned int m_ingot; //银锭

	unsigned int m_most_won ;
	unsigned int m_win;
	unsigned int m_lost;

	unsigned short m_vips[3]; //铜银金

	bool checkEncryptionAll(); //数据合法性检查

	void initVariables();
	void initUserDefault();
	bool checkEncryption(short flag); //检查加密数字和字符串的匹配型
	std::string encrypt(unsigned int number);
	std::string encrypt(std::string encryption0);

	//宝箱控件
	short m_stage; //宝箱的阶段分为7档
	short m_seconds_alive; //每个阶段分别赋值，每秒递减，为0时进入下一个阶段
	short m_stage_amount;   //连续玩宝箱的stage
	short m_goldbox_amount; //连续玩了N局送奖励
};



#endif

