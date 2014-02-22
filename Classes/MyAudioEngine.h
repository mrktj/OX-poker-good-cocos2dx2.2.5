#ifndef __MY_AUDIO_ENGINE_H__
#define __MY_AUDIO_ENGINE_H__

/************************************
*								    *
*            AudioFile              *
*							        *
*************************************/
typedef enum tagAudioFile
{
	eAudioClick,
	eAudioSkew,
	eAudioShowChipsAll,
	eAudioShowChipsOne,
	eAudioSendOneCard,
	eAudioLose,
	eAudioWin,
	eAudioUnrob,
	eAudioRob,
	eAudioShopSuccess,
	eAudioAddCoin
}enumAudioFile; 

/************************************
*								    *
*            MyAudioEngine          *
*							        *
*************************************/
class MyAudioEngine
{
public:
	static MyAudioEngine *sharedMyAudioEngine();
	static void deleteSharedMyAudioEngine();	
	void playEffect(enumAudioFile eAudioFile);
	void playEffectShowOx(short ox ,bool isBoy);
	void playBackgrondMusic();
	void stopBackgrondMusic();
private:
	MyAudioEngine();
	static MyAudioEngine *mSharedMyAudioEngine;
};


#endif

